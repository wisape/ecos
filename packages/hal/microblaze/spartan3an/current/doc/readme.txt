
Redboot:
========

When compiling redboot which can either be a ROM or ROMRAM type of application, the resulting
binary file (redboot.bin) excludes the .vectors section when this section is located in BRAM.
The location of the .vectors section is controlled CYGHWR_HAL_MICROBLAZE_BRAM_STORE_VECTORS.

The binary file must be programmed in the FPGA external FLASH, using JTAG for instance. The EFL file 
(redboot.elf) contains all the sections and is used to initialize the FPGA internal BRAM which will 
contain the vector section. Usually this is done by selecting the redboot.elf file in Xilinx SDK to 
be the default executable to be used to initialize the BRAM. 

fis delete net
load -h 192.168.0.4 httpd1
fis create -f 0x84c40000 -e 0x44100000 net
fis load net
go
