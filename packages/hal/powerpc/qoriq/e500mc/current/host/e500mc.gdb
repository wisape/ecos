# =====================================================================
#
#     e500mc.gdb
#
# =====================================================================
# ####ECOSHOSTGPLCOPYRIGHTBEGIN####                                         
# -------------------------------------------                               
# This file is part of the eCos host tools.                                 
# Copyright (C) 2012 Free Software Foundation, Inc.                         
#
# This program is free software; you can redistribute it and/or modify      
# it under the terms of the GNU General Public License as published by      
# the Free Software Foundation; either version 2 or (at your option) any    
# later version.                                                            
#
# This program is distributed in the hope that it will be useful, but       
# WITHOUT ANY WARRANTY; without even the implied warranty of                
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
# General Public License for more details.                                  
#
# You should have received a copy of the GNU General Public License         
# along with this program; if not, write to the                             
# Free Software Foundation, Inc., 51 Franklin Street,                       
# Fifth Floor, Boston, MA  02110-1301, USA.                                 
# -------------------------------------------                               
# ####ECOSHOSTGPLCOPYRIGHTEND####                                           
# =====================================================================
######DESCRIPTIONBEGIN####
#
# Author(s):    ccoutand
# Date:         2012-07-20
# Purpose:      Dump /decode registers from target
#
#####DESCRIPTIONEND####
# =====================================================================

define e500mc_msr
  printf "MSR [%p]\n", $msr
  if ( $msr & 0x00000002 )
    printf "  Recoverable exception\n"
  end
  if ( $msr & 0x00000004 )
    printf "  Performance Monitor\n"
  end
  if ( $msr & 0x00000010 )
    printf "  Data Address Space TS1\n"
  else
    printf "  Data Address Space TS0\n"
  end
  if ( $msr & 0x00000010 )
    printf "  Instruction Address Space TS1\n"
  else
    printf "  Instruction Address Space TS0\n"
  end
  if ( $msr & 0x00000100 )
    printf "  Floating-point exception mode 1\n"
  end
  if ( $msr & 0x00000800 )
    printf "  Floating-point exception mode 2\n"
  end
  if ( $msr & 0x00000200 )
    printf "  Debug interrupt enable\n"
  end
  if ( $msr & 0x00000400 )
    printf "  User BTB lock enable\n"
  end
  if ( $msr & 0x00001000 )
    printf "  Machine check enable\n"
  end
  if ( $msr & 0x00002000 )
    printf "  Floating-point available\n"
  end
  if ( $msr & 0x00004000 )
    printf "  Privilage Level\n"
  end
  if ( $msr & 0x00008000 )
    printf "  External Interrupt Enable\n"
  end
  if ( $msr & 0x00020000 )
    printf "  Critical Interrupt Enable\n"
  end
  if ( $msr & 0x00040000 )
    printf "  Wait State Enable\n"
  end
  if ( $msr & 0x02000000 )
    printf "  SPE enable\n"
  end
  if ( $msr & 0x04000000 )
    printf "  User cache lock enable\n"
  end
  if ( $msr & 0x10000000 )
    printf "  Guest State\n"
  end
end

define e500mc_l1cache
end

define e500mc_l2cache
end

define e500mc_l3cache
end

define e500mc_ddr_ecc_status
  if ( $arg0 & 0x80000000 )
    printf "  Multiple memory errors\n"
  end
  if ( $arg0 & 0x00000100 )
    printf "  Address parity error\n"
  end
  if ( $arg0 & 0x00000080 )
    printf "  Automatic calibration error\n"
  end
  if ( $arg0 & 0x00000010 )
    printf "  Corrupted data error\n"
  end
  if ( $arg0 & 0x00000008 )
    printf "  Multiple-bit error\n"
  end
  if ( $arg0 & 0x00000004 )
    printf "  Single-bit ECC error\n"
  end
  if ( $arg0 & 0x00000001 )
    printf "  Memory select error\n"
  end
  printf "  Capture Address Register   0x%08x\n", $arg1
  printf "  Capture Data High Register 0x%08x\n", $arg2
  printf "  Capture Data Low Register  0x%08x\n", $arg3
  printf "  Capture ECC Register       0x%08x\n", $arg4
  printf "  Capture Attributes         0x%08x\n", $arg5
  printf "    Data beat number         %d\n",     (($arg5 & 0x70000000) >> 28)
  printf "    Transaction size         %d\n",     (($arg5 & 0x03000000) >> 24)
  printf "    Transaction source       0x%02x\n", (($arg5 & 0x00FF0000) >> 16)
  printf "    Transaction type         %d\n",     (($arg5 & 0x00003000) >> 12)
end

define e500mc_ecc_config
  if ( $arg0 & 0x80000000 ) == 0 
    printf "  Multiple memory errors      : Active\n"
  end
  if ( $arg0 & 0x00000100 ) == 0
    printf "  Address parity error        : Active\n"
  end
  if ( $arg0 & 0x00000080 ) == 0
    printf "  Automatic calibration error : Active\n"
  end
  if ( $arg0 & 0x00000010 ) == 0
    printf "  Corrupted data error        : Active\n"
  end
  if ( $arg0 & 0x00000008 ) == 0
    printf "  Multiple-bit error          : Active\n"
  end
  if ( $arg0 & 0x00000004 ) == 0
    printf "  Single-bit ECC error        : Active\n"
  end
  if ( $arg0 & 0x00000001 ) == 0
    printf "  Memory select error         : Active\n"
  end
end

define e500mc_ddr1_ecc_status
  printf "Memory Controller 1 ECC errors\n"
  printf "==============================\n"
  set $err_det      = *0xD0008E40
  set $capt_addr    = *0xD0008E50 
  set $capt_data_hi = *0xD0008E20
  set $capt_data_lo = *0xD0008E24
  set $capt_ecc     = *0xD0008E28
  set $capt_attr    = *0xD0008E4C
  if( $err_det == 0 )
    printf "  No Error detected\n"
  end
  e500mc_ddr_ecc_status $err_det $capt_addr $capt_data_hi $capt_data_lo $capt_ecc $capt_attr
end

define e500mc_ddr2_ecc_status
  printf "Memory Controller 2 ECC errors\n"
  printf "==============================\n"
  set $err_det      = *0xD0009E40
  set $capt_addr    = *0xD0009E50 
  set $capt_data_hi = *0xD0009E20
  set $capt_data_lo = *0xD0009E24
  set $capt_ecc     = *0xD0009E28
  set $capt_attr    = *0xD0009E4C
  if( $err_det == 0 )
    printf "  No Error detected\n"
  end
  e500mc_ddr_ecc_status $err_det $capt_addr $capt_data_hi $capt_data_lo $capt_ecc $capt_attr
end

define e500mc_ddr1_ecc_config
  printf "Memory Controller 1 ECC Configuration\n"
  printf "=====================================\n"
  set $detect = *0xD0008E44
  e500mc_ecc_config $detect
end

define e500mc_ddr2_ecc_config
  printf "Memory Controller 2 ECC Configuration\n"
  printf "=====================================\n"
  set $detect = *0xD0009E44
  e500mc_ecc_config $detect
end

document e500mc_msr
Dump and decode MSR special register
end

document e500mc_l1cache
Dump and decode MSR special register
end

document e500mc_l2cache
Dump and decode MSR special register
end

document e500mc_l3cache
Dump and decode MSR special register
end

document e500mc_ddr1_ecc_status
Dump ECC errors from Memory controller 1
end

document e500mc_ddr1_ecc_config
Dump ECC config from Memory controller 1
end

document e500mc_ddr2_ecc_status
Dump ECC errors from Memory controller 2
end

document e500mc_ddr2_ecc_config
Dump ECC config from Memory controller 2
end