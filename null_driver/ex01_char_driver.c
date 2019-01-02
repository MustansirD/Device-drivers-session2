/*
fileName	ex01_char_driver.c
author	:	Mustansir
teamLead:	Rajesh Dommaraju
Details	:	
		
License	:	Spanidea Systems Pvt. Ltd.
*/

//---------------------------------------------------------------------------

/*INCLUDES*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/device.h>
#include<linux/cdev.h>	//contains definition for cdev_init and cdev_exit
#include<linux/kdev_t.h>//Macros for major and minor number
#include<linux/fs.h> 	//Contains function to register/unregister a device
			// Contains definition of File_Operations structure


//----------------------------------------------------------------------------

/*Local Macros*/

#define GPL_LICENSE		"GPL"
#define DRIVER_MODULE_AUTHOR	"SPANIDEA"
#define DRIVER_MODULE_DESC	"Automatic creation of device file on module load"
#define DRIVER_MODULE_VERSION	"1.0.0"
#define DRIVER_NAME		"ex03_null_driver"
//----------------------------------------------------------------------------

/*Local typedef */

//---------------------------------------------------------------------------

/*Global variables*/

static dev_t first;		//Global variable for the first device number
static struct class *cl;	//Global variable for device class
static struct cdev mycdev;	//Global variable for the character device 
				//structure
//static int result;		// To determine the success/failure when 
				// registering a device


//---------------------------------------------------------------------------

/*Local variables*/

//---------------------------------------------------------------------------

/*Local Functions prototype*/

static int my_open(struct inode *i,struct file *f);
static int my_close(struct inode *i,struct file *f);
static ssize_t my_read(struct file *f,char __user *buf,size_t len,loff_t *off);
static ssize_t my_write(struct file *f,const char __user *buf,size_t len,loff_t *off);

static struct file_operations fops = {
        .owner = THIS_MODULE,	//owner is a pointer which is initialised to 
				//THiS_MODULE, macro defined in linux/module.h
				//
        .open = my_open,
        .release = my_close,
	.read = my_read,
        .write = my_write
};

static int init_func(void);
static void exit_func(void);

//--------------------------------------------------------------------------
/*
function        :
desc            :
input_param     :
output_param    :
*/

static int my_open(struct inode *i,struct file *f){
	printk(KERN_INFO "Driver: open()\n");
	return 0;
}

//----------------------------------------------------------------------------

/*
function        :
desc            :
input_param     :
output_param    :
*/

static int my_close(struct inode *i,struct file *f){
	printk(KERN_INFO "Driver:close()\n");
	return 0;
}

//---------------------------------------------------------------------------

/*
function        :
desc            :
input_param     :
output_param    :
*/


static ssize_t my_read(struct file *f,char __user *buf,size_t len,loff_t *off){
	printk(KERN_INFO "Driver:read()\n");
	return 0;
}

//----------------------------------------------------------------------------

/*
function        :
desc            :
input_param     :
output_param    :
*/


static ssize_t my_write(struct file *f,const char __user *buf,size_t len,loff_t *off){
        printk(KERN_INFO "Driver:write()\n");
        return 1;
}





//---------------------------------------------------------------------------

/*
function	:init_func
desc		:This is called when the module is loaded into the kernel by the 		 insmod utitity
input_param	:none
output_param	:none
*/ 

static int init_func(void)
{
	printk(KERN_ALERT "In the init function of ex03_null_driver.c file of se		ssion2\n");
	//-------------------------------------------------------------------
	/*
	function name	:alloc_chrdev_region()
	desc          	:Gets dynamically major number in the first parameter 
	input_param     :<major,minor> pair,first minor number,count of minor de			 devices,name of the driver.
	output_param    :int //returns 0 on failure and positive number on 
			 success.

	*/
	// cat /proc/devices
	if(alloc_chrdev_region(&first,0,1,"abc_driver")<0){
	 printk(KERN_ALERT "Regusteration failed");
	 return -1;
	}

	// ls /sys/class
	//"abc_driver" will displayed under /sys/class
	if((cl = class_create(THIS_MODULE,"abc_driver"))==NULL){
	 unregister_chrdev_region(first,1);
	 return -1;
	}

	// ls /dev
	//"abc_driver" will be the device that will be dispalyed under /dev
	if(device_create(cl,NULL,first,NULL,"abc_driver")==NULL){
	 class_destroy(cl);
	 unregister_chrdev_region(first,1);
	 return -1;
	}
	
	
	cdev_init(&mycdev,&fops);
	
	if(cdev_add(&mycdev,first,1)==-1){
		device_destroy(cl,first);
		class_destroy(cl);
		unregister_chrdev_region(first,1);
	
	}
	
	return 0 ;

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
	
	printk(KERN_INFO "In the exit function of ex02_null_driver module\n");

	//------------------------------------------------------------------

	
}

//----------------------------------------------------------------------------


/*
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.write = my_write
};
*/



module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE(GPL_LICENSE);
MODULE_AUTHOR(DRIVER_MODULE_AUTHOR);
MODULE_VERSION(DRIVER_MODULE_VERSION);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);

