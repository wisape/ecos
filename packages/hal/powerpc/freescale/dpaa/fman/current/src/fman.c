//==========================================================================
//
//      fman.c
//
//      HAL support for Freescale Frame Manager
//
//==========================================================================
// ####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 2013 Free Software Foundation, Inc.
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
// Date:         2013-01-20
// Purpose:
// Description:  Freescale Frame Manager device driver
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/hal_powerpc_freescale_dpaa_fman.h>

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/infra/cyg_ass.h>          // CYG_FAIL

#include <cyg/hal/hal_endian.h>
#include <cyg/hal/hal_intr.h>

#ifdef CYGPKG_KERNEL
#include <cyg/kernel/kapi.h>
#endif

#include <string.h>                    // for memset

#include "cyg/io/fman_fw.h"
#include "cyg/io/fman.h"
#include "cyg/io/fman_port.h"

#include "cyg/crc/crc.h"

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN_CHATTER
#define fman_printf(args...)   diag_printf(args)
#else
#define fman_printf(args...)   /* NOOP */
#endif

#define fman_err_log(args...)  diag_printf(args)

#include CYGDAT_HAL_POWERPC_FREESCALE_DPAA_FMAN_INL
#include CYGDAT_HAL_POWERPC_FREESCALE_DPAA_FMAN_IMEM_INL


//
// Load Frame Manager microcodes
//
cyg_uint32
fman_load_microcode( struct fman_t *fman )
{
   volatile cyg_uint32 *pmicrocode = fman->pmicrocode;
   cyg_uint32 crc32, bytes_size;
   cyg_uint32 len = FMAN_GET_FIRMWARE_H_LEN(pmicrocode);
   cyg_uint8  *pmicrocode_base = (cyg_uint8 *) pmicrocode;
   cyg_uint8  i;
   cyg_uint32 *base;
   cyg_uint32 j;

   //
   // Debug print
   fman_printf("Loading microcode from %p\n", pmicrocode);
   fman_printf("  %s\n",
        (char *) FMAN_GET_FIRMWARE_H_ID(pmicrocode));
   fman_printf("  Length      :%d\n",
        FMAN_GET_FIRMWARE_H_LEN(pmicrocode));
   fman_printf("  Version     :%d\n",
        FMAN_GET_FIRMWARE_H_VERSION(pmicrocode));
   fman_printf("  Count       :%d\n",
        FMAN_GET_FIRMWARE_H_COUNT(pmicrocode));
   fman_printf("  SOC model   :%d\n",
        FMAN_GET_FIRMWARE_H_SOC_MODEL(pmicrocode));
   fman_printf("  SOC version :%d.%d\n",
        FMAN_GET_FIRMWARE_H_VER_MAJOR(pmicrocode),
        FMAN_GET_FIRMWARE_H_VER_MIN(pmicrocode));

   //
   // Verify CRC32
   if(len > 0){
     crc32 = cyg_crc32((unsigned char*) pmicrocode, len-sizeof(cyg_uint32));
     if( crc32 !=  FMAN_GET_FIRMWARE_CRC32(pmicrocode) )
       return FMAN_ERR_UC_CRC32;
   }
   else
     return FMAN_ERR_UC_LEN;

   //
   // Load microcodes
   for (i = 0; i < FMAN_GET_FIRMWARE_H_COUNT(pmicrocode); i++) {
     bytes_size = FMAN_GET_MICROCODE_H_COUNT(pmicrocode, i) << 2;
     base       = (cyg_uint32 *)
                  (pmicrocode_base + FMAN_GET_MICROCODE_H_OFFSET(pmicrocode, i));

     fman_printf("Microcode base: %p, size: %d, offset %d\n",
        base,
        bytes_size,
        FMAN_GET_MICROCODE_H_OFFSET(pmicrocode, i));

     // Set auto-increment enable
     fman->ccd->fmcd_addr = FMAN_FMCD_ADDR_AIE;
     HAL_MEMORY_BARRIER();

     // Write microcode
     for (j = 0; j < (bytes_size >> 2); j++){
        fman->ccd->fmcd_data = base[j];
     }

     // Padding aligned on 16 bytes
     while( ( j % 4 ) !=0 ){
        fman->ccd->fmcd_data = 0xffffffff;
        j++;
     }
     HAL_MEMORY_BARRIER();

    // Allow access by the FMan
    fman->ccd->fmcd_ready = FMAN_FMCD_READY_RDY;
   }

   return FMAN_OK;
}

//
// Get Frame Manager Interface reference
//
externC struct fman_t *
fman_get_ptr( cyg_uint8 if_num )
{
#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN1
  if( if_num == FMAN1_INDEX )
    return &fman1;
#endif

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN2
  if( if_num == FMAN2_INDEX )
    return &fman2;
#endif

  return NULL;
}

//
// Perform the Buffer Manager Interface Initialization
//
static cyg_uint32
fman_bmi_init(struct fman_t *fman, struct fman_bmi_cfg *cfg){
  cyg_uint32 fbps, fbpo, pool_base;
  cyg_int16 tdma CYGBLD_ATTRIB_UNUSED;
  cyg_int16 tntsks CYGBLD_ATTRIB_UNUSED;
  cyg_uint8 p = 0, i;

  //
  // Allocate and set the Free Buffer Pool size and offset value
  pool_base =
        fman_imem_alloc( &fman->imem_pool, cfg->pool_size, FMAN_FMBM_BUFFER_ALIGN);

  if( pool_base == 0 ){
    fman_err_log("FMAN%d, enable to allocate memory!\n", fman->id);
    return FMAN_ERR_NOMEM;
  }

  fbps = cfg->pool_size / FMAN_FMBM_BUFFER_SIZE;
  fbpo = ( pool_base - (cyg_uint32) fman->imem ) / FMAN_FMBM_BUFFER_SIZE;
  fman_printf("FMAN%d, buffer pool based @0x%x (%d / %d)\n",
                                          fman->id, pool_base, fbps, fbpo);
  fman->bmi_com->fmbm_cfg1 =
    FMAN_FMBM_CFG1_SET_FBPS((fbps-1)) | FMAN_FMBM_CFG1_FBPO(fbpo);

  //
  // It is recommended not to modify FMBM_CFG2, but it is required
  // to make sure the total maximum outstanding DMA transfer per
  // channel does not exceed the maximum configured in FMBM_CFG2
  tdma   = (cyg_int16) FMAN_FMBM_CFG2_TDMA(fman->bmi_com->fmbm_cfg2);
  tntsks = (cyg_int16) FMAN_FMBM_CFG2_GET_TNTSKS(fman->bmi_com->fmbm_cfg2);
  fman_printf("FMAN%d, Max outstanding DMA: %d, Max tasks: %d\n",
                                          fman->id, tdma, tntsks);

  //
  // Set priorities per ports (by default, set lowest priority
  // to all ports )
  for (i=0; i<9; i++){
    fman->bmi_com->fmbm_arb[i] = 0;
  }

  //
  // Set allocation of tasks, DMA slots and FIFOs per port
  // The sum of MXD on all ports should not exceed the size
  // of the FMBM_CFG2[TDMA].
  // EXT field is not valid (treat as reserved) for ports
  // other than RX ports FMAN_FMBM_PP_SET_MXD
  for (p=FMAN_OFFLINE_HOST_1; p<=FMAN_OFFLINE_HOST_7; p++){
    // Max task = 2, 1 Max DMA transfer, no extra as recommended
    fman->bmi_com->fmbm_pp[p-1]  =
       FMAN_FMBM_PP_SET_MXT(2) | FMAN_FMBM_PP_SET_MXD(1);
    fman->bmi_com->fmbm_pfs[p-1] = FMAN_FMBM_PFS_IFSZ(8);
  }
  for (p=FMAN_ETH1_1G_RX; p<=FMAN_ETHx_1G_RX_MAX; p++){
    // Max task = 4, 1 Max DMA transfer, no extra as recommended
    fman->bmi_com->fmbm_pp[p-1]  =
       FMAN_FMBM_PP_SET_MXT(4) | FMAN_FMBM_PP_SET_MXD(1);
    fman->bmi_com->fmbm_pfs[p-1] = FMAN_FMBM_PFS_IFSZ(44);
  }
  for (p=FMAN_ETH1_1G_TX; p<=FMAN_ETHx_1G_TX_MAX; p++){
    // Max task = 4, 1 Max DMA transfer, no extra as recommended
    fman->bmi_com->fmbm_pp[p-1]  =
       FMAN_FMBM_PP_SET_MXT(4) | FMAN_FMBM_PP_SET_MXD(1);
    fman->bmi_com->fmbm_pfs[p-1] = FMAN_FMBM_PFS_IFSZ(44);
  }
  for (p=FMAN_ETH1_10G_ETH9_1G_RX; p<=FMAN_ETHx_10G_RX_MAX; p++){
    // Max task = 10, 8 Max DMA transfer, no extra as recommended
    fman->bmi_com->fmbm_pp[p-1]  =
       FMAN_FMBM_PP_SET_MXT(10) | FMAN_FMBM_PP_SET_MXD(8);
    fman->bmi_com->fmbm_pfs[p-1] = FMAN_FMBM_PFS_IFSZ(48);
  }
  for (p=FMAN_ETH1_10G_ETH9_1G_TX; p<=FMAN_ETHx_10G_TX_MAX; p++){
    // Max task = 14, 12 Max DMA transfer, no extra as recommended
    fman->bmi_com->fmbm_pp[p-1]  =
       FMAN_FMBM_PP_SET_MXT(14) | FMAN_FMBM_PP_SET_MXD(12);
    fman->bmi_com->fmbm_pfs[p-1] = FMAN_FMBM_PFS_IFSZ(48);
  }

  // Set interrupt enable
  fman->bmi_com->fmbm_ier = 0;

  fman->bmi_com->fmbm_ievr = ( FMAN_FMBM_IxR_PEC | FMAN_FMBM_IxR_LEC |
                               FMAN_FMBM_IxR_SEC | FMAN_FMBM_IxR_DEC );

  // Initialize BMI linked list
  fman->bmi_com->fmbm_init = FMAN_FMBM_INIT_STR;

  HAL_MEMORY_BARRIER();

  return FMAN_OK;
}

//
// Perform the Queue Manager Interface Initialization
//
static cyg_uint32
fman_qmi_init(volatile struct fman_qmi_com* qmi_com, struct fman_qmi_cfg *cfg){
  // Not documented?
  qmi_com->fmqm_gc &= ~(FMAN_FMQM_GC_ENQ_EN | FMAN_FMQM_GC_DEQ_EN);

  // Enable QMI statistics
  qmi_com->fmqm_gc |= FMAN_FMQM_GC_STEN;

  // Disable / Clear interrupt
  qmi_com->fmqm_eien = 0x0;
  qmi_com->fmqm_eie |= (FMAN_FMQM_EIE_DEE | FMAN_FMQM_EIE_DFUPE);
#ifndef CYGHWR_HAL_POWERPC_QORIQ_P1023
  qmi_com->fmqm_ien  = 0x0;              // N/A for P1023
  qmi_com->fmqm_ie  |= FMAN_FMQM_IE_SEE; // N/A for P1023
#endif

  HAL_MEMORY_BARRIER();

  return FMAN_OK;
}

//
// Perform the Frame Processing Manager Initialization
//
static cyg_uint32
fman_fpm_init(volatile struct fman_fpm *fpm, struct fman_fpm_cfg *cfg){
  cyg_uint8 i = 0;
  cyg_uint8 fman_num = 0;

  //
  // User must associate only one FMan controller to the Tx/Rx ports
  // while in independent mode. This is done by setting FMFP_PRC[CPA]
  // (FPM Port ID Control) so that each even port ID corresponds
  // to FMan controller #1 and each odd port ID corresponds to FMan
  // controller #2.
  for(i = FMAN_OFFLINE_HOST_1; i<= FMAN_ETH1_10G_ETH10_1G_TX;i++) {
    fman_num = (i & 0x1) + 1;
    fpm->fmfp_prc = FMAN_FMFP_PRC_CPA(fman_num) |
                    FMAN_FMFP_PRC_SET_ORA(fman_num) |
                    FMAN_FMFP_PRC_SET_PORID(i);
  }

  fpm->fmfp_ee  = ( FMAN_FMFP_EE_DER  | FMAN_FMFP_EE_CER | FMAN_FMFP_EE_UEC |
                    FMAN_FMFP_EE_EHM );

  // Disable dispatch limit
  fpm->fmfp_mxd = 0;

  // Set dispatch threshold
  //fpm->fmfp_dist1 =
  //fpm->fmfp_dist2 =

  // Clear and enable error interrupt
  fpm->fmfp_ee |= ( FMAN_FMFP_EE_SECC | FMAN_FMFP_EE_STL | FMAN_FMFP_EE_DECC );
  for (i = 0; i < FMAN_NUM_OF_EVENT_REG; i++) {
    fpm->fmfp_cev[i] = 0xffffffff;
    fpm->fmfp_cee[i] = 0x00000000;
  }

#ifndef CYGHWR_HAL_POWERPC_QORIQ_P1023
  //fpm->fmfp_ien
#endif

  // Configure timestamp
  //fpm->fmfp_tsc1 =
  //fpm->fmfp_tsc2 =

  // Set the mode for the FMAN memory
  //fpm->fm_rcr

  return FMAN_OK;
}

//
// Perform the DMA Initialization
//
static cyg_uint32
fman_dma_init(volatile struct fman_dma *dma, struct fman_dma_cfg *cfg,
                                                       cyg_uint8 fman_num){
  cyg_uint32 fmdm_sr = dma->fmdm_sr;
  cyg_uint32 i       = 0;

  // Clear status register
  fmdm_sr &= FMAN_FMDM_SR_STATUS_W1C_MASK;
  dma->fmdm_sr = fmdm_sr;

  for (i = 0; i < FM_MAX_PORTS/2; i++) {
    dma->fmdm_plrx[i]= FMAN_FMDM_PLR_EVEN_PORT_LIODN_BASE((16 << fman_num)) |
                       FMAN_FMDM_PLR_ODD_PORT_LIODN_BASE((16 << fman_num));
  }

  return FMAN_OK;
}

//
// Frame Manager Initialization
//
externC cyg_uint32
fman_init( cyg_uint8 fman_num )
{
  struct fman_t *fman;
  volatile cyg_uint32 timeout = 10000, ret;

  fman_printf("\nFMAN%d - Enter initialization\n", fman_num);
  fman_printf("=============================\n");

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN port");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return FMAN_ERR_NOREF;

  //
  // Do not proceed if the Frame Manager is already up
  // and running
  if( fman->state != FMAN_INIT_PENDING )
    return FMAN_ERR_INIT_FAILED;

  //
  // Sanity check
  if (FMAN_IMGP_RAM_SIZE != sizeof(struct fman_imgp_ram)) {
    fman_err_log("FMAN init failed, sizeof fman_imgp_ram %d\n",
                                  sizeof(struct fman_imgp_ram));
    return FMAN_ERR_INIT_FAILED;
  }

  //
  // Note: The user must disable ECC detection in FMan by setting
  // FM_RCR[FEE]=0 and FM_RCR[SFE]=0 before setting this bit
  fman->fpm->fm_rstc |= FMAN_FM_RSTC_RFM;

  hal_delay_us(1);

  while( (fman->fpm->fm_rstc & FMAN_FM_RSTC_RFM) && --timeout );

  if (!timeout)
    return FMAN_ERR_RESET_FAILED;

  //
  // Load microcode
  if( fman_load_microcode( fman ) != FMAN_OK ) {
    fman_err_log("Error loading FMAN microcode\n");
    return FMAN_ERR_INIT_FAILED;
  }

  //
  // Clear FMAN internal memory
  fman_imem_init(&fman->imem_pool, 1);

  //
  // Setup Queue Manager Interface
  if((ret = fman_qmi_init( fman->qmi_com, &fman->cfg->qmi )))
    return ret;

  //
  // Setup Frame Processing Unit
  if((ret = fman_fpm_init( fman->fpm, &fman->cfg->fpm )))
    return ret;

  //
  // Setup DMA
  if((ret = fman_dma_init( fman->dma, &fman->cfg->dma , fman_num )))
    return ret;

  //
  // Setup Buffer Manager Interface
  if((ret = fman_bmi_init( fman, &fman->cfg->bmi )))
    return ret;

  fman->state = FMAN_INIT_COMPLETED;

  return FMAN_OK;
}

//
// Constructor entry point
//
externC void
fman_init_all( void ){
#if defined(CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN1) || \
    defined(CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN2)
  cyg_uint32 init_ret;
#endif

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN1
  init_ret = fman_init( FMAN1_INDEX );
  if(init_ret != FMAN_OK) {
    fman_err_log("FMAN%d Initialization failed with error %d\n",
       FMAN1_INDEX, init_ret);
  }
#endif
#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN2
  init_ret= fman_init( FMAN2_INDEX );
  if(init_ret != FMAN_OK) {
    fman_err_log("FMAN%d Initialization failed with error %d\n",
       FMAN2_INDEX, init_ret);
  }
#endif
}

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN_DEBUG_CODE

externC void
fman_disp_registers( cyg_uint8 fman_num )
{
  struct fman_t *fman;
  cyg_uint8 i = 0;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return;

  diag_printf("FMAN%d - Registers\n",
                 fman_num);
  diag_printf("------------------\n");

  diag_printf("FMFP_CEE\n");
  for(i=0; i<FMAN_NUM_OF_EVENT_REG; i++)
    diag_printf("  [%d]    : %08x\n",
                 i, fman->fpm->fmfp_cee[i]);
  diag_printf("FMFP_CEV\n");
  for(i=0; i<FMAN_NUM_OF_EVENT_REG; i++)
    diag_printf("  [%d]    : %08x\n",
                 i, fman->fpm->fmfp_cev[i]);
  diag_printf("FMFP_FCEV\n");
  for(i=0; i<FMAN_NUM_OF_EVENT_REG; i++)
    diag_printf("  [%d]    : %08x\n",
                 i, fman->fpm->fmfp_fcev[i]);

  return;
}

#endif // CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN_DEBUG_CODE

//
// TODO: To be investigated, not very efficient to do FMAN lookup for each
// interrupt...

externC void
fman_clear_event( cyg_uint8 fman_num, cyg_uint8 reg_num, cyg_uint32 event)
{
  struct fman_t *fman;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");


  CYG_ASSERT(reg_num > 3, "Invalid FMAN CEE register");
 
  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return;

  fman->fpm->fmfp_cev[reg_num] = event;

  return;
}

//
// TODO: To be investigated, not very efficient to do FMAN lookup for each
// interrupt...

externC cyg_uint32
fman_get_event( cyg_uint8 fman_num, cyg_uint8 reg_num )
{
  struct fman_t *fman;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");


  CYG_ASSERT(reg_num > 3, "Invalid FMAN CEE register");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return 0;

  return fman->fpm->fmfp_cev[reg_num];

}

externC void
fman_umask_event( cyg_uint8 fman_num, cyg_uint8 reg_num, cyg_uint32 event)
{
  struct fman_t *fman;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");


  CYG_ASSERT(reg_num > 3, "Invalid FMAN CEE register");

  //
  // Retrieve reference to the FMAN interface
  if ( ( fman = fman_get_ptr( fman_num ) ) == NULL )
    return;

  fman->fpm->fmfp_cee[reg_num] = event;

  return;
}

externC cyg_int8
fman_allocate_event( cyg_uint8 fman_num )
{
  static cyg_uint8 fmfp_event_idx[2] = {0, 0};
  cyg_int8 ret = -1;
  cyg_uint32 istate;

  CYG_ASSERT((fman_num =! FMAN1_INDEX || fman_num =! FMAN2_INDEX), \
                                "Invalid FMAN controller");

  HAL_DISABLE_INTERRUPTS(istate);

  if ( fmfp_event_idx[fman_num-1] < 3 ) {
      ret = fmfp_event_idx[fman_num-1];
      fmfp_event_idx[fman_num-1]++;
  }

  HAL_RESTORE_INTERRUPTS(istate);

  return ret;
}
