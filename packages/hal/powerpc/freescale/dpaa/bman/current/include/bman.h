#ifndef CYGONCE_HAL_POWERPC_FREESCALE_DPAA_BMAN_H
#define CYGONCE_HAL_POWERPC_FREESCALE_DPAA_BMAN_H
//==========================================================================
//
//      bman.h
//
//      HAL support for Freescale DPAA Buffer Manager
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
// Description:  Freescale DPAA Buffer Manager
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P1023)
# define BMAN_NUM_SWP                3 // Software portals
# define BMAN_NUM_DCP                3 // Direct connect portals
# define BMAN_NUM_POOL               3 // Separate pools of free buffers
# define CYGARC_REG_IMM_DPAA_BMAN    0x08A000
#else
# define BMAN_NUM_SWP                10
# define BMAN_NUM_DCP                4
# define BMAN_NUM_POOL               64
# define CYGARC_REG_IMM_DPAA_BMAN    0x31A000
#endif

#define BMAN_MAX_POOL                64

// BMan Configuration and Control Register Memory Map
struct bman_cr_reg
{
  cyg_uint32 bm_poolx_swdet[BMAN_MAX_POOL];
  cyg_uint32 bm_poolx_hwdet[BMAN_MAX_POOL];
  cyg_uint32 bm_poolx_swdxt[BMAN_MAX_POOL];
  cyg_uint32 bm_poolx_hwdxt[BMAN_MAX_POOL];
  cyg_uint32 bm_poolx_sdcnt[BMAN_MAX_POOL];
  cyg_uint32 bm_poolx_hdcnt[BMAN_MAX_POOL];
  cyg_uint32 bm_poolx_content[BMAN_MAX_POOL];
  cyg_uint32 bm_poolx_hdptr[BMAN_MAX_POOL];
  cyg_uint32 fbpr_fpc;
  cyg_uint32 fbpr_fp_lwit;
  cyg_uint32 fbpr_hdptr;
  cyg_uint32 _reserved0[61];
  cyg_uint32 bm_cmd_pmx_cfg[8];
  cyg_uint32 bm_fl_pmx_cfg[8];
  cyg_uint32 _reserved1[48];
  cyg_uint32 bm_ecsr;               // N/A for P1023
  cyg_uint32 bm_ecir;
  cyg_uint32 bm_eadr;               // N/A for P1023
  cyg_uint32 _reserved2;
  cyg_uint32 bm_edatax[8];          // N/A for P1023
  cyg_uint32 bm_sbet;
  cyg_uint32 bm_efcr;               // N/A for P1023
  cyg_uint32 bm_efar;               // N/A for P1023
  cyg_uint32 bm_cecr;               // N/A for P1023
  cyg_uint32 bm_cear;               // N/A for P1023
  cyg_uint32 bm_aecr;               // N/A for P1023
  cyg_uint32 bm_aear;               // N/A for P1023
  cyg_uint32 _reserved3[13];
  cyg_uint32 bm_sbec0;
  cyg_uint32 bm_sbec1;              // N/A for P1023
  cyg_uint32 _reserved4[92];
  cyg_uint32 bm_ip_rev_1;
  cyg_uint32 bm_ip_rev_2;
  cyg_uint32 fbpr_bare;
  cyg_uint32 fbpr_bar;
  cyg_uint32 _reserved5[2];
  cyg_uint32 fbpr_ar;
  cyg_uint32 _reserved6[60];
  cyg_uint32 bm_srcidr;
  cyg_uint32 bm_liodnr;
  cyg_uint32 _reserved7[61];
  cyg_uint32 bm_err_isr;
  cyg_uint32 bm_err_ier;
  cyg_uint32 bm_err_isdr;
  cyg_uint32 bm_err_iir;
  cyg_uint32 bm_err_ifr;
};

// BMAN_POOLn_SWDET, BMAN_POOLn_SWDXT, BMAN_POOLn_HWDET, BMAN_POOLn_HWDXT
#define BMAN_POOLn_THR_COEF_MASK                 0x000000ff
#define BMAN_POOLn_THR_EXP_MASK                  0x00000f00
#define BMAN_POOLn_THR_COEF(_x)                 ((_x & BMAN_POOLn_THR_COEF_MASK))
#define BMAN_POOLn_THR_SET_EXP(_x)              ((_x << 8) & BMAN_POOLn_THR_EXP_MASK)
#define BMAN_POOLn_THR_GET_EXP(_x)              ((_x & BMAN_POOLn_THR_EXP_MASK) >> 8)
// BMAN_POOLn_SDCNT, MAN_POOLn_HDCNT
#define BMAN_POOLn_xDCNT_MASK                    0x000000ff
#define BMAN_POOLn_xDCNT(_x)                    ((_x & BMAN_POOLn_xDCNT_MASK))
// FBPR_FP_LWIT
#define BMAN_FBPR_FP_LWIT_TH_MASK                0x0fffffff
#define BMAN_FBPR_FP_LWIT_TH(_x)                ((_x & BMAN_FBPR_FP_LWIT_TH_MASK))
// BMAN_POOLn_HDPTR, FBPR_HDPTR
#define BMAN_POOLn_FBPR_HDPTR_MASK               0x0fffffff
#define BMAN_POOLn_FBPR_HDPTR(_x)               ((_x & BMAN_POOLn_FBPR_HDPTR_MASK))
// CMD_PMn_CFG
#define BMAN_CMD_PMn_CFG_PRE_MASK                0x0000003f
#define BMAN_CMD_PMn_CFG_PRS_MASK                0x00000fc0
#define BMAN_CMD_PMn_CFG_CFIFO_MASK              0x01f00000
#define BMAN_CMD_PMn_CFG_CBTH_MASK               0x0e000000
#define BMAN_CMD_PMn_CFG_CTTH_MASK               0xf0000000
#define BMAN_CMD_PMn_CFG_PRE(_x)                ((_x & BMAN_CMD_PMn_CFG_PRE_MASK))
#define BMAN_CMD_PMn_CFG_SET_PRS(_x)            ((_x << 6) & BMAN_CMD_PMn_CFG_PRS_MASK)
#define BMAN_CMD_PMn_CFG_GET_PRS(_x)            ((_x & BMAN_CMD_PMn_CFG_PRS_MASK) >> 6)
#define BMAN_CMD_PMn_CFG_CF                      0x00080000
#define BMAN_CMD_PMn_CFG_SET_CFIFO(_x)          ((_x << 20) & BMAN_CMD_PMn_CFG_CFIFO_MASK)
#define BMAN_CMD_PMn_CFG_GET_CFIFO(_x)          ((_x & BMAN_CMD_PMn_CFG_CFIFO_MASK) >> 20)
#define BMAN_CMD_PMn_CFG_SET_CBTH(_x)           ((_x << 25) &  BMAN_CMD_PMn_CFG_CBTH_MASK)
#define BMAN_CMD_PMn_CFG_GET_CBTH(_x)           ((_x & BMAN_CMD_PMn_CFG_CBTH_MASK)  >> 25)
#define BMAN_CMD_PMn_CFG_SET_CTTH(_x)           ((_x << 28) &  BMAN_CMD_PMn_CFG_CTTH_MASK)
#define BMAN_CMD_PMn_CFG_GET_CTTH(_x)           ((_x & BMAN_CMD_PMn_CFG_CTTH_MASK)  >> 28)
// BMAN_FL_PMn_CFG
#define BMAN_FL_PMn_CFG_PRE_MASK                 0x0000003f
#define BMAN_FL_PMn_CFG_PRS_MASK                 0x00000fc0
#define BMAN_FL_PMn_CFG_PRE(_x)                 ((_x & BMAN_FL_PMn_CFG_PRE_MASK))
#define BMAN_FL_PMn_CFG_SET_PRS(_x)             ((_x << 6) & BMAN_FL_PMn_CFG_PRS_MASK)
#define BMAN_FL_PMn_CFG_GET_PRS(_x)             ((_x & BMAN_FL_PMn_CFG_PRS_MASK) >> 6)
// BMAN_ERR_ISR, BMAN_ERR_IER, BMAN_ERR_IFR
#define BMAN_ERR_IxR_EMAI                        0x00000040  // P1023
#define BMAN_ERR_IxR_EMCI                        0x00000020  // P1023
#define BMAN_ERR_IxR_IVCI                        0x00000010
#define BMAN_ERR_IxR_FLWI                        0x00000008
#define BMAN_ERR_IxR_MBEI                        0x00000004
#define BMAN_ERR_IxR_SBEI                        0x00000002
#define BMAN_ERR_IxR_BSCN                        0x00000001
// BMAN_ERR_IIR
#define BMAN_ERR_IIR                             0x00000001
// BMAN_SBET
#define BMAN_SBET_SBET_MASK                      0x000000ff
#define BMAN_SBET_SBET(_x)                      ((_x & BMAN_SBET_SBET_MASK))
#if defined(CYGHWR_HAL_POWERPC_QORIQ_P1023)
# define BMAN_SBET_SET_ECD_MASK                  0x80000000
# define BMAN_SBET_SET_ECD(_x)                  ((_x << 31) & BMAN_SBET_SET_ECD_MASK)
# define BMAN_SBET_GET_ECD(_x)                  ((_x & BMAN_SBET_SET_ECD_MASK) >> 31)
#else
# define BMAN_SBET_SET_ECD_MASK                  0xc0000000
# define BMAN_SBET_SET_ECD(_x)                  ((_x << 30) & BMAN_SBET_SET_ECD_MASK)
# define BMAN_SBET_GET_ECD(_x)                  ((_x & BMAN_SBET_SET_ECD_MASK) >> 30)
#endif
// BMAN_SBEC0-1
#define BMAN_SBECX_MASK                          0x000000ff
#define BMAN_SBECX(_x)                          ((_x & 0x000000ff))
// BMAN_ECSR
#define BMAN_ECSR_ME                             0x80000000
#define BMAN_ECSR_IVCI                           0x00000010
#define BMAN_ECSR_MBEI                           0x00000004
#define BMAN_ECSR_SBEI                           0x00000002
// BMAN_ECIR
#define BMAN_ECIR_PID_MASK                       0x0000003f
#define BMAN_ECIR_NUMB_MASK                      0x00000f00
#define BMAN_ECIR_PORTAL_MASK                    0x0f000000
#define BMAN_ECIR_PID(_x)                       ((_x & BMAN_ECIR_PID_MASK))
#define BMAN_ECIR_SET_NUMB(_x)                  ((_x <<  8) &   BMAN_ECIR_NUMB_MASK)
#define BMAN_ECIR_SET_PORTAL(_x)                ((_x << 24) & BMAN_ECIR_PORTAL_MASK)
#define BMAN_ECIR_GET_PORTAL(_x)                ((_x & BMAN_ECIR_PORTAL_MASK) >> 24)
#define BMAN_ECIR_GET_NUMB(_x)                  ((_x & BMAN_ECIR_NUMB_MASK)   >>  8)
// BMAN_EADR
#define BMAN_EADR_EADR_MASK                      0x000003ff
#define BMAN_ECIR_MEM_ID_MASK                    0x07000000
#define BMAN_EADR_EADR(_x)                      ((_x & BMAN_EADR_EADR_MASK))
#define BMAN_ECIR_SET_MEM_ID(_x)                ((_x << 24) & BMAN_ECIR_MEM_ID_MASK)
#define BMAN_ECIR_GET_MEM_ID(_x)                ((_x & BMAN_ECIR_MEM_ID_MASK) >> 24)
// BMAN_EFCR
#define BMAN_EFCR_LID_MASK                       0x0000007f
#define BMAN_EFCR_LID(_x)                       ((_x & BMAN_EFCR_LID_MASK))
#define BMAN_EFCR_M                              0x00000080
#define BMAN_EFCR_E                              0x00000100
#define BMAN_EFCR_C                              0x00000200
// BMAN_EFAR
#define BMAN_EFAR_ADDR_MASK                      0x0fffffff
#define BMAN_EFAR_ADDR(_x)                      ((_x & BMAN_EFAR_ADDR_MASK))
// BMAN_CECR
#define BMAN_CECR_LID_MASK                       0x000000ff
#define BMAN_CECR_LID(_x)                       ((_x & BMAN_CECR_LID_MASK))
// BMAN_CEAR
#define BMAN_CEAR_ADDR_MASK                      0x0fffffff
#define BMAN_CEAR_ADDR(_x)                      ((_x & BMAN_CEAR_ADDR_MASK))
// BMAN_AECR
#define BMAN_AECR_ADDR_MASK                      0x0fffffff
#define BMAN_AECR_ADDR(_x)                      ((_x & BMAN_AECR_ADDR_MASK))
// BMAN_AEAR
#define BMAN_AEAR_LID_MASK                       0x000000ff
#define BMAN_AEAR_LID(_x)                       ((_x & BMAN_AEAR_LID_MASK))

//7.3.3.1.1 Writing a Command into the CR
//To write a command into the CR, software must perform the following tasks:
//1. dcbz
//2. Write all words of the command other than word 0.
//3. lwsync
//4. Write word 0 (containing the command verb and alternating valid bit).
//5. dcbf to flush the command from cache to the BMan

#endif
