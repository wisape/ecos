#ifndef CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_H
#define CYGONCE_HAL_POWERPC_FREESCALE_DPAA_FMAN_H
//==========================================================================
//
//      fman.h
//
//      HAL support for Freescale DPAA Frame Manager
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
// Description:  Freescale DPAA Frame Manager
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#define FM_MAX_PORTS                            64

#define FMAN_FIRMWARE_H_LEN                                                   \
                       31
#define FMAN_GET_FIRMWARE_H_LEN(_p)                                           \
                    (  CYG_BE32_TO_CPU(_p[0]) )
#define FMAN_GET_FIRMWARE_H_VERSION(_p)                                       \
                    (  CYG_BE32_TO_CPU(_p[1]) & 0x000000FF )
#define FMAN_GET_FIRMWARE_H_ID(_p)                                            \
                    ( &_p[2] )
#define FMAN_GET_FIRMWARE_H_COUNT(_p)                                         \
                    (  CYG_BE32_TO_CPU(_p[17]) & 0x000000FF )
#define FMAN_GET_FIRMWARE_H_SOC_MODEL(_p)                                     \
                    ( (CYG_BE32_TO_CPU(_p[18]) & 0xFFFF0000 ) >> 16 )
#define FMAN_GET_FIRMWARE_H_VER_MAJOR(_p)                                     \
                    ( (CYG_BE32_TO_CPU(_p[18]) & 0x0000FF00 ) >> 8 )
#define FMAN_GET_FIRMWARE_H_VER_MIN(_p)                                       \
                    (  CYG_BE32_TO_CPU(_p[18]) & 0x000000FF )
#define FMAN_GET_FIRMWARE_CRC32(_p)                                           \
  ( CYG_BE32_TO_CPU(_p[(FMAN_GET_FIRMWARE_H_LEN(_p)-sizeof(cyg_uint32))>>2] ) )

#define FMAN_MICROCODE_H_LEN                                                  \
                       30
#define FMAN_GET_MICROCODE_H_COUNT(_p, _i)                                    \
  ( CYG_BE32_TO_CPU(_p[FMAN_FIRMWARE_H_LEN+_i*FMAN_MICROCODE_H_LEN+26]) )
#define FMAN_GET_MICROCODE_H_OFFSET(_p, _i)                                  \
  ( CYG_BE32_TO_CPU(_p[FMAN_FIRMWARE_H_LEN+_i*FMAN_MICROCODE_H_LEN+27]) )

// FMAN internal
#define FMAN_OK                                 0
#define FMAN_ERR_UC_LEN                         1
#define FMAN_ERR_UC_CRC32                       2
#define FMAN_ERR_RESET_FAILED                   3
#define FMAN_ERR_INIT_FAILED                    4
#define FMAN_ERR_NOREF                          5
#define FMAN_ERR_NOMEM                          6
#define FMAN_ERR_PORT_BUSY                      7
#define FMAN_ERR_WRONG_TYPE                     8


// RX/TX queue descriptor
struct fman_qd {
  cyg_uint16 ctrl;               // control / status field (N/A for TX)
  cyg_uint16 ring_base_hi;       // Ring base address (36 bits)
  cyg_uint32 ring_base_lo;
  cyg_uint16 ring_size;          // Size in bytes
  cyg_uint16 offset_in;          // Offset from the BD base to the next BD
  cyg_uint16 offset_out;         // Offset to the next BD to be fetched by the host
  cyg_uint8  _reserved1[0x12];
};

#define FMAN_QD_RX_FRAME_IM                     0x0010 // RxF Interrupt mask
#define FMAN_QD_BUSY_IM                         0x0008 // RxBD busy Interrupt mask
#define FMAN_QD_BUSY_FPMEVT_SEL_MASK            0x0003 // FPM Event 0-3 select
#define FMAN_QD_BUSY_SET_FPMEVT_SEL(_x)         (_x & FMAN_QD_BUSY_FPMEVT_SEL_MASK)

// RX/TX buffer descriptor
struct fman_bd {
  cyg_uint16  ctrl;
  cyg_uint16  length;
  cyg_uint16  _reserved1[0x3];
  cyg_uint16  buf_hi;
  cyg_uint32  buf_lo;
};


// RX BD control / status flag
#define FMAN_BD_Rx_Empty                        0x8000  // Buffer is empty
#define FMAN_BD_Rx_Last                         0x0800  // Last buffer in frame
#define FMAN_BD_Rx_First                        0x0400
#define FMAN_BD_Rx_Phy_Err                      0x0008
#define FMAN_BD_Rx_Size_Err                     0x0004

// TX BD control / status flag
#define FMAN_BD_Tx_Ready                        0x8000  // Frame ready
#define FMAN_BD_Tx_Last                         0x0800  // Last buffer in frame


// Independent Mode Global Parameter RAM
// Independent mode global parameter RAM size should be 256 bytes
// and should be 256 byte aligned.

struct fman_imgp_ram {
  cyg_uint32  imr;
  cyg_uint32  rx_qd_ptr;
  cyg_uint32  tx_qd_ptr;
  cyg_uint16  mrblr;
  cyg_uint16  rx_qd_bsy_cnt;
  cyg_uint8  _reserved1[0x10];
  struct fman_qd rx_qd;
  struct fman_qd tx_qd;
  cyg_uint8  _reserved2[0xA0];
};

// Independent Mode Register Field Descriptions
#define FMAN_IMGP_IMR_GBL                       0x20000000
#define FMAN_IMGP_IMR_GRACEFUL_STOP             0x00800000

#define PRAM_MODE_GLOBAL
#define PRAM_MODE_GRACEFUL_STOP

#define FMAN_IMGP_RAM_SIZE                      256
#define FMAN_IMGP_PRAM_ALIGN                    FMAN_IMGP_RAM_SIZE

// Hardware PortIDs
enum fman_hw_port {
  FMAN_OFFLINE_HOST_1 = 0x01,
  FMAN_OFFLINE_HOST_2,
  FMAN_OFFLINE_HOST_3,
  FMAN_OFFLINE_HOST_4,
  FMAN_OFFLINE_HOST_5,
  FMAN_OFFLINE_HOST_6,
  FMAN_OFFLINE_HOST_7,
  FMAN_ETH1_1G_RX,
  FMAN_ETH2_1G_RX,
  FMAN_ETH3_1G_RX,
  FMAN_ETH4_1G_RX,
  FMAN_ETH5_1G_RX,
  FMAN_ETH6_1G_RX,
  FMAN_ETH7_1G_RX,
  FMAN_ETH8_1G_RX,
  FMAN_ETH1_10G_ETH9_1G_RX,
  FMAN_ETH1_10G_ETH10_1G_RX,
  FMAN_HW_PORT_RES1,
  FMAN_HW_PORT_RES2,
  FMAN_HW_PORT_RES3,
  FMAN_HW_PORT_RES4,
  FMAN_HW_PORT_RES5,
  FMAN_HW_PORT_RES6,
  FMAN_HW_PORT_RES7,
  FMAN_HW_PORT_RES8,
  FMAN_HW_PORT_RES9,
  FMAN_HW_PORT_RES10,
  FMAN_HW_PORT_RES11,
  FMAN_HW_PORT_RES12,
  FMAN_HW_PORT_RES13,
  FMAN_HW_PORT_RES14,
  FMAN_HW_PORT_RES15,
  FMAN_HW_PORT_RES16,
  FMAN_HW_PORT_RES17,
  FMAN_HW_PORT_RES18,
  FMAN_HW_PORT_RES19,
  FMAN_HW_PORT_RES20,
  FMAN_HW_PORT_RES21,
  FMAN_HW_PORT_RES22,
  FMAN_ETH1_1G_TX,
  FMAN_ETH2_1G_TX,
  FMAN_ETH3_1G_TX,
  FMAN_ETH4_1G_TX,
  FMAN_ETH5_1G_TX,
  FMAN_ETH6_1G_TX,
  FMAN_ETH7_1G_TX,
  FMAN_ETH8_1G_TX,
  FMAN_ETH1_10G_ETH9_1G_TX,
  FMAN_ETH1_10G_ETH10_1G_TX,
};

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P4080) || \
    defined(CYGHWR_HAL_POWERPC_QORIQ_P4040)
# define FMAN_ETHx_1G_TX_MAX FMAN_ETH4_1G_TX
# define FMAN_ETHx_1G_RX_MAX FMAN_ETH4_1G_RX
#elif defined(CYGHWR_HAL_POWERPC_QORIQ_P3040) || \
      defined(CYGHWR_HAL_POWERPC_QORIQ_P3041) || \
      defined(CYGHWR_HAL_POWERPC_QORIQ_P2041) || \
      defined(CYGHWR_HAL_POWERPC_QORIQ_P2040)
# define FMAN_ETHx_1G_TX_MAX FMAN_ETH5_1G_TX
# define FMAN_ETHx_1G_RX_MAX FMAN_ETH5_1G_RX
#elif defined(CYGHWR_HAL_POWERPC_QORIQ_P1023)
# define FMAN_ETHx_1G_TX_MAX FMAN_ETH2_1G_TX
# define FMAN_ETHx_1G_RX_MAX FMAN_ETH2_1G_RX
#endif

#define FMAN_ETHx_10G_TX_MAX FMAN_ETH1_10G_ETH9_1G_TX
#define FMAN_ETHx_10G_RX_MAX FMAN_ETH1_10G_ETH9_1G_RX

typedef enum {
    FMAN_TX_PORT = 0x01,
    FMAN_RX_PORT,
    FMAN_OFFLINE_PORT,
} fman_port_t;

typedef enum {
    FMAN_CNT_RST = 0x01,
    FMAN_CNT_EN,
    FMAN_CNT_DIS,
} fman_cnt_action_t;

//
// FMAN register mapping
#if defined(CYGHWR_HAL_POWERPC_QORIQ_P2040) ||   \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P2041) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P3040) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P3041) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P4040) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P4080)
# define CYGARC_REG_IMM_DPAA_FMAN1               0x400000
# define CYGNUM_FREESCALE_DPAA_FMAN_MEM_SIZE     0x028000  // 160KB
# define CYGNUM_FMAN_BMI_MAX_NUM_OF_TASKS        128
# define CYGNUM_FMAN_BMI_MAX_NUM_OUTS_DMA        32
#else
# define CYGARC_REG_IMM_DPAA_FMAN1               0x010000  // 64KB
# define CYGNUM_FREESCALE_DPAA_FMAN_MEM_SIZE
# define CYGNUM_FMAN_BMI_MAX_NUM_OF_TASKS        64
# define CYGNUM_FMAN_BMI_MAX_NUM_OUTS_DMA        16
#endif

#if defined(CYGHWR_HAL_POWERPC_QORIQ_P4040) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P4080) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P5020) || \
        defined(CYGHWR_HAL_POWERPC_QORIQ_P5040)
# define CYGARC_REG_IMM_DPAA_FMAN2               0x500000
#endif

#define FMAN1_INDEX                              1
#define FMAN2_INDEX                              2

#define CYGARC_REG_IMM_DPAA_FMAN_MEM             0x000000
#define CYGARC_REG_IMM_DPAA_FMAN_HW_PORT_MEM     0x080000
#define CYGARC_REG_IMM_DPAA_FMAN_POLICER         0x0C0000
#define CYGARC_REG_IMM_DPAA_FMAN_KEYGEN          0x0C1000
#define CYGARC_REG_IMM_DPAA_FMAN_DMA             0x0C2000
#define CYGARC_REG_IMM_DPAA_FMAN_FPM             0x0C3000
#define CYGARC_REG_IMM_DPAA_FMAN_CCD             0x0C4000
#define CYGARC_REG_IMM_DPAA_FMAN_SOFT_PARSER     0x0C7000
#if defined(CYGHWR_HAL_POWERPC_QORIQ_P1023)
# define CYGARC_REG_IMM_DPAA_FMAN_MACSEC         0x0D8000
#endif
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC1          0x0E0000
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC2          0x0E2000
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC3          0x0E4000
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC4          0x0E6000
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC5          0x0E8000
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC6          0x0EA000
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC7          0x0EC000
#define CYGARC_REG_IMM_DPAA_FMAN_DTSEC8          0x0EE000
#define CYGARC_REG_IMM_DPAA_FMAN_10GEC           0x0F0000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO1           0x0E1000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO2           0x0E3000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO3           0x0E5000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO4           0x0E7000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO5           0x0E9000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO6           0x0EB000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO7           0x0ED000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO8           0x0EF000
#define CYGARC_REG_IMM_DPAA_FMAN_MDIO9           0x0F1000
#define CYGARC_REG_IMM_DPAA_FMAN_IEEE_1588       0x0FE000

// FMan Hardware Port Page Memory Map
#define CYGARC_REG_IMM_DPAA_FMAN_BMI_COM         0x0000
#define CYGARC_REG_IMM_DPAA_FMAN_QMI_COM         0x0400
#define CYGARC_REG_IMM_DPAA_FMAN_BMI_(_p)        (0x1000*_p)
#define CYGARC_REG_IMM_DPAA_FMAN_QMI_(_p)        (0x1000*_p+0x0400)

#define CYGARC_REG_IMM_DPAA_FMAN_KEYGEN_GSR      0x24

// Frame Queue Descriptor (FQD) Context A
struct fman_fqd {
  cyg_uint32  ctxta;
  cyg_uint32  _reserved1;
};

#define FMAN_FQD_CTXTA_OVFQ                      0x80000000
#define FMAN_FQD_CTXTA_ICMD                      0x40000000
#define FMAN_FQD_CTXTA_A1V                       0x20000000

// TDD Context A page 631

#define FMAN_FQD_CTXTB_FQID                      0x00000FFF

//
// Frame descriptor
struct fman_fd {
  cyg_uint32    pid_addrh;                // BPID, address high etc..
  cyg_uint32    addrl;                    // Address Low
  cyg_uint32    form_offs_length;         // Format Offset and length
  cyg_uint32    cmd_status;               // Command / Status
};

//
// Frame descriptor fields definition
#define FMAN_FD_DD_MASK                          0xc0000000
#define FMAN_FD_PID_MASK                         0x3f000000
#define FMAN_FD_ELIODN_MASK                      0x0000f000
#define FMAN_FD_BPID_MASK                        0x00ff0000
#define FMAN_FD_FORMAT_MASK                      0xe0000000
#define FMAN_FD_OFFSET_MASK                      0x1ff00000
#define FMAN_FD_LENGTH_MASK                      0x000fffff
#define FMAN_FD_ADDRH_MASK                       0x000000ff

#define FMAN_FD_LENGTH(_x)                      (_x & FMAN_FD_LENGTH_MASK)
#define FMAN_FD_ADDRH(_x)                       (_x & FMAN_FD_ADDRH_MASK)

#define FMAN_FD_SET_DD(_x)                      ((_x << 30) & FMAN_FD_DD_MASK     )
#define FMAN_FD_SET_PID(_x)                     ((_x << 24) & FMAN_FD_PID_MASK    )
#define FMAN_FD_SET_ELIODN(_x)                  ((_x << 12) & FMAN_FD_ELIODN_MASK )
#define FMAN_FD_SET_BPID(_x)                    ((_x << 16) & FMAN_FD_BPID_MASK   )
#define FMAN_FD_SET_FORMAT(_x)                  ((_x << 29) & FMAN_FD_FORMAT_MASK )
#define FMAN_FD_SET_OFFSET(_x)                  ((_x << 20) & FMAN_FD_OFFSET_MASK )

#define FMAN_FD_GET_DD(_x)                      ((_x & FMAN_FD_DD_MASK     ) >> 30)
#define FMAN_FD_GET_PID(_x)                     ((_x & FMAN_FD_PID_MASK    ) >> 24)
#define FMAN_FD_GET_ELIODN(_x)                  ((_x & FMAN_FD_ELIODN_MASK ) >> 12)
#define FMAN_FD_GET_BPID(_x)                    ((_x & FMAN_FD_BPID_MASK   ) >> 16)
#define FMAN_FD_GET_FORMAT(_x)                  ((_x & FMAN_FD_FORMAT_MASK ) >> 29)
#define FMAN_FD_GET_OFFSET(_x)                  ((_x & FMAN_FD_OFFSET_MASK ) >> 20)

// The DPAA UG defines other types but they are not supported by FMAN, only
// by the SEC, PME etc..
typedef enum FD_FORMAT {
    FD_FORMAT_SHORT_SBUF  = 0x0,           // Short single buffer simple frame
    FD_FORMAT_SHORT_MBUF  = 0x4            // Short multi buffer simple frame
} FD_FORMAT;

//
// Frame descriptor, RX status field
#define FMAN_FD_RX_STATUS_DCL4C                  0x10000000 // L4 (IP/TCP/UDP) Checksum
#define FMAN_FD_RX_STATUS_DME                    0x01000000 // DMA error
#define FMAN_FD_RX_STATUS_MS                     0x00800000 // P1023: MACSEC Frame
#define FMAN_FD_RX_STATUS_FPE                    0x00080000 // Frame Physical Err
#define FMAN_FD_RX_STATUS_FSE                    0x00040000 // Frame Size Err
#define FMAN_FD_RX_STATUS_DIS                    0x00020000 // Discard
#define FMAN_FD_RX_STATUS_EOF                    0x00008000 // Extract Out of Frame Err
#define FMAN_FD_RX_STATUS_NSS                    0x00004000 // No Scheme Selection
#define FMAN_FD_RX_STATUS_KSO                    0x00002000 // Key Size Over flow Err
#define FMAN_FD_RX_STATUS_FCL_GREEN              0x00000000 // Frame color
#define FMAN_FD_RX_STATUS_FCL_YELLOW             0x00000400
#define FMAN_FD_RX_STATUS_FCL_RED                0x00000800
#define FMAN_FD_RX_STATUS_FCL_REJ                0x00000C00
#define FMAN_FD_RX_STATUS_IPP                    0x00000200 // Illegal Policer Profile
#define FMAN_FD_RX_STATUS_FLM                    0x00000100 // Frame Length Mismatch
#define FMAN_FD_RX_STATUS_PTE                    0x00000080 // Parser Time-out
#define FMAN_FD_RX_STATUS_ISP                    0x00000040 // Invalid Soft Parser instruction
#define FMAN_FD_RX_STATUS_PHE                    0x00000020 // Header Error
#define FMAN_FD_RX_STATUS_FRDR                   0x00000010 // Frame drop
// BLE Does not exist in P4080 Rev1
#define FMAN_FD_STATUS_BLE                       0x00000008 // Block limit is exceeded
#define FMAN_FD_STATUS_L4CV                      0x00000004 // L4 Checksum Validation

//
// Frame descriptor, TX command field
#define FMAN_FD_TX_CMD_FCO                       0x80000000 // Frame queue Context Override
#define FMAN_FD_TX_CMD_RPD                       0x40000000 // Read Prepended Data
#define FMAN_FD_TX_CMD_UPD                       0x20000000 // Update Prepended Data
#define FMAN_FD_TX_CMD_DTC                       0x10000000 // Do IP/TCP/UDP Checksum
// Confirmation Frame Queue
#define FMAN_FD_TX_CMD_CFQ_MASK                  0x00ffffff
#define FMAN_FD_TX_CMD_CFQ(_x)                  (_x & FMAN_FD_TX_CMD_CFQ_MASK)

//
// Frame descriptor, TX status field
#define FMAN_FD_TX_STATUS_UFD                    0x04000000 // Unsupported FD format
#define FMAN_FD_TX_STATUS_LGE                    0x02000000 // Length error
#define FMAN_FD_TX_STATUS_DME                    0x01000000 // DMA Error
#define FMAN_FD_TX_STATUS_FCO                    FMAN_FD_TX_CMD_FCO
#define FMAN_FD_TX_STATUS_RPD                    FMAN_FD_TX_CMD_RPD
#define FMAN_FD_TX_STATUS_UPD                    FMAN_FD_TX_CMD_UPD
#define FMAN_FD_TX_STATUS_DTC                    FMAN_FD_TX_CMD_DTC

//
// Frame descriptor, offline command field
#define FMAN_FD_OFL_CMD_RPD                      0x40000000
#define FMAN_FD_OFL_CMD_UPD                      0x20000000
#define FMAN_FD_OFL_CMD_DCL4C                    0x10000000
#define FMAN_FD_OFL_CMD_UFD                      0x04000000
#define FMAN_FD_OFL_CMD_LGE                      0x02000000
#define FMAN_FD_OFL_CMD_DIS                      0x00020000
#define FMAN_FD_OFL_CMD_EOF                      0x00008000
#define FMAN_FD_OFL_CMD_NSS                      0x00004000
#define FMAN_FD_OFL_CMD_KSO                      0x00002000
#define FMAN_FD_OFL_CMD_FCL_MASK                 0x00000c00
#define FMAN_FD_OFL_CMD_IPP                      0x00000200
#define FMAN_FD_OFL_CMD_FLM                      0x00000100
#define FMAN_FD_OFL_CMD_PTE                      0x00000080
#define FMAN_FD_OFL_CMD_ISP                      0x00000040
#define FMAN_FD_OFL_CMD_PHE                      0x00000020
#define FMAN_FD_OFL_CMD_FRDR                     0x00000010
#define FMAN_FD_OFL_CMD_BLE                      0x00000008
#define FMAN_FD_OFL_CMD_L4CV                     0x00000004

//
// Frame descriptor, offline status field
#define FMAN_FD_OFL_STATUS_RPD                   FMAN_FD_OFL_CMD_RPD
#define FMAN_FD_OFL_STATUS_DCL4C                 FMAN_FD_OFL_CMD_DCL4C
#define FMAN_FD_OFL_STATUS_UFD                   FMAN_FD_OFL_CMD_UFD
#define FMAN_FD_OFL_STATUS_LGE                   FMAN_FD_OFL_CMD_LGE
#define FMAN_FD_OFL_STATUS_DME                   0x01000000
#define FMAN_FD_OFL_STATUS_CRE                   0x00200000
#define FMAN_FD_OFL_STATUS_CHE                   0x00100000
#define FMAN_FD_OFL_STATUS_DIS                   FMAN_FD_OFL_CMD_DIS
#define FMAN_FD_OFL_STATUS_EOF                   FMAN_FD_OFL_CMD_EOF
#define FMAN_FD_OFL_STATUS_NSS                   FMAN_FD_OFL_CMD_NSS
#define FMAN_FD_OFL_STATUS_KSO                   FMAN_FD_OFL_CMD_KSO
#define FMAN_FD_OFL_STATUS_FCL_MASK              FMAN_FD_OFL_CMD_FCL_MASK
#define FMAN_FD_OFL_STATUS_IPP                   FMAN_FD_OFL_CMD_IPP
#define FMAN_FD_OFL_STATUS_FLM                   FMAN_FD_OFL_CMD_FLM
#define FMAN_FD_OFL_STATUS_PTE                   FMAN_FD_OFL_CMD_PTE
#define FMAN_FD_OFL_STATUS_ISP                   FMAN_FD_OFL_CMD_ISP
#define FMAN_FD_OFL_STATUS_PHE                   FMAN_FD_OFL_CMD_PHE
#define FMAN_FD_OFL_STATUS_FRDR                  FMAN_FD_OFL_CMD_FRDR
#define FMAN_FD_OFL_CMD_FRDR                     0x00000010

//
// Frame descriptor, Host Command/Status Word
#define FMAN_FD_HOST_CMD_FCO                     0x80000000
#define FMAN_FD_HOST_CMD_CFQ_MASK                0x00ffffff
#define FMAN_FD_HOST_CMD_CFQ(_x)                (_x & HOST)

#define FMAN_FD_HOST_STATUS_FCO                  FMAN_FD_HOST_CMD_FCO
#define FMAN_FD_HOST_STATUS_UFD                  0x04000000
#define FMAN_FD_HOST_STATUS_LGE                  0x02000000
#define FMAN_FD_HOST_STATUS_DME                  0x01000000
//
// Scatter / gather for multi-buffer Frame
struct fman_sg {
  cyg_uint32    addrh;
  cyg_uint32    addrl;
  cyg_uint32    length;
  cyg_uint32    bpid;
};

#define FMAN_SG_MAX_LIST_SIZE                    16

#define FMAN_SG_E_BIT                            0x80000000 // Extension bit
#define FMAN_SG_F_BIT                            0x40000000 // Final bit
#define FMAN_SG_LENGTH(_x)                      (_x & 0x3fffffff)
#define FMAN_SG_OFFSET(_x)                      (_x & 0x00001fff)
#define FMAN_SG_SET_BPID(_x)                    ((_x & 0x000000ff) << 16)
#define FMAN_SG_GET_BPID(_x)                    ((_x & 0x00ff0000) >> 16)

#define FMAN_NUM_OF_DBG_FLOW                     3
#define FMAN_NUM_OF_EVENT_REG                    4

// FPM Memory Map
struct fman_fpm
{
  cyg_uint32 fmfp_tnc;                          // For internal use only
  cyg_uint32 fmfp_prc;                          // For internal use only
  cyg_uint32 _reserved0;
  cyg_uint32 fmfp_mxd;
  cyg_uint32 fmfp_dist1;
  cyg_uint32 fmfp_dist2;
  cyg_uint32 fm_epi;
  cyg_uint32 fm_rie;
  cyg_uint32 fmfp_fcev[FMAN_NUM_OF_EVENT_REG];
  cyg_uint32 _reserved1[0x04];
  cyg_uint32 fmfp_cee[FMAN_NUM_OF_EVENT_REG];
  cyg_uint32 _reserved2[0x04];
  cyg_uint32 fmfp_tsc1;
  cyg_uint32 fmfp_tsc2;
  cyg_uint32 fmfp_tsp;
  cyg_uint32 fmfp_tsf;
  cyg_uint32 fm_rcr;
  cyg_uint32 fmfp_extc;
  cyg_uint32 _reserved3[0x02];
  cyg_uint32 fmfp_drd[16];
  cyg_uint32 fmfp_dra;
  cyg_uint32 fm_ip_rev_1;
  cyg_uint32 fm_ip_rev_2;
  cyg_uint32 fm_rstc;
  cyg_uint32 fmfp_cld;
  cyg_uint32 fm_npi;
  cyg_uint32 _reserved4;
  cyg_uint32 fmfp_ee;
  cyg_uint32 fmfp_cev[FMAN_NUM_OF_EVENT_REG];
  cyg_uint32 _reserved5[0x04];
  cyg_uint32 fmfp_ps[0x40];
  cyg_uint32 fmfp_clfabc;
  cyg_uint32 fmfp_clfcc;
  cyg_uint32 fmfp_clfaval;
  cyg_uint32 fmfp_clfbval;
  cyg_uint32 fmfp_clfcval;
  cyg_uint32 fmfp_clfamsk;
  cyg_uint32 fmfp_clfbmsk;
  cyg_uint32 fmfp_clfcmsk;
  cyg_uint32 fmfp_clfamc;
  cyg_uint32 fmfp_clfbmc;
  cyg_uint32 fmfp_clfcmc;
  cyg_uint32 _reserved6[0x75];
  cyg_uint32 fmfp_ts[128];
};

// FMFP_PRC
#define FMAN_FMFP_PRC_CPA_MASK                   0x00000003
#define FMAN_FMFP_PRC_ORA_MASK                   0x00030000
#define FMAN_FMFP_PRC_PORID_MASK                 0x3f000000
#define FMAN_FMFP_PRC_RSP                        0x00800000
#define FMAN_FMFP_PRC_CPA(_x)                   ((_x & FMAN_FMFP_PRC_CPA_MASK))
#define FMAN_FMFP_PRC_SET_ORA(_x)               ((_x << 16) & FMAN_FMFP_PRC_ORA_MASK  )
#define FMAN_FMFP_PRC_SET_PORID(_x)             ((_x << 24) & FMAN_FMFP_PRC_PORID_MASK)
// FMFP_MXD
#define FMAN_FMFP_MXD_DISP_LIM_MASK              0x1f000000
#define FMAN_FMFP_MXD_SET_DISP_LIM(_x)          ((_x << 24) & FMAN_FMFP_MXD_DISP_LIM_MASK )
#define FMAN_FMFP_MXD_GET_DISP_LIM(_x)          ((_x & FMAN_FMFP_MXD_DISP_LIM_MASK) >> 24 )
// FMFP_DIST1
#define FMAN_FMFP_DIST1_BMITHR_MASK              0x000000ff
#define FMAN_FMFP_DIST1_POLTHR_MASK              0x0000ff00
#define FMAN_FMFP_DIST1_KGNTHR_MASK              0x00ff0000
#define FMAN_FMFP_DIST1_PARTHR_MASK              0xff000000
#define FMAN_FMFP_DIST1_BMITHR(_x)              ((_x & FMAN_FMFP_DIST1_BMITHR_MASK))
#define FMAN_FMFP_DIST1_SET_POLTHR(_x)          ((_x << 8)  & FMAN_FMFP_DIST1_POLTHR_MASK)
#define FMAN_FMFP_DIST1_SET_KGNTHR(_x)          ((_x << 16) & FMAN_FMFP_DIST1_KGNTHR_MASK)
#define FMAN_FMFP_DIST1_SET_PARTHR(_x)          ((_x << 24) & FMAN_FMFP_DIST1_PARTHR_MASK)
#define FMAN_FMFP_DIST1_GET_POLTHR(_x)          ((_x & FMAN_FMFP_DIST1_POLTHR_MASK) >> 8 )
#define FMAN_FMFP_DIST1_GET_KGNTHR(_x)          ((_x & FMAN_FMFP_DIST1_KGNTHR_MASK) >> 16)
#define FMAN_FMFP_DIST1_GET_PARTHR(_x)          ((_x & FMAN_FMFP_DIST1_PARTHR_MASK) >> 24)
// FMFP_DIST2
#define FMAN_FMFP_DIST2_QDEQTHR_MASK             0x000000ff
#define FMAN_FMFP_DIST2_FMC2THR_MASK             0x0000ff00
#define FMAN_FMFP_DIST2_FMC1THR_MASK             0x00ff0000
#define FMAN_FMFP_DIST2_QENQTHR_MASK             0xff000000
#define FMAN_FMFP_DIST2_QDEQTHR(_x)             ((_x & FMAN_FMFP_DIST2_QDEQTHR_MASK))
#define FMAN_FMFP_DIST2_SET_FMC2THR(_x)         ((_x << 8)  & FMAN_FMFP_DIST2_FMC2THR_MASK)
#define FMAN_FMFP_DIST2_SET_FMC1THR(_x)         ((_x << 16) & FMAN_FMFP_DIST2_FMC1THR_MASK)
#define FMAN_FMFP_DIST2_SET_QENQTHR(_x)         ((_x << 24) & FMAN_FMFP_DIST2_QENQTHR_MASK)
#define FMAN_FMFP_DIST2_GET_FMC2THR(_x)         ((_x & FMAN_FMFP_DIST2_FMC2THR_MASK) >> 8 )
#define FMAN_FMFP_DIST2_GET_FMC1THR(_x)         ((_x & FMAN_FMFP_DIST2_FMC1THR_MASK) >> 16)
#define FMAN_FMFP_DIST2_GET_QENQTHR(_x)         ((_x & FMAN_FMFP_DIST2_QENQTHR_MASK) >> 24)
// FM_EPI
#define FMAN_FM_EPI_FPM_ERR                      0x80000000
#define FMAN_FM_EPI_BMI_ERR                      0x00800000
#define FMAN_FM_EPI_QMI_ERR                      0x00400000
#define FMAN_FM_EPI_PRSR_ERR                     0x00200000
#define FMAN_FM_EPI_KGN_ERR                      0x00100000
#define FMAN_FM_EPI_PLCR_ERR                     0x00080000
#define FMAN_FM_EPI_MUR_ERR                      0x00040000
#define FMAN_FM_EPI_IRM_E_ERR                    0x00020000
#define FMAN_FM_EPI_DMA_ERR                      0x00010000
#define FMAN_FM_EPI_P10G1_ERR                    0x00008000
#define FMAN_FM_EPI_P1G0_ERR                     0x00004000
#define FMAN_FM_EPI_P1G1_ERR                     0x00002000
#define FMAN_FM_EPI_P1G2_ERR                     0x00001000
#define FMAN_FM_EPI_P1G3_ERR                     0x00000800
#define FMAN_FM_EPI_P1G4_ERR                     0x00000400
#define FMAN_FM_EPI_P1G5_ERR                     0x00000200
#define FMAN_FM_EPI_P1G6_ERR                     0x00000100
#define FMAN_FM_EPI_P1G7_ERR                     0x00000080
#define FMAN_FM_EPI_MCSC_ERR                     0x00000001
// FM_RIE
#define FMAN_FM_RIE_IRM_ERR                      0x00040000
#define FMAN_FM_RIE_MUR_ERR                      0x00020000
// FMFP_TSC1
#define FMAN_FMFP_TSC1_TEN                       0x80000000
#define FMAN_FMFP_TSC1_TSPV_MASK                 0x000000ff
#define FMAN_FMFP_TSC1_TSPV(_x)                 ((_x & FMAN_FMFP_TSC1_TSPV_MASK))
// FMFP_TSC2
#define FMAN_FMFP_TSC2_TSIV_FRAC_MASK            0x0000ffff
#define FMAN_FMFP_TSC2_TSIV_INT_MASK             0x00ff0000
#define FMAN_FMFP_TSC2_TSIV_FRAC(_x)            ((_x & FMAN_FMFP_TSC2_TSIV_FRAC_MASK))
#define FMAN_FMFP_TSC2_SET_TSIV_INT(_x)         ((_x << 16) & FMAN_FMFP_TSC2_TSIV_INT_MASK)
#define FMAN_FMFP_TSC2_GET_TSIV_INT(_x)         ((_x & FMAN_FMFP_TSC2_TSIV_INT_MASK) >> 16)
// FMFP_TSF
#define FMAN_FMFP_TSF_GET_TS_FRACT(_x)          ((_x & 0xffff0000) >> 16)
// FM_RCR
#define FMAN_RCR_FEE                             0x80000000
#define FMAN_RCR_IEE                             0x40000000
#define FMAN_RCR_MET                             0x20000000
#define FMAN_RCR_IET                             0x10000000
#define FMAN_RCR_SFE                             0x08000000
#define FMAN_RCR_MDEC                            0x00008000
#define FMAN_RCR_IDEC                            0x00004000
// FMFP_EXTC
#define FMAN_FMFP_EXTC_INVx(_x)                 (0x80000000 >> _x)
// FMFP_DRA
#define FMAN_FMFP_DRA_TNUM_MASK                  0xff000000
#define FMAN_FMFP_DRA_SET_TNUM(_x)              ((_x << 24) & FMAN_FMFP_DRA_TNUM_MASK)
#define FMAN_FMFP_DRA_GET_TNUM(_x)              ((_x & FMAN_FMFP_DRA_TNUM_MASK) >> 24)
// FMAN_IP_REV_1
#define FMAN_IP_REV_1_GET_IP_MN(_x)             ((_x & 0x000000ff))
#define FMAN_IP_REV_1_GET_IP_MJ(_x)             ((_x & 0x0000ff00) >> 8)
#define FMAN_IP_REV_1_GET_IP_ID(_x)             ((_x & 0xffff0000) >> 16)
// FM_RSTC
#define FMAN_FM_RSTC_MRD                         0x00008000
#define FMAN_FM_RSTC_RFM                         0x80000000
// FM_NPI
#define FMAN_FM_NPI_QMI                          0x40000000
#define FMAN_FM_NPI_PRSR                         0x20000000
#define FMAN_FM_NPI_PLCR                         0x08000000
#define FMAN_FM_NPI_TMR                          0x01000000
#define FMAN_FM_NPI_P1G0T                        0x00080000
#define FMAN_FM_NPI_P1G1T                        0x00040000
#define FMAN_FM_NPI_P1G2T                        0x00020000
#define FMAN_FM_NPI_P1G3T                        0x00010000
#define FMAN_FM_NPI_FCEVT0                       0x00008000
#define FMAN_FM_NPI_FCEVT1                       0x00004000
#define FMAN_FM_NPI_FCEVT2                       0x00002000
#define FMAN_FM_NPI_FCEVT3                       0x00001000
#define FMAN_FM_NPI_MCS                          0x00000200
#define FMAN_FM_NPI_P1G4T                        0x00000040
// FMFP_EE
#define FMAN_FMFP_EE_DER                         0x00000001
#define FMAN_FMFP_EE_CER                         0x00000002
#define FMAN_FMFP_EE_UEC                         0x00000004
#define FMAN_FMFP_EE_EHM                         0x00000008
#define FMAN_FMFP_EE_SECC_EN                     0x00002000
#define FMAN_FMFP_EE_STL_EN                      0x00004000
#define FMAN_FMFP_EE_DECC_EN                     0x00008000
#define FMAN_FMFP_EE_RFM                         0x00010000
#define FMAN_FMFP_EE_SECC                        0x20000000
#define FMAN_FMFP_EE_STL                         0x40000000
#define FMAN_FMFP_EE_DECC                        0x80000000
// FMFP_PSn
#define FMAN_FMFP_PSn_ORA                        0x00010000
#define FMAN_FMFP_PSn_STL                        0x00800000
#define FMAN_FMFP_PSn_CPA_MASK                   0xc0000000
#define FMAN_FMFP_PSn_SET_CPA(_x)               ((_x << 30) & FMAN_FMFP_PSn_CPA_MASK)
#define FMAN_FMFP_PSn_GET_CPA(_x)               ((_x & FMAN_FMFP_PSn_CPA_MASK) >> 30)
// FMFP_CLFABC
#define FMAN_FMFP_CLFABC_FSEL_B(_x)             ((_x & 0x0000001f))
#define FMAN_FMFP_CLFABC_AND_B                   0x00001000
#define FMAN_FMFP_CLFABC_AND_A                   0x10000000
#define FMAN_FMFP_CLFABC_CMPOP_A                 0xe0000000
#define FMAN_FMFP_CLFABC_FSEL_A                  0x001f0000
#define FMAN_FMFP_CLFABC_CMPOP_B                 0x0000e000
#define FMAN_FMFP_CLFABC_SET_FSEL_A(_x)         ((_x << 16) &  FMAN_FMFP_CLFABC_FSEL_A)
#define FMAN_FMFP_CLFABC_SET_CMPOP_A(_x)        ((_x << 29) & FMAN_FMFP_CLFABC_CMPOP_A)
#define FMAN_FMFP_CLFABC_SET_CMPOP_B(_x)        ((_x << 13) & FMAN_FMFP_CLFABC_CMPOP_B)
#define FMAN_FMFP_CLFABC_GET_FSEL_A(_x)         ((_x & FMAN_FMFP_CLFABC_FSEL_A)  >> 16)
#define FMAN_FMFP_CLFABC_GET_CMPOP_B(_x)        ((_x & FMAN_FMFP_CLFABC_CMPOP_B) >> 13)
#define FMAN_FMFP_CLFABC_GET_CMPOP_A(_x)        ((_x & FMAN_FMFP_CLFABC_CMPOP_A) >> 29)
// FMFP_CLFCC
#define FMAN_FMFP_CLFCC_FSEL_C_MASK              0x001f0000
#define FMAN_FMFP_CLFCC_CMPOP_C_MASK             0xe0000000
#define FMAN_FMFP_CLFCC_SET_FSEL_C(_x)          ((_x << 16) &  FMAN_FMFP_CLFCC_FSEL_C_MASK)
#define FMAN_FMFP_CLFCC_SET_CMPOP_C(_x)         ((_x << 29) & FMAN_FMFP_CLFCC_CMPOP_C_MASK)
#define FMAN_FMFP_CLFCC_GET_FSEL_C(_x)          ((_x & FMAN_FMFP_CLFCC_FSEL_C_MASK)  >> 16)
#define FMAN_FMFP_CLFCC_GET_CMPOP_C(_x)         ((_x & FMAN_FMFP_CLFCC_CMPOP_C_MASK) >> 29)
// FMFP_TSn
#define FMAN_FMFP_TSn_STL                        0x02000000
#define FMAN_FMFP_TSn_PRK                        0x01000000
#define FMAN_FMFP_TSn_INEX                       0x00800000
#define FMAN_FMFP_TSn_EXEC                       0x00400000
#define FMAN_FMFP_TSn_EOT                        0x00200000
#define FMAN_FMFP_TSn_SYNC                       0x00100000
#define FMAN_FMFP_TSn_ORD                        0x00080000
#define FMAN_FMFP_TSn_NIG                        0x00040000
#define FMAN_FMFP_TSn_DER                        0x00020000
#define FMAN_FMFP_TSn_ORR                        0x00010000
#define FMAN_FMFP_TSn_DISPC(_x)                 ((_x & 0x0000001f))
#define FMAN_FMFP_TSn_ODC(_x)                   ((_x & 0x00001f00) >> 8)
#define FMAN_FMFP_TSn_CTA(_x)                   ((_x & 0xc0000000) >> 30)

// BMI Common Registers
struct fman_bmi_com
{
  cyg_uint32 fmbm_init;
  cyg_uint32 fmbm_cfg1;
  cyg_uint32 fmbm_cfg2;
  cyg_uint32 _reserved1[0x05];
  cyg_uint32 fmbm_ievr;
  cyg_uint32 fmbm_ier;
  cyg_uint32 fmbm_ifr;
  cyg_uint32 _reserved2[0x05];
  cyg_uint32 fmbm_arb[8];
  cyg_uint32 _reserved3[0x0C];
  cyg_uint32 fmbm_dtc[FMAN_NUM_OF_DBG_FLOW];
  cyg_uint32 _reserved4;
  cyg_uint32 fmbm_dcv[FMAN_NUM_OF_DBG_FLOW][4];
  cyg_uint32 fmbm_dcm[FMAN_NUM_OF_DBG_FLOW][4];
  cyg_uint32 fmbm_gde;
  cyg_uint32 fmbm_pp[63];
  cyg_uint32 _reserved5;
  cyg_uint32 fmbm_pfs[63];
  cyg_uint32 _reserved6;
  cyg_uint32 fmbm_spliodn[63];
};

// FMBM_INIT
#define FMAN_FMBM_INIT_STR                       0x80000000
// FMBM_CFG1
#define FMAN_FMBM_CFG1_MASK                      0x000007ff
#define FMAN_FMBM_CFG1_FBPO(_x)                 ((_x & FMAN_FMBM_CFG1_MASK))
#define FMAN_FMBM_CFG1_FBPS_MASK                 0x07ff0000
#define FMAN_FMBM_CFG1_SET_FBPS(_x)             ((_x << 16) & FMAN_FMBM_CFG1_FBPS_MASK)
#define FMAN_FMBM_CFG1_GET_FBPS(_x)             ((_x & FMAN_FMBM_CFG1_FBPS_MASK) >> 16)
// FMBM_CFG2
#define FMAN_FMBM_CFG2_TDMA_MASK                 0x0000003f
#define FMAN_FMBM_CFG2_TDMA(_x)                 ((_x & FMAN_FMBM_CFG2_TDMA_MASK))
#define FMAN_FMBM_CFG2_TNTSKS                    0x007f0000
#define FMAN_FMBM_CFG2_SET_TNTSKS(_x)           ((_x << 16) & FMAN_FMBM_CFG2_TNTSKS)
#define FMAN_FMBM_CFG2_GET_TNTSKS(_x)           ((_x & FMAN_FMBM_CFG2_TNTSKS) >> 16)
// FMBM_IEVR, IER, IFR
#define FMAN_FMBM_IxR_PEC                        0x80000000
#define FMAN_FMBM_IxR_LEC                        0x40000000
#define FMAN_FMBM_IxR_SEC                        0x20000000
#define FMAN_FMBM_IxR_DEC                        0x10000000
// FMBM_ARB
#define FMAN_FMBM_ARB_WEIGHTn_MASK(_n)          ((     0x0000000f) << (_n * 4))
#define FMAN_FMBM_ARB_SET_WEIGHTn(_w, _n)       ((_x & 0x0000000f) << (_n * 4))
#define FMAN_FMBM_ARB_GET_WEIGHTn(_w, _n)       ((_x >> (_n * 4)) & 0x0000000f)
// FMBM_PP
#define FMAN_FMBM_PP_EXD_MASK                    0x0000000f
#define FMAN_FMBM_PP_MXD_MASK                    0x00000f00
#define FMAN_FMBM_PP_EXT_MASK                    0x000f0000
#define FMAN_FMBM_PP_MXT_MASK                    0x3f000000
#define FMAN_FMBM_PP_EXD(_x)                    ((_x & FMAN_FMBM_PP_EXD_MASK))
#define FMAN_FMBM_PP_SET_MXD(_x)                ((_x <<  8) & FMAN_FMBM_PP_MXD_MASK)
#define FMAN_FMBM_PP_SET_EXT(_x)                ((_x << 16) & FMAN_FMBM_PP_EXT_MASK)
#define FMAN_FMBM_PP_SET_MXT(_x)                ((_x << 24) & FMAN_FMBM_PP_MXT_MASK)
#define FMAN_FMBM_PP_GET_MXD(_x)                ((_x & FMAN_FMBM_PP_MXD_MASK) >> 8 )
#define FMAN_FMBM_PP_GET_EXT(_x)                ((_x & FMAN_FMBM_PP_EXT_MASK) >> 16)
#define FMAN_FMBM_PP_GET_MXT(_x)                ((_x & FMAN_FMBM_PP_MXT_MASK) >> 24)
// FMBM_PFS
#define FMAN_FMBM_PFS_IFSZ_MASK                  0x000003ff
#define FMAN_FMBM_PFS_IFSZ(_x)                  ((_x & FMAN_FMBM_PFS_IFSZ_MASK))
#define FMAN_FMBM_PFS_EXBS_MASK                  0x03ff0000
#define FMAN_FMBM_PFS_SET_EXBS(_x)              ((_x << 16) & FMAN_FMBM_PFS_EXBS_MASK)
#define FMAN_FMBM_PFS_GET_EXBS(_x)              ((_x & FMAN_FMBM_PFS_EXBS_MASK) >> 16)
// FMBM_SPLIODN
#define FMAN_FMBM_SPLIODN_MASK                   0x000000ff
#define FMAN_FMBM_SPLIODN(_x)                   ((_x & FMAN_FMBM_SPLIODN_MASK))

#define FMAN_FMBM_BUFFER_SIZE                    256
#define FMAN_FMBM_BUFFER_ALIGN                   FMAN_FMBM_BUFFER_SIZE

// In IM mode, the first 256 bytes cannot be used for allocating Independent
// Mode Global Parameter RAM so this block is set has reserved
#define FMAN_FMBM_IMEM_RES_BLOCK                 1

// BMI O/H , RX and TX Port Registers
struct fman_bmi_port
{
  union
  {
    struct
    {
      cyg_uint32 fmbm_ocfg;
      cyg_uint32 fmbm_ost;
      cyg_uint32 fmbm_oda;
      cyg_uint32 fmbm_oicp;
      cyg_uint32 fmbm_ofdne;
      cyg_uint32 fmbm_ofne;
      cyg_uint32 fmbm_ofca;
      cyg_uint32 fmbm_ofpne;
      cyg_uint32 fmbm_opso;
      cyg_uint32 fmbm_opp;
      cyg_uint32 fmbm_occb;
      cyg_uint32 fmbm_oim;
      cyg_uint32 fmbm_ofp;         // N/A for P4080, P2040/P2041, P3041, P5020
      cyg_uint32 _reserved1[0x03];
      cyg_uint32 fmbm_opri;
      cyg_uint32 _reserved2[0x07];
      cyg_uint32 fmbm_ofqid;
      cyg_uint32 fmbm_oefqid;
      cyg_uint32 fmbm_ofsdm;
      cyg_uint32 fmbm_ofsem;
      cyg_uint32 fmbm_ofene;
      cyg_uint32 fmbm_orlmts;
      cyg_uint32 fmbm_orlmt;
      cyg_uint32 _reserved3[0x21];
      cyg_uint32 fmbm_oebmpi;
      cyg_uint32 _reserved4[0x17];
      cyg_uint32 fmbm_ocgm;
      cyg_uint32 _reserved5[0x07];
      cyg_uint32 fmbm_ompd;
      cyg_uint32 _reserved6[0x1F];
      cyg_uint32 fmbm_ostc;
      cyg_uint32 fmbm_ofrc;
      cyg_uint32 fmbm_ofdc;
      cyg_uint32 fmbm_ofledc;
      cyg_uint32 fmbm_ofufdc;
      cyg_uint32 fmbm_offc;
      cyg_uint32 fmbm_ofwdc;
      cyg_uint32 fmbm_ofldec;
      cyg_uint32 fmbm_obdc;
      cyg_uint32 _reserved7[0x17];
      cyg_uint32 fmbm_opc;
      cyg_uint32 fmbm_opcp;
      cyg_uint32 fmbm_occn;
      cyg_uint32 fmbm_otuc;
      cyg_uint32 fmbm_oduc;
      cyg_uint32 fmbm_ofuc;
      cyg_uint32 _reserved8[0x1A];
      cyg_uint32 fmbm_odcfg;
      cyg_uint32 _reserved9[0x3F];
    } oh;

    struct
    {
      cyg_uint32 fmbm_rcfg;
      cyg_uint32 fmbm_rst;
      cyg_uint32 fmbm_rda;
      cyg_uint32 fmbm_rfp;
      cyg_uint32 fmbm_rfed;
      cyg_uint32 fmbm_ricp;
      cyg_uint32 fmbm_rim;
      cyg_uint32 fmbm_rebm;
      cyg_uint32 fmbm_rfne;
      cyg_uint32 fmbm_rfca;
      cyg_uint32 fmbm_rfpne;
      cyg_uint32 fmbm_rpso;
      cyg_uint32 fmbm_rpp;
      cyg_uint32 fmbm_rccb;
      cyg_uint32 fmbm_reth;
      cyg_uint32 _reserved1[0x01];
      cyg_uint32 fmbm_rpri;
      cyg_uint32 _reserved2[0x07];
      cyg_uint32 fmbm_rfqid;
      cyg_uint32 fmbm_refqid;
      cyg_uint32 fmbm_rfsdm;
      cyg_uint32 fmbm_rfsem;
      cyg_uint32 fmbm_rfene;
      cyg_uint32 _reserved3[0x23];
      cyg_uint32 fmbm_rebmpi;
      cyg_uint32 _reserved4[0x03];
      cyg_uint32 fmbm_rebmpi_2;
      cyg_uint32 _reserved5[0x03];
      cyg_uint32 fmbm_racnt;
      cyg_uint32 _reserved6[0x0F];
      cyg_uint32 fmbm_rcgm;
      cyg_uint32 _reserved7[0x07];
      cyg_uint32 fmbm_rmpd;
      cyg_uint32 _reserved8[0x1F];
      cyg_uint32 fmbm_rstc;
      cyg_uint32 fmbm_rfrc;
      cyg_uint32 fmbm_rbfc;
      cyg_uint32 fmbm_rlfc;
      cyg_uint32 fmbm_rffc;
      cyg_uint32 fmbm_rfdc;
      cyg_uint32 fmbm_rfldec;
      cyg_uint32 fmbm_rodc;
      cyg_uint32 fmbm_rbdc;
      cyg_uint32 _reserved9[0x17];
      cyg_uint32 fmbm_rpc;
      cyg_uint32 fmbm_rpcp;
      cyg_uint32 fmbm_rccn;
      cyg_uint32 fmbm_rtuc;
      cyg_uint32 fmbm_rrquc;
      cyg_uint32 fmbm_rduc;
      cyg_uint32 fmbm_rfuc;
      cyg_uint32 fmbm_rpac;
      cyg_uint32 _reserved10[0x18];
      cyg_uint32 fmbm_rdcfg;
      cyg_uint32 _reserved11[0x3F];
    } rx;

    struct
    {
      cyg_uint32 fmbm_tcfg;
      cyg_uint32 fmbm_tst;
      cyg_uint32 fmbm_tda;
      cyg_uint32 fmbm_tfp;
      cyg_uint32 fmbm_tfed;
      cyg_uint32 fmbm_ticp;
      cyg_uint32 fmbm_tfdne;
      cyg_uint32 fmbm_tfca;
      cyg_uint32 fmbm_tcfqid;
      cyg_uint32 fmbm_tefqid;
      cyg_uint32 fmbm_tfene;
      cyg_uint32 fmbm_trlmts;
      cyg_uint32 fmbm_trlmt;
      cyg_uint32 _reserved1[0x73];
      cyg_uint32 fmbm_tstc;
      cyg_uint32 fmbm_tfrc;
      cyg_uint32 fmbm_tfdc;
      cyg_uint32 fmbm_tfledc;
      cyg_uint32 fmbm_tfufdc;
      cyg_uint32 fmbm_tbdc;
      cyg_uint32 _reserved9[0x1A];
      cyg_uint32 fmbm_tpc;
      cyg_uint32 fmbm_tpcp;
      cyg_uint32 fmbm_tccn;
      cyg_uint32 fmbm_ttuc;
      cyg_uint32 fmbm_ttcquc;
      cyg_uint32 fmbm_tduc;
      cyg_uint32 fmbm_tfuc;
      cyg_uint32 _reserved10[0x19];
      cyg_uint32 fmbm_tdcfg;
      cyg_uint32 _reserved11[0x3F];
    } tx;
  } u;
};


//
// Rx Port Register

// FMBM_RCFG
#define FMAN_FMBM_RCFG_EN                        0x80000000
#define FMAN_FMBM_RCFG_FDOVR                     0x02000000
#define FMAN_FMBM_RCFG_IM                        0x01000000
#define FMAN_FMBM_RCFG_MS                        0x00800000
#define FMAN_FMBM_RCFG_MS_SEL_MASK               0x001f0000
#define FMAN_FMBM_RCFG_SET_MS_SEL(_x)           ((_x << 16) & FMAN_FMBM_RCFG_MS_SEL_MASK)
#define FMAN_FMBM_RCFG_GET_MS_SEL(_x)           ((_x & FMAN_FMBM_RCFG_MS_SEL_MASK) >> 16)
// FMBM_RST
#define FMAN_FMBM_RST_BSY                        0x80000000
#define FMAN_FMBM_RST_GET_RPID(_x)              ((_x & 0x003f0000) >> 16)
// FMBM_RDA
#define FMAN_FMBM_RDA_SWAP                       0xc0000000
#define FMAN_FMBM_RDA_ICC                        0x30000000
#define FMAN_FMBM_RDA_FHC                        0x0c000000
#define FMAN_FMBM_RDA_SGC                        0x03000000
#define FMAN_FMBM_RDA_WOPT                       0x00300000
#define FMAN_FMBM_RDA_SET_SWAP(_x)              ((_x << 30) & FMAN_FMBM_RDA_SWAP)
#define FMAN_FMBM_RDA_SET_ICC(_x)               ((_x << 28) & FMAN_FMBM_RDA_ICC )
#define FMAN_FMBM_RDA_SET_FHC(_x)               ((_x << 26) & FMAN_FMBM_RDA_FHC )
#define FMAN_FMBM_RDA_SET_SGC(_x)               ((_x << 24) & FMAN_FMBM_RDA_SGC )
#define FMAN_FMBM_RDA_SET_WOPT(_x)              ((_x << 20) & FMAN_FMBM_RDA_WOPT)
#define FMAN_FMBM_RDA_GET_SWAP(_x)              ((_x & FMAN_FMBM_RDA_SWAP) >> 30)
#define FMAN_FMBM_RDA_GET_ICC(_x)               ((_x & FMAN_FMBM_RDA_ICC)  >> 28)
#define FMAN_FMBM_RDA_GET_FHC(_x)               ((_x & FMAN_FMBM_RDA_FHC)  >> 26)
#define FMAN_FMBM_RDA_GET_SGC(_x)               ((_x & FMAN_FMBM_RDA_SGC)  >> 24)
#define FMAN_FMBM_RDA_GET_WOPT(_x)              ((_x & FMAN_FMBM_RDA_WOPT) >> 20)
// FMBM_RFP
#define FMAN_FMBM_RFP_FTH_MASK                   0x000003ff
#define FMAN_FMBM_RFP_PEL_MASK                   0x03ff0000
#define FMAN_FMBM_RFP_FTH(_x)                   ((_x & FMAN_FMBM_RFP_FTH_MASK))
#define FMAN_FMBM_RFP_SET_PEL(_x)               ((_x << 16) & FMAN_FMBM_RFP_PEL_MASK)
#define FMAN_FMBM_RFP_GET_PEL(_x)               ((_x & FMAN_FMBM_RFP_PEL_MASK) >> 16)
// FMBM_RFED
#define FMAN_FMBM_RFED_CFED_MASK                 0x001f0000
#define FMAN_FMBM_RFED_CSI_MASK                  0x1f000000
#define FMAN_FMBM_RFED_SET_CFED(_x)             ((_x << 16) & FMAN_FMBM_RFED_CFED_MASK)
#define FMAN_FMBM_RFED_SET_CSI(_x)              ((_x << 24) & FMAN_FMBM_RFED_SET_CSI  )
#define FMAN_FMBM_RFED_GET_CFED(_x)             ((_x & FMAN_FMBM_RFED_CFED_MASK) >> 16)
#define FMAN_FMBM_RFED_GET_CSI(_x)              ((_x & FMAN_FMBM_RFED_CSI_MASK)  >> 24)
// FMBM_RICP
#define FMAN_FMBM_RICP_ICSZ_MASK                 0x0000001f
#define FMAN_FMBM_RICP_ICIOF_MASK                0x00000f00
#define FMAN_FMBM_RICP_ICEOF_MASK                0x001f0000
#define FMAN_FMBM_RICP_ICSZ(_x)                 ((_x & 0x0000001f))
#define FMAN_FMBM_RICP_SET_ICIOF(_x)            ((_x <<  8) & FMAN_FMBM_RICP_ICIOF_MASK)
#define FMAN_FMBM_RICP_SET_ICEOF(_x)            ((_x << 16) & FMAN_FMBM_RICP_ICEOF_MASK)
#define FMAN_FMBM_RICP_GET_ICIOF(_x)            ((_x & FMAN_FMBM_RICP_ICIOF_MASK)  >> 8)
#define FMAN_FMBM_RICP_GET_ICEOF(_x)            ((_x & FMAN_FMBM_RICP_ICEOF_MASK) >> 16)
// FMBM_RIM
#define FMAN_FMBM_RIM_FOF_MASK                   0xf0000000
#define FMAN_FMBM_RIM_SET_FOF(_x)               ((_x << 28) & FMAN_FMBM_RIM_FOF_MASK)
#define FMAN_FMBM_RIM_GET_FOF(_x)               ((_x & FMAN_FMBM_RIM_FOF_MASK) >> 28)
// FMBM_REBM
#define FMAN_FMBM_REBM_BEM_MASK                  0x000001ff
#define FMAN_FMBM_REBM_BSM_MASK                  0x01ff0000
#define FMAN_FMBM_REBM_BEM(_x)                  ((_x & FMAN_FMBM_REBM_BEM_MASK))
#define FMAN_FMBM_REBM_SET_BSM(_x)              ((_x << 16) & FMAN_FMBM_REBM_BSM_MASK)
#define FMAN_FMBM_REBM_GET_BSM(_x)              ((_x & FMAN_FMBM_REBM_BSM_MASK) >> 16)
// FMBM_RFNE
#define FMAN_FMBM_RFNE_NIA_MASK                  0x00ffffff
#define FMAN_FMBM_RFNE_NIA(_x)                  ((_x & FMAN_FMBM_RFNE_NIA_MASK))
#define FMAN_FMBM_RFNE_FDCS_MASK                 0xff000000
#define FMAN_FMBM_RFNE_SET_FDCS(_x)             ((_x << 24) & FMAN_FMBM_RFNE_FDCS_MASK)
#define FMAN_FMBM_RFNE_GET_FDCS(_x)             ((_x & FMAN_FMBM_RFNE_FDCS_MASK) >> 24)
// FMBM_RFCA
#define FMAN_FMBM_RFCA_OR                        0x80000000
#define FMAN_FMBM_RFCA_MR_MASK                   0x003f0000
#define FMAN_FMBM_RFCA_SYNC_MASK                 0x03000000
#define FMAN_FMBM_RFCA_COLOR_MASK                0x0c000000
#define FMAN_FMBM_RFCA_SET_MR(_x)               ((_x << 16) & FMAN_FMBM_RFCA_MR_MASK   )
#define FMAN_FMBM_RFCA_SET_SYNC(_x)             ((_x << 24) & FMAN_FMBM_RFCA_SYNC_MASK )
#define FMAN_FMBM_RFCA_SET_COLOR(_x)            ((_x << 26) & FMAN_FMBM_RFCA_COLOR_MASK)
#define FMAN_FMBM_RFCA_GET_MR(_x)               ((_x & FMAN_FMBM_RFCA_MR_MASK)    >> 16)
#define FMAN_FMBM_RFCA_GET_SYNC(_x)             ((_x & FMAN_FMBM_RFCA_SYNC_MASK)  >> 24)
#define FMAN_FMBM_RFCA_GET_COLOR(_x)            ((_x & FMAN_FMBM_RFCA_COLOR_MASK) >> 26)
// FMBM_RFPNE
#define FMAN_FMBM_RFPNE_HPNIA_MASK               0x00ffffff
#define FMAN_FMBM_RFPNE_HPNIA(_x)               ((_x & FMAN_FMBM_RFPNE_HPNIA_MASK))
// FMBM_RPSO
#define FMAN_FMBM_RPSO_PSO_MASK                  0x000000ff
#define FMAN_FMBM_RPSO_PSO(_x)                  ((_x & FMAN_FMBM_RPSO_PSO_MASK))
// FMBM_RPP
#define FMAN_FMBM_RPP_PNUM_MASK                  0x000000ff
#define FMAN_FMBM_RPP_PNUM(_x)                  ((_x & FMAN_FMBM_RPP_PNUM_MASK))
// FMBM_RETH
#define FMAN_FMBM_RETH_ETHE                      0x80000000
#define FMAN_FMBM_RETH_ETH_MASK                  0x000003ff
#define FMAN_FMBM_RETH_ETH(_x)                  ((_x & FMAN_FMBM_RETH_ETH_MASK))
// FMBM_RFQID
#define FMAN_FMBM_RFQID_DFQID_MASK               0x00ffffff
#define FMAN_FMBM_RFQID_DFQID(_x)               ((_x & FMAN_FMBM_RFQID_DFQID_MASK))
// FMBM_REFQID
#define FMAN_FMBM_REFQID_EFQID_MASK              0x00ffffff
#define FMAN_FMBM_REFQID_EFQID(_x)              ((_x & FMAN_FMBM_REFQID_EFQID_MASK))
// FMBM_RFENE
#define FMAN_FMBM_RFENE_NIA_MASK                 0x00ffffff
#define FMAN_FMBM_RFENE_NIA(_x)                 ((_x & FMAN_FMBM_RFENE_NIA_MASK))
// FMBM_REBMPI
#define FMAN_FMBM_REBMPI_VAL                     0x80000000
#define FMAN_FMBM_REBMPI_ACE                     0x40000000
#define FMAN_FMBM_REBMPI_BP                      0x20000000
#define FMAN_FMBM_REBMPI_PBS_MASK                0x0000ffff
#define FMAN_FMBM_REBMPI_BPID_MASK               0x001f0000
#define FMAN_FMBM_REBMPI_PBS(_x)                ((_x & FMAN_FMBM_REBMPI_PBS_MASK))
#define FMAN_FMBM_REBMPI_SET_BPID(_x)           ((_x << 16) & FMAN_FMBM_REBMPI_BPID_MASK)
#define FMAN_FMBM_REBMPI_GET_BPID(_x)           ((_x & FMAN_FMBM_REBMPI_BPID_MASK) >> 16)
// FMBM_RMPD
#define FMAN_FMBM_RMPD_SBPD_MASK                 0x000000ff
#define FMAN_FMBM_RMPD_NBPD_MASK                 0x00070000
#define FMAN_FMBM_RMPD_NBPDE_MASK                0xff000000
#define FMAN_FMBM_RMPD_SBPD(_x)                 ((_x & FMAN_FMBM_RMPD_SBPD_MASK))
#define FMAN_FMBM_RMPD_SET_NBPD(_x)             ((_x << 16) &   FMAN_FMBM_RMPD_NBPD_MASK)
#define FMAN_FMBM_RMPD_SET_NBPDE(_x)            ((_x << 24) &  FMAN_FMBM_RMPD_NBPDE_MASK)
#define FMAN_FMBM_RMPD_GET_NBPD(_x)             ((_x & FMAN_FMBM_RMPD_NBPD_MASK)  >> 16))
#define FMAN_FMBM_RMPD_GET_NBPDE(_x)            ((_x & FMAN_FMBM_RMPD_NBPDE_MASK) >> 24))
// FMBM_RSTC
#define FMAN_FMBM_RSTC_EN                        0x80000000
// FMBM_RPCP
#define FMAN_FMBM_RPCP_FUCV_MASK                 0x000003ff
#define FMAN_FMBM_RPCP_DCV_MASK                  0x0000f000
#define FMAN_FMBM_RPCP_RCV_MASK                  0x003f0000
#define FMAN_FMBM_RPCP_TCV_MASK                  0x3f000000
#define FMAN_FMBM_RPCP_FUCV(_x)                 ((_x & FMAN_FMBM_RPCP_FUCV_MASK))
#define FMAN_FMBM_RPCP_SET_DCV(_x)              ((_x << 12) & FMAN_FMBM_RPCP_DCV_MASK)
#define FMAN_FMBM_RPCP_SET_RCV(_x)              ((_x << 16) & FMAN_FMBM_RPCP_RCV_MASK)
#define FMAN_FMBM_RPCP_SET_TCV(_x)              ((_x << 24) & FMAN_FMBM_RPCP_TCV_MASK)
#define FMAN_FMBM_RPCP_GET_DCV(_x)              ((_x & FMAN_FMBM_RPCP_DCV_MASK) >> 12)
#define FMAN_FMBM_RPCP_GET_RCV(_x)              ((_x & FMAN_FMBM_RPCP_RCV_MASK) >> 16)
#define FMAN_FMBM_RPCP_GET_TCV(_x)              ((_x & FMAN_FMBM_RPCP_TCV_MASK) >> 24)
// FMBM_RPC
#define FMAN_FMBM_RPC_EN                         0x80000000

//
// Tx Port Register

// FMBM_TCFG
#define FMAN_FMBM_TCFG_EN                        0x80000000
#define FMAN_FMBM_TCFG_IM                        0x01000000
// FMBM_TST
#define FMAN_FMBM_TST_BSY                        0x80000000
#define FMAN_FMBM_TST_GET_TPID(_x)              ((_x & 0x003f0000) >> 16)
// FMBM_TDA
#define FMAN_FMBM_TDA_SWAP                       0xc0000000
#define FMAN_FMBM_TDA_ICC                        0x30000000
#define FMAN_FMBM_TDA_SET_SWAP(_x)              ((_x << 30) & FMAN_FMBM_TDA_SWAP)
#define FMAN_FMBM_TDA_SET_ICC(_x)               ((_x << 28) &  FMAN_FMBM_TDA_ICC)
#define FMAN_FMBM_TDA_GET_SWAP(_x)              ((_x & FMAN_FMBM_TDA_SWAP) >> 30)
#define FMAN_FMBM_TDA_GET_ICC(_x)               ((_x & FMAN_FMBM_TDA_ICC)  >> 28)
// FMBM_TFP
#define FMAN_FMBM_TFP_FLCL_MASK                  0x000003ff
#define FMAN_FMBM_TFP_DPDE_MASK                  0x0000f000
#define FMAN_FMBM_TFP_MFL_MASK                   0x03ff0000
#define FMAN_FMBM_TFP_FLCL(_x)                  ((_x & 0x000003ff))
#define FMAN_FMBM_TFP_SET_DPDE(_x)              ((_x << 12) & FMAN_FMBM_TFP_DPDE_MASK)
#define FMAN_FMBM_TFP_SET_MFL(_x)               ((_x << 16) &  FMAN_FMBM_TFP_MFL_MASK)
#define FMAN_FMBM_TFP_GET_DPDE(_x)              ((_x & FMAN_FMBM_TFP_DPDE_MASK) >> 12)
#define FMAN_FMBM_TFP_GET_MFL(_x)               ((_x & FMAN_FMBM_TFP_MFL_MASK)  >> 16)
// FMBM_TFED
#define FMAN_FMBM_TFED_GET_CSI(_x)              ((_x & 0x1f000000) >> 24)
// FMBM_TICP
#define FMAN_FMBM_TICP_ICSZ_MASK                 0x0000001f
#define FMAN_FMBM_TICP_ICIOF_MASK                0x00000f00
#define FMAN_FMBM_TICP_ICEOF_MASK                0x001f0000
#define FMAN_FMBM_TICP_ICSZ(_x)                 ((_x & FMAN_FMBM_TICP_ICSZ_MASK))
#define FMAN_FMBM_TICP_SET_ICIOF(_x)            ((_x <<  8) & FMAN_FMBM_TICP_ICIOF_MASK)
#define FMAN_FMBM_TICP_SET_ICEOF(_x)            ((_x << 16) & FMAN_FMBM_TICP_ICEOF_MASK)
#define FMAN_FMBM_TICP_GET_ICIOF(_x)            ((_x & FMAN_FMBM_TICP_ICIOF_MASK)  >> 8)
#define FMAN_FMBM_TICP_GET_ICEOF(_x)            ((_x & FMAN_FMBM_TICP_ICEOF_MASK) >> 16)
// FMBM_TFDNE
#define FMAN_FMBM_TFDNE_NIA_MASK                 0x00ffffff
#define FMAN_FMBM_TFDNE_NIA(_x)                 ((_x & FMAN_FMBM_TFDNE_NIA_MASK))
// FMBM_TFCA
#define FMAN_FMBM_TFCA_OR                        0x80000000
#define FMAN_FMBM_TFCA_MR_MASK                   0x003f0000
#define FMAN_FMBM_TFCA_COLOR_MASK                0x0c000000
#define FMAN_FMBM_TFCA_A0_MASK                   0x0000ff00
#define FMAN_FMBM_TFCA_SET_MR(_x)               ((_x << 16) &    FMAN_FMBM_TFCA_MR_MASK)
#define FMAN_FMBM_TFCA_SET_COLOR(_x)            ((_x << 26) & FMAN_FMBM_TFCA_COLOR_MASK)
#define FMAN_FMBM_TFCA_SET_A0(_x)               ((_x <<  8) &    FMAN_FMBM_TFCA_A0_MASK)
#define FMAN_FMBM_TFCA_GET_MR(_x)               ((_x & FMAN_FMBM_TFCA_MR_MASK)    >> 16)
#define FMAN_FMBM_TFCA_GET_COLOR(_x)            ((_x & FMAN_FMBM_TFCA_COLOR_MASK) >> 26)
#define FMAN_FMBM_TFCA_GET_A0(_x)               ((_x & FMAN_FMBM_TFCA_A0_MASK)     >> 8)
// FMBM_TCFQID
#define FMAN_FMBM_TCFQID_TCFQID_MASK             0x00ffffff
#define FMAN_FMBM_TCFQID_TCFQID(_x)             ((_x & FMAN_FMBM_TCFQID_TCFQID_MASK))
// FMBM_TEFQID
#define FMAN_FMBM_TEFQID_EFQID_MASK              0x00ffffff
#define FMAN_FMBM_TEFQID_EFQID(_x)              ((_x & FMAN_FMBM_TEFQID_EFQID_MASK))
// FMBM_TFENE
#define FMAN_FMBM_TFENE_NIA_MASK                 0x00ffffff
#define FMAN_FMBM_TFENE_NIA(_x)                 ((_x & FMAN_FMBM_TFENE_NIA_MASK))
// FMBM_TRLMTS
#define FMAN_FMBM_TRLMTS_EN                      0x80000000
#define FMAN_FMBM_TRLMTS_TSBS_MASK               0x001f0000
#define FMAN_FMBM_TRLMTS_SET_TSBS(_x)           ((_x << 16) & FMAN_FMBM_TRLMTS_TSBS_MASK)
#define FMAN_FMBM_TRLMTS_GET_TSBS(_x)           ((_x & FMAN_FMBM_TRLMTS_TSBS_MASK) >> 16)
// FMBM_TRLMT
#define FMAN_FMBM_TRLMT_RLM_MASK                 0x000003ff
#define FMAN_FMBM_TRLMT_RLM(_x)                 ((_x & FMAN_FMBM_TRLMT_RLM_MASK))
#define FMAN_FMBM_TRLMT_MBS_MASK                 0x03ff0000
#define FMAN_FMBM_TRLMT_SET_MBS(_x)             ((_x << 16) & FMAN_FMBM_TRLMT_MBS_MASK)
#define FMAN_FMBM_TRLMT_GET_MBS(_x)             ((_x & FMAN_FMBM_TRLMT_MBS_MASK) >> 16)
// FMBM_TSTC
#define FMAN_FMBM_TSTC_EN                        0x80000000
// FMBM_TPC
#define FMAN_FMBM_TPC_EN                         0x80000000
// FMBM_TPCP
#define FMAN_FMBM_TPCP_FUCV_MASK                 0x000003ff
#define FMAN_FMBM_TPCP_DCV_MASK                  0x0000f000
#define FMAN_FMBM_TPCP_TCCV_MASK                 0x00070000
#define FMAN_FMBM_TPCP_TCV_MASK                  0x3f000000
#define FMAN_FMBM_TPCP_FUCV(_x)                 ((_x & FMAN_FMBM_TPCP_FUCV_MASK))
#define FMAN_FMBM_TPCP_SET_DCV(_x)              ((_x << 12) &  FMAN_FMBM_TPCP_DCV_MASK)
#define FMAN_FMBM_TPCP_SET_TCCV(_x)             ((_x << 16) & FMAN_FMBM_TPCP_TCCV_MASK)
#define FMAN_FMBM_TPCP_SET_TCV(_x)              ((_x << 24) &  FMAN_FMBM_TPCP_TCV_MASK)
#define FMAN_FMBM_TPCP_GET_DCV(_x)              ((_x & FMAN_FMBM_TPCP_DCV_MASK)  >> 12)
#define FMAN_FMBM_TPCP_GET_TCCV(_x)             ((_x & FMAN_FMBM_TPCP_TCCV_MASK) >> 16)
#define FMAN_FMBM_TPCP_GET_TCV(_x)              ((_x & FMAN_FMBM_TPCP_TCV_MASK)  >> 24)

// Offline Port/Host Command Port Registers
// FMBM_OCFG
#define FMAN_FMBM_OCFG_EN                        0x80000000
#define FMAN_FMBM_OCFG_FDOVR                     0x01000000
// FMBM_OST
#define FMAN_FMBM_OST_BSY                        0x80000000
#define FMAN_FMBM_OST_GET_RPID(_x)              ((_x & 0x003f0000) >> 16)
// FMBM_ODA
#define FMAN_FMBM_ODA_SWAP                       0xc0000000
#define FMAN_FMBM_ODA_ICC                        0x30000000
#define FMAN_FMBM_ODA_FHC                        0x0c000000
#define FMAN_FMBM_ODA_SGC                        0x03000000
#define FMAN_FMBM_ODA_WOPT                       0x00300000
#define FMAN_FMBM_ODA_SET_SWAP(_x)              ((_x << 30) & FMAN_FMBM_ODA_SWAP)
#define FMAN_FMBM_ODA_SET_ICC(_x)               ((_x << 28) & FMAN_FMBM_ODA_ICC )
#define FMAN_FMBM_ODA_SET_FHC(_x)               ((_x << 26) & FMAN_FMBM_ODA_FHC )
#define FMAN_FMBM_ODA_SET_SGC(_x)               ((_x << 24) & FMAN_FMBM_ODA_SGC )
#define FMAN_FMBM_ODA_SET_WOPT(_x)              ((_x << 20) & FMAN_FMBM_ODA_WOPT)
#define FMAN_FMBM_ODA_GET_SWAP(_x)              ((_x & FMAN_FMBM_ODA_SWAP) >> 30)
#define FMAN_FMBM_ODA_GET_ICC(_x)               ((_x & FMAN_FMBM_ODA_ICC)  >> 28)
#define FMAN_FMBM_ODA_GET_FHC(_x)               ((_x & FMAN_FMBM_ODA_FHC)  >> 26)
#define FMAN_FMBM_ODA_GET_SGC(_x)               ((_x & FMAN_FMBM_ODA_SGC)  >> 24)
#define FMAN_FMBM_ODA_GET_WOPT(_x)              ((_x & FMAN_FMBM_ODA_WOPT) >> 20)
// FMBM_OICP
#define FMAN_FMBM_OICP_ICSZ_MASK                 0x0000001f
#define FMAN_FMBM_OICP_ICIOF_MASK                0x00000f00
#define FMAN_FMBM_OICP_ICEOF_MASK                0x001f0000
#define FMAN_FMBM_OICP_ICSZ(_x)                 ((_x & FMAN_FMBM_OICP_ICSZ_MASK))
#define FMAN_FMBM_OICP_SET_ICIOF(_x)            ((_x <<  8) & FMAN_FMBM_OICP_ICIOF_MASK)
#define FMAN_FMBM_OICP_SET_ICEOF(_x)            ((_x << 16) & FMAN_FMBM_OICP_ICEOF_MASK)
#define FMAN_FMBM_OICP_GET_ICIOF(_x)            ((_x & FMAN_FMBM_OICP_ICIOF_MASK)  >> 8)
#define FMAN_FMBM_OICP_GET_ICEOF(_x)            ((_x & FMAN_FMBM_OICP_ICEOF_MASK) >> 16)
// FMBM_OFDNE
#define FMAN_FMBM_OFDNE_NIA_MASK                 0x00ffffff
#define FMAN_FMBM_OFDNE_NIA(_x)                 ((_x & FMAN_FMBM_OFDNE_NIA_MASK))
// FMBM_OFEN
#define FMAN_FMBM_OFEN_NIA_MASK                  0x00ffffff
#define FMAN_FMBM_OFEN_NIA(_x)                  ((_x & FMAN_FMBM_OFEN_NIA_MASK))
// FMBM_OFCA
#define FMAN_FMBM_OFCA_OR                        0x80000000
#define FMAN_FMBM_OFCA_MR_MASK                   0x003f0000
#define FMAN_FMBM_OFCA_SYNC_MASK                 0x03000000
#define FMAN_FMBM_OFCA_COLOR_MASK                0x0c000000
#define FMAN_FMBM_OFCA_SET_MR(_x)               ((_x << 16) & FMAN_FMBM_OFCA_MR_MASK   )
#define FMAN_FMBM_OFCA_SET_SYNC(_x)             ((_x << 24) & FMAN_FMBM_OFCA_SYNC_MASK )
#define FMAN_FMBM_OFCA_SET_COLOR(_x)            ((_x << 26) & FMAN_FMBM_OFCA_COLOR_MASK)
#define FMAN_FMBM_OFCA_GET_MR(_x)               ((_x & FMAN_FMBM_OFCA_MR_MASK)    >> 16)
#define FMAN_FMBM_OFCA_GET_SYNC(_x)             ((_x & FMAN_FMBM_OFCA_SYNC_MASK)  >> 24)
#define FMAN_FMBM_OFCA_GET_COLOR(_x)            ((_x & FMAN_FMBM_OFCA_COLOR_MASK) >> 26)
// FMBM_OFPNE
#define FMAN_FMBM_OFPNE_HPNIA_MASK               0x00ffffff
#define FMAN_FMBM_OFPNE_HPNIA(_x)               ((_x & FMAN_FMBM_OFPNE_HPNIA_MASK))
// FMBM_OPSO
#define FMAN_FMBM_OPSO_PSO_MASK                  0x000000ff
#define FMAN_FMBM_OPSO_PSO(_x)                  ((_x & FMAN_FMBM_OPSO_PSO_MASK))
// FMBM_OPP
#define FMAN_FMBM_OPP_PNUM_MASK                  0x000000ff
#define FMAN_FMBM_OPP_PNUM(_x)                  ((_x & FMAN_FMBM_OPP_PNUM_MASK))
// FMBM_OIM
#define FMAN_FMBM_OIM_FOF_MASK                   0xf0000000
#define FMAN_FMBM_OIM_SET_FOF(_x)               ((_x << 28) & FMAN_FMBM_OIM_FOF_MASK)
#define FMAN_FMBM_OIM_GET_FOF(_x)               ((_x & FMAN_FMBM_OIM_FOF_MASK) >> 28)
// FMBM_OFP
#define FMAN_FMBM_OFP_DPDE_MASK                  0x0000f000
#define FMAN_FMBM_OFP_SET_DPDE(_x)              ((_x << 12) & FMAN_FMBM_OFP_DPDE_MASK)
#define FMAN_FMBM_OFP_GET_DPDE(_x)              ((_x & FMAN_FMBM_OFP_DPDE_MASK) >> 12)
// FMBM_OFQID
#define FMAN_FMBM_OFQID_DFQID_MASK               0x00ffffff
#define FMAN_FMBM_OFQID_DFQID(_x)               ((_x & FMAN_FMBM_OFQID_DFQID))
// FMBM_OEFQID
#define FMAN_FMBM_OEFQID_EFQID_MASK              0x00ffffff
#define FMAN_FMBM_OEFQID_EFQID(_x)              ((_x & FMAN_FMBM_OEFQID_EFQID_MASK))
// FMBM_OFENE
#define FMAN_FMBM_OFENE_NIA_MASK                 0x00ffffff
#define FMAN_FMBM_OFENE_NIA(_x)                 ((_x & FMAN_FMBM_OFENE_NIA_MASK))
// FMBM_ORLMTS
#define FMAN_FMBM_ORLMTS_EN                      0x80000000
#define FMAN_FMBM_ORLMTS_LRS_MASK                0x00000003
#define FMAN_FMBM_ORLMTS_TSBS_MASK               0x001f0000
#define FMAN_FMBM_ORLMTS_LRS(_x)                ((_x & FMAN_FMBM_ORLMTS_LRS_MASK))
#define FMAN_FMBM_ORLMTS_SET_TSBS(_x)           ((_x << 16) & FMAN_FMBM_ORLMTS_TSBS_MASK)
#define FMAN_FMBM_ORLMTS_GET_TSBS(_x)           ((_x & FMAN_FMBM_ORLMTS_TSBS_MASK) >> 16)
// FMBM_ORLMT
#define FMAN_FMBM_ORLMT_BSG                      0x80000000
#define FMAN_FMBM_ORLMT_RLM_MASK                 0x000003ff
#define FMAN_FMBM_ORLMT_MBS_MASK                 0x03ff0000
#define FMAN_FMBM_ORLMT_RLM(_x)                 ((_x & FMAN_FMBM_ORLMT_RLM_MASK))
#define FMAN_FMBM_ORLMT_SET_MBS(_x)             ((_x << 16) & FMAN_FMBM_ORLMT_MBS_MASK)
#define FMAN_FMBM_ORLMT_GET_MBS(_x)             ((_x & FMAN_FMBM_ORLMT_MBS_MASK) >> 16)
// FMBM_OEBMPI
#define FMAN_FMBM_OEBMPI_VAL                     0x80000000
#define FMAN_FMBM_OEBMPI_BPID_MASK               0x0000001f
#define FMAN_FMBM_OEBMPI_BPID(_x)               ((_x & FMAN_FMBM_OEBMPI_BPID_MASK))
// FMBM_OPCP
#define FMAN_FMBM_OPCP_FUCV_MASK                 0x000003ff
#define FMAN_FMBM_OPCP_DCV_MASK                  0x0000f000
#define FMAN_FMBM_OPCP_TCV_MASK                  0x3f000000
#define FMAN_FMBM_OPCP_FUCV(_x)                 ((_x & FMAN_FMBM_OPCP_FUCV_MASK))
#define FMAN_FMBM_OPCP_SET_DCV(_x)              ((_x << 12) & FMAN_FMBM_OPCP_DCV_MASK)
#define FMAN_FMBM_OPCP_SET_TCV(_x)              ((_x << 24) & FMAN_FMBM_OPCP_DCV_MASK)
#define FMAN_FMBM_OPCP_GET_DCV(_x)              ((_x & FMAN_FMBM_OPCP_DCV_MASK) >> 12)
#define FMAN_FMBM_OPCP_GET_TCV(_x)              ((_x & FMAN_FMBM_OPCP_TCV_MASK) >> 24)
// FMBM_OMPD
#define FMAN_FMBM_OMPD_NBPD                      0x00010000
#define FMAN_FMBM_OMPD_NBPDE_MASK                0xc0000000
#define FMAN_FMBM_OMPD_SBPD_MASK                 0x000000c0
#define FMAN_FMBM_OMPD_SET_NBPDE(_x)            ((_x << 30) & FMAN_FMBM_OMPD_NBPDE_MASK)
#define FMAN_FMBM_OMPD_SET_SBPD(_x)             ((_x <<  6) &  FMAN_FMBM_OMPD_SBPD_MASK)
#define FMAN_FMBM_OMPD_GET_NBPDE(_x)            ((_x & FMAN_FMBM_OMPD_NBPDE_MASK) >> 30)
#define FMAN_FMBM_OMPD_GET_SBPD(_x)             ((_x & FMAN_FMBM_OMPD_SBPD_MASK)  >>  6)
// FMBM_OPC
#define FMAN_FMBM_OPC_EN                         0x80000000
// FMBM_OSTC
#define FMAN_FMBM_OSTC_EN                        0x80000000

// QMI Common Registers

struct fman_qmi_dbg
{
  cyg_uint32 fmqm_dtcx1;  // x = A, B or C - B and C N/A for P1023
  cyg_uint32 fmqm_dtvx1;
  cyg_uint32 fmqm_dtmx1;
  cyg_uint32 fmqm_dtcx;
  cyg_uint32 fmqm_dtcx2;
  cyg_uint32 fmqm_dtvx2;
};

struct fman_qmi_com
{
  cyg_uint32 fmqm_gc;
  cyg_uint32 _reserved1;
  cyg_uint32 fmqm_eie;
  cyg_uint32 fmqm_eien;
  cyg_uint32 fmqm_eif;
  cyg_uint32 fmqm_ie;               // N/A for P1023
  cyg_uint32 fmqm_ien;              // N/A for P1023
  cyg_uint32 fmqm_if;               // N/A for P1023
  cyg_uint32 fmqm_gs;
  cyg_uint32 _reserved2;
  cyg_uint32 fmqm_etfc;
  cyg_uint32 fmqm_dtfc;
  cyg_uint32 fmqm_dc0;
  cyg_uint32 fmqm_dc1;              // N/A for P1023
  cyg_uint32 fmqm_dc2;              // N/A for P1023
  cyg_uint32 fmqm_dc3;              // N/A for P1023
  cyg_uint32 _reserved3[0x0B];      // N/A for P4080, P2040/P2041, P3041, P5020
  cyg_uint32 fmqm_tapc;             // N/A for P4080, P2040/P2041, P3041, P5020
  cyg_uint32 fmqm_dmcvc;            // N/A for P4080, P2040/P2041, P3041, P5020
  cyg_uint32 fmqm_difdcc;           // N/A for P4080, P2040/P2041, P3041, P5020
  cyg_uint32 fmqm_da1vc;            // N/A for P4080, P2040/P2041, P3041, P5020
  cyg_uint32 _reserved4;
  cyg_uint32 fmqm_dtrc;
  cyg_uint32 fmqm_efddd;
  cyg_uint32 _reserved5[0x02];
  struct fman_qmi_dbg qmi_dbg[FMAN_NUM_OF_DBG_FLOW];
};

// FMQM_GC
#define FMAN_FMQM_GC_ENQ_EN                      0x80000000  // Not documented
#define FMAN_FMQM_GC_DEQ_EN                      0x40000000  // Not documented
#define FMAN_FMQM_GC_DEQ_THR_MASK                0x0000003f
#define FMAN_FMQM_GC_ENQ_THR_MASK                0x00003f00
#define FMAN_FMQM_GC_DEQ_THR(_x)                ((_x & FMAN_FMQM_GC_DEQ_THR_MASK))
#define FMAN_FMQM_GC_STEN                        0x10000000
#define FMAN_FMQM_GC_SET_ENQ_THR(_x)            ((_x << 8) & FMAN_FMQM_GC_ENQ_THR_MASK)
#define FMAN_FMQM_GC_GET_ENQ_THR(_x)            ((_x & FMAN_FMQM_GC_ENQ_THR_MASK) >> 8)
// FMQM_EIE
#define FMAN_FMQM_EIE_DEE                        0x80000000
#define FMAN_FMQM_EIE_DFUPE                      0x40000000
// FMQM_EIEN
#define FMAN_FMQM_EIEN_DEE                       0x80000000
#define FMAN_FMQM_EIEN_DFUPE                     0x40000000
// FMQM_EIF
#define FMAN_FMQM_EIF_DEE                        0x80000000
#define FMAN_FMQM_EIF_DFUPE                      0x40000000
// FMQM_IE
#define FMAN_FMQM_IE_SEE                         0x80000000
// FMQM_IF
#define FMAN_FMQM_IF_FSEE                        0x80000000
// FMQM_GS
#define FMAN_FMQM_GS_BSY_DT                      0x80000000
#define FMAN_FMQM_GS_BSY_ET                      0x40000000
#define FMAN_FMQM_GS_BSY_DF                      0x20000000
#define FMAN_FMQM_GS_BSY_EF                      0x10000000
#define FMAN_FMQM_GS_SRS                         0x00010000
#define FMAN_FMQM_GS_HNB                         0x00000002
// FMQM_TAPC
#define FMAN_FMQM_TAPC_TAP_MASK                  0xffc00000
#define FMAN_FMQM_TAPC_SET_TAP(_x)              ((_x << 22) & FMAN_FMQM_TAPC_TAP_MASK)
#define FMAN_FMQM_TAPC_GET_TAP(_x)              ((_x & FMAN_FMQM_TAPC_TAP_MASK) >> 22)
// FMQM_DTRC
#define FMAN_FMQM_DTRC_TL_A_MASK                 0x30000000
#define FMAN_FMQM_DTRC_TL_B_MASK                 0xc0000000
#define FMAN_FMQM_DTRC_TL_C_MASK                 0x03000000
#define FMAN_FMQM_DTRC_TL_CO_MASK                0x00300000
#define FMAN_FMQM_DTRC_SET_TL_A(_x)             ((_x << 28) &  FMAN_FMQM_DTRC_TL_A_MASK)
#define FMAN_FMQM_DTRC_SET_TL_B(_x)             ((_x << 26) &  FMAN_FMQM_DTRC_TL_B_MASK)
#define FMAN_FMQM_DTRC_SET_TL_C(_x)             ((_x << 24) &  FMAN_FMQM_DTRC_TL_C_MASK)
#define FMAN_FMQM_DTRC_SET_TR_CO(_x)            ((_x << 20) & FMAN_FMQM_DTRC_TL_CO_MASK)
#define FMAN_FMQM_DTRC_GET_TL_A(_x)             ((_x &  FMAN_FMQM_DTRC_TL_A_MASK) >> 28)
#define FMAN_FMQM_DTRC_GET_TL_B(_x)             ((_x &  FMAN_FMQM_DTRC_TL_B_MASK) >> 26)
#define FMAN_FMQM_DTRC_GET_TL_C(_x)             ((_x &  FMAN_FMQM_DTRC_TL_C_MASK) >> 24)
#define FMAN_FMQM_DTRC_GET_TR_CO(_x)            ((_x & FMAN_FMQM_DTRC_TL_CO_MASK) >> 20)
// FMQM_DTCn
#define FMAN_FMQM_DTCn_CMPOP_MASK                0xe0000000
#define FMAN_FMQM_DTCn_AND                       0x10000000
#define FMAN_FMQM_DTCn_FSEL_MASK                 0x00030000
#define FMAN_FMQM_DTCn_SET_CMPOP(_x)            ((_x << 29) &  FMAN_FMQM_DTCn_CMPOP_MASK)
#define FMAN_FMQM_DTCn_SET_FSEL(_x)             ((_x << 16) &   FMAN_FMQM_DTCn_FSEL_MASK)
#define FMAN_FMQM_DTCn_GET_CMPOP(_x)            ((_x &  FMAN_FMQM_DTCn_CMPOP_MASK) >> 29)
#define FMAN_FMQM_DTCn_GET_FSEL(_x)             ((_x &   FMAN_FMQM_DTCn_FSEL_MASK) >> 16)

// QMI O/H , RX and TX Port Registers
struct fman_qmi_port
{
  union
  {
    struct
    {
      cyg_uint32 fmqm_pnc;
      cyg_uint32 fmqm_pns;
      cyg_uint32 fmqm_pnts;
      cyg_uint32 _reserved1[0x04];
      cyg_uint32 fmqm_pnen;
      cyg_uint32 fmqm_pnetfc;
      cyg_uint32 _reserved2[0x02];
      cyg_uint32 fmqm_pndn;
      cyg_uint32 fmqm_pndc;
      cyg_uint32 fmqm_pndtfc;
      cyg_uint32 fmqm_pndfnoc;
      cyg_uint32 fmqm_pndcc;
      cyg_uint32 _reserved3[0xF0];
    } oh;

    struct
    {
      cyg_uint32 fmqm_pnc;
      cyg_uint32 fmqm_pns;
      cyg_uint32 fmqm_pnts;
      cyg_uint32 _reserved1[0x04];
      cyg_uint32 fmqm_pnen;
      cyg_uint32 fmqm_pnetfc;
      cyg_uint32 _reserved2[0x02];
      cyg_uint32 fmqm_pndn;
      cyg_uint32 fmqm_pndc;
      cyg_uint32 fmqm_pndtfc;
      cyg_uint32 fmqm_pndfnoc;
      cyg_uint32 fmqm_pndcc;
      cyg_uint32 _reserved3[0xF0];
    } tx;

    struct
    {
      cyg_uint32 fmqm_pnc;
      cyg_uint32 fmqm_pns;
      cyg_uint32 fmqm_pnts;
      cyg_uint32 _reserved1[0x04];
      cyg_uint32 fmqm_pnen;
      cyg_uint32 fmqm_pnetfc;
      cyg_uint32 _reserved2[0xF7];
    } rx;

    struct
    {
      cyg_uint32 fmqm_pnc;
      cyg_uint32 fmqm_pns;
      cyg_uint32 fmqm_pnts;
      cyg_uint32 _reserved1[0x04];
      cyg_uint32 fmqm_pnen;
      cyg_uint32 _reserved2[0xF8];
    } common;
  } u;
};


// FMQM_PnC
#define FMAN_FMQM_PnC_EN                         0x80000000
#define FMAN_FMQM_PnC_STEN                       0x10000000
// FMQM_PnS
#define FMAN_FMQM_PnS_PBSY_DT                    0x80000000
#define FMAN_FMQM_PnS_PBSY_ET                    0x40000000
#define FMAN_FMQM_PnS_PBSY_DF                    0x20000000
// FMQM_PnTS
#define FMAN_FMQM_PnTS_PDFS_MASK                 0x000000ff
#define FMAN_FMQM_PnTS_PDTS_MASK                 0x00007f00
#define FMAN_FMQM_PnTS_PETS_MASK                 0x7f000000
#define FMAN_FMQM_PnTS_PDFS(_x)                  ((_x & FMAN_FMQM_PnTS_PDFS_MASK))
#define FMAN_FMQM_PnTS_SET_PDTS(_x)              ((_x <<  8) & FMAN_FMQM_PnTS_PDTS_MASK)
#define FMAN_FMQM_PnTS_SET_PETS(_x)              ((_x << 24) & FMAN_FMQM_PnTS_PETS_MASK)
#define FMAN_FMQM_PnTS_GET_PDTS(_x)              ((_x & FMAN_FMQM_PnTS_PDTS_MASK) >> 08)
#define FMAN_FMQM_PnTS_GET_PETS(_x)              ((_x & FMAN_FMQM_PnTS_PETS_MASK) >> 24)
// FMQM_PnEN
#define FMAN_FMQM_PnEN_NIA_MASK                  0x00ffffff
#define FMAN_FMQM_PnEN_NIA(_x)                  ((_x & FMAN_FMQM_PnEN_NIA_MASK))
// FMQM_PnDN
#define FMAN_FMQM_PnDN_NIA_MASK                  0x00ffffff
#define FMAN_FMQM_PnDN_NIA(_x)                  ((_x & FMAN_FMQM_PnDN_NIA_MASK))
// FMQM_PnDC
#define FMAN_FMQM_PnDC_BC_MASK                   0x0000ffff
#define FMAN_FMQM_PnDC_SP_MASK                   0x00f00000
#define FMAN_FMQM_PnDC_OPT_MASK                  0x70000000
#define FMAN_FMQM_PnDC_PRI                       0x80000000
#define FMAN_FMQM_PnDC_PF                        0x02000000
#define FMAN_FMQM_PnDC_FRM                       0x01000000
#define FMAN_FMQM_PnDC_BC(_x)                   ((_x & FMAN_FMQM_BC_MASK))
#define FMAN_FMQM_PnDC_SET_SP(x)                ((_x << 20) &   FMAN_FMQM_PnDC_SP_MASK)
#define FMAN_FMQM_PnDC_SET_OPT(x)               ((_x << 30) &  FMAN_FMQM_PnDC_OPT_MASK)
#define FMAN_FMQM_PnDC_GET_SP(x)                ((_x & FMAN_FMQM_PnDC_SP_MASK)   >> 20)
#define FMAN_FMQM_PnDC_GET_OPT(x)               ((_x & FMAN_FMQM_PnDC_OPT_MASK)  >> 30)

// FMan DMA Memory Map

struct fman_dma
{
  cyg_uint32 fmdm_sr;
  cyg_uint32 fmdm_mr;
  cyg_uint32 fmdm_tr;
  cyg_uint32 fmdm_hy;
  cyg_uint32 fmdm_setr;
  cyg_uint32 fmdm_tah;
  cyg_uint32 fmdm_tal;
  cyg_uint32 fmdm_tcid;
  cyg_uint32 _reserved1[0x02];
  cyg_uint32 fmdm_wcr;                     // N/A for P4080
  cyg_uint32 fmdm_ebcr;                    // N/A for P4080
  cyg_uint32 _reserved2[0x09];
  cyg_uint32 fmdm_dcr;
  cyg_uint32 fmdm_emsr;
  cyg_uint32 reserved;
  cyg_uint32 fmdm_plrx[FM_MAX_PORTS/2];
  cyg_uint32 _reserved3[0x0B];
};

// FMDM_SR
#define FMAN_FMDM_SR_CMDQNE                      0x10000000
#define FMAN_FMDM_SR_BER                         0x08000000
#define FMAN_FMDM_SR_RDB_ECC                     0x04000000
#define FMAN_FMDM_SR_WRB_SECC                    0x02000000
#define FMAN_FMDM_SR_WRB_FECC                    0x01000000
#define FMAN_FMDM_SR_DPEXT_SECC                  0x00800000
#define FMAN_FMDM_SR_DPEXT_FECC                  0x00400000
#define FMAN_FMDM_SR_DPDAT_SECC                  0x00200000
#define FMAN_FMDM_SR_DPDAT_FECC                  0x00100000
#define FMAN_FMDM_SR_SPDAT_FECC                  0x00080000
#define FMAN_FMDM_SR_STATUS_W1C_MASK             0x0FF80000
// FMDM_MR
#define FMAN_FMDM_MR_CO_MASK                     0xc0000000
#define FMAN_FMDM_MR_AIDO                        0x20000000
#define FMAN_FMDM_MR_SBER                        0x10000000
#define FMAN_FMDM_MR_ERRD_MSK                    0x00800000
#define FMAN_FMDM_MR_ERWR_MSK                    0x00400000
#define FMAN_FMDM_MR_BER_MSK                     0x00200000
#define FMAN_FMDM_MR_EB_MSK                      0x00100000
#define FMAN_FMDM_MR_ERRD_EME                    0x00080000
#define FMAN_FMDM_MR_ERWR_EME                    0x00040000
#define FMAN_FMDM_MR_EB_EME_MASK                 0x00030000
#define FMAN_FMDM_MR_CEN_MASK                    0x0000e000
#define FMAN_FMDM_MR_DBG_CNT_MASK                0x00000380
#define FMAN_FMDM_MR_BMI_EMR                     0x00000040
#define FMAN_FMDM_MR_ECC_MSK                     0x00000020
#define FMAN_FMDM_MR_AIDM                        0x00000010
#define FMAN_FMDM_MR_SET_CO(x)                  ((_x << 29) &      FMAN_FMDM_MR_CO_MASK)
#define FMAN_FMDM_MR_SET_EB_EME(x)              ((_x << 16) &  FMAN_FMDM_MR_EB_EME_MASK)
#define FMAN_FMDM_MR_SET_CEN(x)                 ((_x << 13) &     FMAN_FMDM_MR_CEN_MASK)
#define FMAN_FMDM_MR_SET_DBG_CNT(x)             ((_x <<  7) & FMAN_FMDM_MR_DBG_CNT_MASK)
#define FMAN_FMDM_MR_GET_CO(x)                  ((_x &      FMAN_FMDM_MR_CO_MASK) >> 29)
#define FMAN_FMDM_MR_GET_EB_EME(x)              ((_x &  FMAN_FMDM_MR_EB_EME_MASK) >> 16)
#define FMAN_FMDM_MR_GET_CEN(x)                 ((_x &     FMAN_FMDM_MR_CEN_MASK) >> 13)
#define FMAN_FMDM_MR_GET_DBG_CNT(x)             ((_x & FMAN_FMDM_MR_DBG_CNT_MASK) >>  7)
// FMDM_TR
#define FMAN_FMDM_TR_WBTH_MASK                   0x0000007f
#define FMAN_FMDM_TR_WBTH(_x)                   ((_x & FMAN_FMDM_TR_WBTH_MASK))
#define FMAN_FMDM_TR_RBTH_MASK                   0x007f0000
#define FMAN_FMDM_TR_CQTH_MASK                   0xff000000
#define FMAN_FMDM_TR_SET_RBTH(x)                ((_x << 16) & FMAN_FMDM_TR_RBTH_MASK)
#define FMAN_FMDM_TR_SET_CQTH(x)                ((_x << 24) & FMAN_FMDM_TR_CQTH_MASK)
#define FMAN_FMDM_TR_GET_RBTH(x)                ((_x & FMAN_FMDM_TR_RBTH_MASK) >> 16)
#define FMAN_FMDM_TR_GET_CQTH(x)                ((_x & FMAN_FMDM_TR_CQTH_MASK) >> 24)
// FMDM_HY
#define FMAN_FMDM_HY_WBHY_MASK                   0x0000007f
#define FMAN_FMDM_HY_WBHY(_x)                   ((_x & FMAN_FMDM_TR_WBTH_MASK))
#define FMAN_FMDM_HY_RBHY_MASK                   0x007f0000
#define FMAN_FMDM_HY_CQHY_MASK                   0xff000000
#define FMAN_FMDM_HY_SET_RBHY(x)                ((_x << 16) & FMAN_FMDM_HY_RBHY_MASK)
#define FMAN_FMDM_HY_SET_CQHY(x)                ((_x << 24) & FMAN_FMDM_HY_CQHY_MASK)
#define FMAN_FMDM_HY_GET_RBHY(x)                ((_x & FMAN_FMDM_HY_RBHY_MASK) >> 16)
#define FMAN_FMDM_HY_GET_CQHY(x)                ((_x & FMAN_FMDM_HY_CQHY_MASK) >> 24)
// FMDM_TCID
#define FMAN_FMDM_TCID_LIODN_MASK                0x00000fff
#define FMAN_FMDM_TCID_LIODN(_x)                ((_x & FMAN_FMDM_TCID_LIODN_MASK))
#define FMAN_FMDM_TCID_PORT_ID_MASK              0xff000000
#define FMAN_FMDM_TCID_TNUM_MASK                 0x00ff0000
#define FMAN_FMDM_TCID_DONET_MASK                0x0000f000
#define FMAN_FMDM_TCID_SET_PORT_ID(x)           ((_x << 24) & FMAN_FMDM_TCID_PORT_ID_MASK)
#define FMAN_FMDM_TCID_SET_TNUM(x)              ((_x << 16) &    FMAN_FMDM_TCID_TNUM_MASK)
#define FMAN_FMDM_TCID_SET_DONET(x)             ((_x << 12) &   FMAN_FMDM_TCID_DONET_MASK)
#define FMAN_FMDM_TCID_GET_PORT_ID(x)           ((_x & FMAN_FMDM_TCID_PORT_ID_MASK) >> 24)
#define FMAN_FMDM_TCID_GET_TNUM(x)              ((_x &    FMAN_FMDM_TCID_TNUM_MASK) >> 16)
#define FMAN_FMDM_TCID_GET_DONET(x)             ((_x &   FMAN_FMDM_TCID_DONET_MASK) >> 12)
// FMDM_EBCR
#define FMAN_FMDM_EBCR_BA_MASK                   0x0000ffff
#define FMAN_FMDM_EBCR_BA(_x)                   ((_x & FMAN_FMDM_EMSR_EMSTR_MASK))
// FMDM_EMSR
#define FMAN_FMDM_EMSR_EMSTR_MASK                0x0000ffff
#define FMAN_FMDM_EMSR_EMSTR(_x)                ((_x & FMAN_FMDM_EMSR_EMSTR_MASK))
// FMDM_PLR
#define FMAN_FMDM_PLR_EVEN_PORT_LIODN_BASE(_x)  ((_x << 16) & 0x0fff0000)
#define FMAN_FMDM_PLR_ODD_PORT_LIODN_BASE(_x)   ((_x <<  0) & 0x00000fff)
// FMan controller Configuration Data

struct fman_ccd
{
  cyg_uint32 fmcd_addr;
  cyg_uint32 fmcd_data;
  cyg_uint32 _reserved1;
  cyg_uint32 fmcd_ready;
  cyg_uint32 _reserved2[0x1FC];
  cyg_uint32 fmcd_data2[0x1FF];
};

// FMCD_ADDR
#define FMAN_FMCD_ADDR_AIE                       0x80000000
#define FMAN_FMCD_ADDR_MASK                      0x7fffffff
#define FMAN_FMCD_ADDR(_x)                      ((_x & FMAN_FMCD_ADDR_MASK))
// FMCD_READY
#define FMAN_FMCD_READY_RDY                      0x80000000

struct fman_port {
  volatile struct fman_bmi_port bmi;
  volatile struct fman_qmi_port qmi;
  volatile cyg_uint32           parser[0x100];
  volatile cyg_uint32           _reserved1[0x100];
};

struct fman_port_array{
  volatile struct fman_port p[FM_MAX_PORTS]; // Port 0 is reserved for common registers
};

// FMAN Internal Memory application layer
struct imem_pool_t {
  cyg_uint32 base;
  cyg_uint32 size;
  cyg_uint32 next_buf;
};

// We must define FMAN configuration from CDL, this is TBD
struct fman_bmi_cfg {
  cyg_uint32 pool_size;
};
struct fman_qmi_cfg {
  cyg_uint32 tbd;
};
struct fman_fpm_cfg {
  cyg_uint32 tbd;
};
struct fman_dma_cfg {
  cyg_uint32 tbd;
};
struct fman_cfg {
  struct fman_bmi_cfg bmi;
  struct fman_qmi_cfg qmi;
  struct fman_fpm_cfg fpm;
  struct fman_dma_cfg dma;
};

// FMAN Internal structure
struct fman_t {
  volatile cyg_uint32                 *pmicrocode;
  volatile cyg_uint32                 *imem;     // FMAN Internal Memory
  volatile struct fman_dma            *dma;      // DMA Memory Map
  volatile struct fman_fpm            *fpm;      // FPM Memory Map
  volatile struct fman_ccd            *ccd;      // FMan controller Configuration Data
  volatile struct fman_qmi_com        *qmi_com;  // QMI Common Registers
  volatile struct fman_bmi_com        *bmi_com;  // BMI Memory Map
  volatile struct fman_port_array     *port;     // O/H , RX and TX Port, Parser Registers
  cyg_uint32 state;
  cyg_uint32 id;                                 // FMAN controller identifier, 1 or 2
  struct imem_pool_t                  imem_pool;
  struct fman_cfg                     *cfg;      // FMAN configuration, for future use
};

// Next Invoked Action (NIA)
#define FMAN_NIA_MASK                            0x007c0000
#define FMAN_NIA_SET_ENG(_x)                    ((_x << 18) & FMAN_NIA_MASK)
#define FMAN_NIA_ORR                             0x00800000

#define FMAN_NIA_ENG_FMANC                       0x00
#define FMAN_NIA_ENG_PARSER                      0x11
#define FMAN_NIA_ENG_KEYGEN                      0x12
#define FMAN_NIA_ENG_POLICER                     0x13
#define FMAN_NIA_ENG_BMI                         0x14
#define FMAN_NIA_ENG_QMI_ENQ                     0x15
#define FMAN_NIA_ENG_BMI_DEQ                     0x16

// Action code for FMAN controller
#define FMAN_NIA_FMANC_AC_CC                     0x00000006
#define FMAN_NIA_FMANC_AC_IND_MODE_TX            0x00000008
#define FMAN_NIA_FMANC_AC_IND_MODE_RX            0x0000000A
#define FMAN_NIA_FMANC_AC_HC                     0x0000000c

#define FMAN_INIT_PENDING                        0x0
#define FMAN_INIT_COMPLETED                      0x1

#define FMAN_PORT_OFF                            0x0
#define FMAN_PORT_ON                             0x1

#define CYGNUM_REG_IMM_DPAA_FMAN_IMEM_BASE(_x)      \
   (volatile cyg_uint32 *) ( CYGARC_IMM_BASE + _x )
#define CYGNUM_REG_IMM_DPAA_FMAN_CCD_BASE(_x)       \
   (volatile struct fman_ccd *) ( CYGARC_IMM_BASE + _x + CYGARC_REG_IMM_DPAA_FMAN_CCD )
#define CYGNUM_REG_IMM_DPAA_FMAN_FPM_BASE(_x)       \
   (volatile struct fman_fpm *) ( CYGARC_IMM_BASE + _x + CYGARC_REG_IMM_DPAA_FMAN_FPM )
#define CYGNUM_REG_IMM_DPAA_FMAN_DMA_BASE(_x)       \
   (volatile struct fman_dma *) ( CYGARC_IMM_BASE + _x + CYGARC_REG_IMM_DPAA_FMAN_DMA )
#define CYGNUM_REG_IMM_DPAA_FMAN_BMI_COM_BASE(_x)   \
   (volatile struct fman_bmi_com *) ( CYGARC_IMM_BASE + _x + CYGARC_REG_IMM_DPAA_FMAN_HW_PORT_MEM )
#define CYGNUM_REG_IMM_DPAA_FMAN_QMI_COM_BASE(_x)   \
   (volatile struct fman_qmi_com *) ( CYGARC_IMM_BASE + _x + CYGARC_REG_IMM_DPAA_FMAN_HW_PORT_MEM + sizeof(struct fman_bmi_com) )

// Let the port registers include the common area in order to used PortID
// directly as index instead of subtracting one all the time. But careful
// not to use port 0, this is reserved area.
#define CYGNUM_REG_IMM_DPAA_FMAN_PORT_BASE(_x)      \
   (volatile struct fman_port_array *) ( CYGARC_IMM_BASE + _x + CYGARC_REG_IMM_DPAA_FMAN_HW_PORT_MEM )

#ifdef CYGARC_REG_IMM_DPAA_FMAN2
#define CYGNUM_DPAA_FMAN_PORT_BASE_ADDR(_id)                   \
({                                                             \
  cyg_uint32 _addr = 0;                                        \
  if( _id == 1 )                                               \
    _addr = CYGARC_REG_IMM_DPAA_FMAN1;                         \
   else if( _id == 2 )                                         \
    _addr = CYGARC_REG_IMM_DPAA_FMAN2;                         \
  _addr;                                                       \
})
#else
#define CYGNUM_DPAA_FMAN_PORT_BASE_ADDR(_id)                   \
({                                                             \
  cyg_uint32 _addr = 0;                                        \
  if( _id == 1 )                                               \
    _addr = CYGARC_REG_IMM_DPAA_FMAN1;                         \
  _addr;                                                       \
})
#endif

cyg_uint32 fman_imem_alloc(struct imem_pool_t *, cyg_uint32 , cyg_uint32 );
struct fman_t * fman_get_ptr( cyg_uint8 );
cyg_uint32 fman_qmi_port_off(volatile struct fman_qmi_port *);
cyg_uint32 fman_qmi_port_on(volatile struct fman_qmi_port *);
cyg_uint32 fman_bmi_port_off(volatile struct fman_bmi_port *, fman_port_t);
cyg_uint32 fman_bmi_port_on(volatile struct fman_bmi_port *, fman_port_t);

#define FMAN_IMEM_ETH_ALLOC( _fman, _size, _align )            \
({                                                             \
  cyg_uint32 _addr = 0;                                        \
  struct fman_t *fman;                                         \
  fman  = fman_get_ptr(_fman);                                 \
  if( fman )                                                   \
    _addr = fman_imem_alloc(&fman->imem_pool, _size, _align);  \
  _addr;                                                       \
})

#define FMAN_IMEM_BASE( _fman )                                \
({                                                             \
  cyg_uint32 _addr = 0;                                        \
  struct fman_t *fman;                                         \
  fman  = fman_get_ptr(_fman);                                 \
  if( fman )                                                   \
    _addr = (cyg_uint32) fman->imem;                           \
  _addr;                                                       \
})

#define FMAN_IMEM_IMGP_SET_GRACEFUL_STOP(_imgp, _onoff)        \
{                                                              \
  if( _onoff == FMAN_PORT_OFF )                                \
   _imgp->imr |= FMAN_IMGP_IMR_GRACEFUL_STOP;                  \
  else                                                         \
   _imgp->imr &= ~FMAN_IMGP_IMR_GRACEFUL_STOP;                 \
  HAL_MEMORY_BARRIER();                                        \
}

#define FMAN_ETH_RX_PORT_ONOFF( _fman, _p, _imgp, _onoff, _im) \
{                                                              \
  struct fman_t *fman;                                         \
  fman  = fman_get_ptr(_fman);                                 \
  if( fman ) {                                                 \
    if( _onoff == FMAN_PORT_OFF ) {                            \
      fman_bmi_port_off(&fman->port->p[_p].bmi, FMAN_RX_PORT); \
      if( _im == 0 )                                           \
        fman_qmi_port_off(&fman->port->p[_p].qmi);             \
      if( _im == 1 )                                           \
        FMAN_IMEM_IMGP_SET_GRACEFUL_STOP(_imgp, _onoff);       \
    } else {                                                   \
      if( _im == 1)                                            \
        FMAN_IMEM_IMGP_SET_GRACEFUL_STOP(_imgp, _onoff);       \
      if( _im == 0)                                            \
        fman_qmi_port_on(&fman->port->p[_p].qmi);              \
      fman_bmi_port_on(&fman->port->p[_p].bmi, FMAN_RX_PORT);  \
    }                                                          \
  }                                                            \
}

#define FMAN_ETH_TX_PORT_ONOFF( _fman, _p, _imgp, _onoff, _im) \
{                                                              \
  struct fman_t *fman;                                         \
  fman  = fman_get_ptr(_fman);                                 \
  if( fman ) {                                                 \
    if( _onoff == FMAN_PORT_OFF ) {                            \
      fman_bmi_port_off(&fman->port->p[_p].bmi, FMAN_TX_PORT); \
      if( _im == 0 )                                           \
        fman_qmi_port_off(&fman->port->p[_p].qmi);             \
      if( _im == 1 )                                           \
        FMAN_IMEM_IMGP_SET_GRACEFUL_STOP(_imgp, _onoff);       \
    } else {                                                   \
      if( _im == 1 )                                           \
        FMAN_IMEM_IMGP_SET_GRACEFUL_STOP(_imgp, _onoff);       \
      if( _im == 0 )                                           \
        fman_qmi_port_on(&fman->port->p[_p].qmi);              \
      fman_bmi_port_on(&fman->port->p[_p].bmi, FMAN_TX_PORT);  \
    }                                                          \
  }                                                            \
}

#ifdef CYGPKG_HAL_POWERPC_FREESCALE_DPAA_FMAN_DEBUG_CODE
externC void fman_disp_tx_port( cyg_uint8 , cyg_uint8 );
externC void fman_disp_rx_port( cyg_uint8 , cyg_uint8 );
externC void fman_disp_registers( cyg_uint8 );
#endif

// FPM FMan Controller Event Register
#define FMAN_FPM_FMCEV_RXF                0x80000000 // Receive frame interrupt event
#define FMAN_FPM_FMCEV_BSY                0x40000000 // BD ring busy interrupt event

externC void fman_clear_event( cyg_uint8 , cyg_uint8 , cyg_uint32 );
externC void fman_umask_event( cyg_uint8 , cyg_uint8 , cyg_uint32 );
externC cyg_uint32 fman_get_event( cyg_uint8 , cyg_uint8 );
externC cyg_int8 fman_allocate_event( cyg_uint8 );

#endif
