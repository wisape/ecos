//==========================================================================
//
//      var_smp.c
//
//      HAL SMP implementation
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002, 2010 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    Original from nickg on I386 platform
// Contributors: Updated for Freescale Quicc III platform by ccoutand
// Date:         2009-12-01
// Purpose:      HAL SMP implementation for Freescale Quicc III
// Description:  This file contains SMP support functions.
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>

#ifdef CYGPKG_HAL_SMP_SUPPORT

#ifdef CYGPKG_KERNEL
#include <pkgconf/kernel.h>
#endif

#include <cyg/hal/ppc_regs.h>
#include <cyg/infra/cyg_type.h>         // Base types
#include <cyg/infra/cyg_trac.h>         // tracing macros
#include <cyg/infra/cyg_ass.h>          // assertion macros
#include <cyg/infra/diag.h>

#include <cyg/hal/drv_api.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_smp.h>

#define SCREEN_DIAGNOSTICS 1

HAL_SPINLOCK_TYPE cyg_hal_ioapic_lock;

/*------------------------------------------------------------------------*/
typedef void (*CYG_CPU_ENTRY)(void);

__externC volatile CYG_WORD32 cyg_hal_smp_cpu_sync_flag[HAL_SMP_CPU_MAX];
__externC volatile CYG_WORD32 cyg_hal_smp_cpu_sync[HAL_SMP_CPU_MAX];
__externC volatile CYG_WORD32 cyg_hal_smp_vsr_sync_flag;
__externC volatile CYG_WORD32 cyg_hal_smp_cpu_running[HAL_SMP_CPU_MAX];
__externC volatile CYG_CPU_ENTRY cyg_hal_smp_cpu_entry[HAL_SMP_CPU_MAX];

/*------------------------------------------------------------------------*/

__externC void cyg_hal_smp_start(void);

// CPU reset
__externC cyg_uint32 _cyg_hal_slave_trampoline;
__externC cyg_uint32 _cyg_hal_slave_trampoline_end;

externC void
cyg_hal_set_secondary_boot_page (void)
{
  volatile cyg_uint32 *bptr =   (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_BPTR);

  *bptr = ((cyg_uint32) &_cyg_hal_slave_trampoline) >> 12;
  
  *bptr |= CYGARC_REG_BPTR_EN;

  return;

}


/*------------------------------------------------------------------------*/
__externC void 
cyg_hal_cpu_start( HAL_SMP_CPU_TYPE cpu )
{
  volatile cyg_uint32 *eebpcr =   (volatile cyg_uint32 *) \
    (CYGARC_IMM_BASE + CYGARC_REG_IMM_EEBPCR);

#ifdef CYG_HAL_QUICC3_CORE0_HIGH_PRIORITY
  /* Give higher priority to core0 */
  *eebpcr |= CYGARC_REG_EEBPCR_SET_CPU_PRI(E500_CORE0, CYGARC_REG_EEBPCR_CPU_PRI_LEV1);
#endif

  /* Start secondary CPU */
  *eebpcr |= (CYGARC_REG_EEBPCR_CPU0_EN << cpu);
 
}


/*------------------------------------------------------------------------*/

__externC void cyg_hal_smp_startup(void);
__externC void cyg_hal_smp_start(void);
__externC void cyg_kernel_smp_startup(void);

__externC void
cyg_hal_cpu_release( HAL_SMP_CPU_TYPE cpu )
{
    cyg_hal_smp_cpu_entry[cpu] = cyg_hal_smp_start;

    while( cyg_hal_smp_cpu_entry[cpu] != 0 )
    {
        hal_delay_us( 100 );
         continue;
    }
}


/*------------------------------------------------------------------------*/

__externC void 
cyg_hal_smp_startup(void)
{
    HAL_SMP_CPU_TYPE cpu;

    cpu  = HAL_SMP_CPU_THIS();
    
#ifndef CYG_HAL_STARTUP_RAM 
    // Wait 1s for the world to settle
    hal_delay_us( 1000000 );
#else
    hal_delay_us( 100 );  
#endif

    // Added for testing standalone ROM and ROMRAM application that
    // uses eCos kernel
#if (defined(CYGPKG_KERNEL_SMP_SUPPORT) \
     && (defined(CYG_HAL_STARTUP_ROM) || \
         defined(CYG_HAL_STARTUP_ROMRAM)))

  // Just wait for the primary CPU to release us
  while(cyg_hal_smp_cpu_entry[cpu] == 0)
  {
     hal_delay_us( 100 );   
  }

  cyg_hal_smp_cpu_entry[cpu] = 0;
#endif

#ifdef CYGPKG_KERNEL_SMP_SUPPORT
    cyg_hal_smp_cpu_running[cpu] = 1;
    cyg_kernel_smp_startup();
#else 
    //
    // The secondary CPUs are parked here by the ROM application. They will wait for the
    // primary CPU to initialise the application entry point.
    // The entry point must allow the secondary CPUs to re-initialised their interrupt
    // stack before anything else.
    for(;;)
    {
        void (*entry)(void);

        while( (entry = cyg_hal_smp_cpu_entry[cpu]) == 0 )
        {
           hal_delay_us( 100 );            
        }

        cyg_hal_smp_cpu_entry[cpu] = 0; 

        if( entry != NULL )
        {
            cyg_hal_smp_cpu_running[cpu] = 1;
            entry();
        }
    }
#endif
}

/*------------------------------------------------------------------------*/

__externC void 
cyg_hal_smp_cpu_start_all(void)
{
    HAL_SMP_CPU_TYPE cpu;

    cyg_hal_set_secondary_boot_page();

    for( cpu = 0; cpu < HAL_SMP_CPU_COUNT(); cpu++ )
    {
        cyg_hal_smp_cpu_sync[cpu] = 0;
        cyg_hal_smp_cpu_sync_flag[cpu] = 0;
        cyg_hal_smp_cpu_running[cpu] = 0;
        cyg_hal_smp_cpu_entry[cpu] = 0;

        if( cpu != HAL_SMP_CPU_THIS() )
            cyg_hal_cpu_start( cpu );
        else cyg_hal_smp_cpu_running[cpu] = 1;
    }
}

/*------------------------------------------------------------------------*/
// SMP message buffers.
// SMP CPUs pass messages to eachother via a small circular buffer
// protected by a spinlock. Each message is a single 32 bit word with
// a type code in the top 4 bits and any argument in the remaining
// 28 bits.

#define SMP_MSGBUF_SIZE 4

static struct smp_msg_t
{
    HAL_SPINLOCK_TYPE           lock;           // protecting spinlock
    volatile CYG_WORD32         msgs[SMP_MSGBUF_SIZE]; // message buffer
    volatile CYG_WORD32         head;           // head of list
    volatile CYG_WORD32         tail;           // tail of list
    volatile CYG_WORD32         reschedule;     // reschedule request
    volatile CYG_WORD32         timeslice;      // timeslice request
} smp_msg[HAL_SMP_CPU_MAX];

/*------------------------------------------------------------------------*/
// Pass a message to another CPU.

__externC void 
cyg_hal_cpu_message( HAL_SMP_CPU_TYPE cpu,
                                    CYG_WORD32 msg,
                                    CYG_WORD32 arg,
                                    CYG_WORD32 wait)
{
    CYG_INTERRUPT_STATE istate;    
    struct smp_msg_t *m = &smp_msg[cpu];
    int i;

    HAL_DISABLE_INTERRUPTS( istate );
    
    // Get access to the message buffer for the selected CPU
    HAL_SPINLOCK_SPIN( m->lock );
 
    if( msg == HAL_SMP_MESSAGE_RESCHEDULE )
        m->reschedule = true;
    else if( msg == HAL_SMP_MESSAGE_TIMESLICE )
        m->timeslice = true;
    else
    {
        CYG_WORD32 next = (m->tail + 1) & (SMP_MSGBUF_SIZE-1);

        // If the buffer is full, wait for space to appear in it.
        // This should only need to be done very rarely.
    
        while( next == m->head )
        {
            HAL_SPINLOCK_CLEAR( m->lock );
            for( i = 0; i < 1000; i++ );
            HAL_SPINLOCK_SPIN( m->lock );        
        }

        m->msgs[m->tail] = msg | arg;

        m->tail = next;
    }
    
    // Now send an interrupt to the CPU.    
    if( cyg_hal_smp_cpu_running[cpu] )
    {
        // Send interrupt
        HAL_INTERRUPT_IPI_SEND(CYGNUM_HAL_SMP_CPU_INTERRUPT_VECTOR( cpu ), cpu);
    }

    HAL_SPINLOCK_CLEAR( m->lock );

    // If we are expected to wait for the command to complete, then
    // spin here until it does. We actually wait for the destination
    // CPU to empty its input buffer. So we might wait for messages
    // from other CPUs as well. But this is benign.
    
    while(wait)
    {
        for( i = 0; i < 1000; i++ );
        
        HAL_SPINLOCK_SPIN( m->lock );

        if( m->head == m->tail )
            wait = false;
        
        HAL_SPINLOCK_CLEAR( m->lock );

    } 

    HAL_RESTORE_INTERRUPTS( istate );  
}

/*------------------------------------------------------------------------*/

#if SCREEN_DIAGNOSTICS
static int isrs[CYGPKG_HAL_SMP_CPU_MAX];
static int dsrs[CYGPKG_HAL_SMP_CPU_MAX];
#endif

__externC CYG_WORD32 
cyg_hal_cpu_message_isr( CYG_WORD32 vector, CYG_ADDRWORD data )
{
    HAL_SMP_CPU_TYPE me = HAL_SMP_CPU_THIS();
    struct smp_msg_t *m = &smp_msg[me];
    CYG_WORD32 ret = CYG_ISR_HANDLED;
    CYG_INTERRUPT_STATE istate;
    
    HAL_DISABLE_INTERRUPTS( istate );
    HAL_SPINLOCK_SPIN( m->lock );

    // First, acknowledge the interrupt.
    HAL_INTERRUPT_ACKNOWLEDGE( vector );

#if SCREEN_DIAGNOSTICS
    isrs[me]++;    
#endif
    
    if( m->reschedule || m->timeslice )
        ret |= CYG_ISR_CALL_DSR;               // Ask for the DSR to be called.
    
    // Now pick messages out of the buffer and handle them
    
    while( m->head != m->tail )
    {
        CYG_WORD32 msg = m->msgs[m->head];

        switch( msg & HAL_SMP_MESSAGE_TYPE )
        {
        case HAL_SMP_MESSAGE_RESCHEDULE:
            ret |= CYG_ISR_CALL_DSR;           // Ask for the DSR to be called.
            break;
        case HAL_SMP_MESSAGE_MASK:
            // Mask the supplied vector
            //  cyg_hal_interrupt_set_mask( msg&HAL_SMP_MESSAGE_ARG, false );
            break;
        case HAL_SMP_MESSAGE_UNMASK:
            // Unmask the supplied vector
            //  cyg_hal_interrupt_set_mask( msg&HAL_SMP_MESSAGE_ARG, true );
            break;
        case HAL_SMP_MESSAGE_REVECTOR:
             // Deal with a change of CPU assignment for a vector. We
             // only actually worry about what happens when the vector
             // is changed to some other CPU. We just mask the
             // interrupt locally.
             // if( hal_interrupt_cpu[msg&HAL_SMP_MESSAGE_ARG] != me )
             //    cyg_hal_interrupt_set_mask( msg&HAL_SMP_MESSAGE_ARG, false );
            break;
        }

        // Update the head pointer after handling the message, so that
        // the wait in cyg_hal_cpu_message() completes after the action
        // requested.
        
        m->head = (m->head + 1) & (SMP_MSGBUF_SIZE-1);
    }

    HAL_SPINLOCK_CLEAR( m->lock );    

    HAL_RESTORE_INTERRUPTS( istate );

    return ret;
}

/*------------------------------------------------------------------------*/
// CPU message DSR.
// This is only executed if the message was
// HAL_SMP_MESSAGE_RESCHEDULE. It calls up into the kernel to effect a
// reschedule.

__externC void cyg_scheduler_set_need_reschedule(void);
__externC void cyg_scheduler_timeslice_cpu(void);

#if SCREEN_DIAGNOSTICS
__externC int cyg_scheduler_sched_lock;
 int rescheds[CYGPKG_HAL_SMP_CPU_MAX];
 int timeslices[CYGPKG_HAL_SMP_CPU_MAX];
#endif

__externC CYG_WORD32
cyg_hal_cpu_message_dsr( CYG_WORD32 vector, CYG_ADDRWORD data )
{
    HAL_SMP_CPU_TYPE me = HAL_SMP_CPU_THIS();
    struct smp_msg_t *m = &smp_msg[me];
    CYG_INTERRUPT_STATE istate;
    CYG_WORD32 reschedule, timeslice;
    
    HAL_DISABLE_INTERRUPTS( istate );
    HAL_SPINLOCK_SPIN( m->lock );

#if SCREEN_DIAGNOSTICS    
    dsrs[me]++;    
#endif
    
    reschedule = m->reschedule;
    timeslice = m->timeslice;
    m->reschedule = m->timeslice = false;

    HAL_SPINLOCK_CLEAR( m->lock );    
    HAL_RESTORE_INTERRUPTS( istate );
        
    if( reschedule )
    {
#if SCREEN_DIAGNOSTICS        
        rescheds[me]++;
#endif        
        cyg_scheduler_set_need_reschedule();
    }
    if( timeslice )
    {
#if SCREEN_DIAGNOSTICS
        timeslices[me]++;
#endif     
        cyg_scheduler_timeslice_cpu();
    }

    return 0;

}

/*------------------------------------------------------------------------*/

__externC void 
cyg_hal_smp_halt_other_cpus(void)
{
    int i;
    HAL_SMP_CPU_TYPE me = HAL_SMP_CPU_THIS();
  
    for( i = 0 ; i < HAL_SMP_CPU_COUNT(); i++ )
    {
        if( i != me && cyg_hal_smp_cpu_running[i] )
        {
            CYG_WORD32 oldsync;

            oldsync = cyg_hal_smp_cpu_sync_flag[i]; 
            cyg_hal_smp_cpu_sync[i] = 0;
// FIXME
#if 0 
            icrhi = i<<24;
            icrlo = CYGNUM_HAL_VECTOR_NMI;  // not really used
            icrlo |= 0x00000400;    // Delivery = NMI

            // Write the ICR register. The interrupt will be raised when
            // the low word is written.
            HAL_APIC_WRITE( HAL_APIC_ICR_HI, icrhi );
            HAL_APIC_WRITE( HAL_APIC_ICR_LO, icrlo );

            // Wait for the ICR to become inactive
            do {           
                HAL_APIC_READ( HAL_APIC_ICR_LO, icrlo );
            } while( (icrlo & 0x00001000) != 0 );
#endif

            // Wait for CPU to halt
            while( cyg_hal_smp_cpu_sync_flag[i] == oldsync )
            {        
                hal_delay_us( 100 );
            }
            
        }
    }
}

__externC void 
cyg_hal_smp_release_other_cpus(void)
{
    int i;
    for( i = 0 ; i < HAL_SMP_CPU_COUNT(); i++ )
    {
        if( i != HAL_SMP_CPU_THIS() && cyg_hal_smp_cpu_running[i] )
        {
            CYG_WORD32 oldsync = cyg_hal_smp_cpu_sync_flag[i];        
            cyg_hal_smp_cpu_sync[i] = 1;
            while( cyg_hal_smp_cpu_sync_flag[i] == oldsync )
                continue;
            cyg_hal_smp_cpu_sync[i] = 0;
        }
    }
}


#endif // CYGPKG_HAL_SMP_SUPPORT

/*------------------------------------------------------------------------*/
/* End of pcmb_smp.c                                                      */
