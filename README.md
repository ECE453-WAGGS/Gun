# Gun

ZedBoard SSH Password: 1

ece453.dts: The device tree file.  Rebuild using build_dtb

ece453_defconfig: Kernel build configuration.  Do not use this file; instead, run 'make menuconfig' from the linux-digilent/ directory to configure the kernel.

examples: Any examples that might be useful for writing our own code.

images: Images used for creating the bootloader.

imaging: Code to perform the image processing.

SDK: Project for the Xilinx SDK.  This shouldn't be tracked in the repository.  You should delete this folder whenever you need to regenerate the FSBL, because otherwise it doesn't work properly for some reason.

Xilinx: XPS project files.  The only files currently being tracked are the constraints file and the IPM HDL files.

kernel_drivers: The device drivers for our IPMs.  To install a driver, run 'make' inside its subdirectory, copy the .ko over to the ZedBoard, and then run 'insmod foo.ko'.

user_drivers: User-mode drivers for the IPMs.  These should be used to interact with the devices since they expose a nicer interface than writing directly to /dev.
