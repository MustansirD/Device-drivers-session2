/*
fileName	ex02_null_driver.c
author	:	Mustansir
teamLead:	Rajesh Dommaraju
Details	:	To create a device file automatically in the /dev file on 
		loading the module into the kernel
		
License	:	Spanidea Systems Pvt. Ltd.
*/

//---------------------------------------------------------------------------

/*INCLUDES*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/fs.h> //Contains function to register/unregister a device


//----------------------------------------------------------------------------

/*Local Macros*/

#define GPL_LICENSE		"GPL"
#define DRIVER_MODULE_AUTHOR	"SPANIDEA"
#define DRIVER_MODULE_DESC	"Automatic creation of device file on module load"
#define DRIVER_MODULE_VERSION	"1.0.0"
#define DRIVER_NAME		"ex02_null_driver"

//----------------------------------------------------------------------------

/*Local typedef */

//---------------------------------------------------------------------------

/*Global variables*/

static dev_t first;		//Global variable for the first device number
static struct class *cl;	//Global variable for device class
static struct cdev mycdev;	//Global variable for the character device 
				//structure
static int result;		// To determine the success/failure when 
				// registering a device
static struct file_operations fops = {};

//---------------------------------------------------------------------------

/*Local variables*/

//---------------------------------------------------------------------------

/*Local Functions prototype*/

static int init_func(void);
static void exit_func(void);

//---------------------------------------------------------------------------

/*
function        :cleanup
desc            :
input_param     :
output_param    :
*/

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
	//-------------------------------------------------------------------
	/*
	function name: register_chrdev
	desc           :The function asks the kernel to assign an available 
			major device number
	input_param     :none
	output_param    :none

	*/
	//Major = register_chrdev(0,"ex01_null_driver",&fops);
	//-------------------------------------------------------------------
	
	/*
	function name	:alloc_chrdev_region
        desc           	:Dynamic allocation of major number
        input_param     :address of ?,first minor number, count,name of driver
        output_param    :returns 0 on successful creation and -1 on failure

        */
	// cat /proc/devices
	result = alloc_chrdev_region(&first,0,1,"abc_driver");
	
	printk("Major number is:%d\t Minor number is:%d\n",MAJOR(first),MINOR(first));

	// ls /sys/class
	cl = class_create( THIS_MODULE, "abc_driver");
	//"chardev" that will displayed under /sys/class


	// ls /dev
	device_create(cl,NULL,first,NULL,"abc_driver");
	//"abc_driver" will be the device that will be dispalyed under /dev
	
	cdev_init(&mycdev,&fops);

	cdev_add(&mycdev,result,1);

	//if(Major<0){
	//printk(KERN_ALERT "Registering device failed:%d\n",Major);
	//}
	//printk("Registering device successful\n");
	
	
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
        printk(KERN_INFO "In the exit function of ex02_null_driver module\n");
	//------------------------------------------------------------------	
	/*
        function name	:
        desc           	: 
                         
        input_param     :
        output_param    :

        */
	cdev_del(&mycdev);
	device_destroy(cl,first);
	class_destroy(cl);

	unregister_chrdev_region(first,1);

	//------------------------------------------------------------------

	printk(KERN_INFO "device stopped");
}

//----------------------------------------------------------------------------

module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_VERSION(DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);

