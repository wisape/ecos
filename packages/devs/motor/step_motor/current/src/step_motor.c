//==========================================================================
//
//      step_motor.c
//
//      Step Motor IO interface
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2008 Free Software Foundation, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later
// version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with eCos; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// As a special exception, if other files instantiate templates or use
// macros or inline functions from this file, or you compile this file
// and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered by
// the GNU General Public License. However the source code for this file
// must still be made available in accordance with section (3) of the GNU
// General Public License v2.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
// -------------------------------------------
// ####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    ccoutand
// Contributors:
// Date:         2010-10-23
// Purpose:
// Description:
//
//####DESCRIPTIONEND####
//
//==========================================================================


//==========================================================================
//                                 INCLUDES
//==========================================================================
#include <pkgconf/system.h>
#include <pkgconf/devs_step_motor.h>

#include <cyg/infra/cyg_type.h>         // base types
#include <cyg/io/io.h>
#include <cyg/io/devtab.h>
#include <cyg/io/gpio.h>
#include <cyg/io/step_motor.h>

#if CYGPKG_DEVS_STEP_MOTOR_DEBUG_LEVEL > 0
   #define debug1_printf(args...) diag_printf(args)
#else
   #define debug1_printf(args...)
#endif

//==========================================================================
// Data type
typedef struct step_motor_info step_motor_info;

typedef void (*motor_move_fn) ( step_motor_info  * , cyg_uint32  );

struct step_motor_info
{
  cyg_uint32            sig_a;
  cyg_uint32            sig_a_bar;
  cyg_uint32            sig_b;
  cyg_uint32            sig_b_bar;
  cyg_uint32            max_speed;
  cyg_uint32            steps;
  cyg_uint32            step_delay;
  cyg_step_motor_info_t user_info;
  motor_move_fn         motor_move;
};


//==========================================================================
// Declarations

static bool step_motor_init( struct cyg_devtab_entry *tab );
static Cyg_ErrNo step_motor_lookup( struct cyg_devtab_entry **tab, \
        struct cyg_devtab_entry  *sub_tab, \
        const char *name );

static Cyg_ErrNo step_motor_write( cyg_io_handle_t handle, \
        const void *_buf, \
        cyg_uint32 *len );

static Cyg_ErrNo step_motor_set_config( cyg_io_handle_t handle, \
        cyg_uint32 key, \
        const void *xbuf, \
        cyg_uint32 *len);
static Cyg_ErrNo step_motor_get_config(cyg_io_handle_t handle, \
        cyg_uint32 key, \
        void *xbuf, \
        cyg_uint32 *len);

static void motor_step_full( step_motor_info  * , cyg_uint32  );
static void motor_step_half( step_motor_info  * , cyg_uint32  );
static void motor_single_step( step_motor_info  * , cyg_uint8  );


#include CYGDAT_DEVS_STEP_MOTOR_INL // Instantiate Step Motor


//--------------------------------------------------------------------------
// Initialize the IOs
static
bool step_motor_init( struct cyg_devtab_entry *tab )
{
  step_motor_info *info = (step_motor_info *)tab->priv;

  // Configure IOs to control the motor
  cyg_hal_gpio_cfg( info->sig_a,     CYG_GPIO_OUT );
  cyg_hal_gpio_cfg( info->sig_a_bar, CYG_GPIO_OUT );
  cyg_hal_gpio_cfg( info->sig_b,     CYG_GPIO_OUT );
  cyg_hal_gpio_cfg( info->sig_b_bar, CYG_GPIO_OUT );

  // Set IOs
  cyg_hal_gpio_write( info->sig_a,     CYG_GPIO_RESET );
  cyg_hal_gpio_write( info->sig_b,     CYG_GPIO_RESET );
  cyg_hal_gpio_write( info->sig_a_bar, CYG_GPIO_RESET );
  cyg_hal_gpio_write( info->sig_b_bar, CYG_GPIO_RESET );

  return true;
}


//--------------------------------------------------------------------------
// Lookup
static Cyg_ErrNo step_motor_lookup(struct cyg_devtab_entry **tab,
                              struct cyg_devtab_entry  *sub_tab,
                              const char               *name)
{
  step_motor_info  *info = (step_motor_info *) (*tab)->priv;
  Cyg_ErrNo res          = -EINVAL;

  if( info->user_info.mode == CYG_DEVS_STEP_MOTOR_FULL_STEP )
  {
    info->motor_move = motor_step_full;
    res = ENOERR;
  }
  else if( info->user_info.mode == CYG_DEVS_STEP_MOTOR_HALF_STEP )
  {
    info->motor_move = motor_step_half;
    res = ENOERR;
  }

  return res;
}


//--------------------------------------------------------------------------
// Set config
static Cyg_ErrNo step_motor_set_config( cyg_io_handle_t handle,
                                        cyg_uint32 key,
                                        const void *xbuf, cyg_uint32 *len)
{
  cyg_devtab_entry_t *t = (cyg_devtab_entry_t *)handle;
  step_motor_info *info = (step_motor_info *)t->priv;
  cyg_step_motor_info_t *buf = (cyg_step_motor_info_t *)xbuf;
  Cyg_ErrNo res   = -EINVAL;

  switch( key )
  {
    case CYG_DEVS_SET_CONFIG_STEP_MOTOR_MODE:
      if( buf->mode == CYG_DEVS_STEP_MOTOR_FULL_STEP )
      {
        info->user_info.mode = buf->mode;
        info->motor_move = motor_step_full;
        res = ENOERR;
      }
      else if( buf->mode == CYG_DEVS_STEP_MOTOR_HALF_STEP )
      {
        info->user_info.mode = buf->mode;
        info->motor_move = motor_step_half;
        res = ENOERR;
      }
      break;
    default:
      break;
  }

  return res;
}


//--------------------------------------------------------------------------
// Get config
static Cyg_ErrNo step_motor_get_config( cyg_io_handle_t handle,
                                        cyg_uint32 key, void *xbuf,
                                        cyg_uint32 *len)
{
  cyg_devtab_entry_t *t = (cyg_devtab_entry_t *)handle;
  step_motor_info *info = (step_motor_info *)t->priv;
  cyg_step_motor_info_t *buf = (cyg_step_motor_info_t *)xbuf;
  Cyg_ErrNo res   = -EINVAL;

  switch( key )
  {
    case CYG_DEVS_GET_CONFIG_STEP_MOTOR_MODE:
      buf->mode = info->user_info.mode;
      res = ENOERR;
      break;
    default:
      break;
  }

  return res;
}


//--------------------------------------------------------------------------
// Helper functions
static void
motor_single_step( step_motor_info  *info , cyg_uint8 sequence )
{
  // Poke IOs
  cyg_hal_gpio_write( info->sig_a,     ((sequence & 0x08 ) >> 3 ) );
  cyg_hal_gpio_write( info->sig_b,     ((sequence & 0x04 ) >> 2 ) );
  cyg_hal_gpio_write( info->sig_a_bar, ((sequence & 0x02 ) >> 1 ) );
  cyg_hal_gpio_write( info->sig_b_bar, ( sequence & 0x01 ) );
}


#define SM_GET_SEQUENCE_FORWARD( seq )  \
({                      \
  cyg_uint8 __tmp;      \
  __tmp = seq & 0x0F;   \
  seq = seq >> 1;       \
  __tmp;                \
})

#define SM_GET_SEQUENCE_BACKWARD( seq )  \
({                                  \
  cyg_uint8 __tmp;                  \
  __tmp = ( ( seq >> 4 ) & 0x0F );  \
  seq = seq << 1;                   \
  __tmp;                            \
})

#define NUM_SEQUENCE_PER_STEP 4

//--------------------------------------------------------------------------
// Full step mode
static void
motor_step_full( step_motor_info  *info , cyg_uint32 direction )
{
  cyg_uint8 backward = ( CYG_GPIO_SET <<  7 ) | \
         ( CYG_GPIO_SET   <<  6 ) | \
         ( CYG_GPIO_RESET <<  5 ) | \
         ( CYG_GPIO_RESET <<  4 ) | \
         ( CYG_GPIO_SET   <<  3 ) | \
         ( CYG_GPIO_SET   <<  2 ) | \
         ( CYG_GPIO_RESET <<  1 ) | \
         ( CYG_GPIO_RESET );
  cyg_uint8 forward = ( CYG_GPIO_RESET <<  7 ) | \
         ( CYG_GPIO_RESET <<  6 ) | \
         ( CYG_GPIO_SET   <<  5 ) | \
         ( CYG_GPIO_SET   <<  4 ) | \
         ( CYG_GPIO_RESET <<  3 ) | \
         ( CYG_GPIO_RESET <<  2 ) | \
         ( CYG_GPIO_SET   <<  1 ) | \
         ( CYG_GPIO_SET );
  cyg_uint8 sequence;
  cyg_uint8 steps = NUM_SEQUENCE_PER_STEP;

  // Set IOs
  while ( steps-- )
  {
    if ( direction != CYG_DEVS_STEP_MOTOR_DIR_BACKWARD )
      sequence = SM_GET_SEQUENCE_FORWARD( forward );
    else
      sequence = SM_GET_SEQUENCE_BACKWARD( backward );
    motor_single_step( info , sequence );
    hal_delay_us( info->step_delay );
  }

}


//--------------------------------------------------------------------------
// Half step mode
static void
motor_step_half( step_motor_info  *info , cyg_uint32 direction )
{
  cyg_uint8 backward_s1 = ( CYG_GPIO_SET <<  7 ) | \
         ( CYG_GPIO_SET   <<  6 ) | \
         ( CYG_GPIO_RESET <<  5 ) | \
         ( CYG_GPIO_RESET <<  4 ) | \
         ( CYG_GPIO_SET   <<  3 ) | \
         ( CYG_GPIO_SET   <<  2 ) | \
         ( CYG_GPIO_RESET <<  1 ) | \
         ( CYG_GPIO_RESET );
  cyg_uint8 backward_s2 = ( CYG_GPIO_RESET <<  7 ) | \
         ( CYG_GPIO_SET   <<  6 ) | \
         ( CYG_GPIO_RESET <<  5 ) | \
         ( CYG_GPIO_RESET <<  4 ) | \
         ( CYG_GPIO_RESET <<  3 ) | \
         ( CYG_GPIO_SET   <<  2 ) | \
         ( CYG_GPIO_RESET <<  1 ) | \
         ( CYG_GPIO_RESET );
  cyg_uint8 forward_s1 = ( CYG_GPIO_RESET <<  7 ) | \
         ( CYG_GPIO_RESET <<  6 ) | \
         ( CYG_GPIO_SET   <<  5 ) | \
         ( CYG_GPIO_SET   <<  4 ) | \
         ( CYG_GPIO_RESET <<  3 ) | \
         ( CYG_GPIO_RESET <<  2 ) | \
         ( CYG_GPIO_SET   <<  1 ) | \
         ( CYG_GPIO_SET );
  cyg_uint8 forward_s2 = ( CYG_GPIO_RESET <<  7 ) | \
         ( CYG_GPIO_RESET <<  6 ) | \
         ( CYG_GPIO_SET   <<  5 ) | \
         ( CYG_GPIO_SET   <<  4 ) | \
         ( CYG_GPIO_RESET <<  3 ) | \
         ( CYG_GPIO_RESET <<  2 ) | \
         ( CYG_GPIO_SET   <<  1 ) | \
         ( CYG_GPIO_SET );
  cyg_uint8 sequence_s1;
  cyg_uint8 sequence_s2;
  cyg_uint8 steps = NUM_SEQUENCE_PER_STEP;

  // Set IOs
  while ( steps-- )
  {
    if ( direction != CYG_DEVS_STEP_MOTOR_DIR_BACKWARD )
    {
      sequence_s1 = SM_GET_SEQUENCE_FORWARD( forward_s1 );
      sequence_s2 = SM_GET_SEQUENCE_FORWARD( forward_s2 );
    }
    else
    {
      sequence_s1 = SM_GET_SEQUENCE_BACKWARD( backward_s1 );
      sequence_s2 = SM_GET_SEQUENCE_BACKWARD( backward_s2 );
    }
    motor_single_step( info , sequence_s1 );
    hal_delay_us( info->step_delay );
    motor_single_step( info , sequence_s2 );
    hal_delay_us( info->step_delay );
  }

}


//--------------------------------------------------------------------------
static Cyg_ErrNo
step_motor_write( cyg_io_handle_t handle, const void *_buf, cyg_uint32 *len )
{
  cyg_devtab_entry_t *t       = (cyg_devtab_entry_t *)handle;
  step_motor_info  *info      = (step_motor_info *)t->priv;
  cyg_step_motor_data_t *data = (cyg_step_motor_data_t *)_buf;
  cyg_uint32 steps = 0;

  // the user need to provide a can message buffer
  if (*len != sizeof(cyg_step_motor_data_t))
  {
    return -EINVAL;
  }

  // Compute delay between each steps
  if ( info->user_info.mode == CYG_DEVS_STEP_MOTOR_FULL_STEP )
    info->step_delay = ( 60 * 1e7 ) / ( data->speed * info->steps * NUM_SEQUENCE_PER_STEP );
  else if( info->user_info.mode == CYG_DEVS_STEP_MOTOR_HALF_STEP )
    info->step_delay = ( 60 * 1e7 ) / ( data->speed * info->steps * (NUM_SEQUENCE_PER_STEP >> 1) );
  else
     return -EINVAL;

  // Move ..
  while(steps++ < data->steps)
  {
     info->motor_move( info , data->direction );
  }

  cyg_hal_gpio_write( info->sig_a,     CYG_GPIO_RESET );
  cyg_hal_gpio_write( info->sig_b,     CYG_GPIO_RESET );
  cyg_hal_gpio_write( info->sig_a_bar, CYG_GPIO_RESET );
  cyg_hal_gpio_write( info->sig_b_bar, CYG_GPIO_RESET );

  return ENOERR;
}

//---------------------------------------------------------------------------
//
