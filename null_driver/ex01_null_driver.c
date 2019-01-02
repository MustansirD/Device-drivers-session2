/*
fileName	ex01_null_driver.c
author	:	Mustansir
teamLead:	Rajesh Dommaraju
Details	:	To understand about registering the device as a character 
		device
		
License	:	Spanidea Systems Pvt. Ltd.
*/

//---------------------------------------------------------------------------

/*INCLUDES*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h> //Contains function to register/unregister a device

//----------------------------------------------------------------------------

/*Local Macros*/

#define GPL_LICENSE		"GPL"
#define DRIVER_MODULE_AUTHOR	"SPANIDEA"
#define DRIVER_MODULE_DESC	"null device driver"
#define DRIVER_MODULE_VERSION	"1.0.0"

//----------------------------------------------------------------------------

/*Local typedef */

//---------------------------------------------------------------------------

/*Global variables*/

static int Major;
static struct file_operations fops = {};

//---------------------------------------------------------------------------

/*Local variables*/

//---------------------------------------------------------------------------

/*Local Functions prototype*/

static int init_func(void);
static void exit_func(void);

//---------------------------------------------------------------------------

/*
function	:init_func
desc		:This is called when the module is loaded into the kernel by the 		 insmod utitity
input_param	:none
output_param	:none
*/ 

static int init_func(void)
{
	printk(KERN_ALERT "In the init function of ex01_null_driver.c file of session2\n");
	
	/*
	function name: register_chrdev
	desc           :The function asks the kernel to assign an available 
			major device number
	input_param     :none
	output_param    :none

	*/
	Major = register_chrdev(0,"ex01_null_driver",&fops);
	if(Major<0){
		printk(KERN_ALERT "Registering device failed:%d\n",Major);
	}
	printk("Registering device successful\n");
	return 0;
}

//---------------------------------------------------------------------------

/*
function        :exit_func
desc            :This is called when the module is unloaded from the kernel by r		 mmod utility
input_param     :none
output_param    :none
*/

static void exit_func(void)
{
        printk(KERN_INFO "In the exit function of ex01_null_driver module\n");
	/*
        function name	: unregister_chrdev
        desc           	:The function asks the kernel to assign an available 
                         major device number
        input_param     :none
        output_param    :none

        */

	unregister_chrdev(Major ,"ex01_null_driver");
	printk(KERN_ALERT "device stopped");
}

//----------------------------------------------------------------------------

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_VERSION(DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);

