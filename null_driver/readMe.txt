Filename	: ex01_null_driver.c
Description	:The program registers the device under /dev with a major number indicating the device class and minor number indicating the specific device in  that class. Run the "cat /proc/devices" will display the list of device classes defined in the system,including the 

Steps	:1. Load the driver in the kernel space using the insmod utility
	:2. Get the device major number by running the "cat /proc/devices" 
	 command under the terminal.A list of device classes defined in the
	 system including device number for ex01_null_driver will be displayed.
	:3.After successful registeration of device driver as character device
	 does not create a DEVICE FILE under /dev. use the following commands
	 to create a device file under /dev
	 "mknod /dev/ex01_null_driver c 244 0"
	 "chmod 0666 /dev/myrandom"
	 "ls -l /dev/ex01_null_driver"
	 An ouptut will be dipslayed 
