
Microblaze Arch HAL:
====================

When CYGHWR_HAL_MICROBLAZE_BRAM_STORE_VECTORS is activated, the eCos for Microblaze 
requires 8KB of BRAM used to setup all exception vectors.
Otherwise, the vectors are stored in external FLASH which requires the FLASH base 
address to be set to 0x0. The spartan3adsp PLF HAL implementation does not support
to disable CYGHWR_HAL_MICROBLAZE_BRAM_STORE_VECTORS at the moment. Vectors must be
located in BRAM. The spartan3an PLF HAL implementation support vectors in BRAM 
or external FLASH.

The Microblaze core MUST have support for MSR clear/set instructions (C_USE_MSR_INSTR)

The Microblaze interrupt controller MUST have support for SIE and CIE registers 
(C_HAS_CIE, C_HAS_CIE)


GCC:
====================

Use following GCC option if microblaze supports barrel shifter: -mxl-barrel-shift

!! Pattern compare seems broken with GCC 4.5 (-mxl-pattern-compare), the following 
code in redboots fails (not re-tested with GCC 4.6.3):

    for (init_entry = __RedBoot_INIT_TAB__; init_entry != &__RedBoot_INIT_TAB_END__;  init_entry++) {
        (*init_entry->fun)();
    }

Latest toolchain used is GCC 4.6.3 / GDB 7.4 / Binutil 2.22. Works fine against 
Xilinx EDK 12.2. GDB does not work properly with the Xilinx EDK 13.X series.


GDB:
====================

Reset the target in XMD console before loading the application with GDB:

XMD% rst


TODO:
====================

There is presently no GDB stub support in eCos for Microblaze. Debugging requires 
the MDM (Microblaze Debug Module) to be present in the FPGA design. This is done by 
default.

A script converting the Xilinx system.mhs to an eCos .ecm file should be added to
prevent mismatch between the Firmware (FPGA) and Software.

More info @ http://www.open-etech.com/Tech/FPGA/Microblaze/

