/*
fileName	ex01_char_driver.c
author	:	Mustansir
teamLead:	Rajesh Dommaraju
Details	:	Workqueue	
		
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
			//
#include<linux/slab.h>	//Contains definition for kmalloc and kfree function
#include<linux/uaccess.h>//Contains definition for copy_to/copy_from user func
#include<linux/sched.h>	//for information regarding current process
#include<linux/interrupt.h>
#include<linux/workqueue.h>//for workqueue

//----------------------------------------------------------------------------

/*Local Macros*/

#define GPL_LICENSE		"GPL"
#define DRIVER_MODULE_AUTHOR	"SPANIDEA"
#define DRIVER_MODULE_DESC	"Interrupts bottom half - workqueue"
#define DRIVER_MODULE_VERSION	"1.0.0"
#define DRIVER_NAME		"workqueue demo"

#define IRQ_NO 1

//----------------------------------------------------------------------------

/*Local typedef */

//---------------------------------------------------------------------------

/*Global variables*/

static dev_t first;		//Global variable for the first device number
static struct class *cl;	//Global variable for device class
static struct cdev mycdev;	//Global variable for the character device 
				//structure
				//struct cdev interfaces our device to the kernel
//static int result;		// To determine the success/failure when 
				// registering a device
static int my_dev1 = 1;
int *my_dev1_ptr = &my_dev1;

//static int my_dev2 = 2;
//int *my_dev2_ptr = &my_dev2;


//---------------------------------------------------------------------------

/*Local variables*/

//---------------------------------------------------------------------------

/*Local Functions prototype*/
void workq_fn(void*);
static irqreturn_t irq_handler1(int,void *);
//static irqreturn_t irq_handler2(int,void *);

static int init_func(void);
static void exit_func(void);

//static struct work_struct workq = {.func=workq_fn};
DECLARE_WORK(workq,workq_fn);//static creation of workqueue

static struct file_operations fops = {};

//-------------------------------------------------------------------------
//---------------------------------------------------------------------------

/*
function	:static irqreturn_t intr_handler(int irq,void *dev)
desc		:This is called when the module is loaded into the kernel by the 				insmod utitity.
input_param	:int irq,the numeric value of interrupt line the handler is servicing
		 dev is generic pointer 
output_param	:irqreturn_t is an int
		 IRQ_NONE - returned when the interrupt handler detects an interrupt for which
		 its device was not originator
		 IRQ_HANDLED,returned if the interrupt handler was correctly invoked,and its
		 device caused the interrupt
*/ 

static irqreturn_t irq_handler1(int irq,void *dev_id1)
{
	printk("unique id of irq_handler1 = %d\n",*((int *)dev_id1));
	printk("Interrupt Occured abc_driver1\n");


	schedule_delayed_work(&workq,100);

	return IRQ_HANDLED;
}

//static irqreturn_t irq_handler2(int irq,void *dev_id2)
//{
//	printk("unique id of irq_handler2 = %d\n",*((int *)dev_id2));
//	printk("Interrupt occured abc_driver2\n");
//	return IRQ_HANDLED;
//}

//-----------------------------------------------------------------------------
/*

function:
desc	:
ip para	:
op para	:
*/

void workq_fn(void *arg)
{
	printk("Executing workqueue function\n");
}



//---------------------------------------------------------------------------

/*
function	:init_func
desc		:This is called when the module is loaded into the kernel by the 				insmod utitity
input_param	:none
output_param	:none
*/ 

static int init_func(void)
{
	printk(KERN_ALERT "In the init function of ex03_null_driver.c file of se		ssion2\n");
	//-------------------------------------------------------------------
	/*
	function name	:int alloc_chrdev_region(dev_t *dev,unsigned int firstmi			 nor,unsigned int count ,char *name);
	desc          	:Gets dynamically major number in the first parameter
	input_param     :dev_t *dev - It is the parameter that will be hold the 			 first number in the allocated range
			 firstminor - is the requested first minor number
			 count - total number of continous device numbers
		 	 requested.
			 name  - name associated with this number range
	output_param    :0 on succesful allocation of device number and a -ve
			 error code on error.

	*/
	// cat /proc/devices
	if(alloc_chrdev_region(&first,0,1,"abc_driver")<0){
	 printk(KERN_ALERT "Regusteration failed");
	 return -1;
	}

	// ls /sys/class
	//"abc_driver" will displayed under /sys/class
/*
function        :struct class * class_create(struct module* owner,const char* name);
desc            :create a struct class
                 
input_param     :struct module *owner -pointer to the module that is own this struct.
                 const char *name - pointer to a string for the name of this class 
output_param    :none
*/
	if((cl = class_create(THIS_MODULE,"abc_driver"))==NULL){
	 unregister_chrdev_region(first,1);
	 return -1;
	}

	// ls /dev
	//"abc_driver" will be the device that will be dispalyed under /dev
/*
function        :struct device * device_create(struct class* class ,struct device* parent,dev_t devt,const char* fmt,... );
desc            :This function can be used by char device class. A struct device will be created
		 in sysfs,registered to the specified class
		 A class can contain device of diff. types like mouse,keyboard,etc. This 
		 structure identifies the device type and contains type-specific informaion
                 
input_param     :struct class* class - pointer to the struct class this device should be 
		 registered to.
		 struct device* parent- pointer to the parent struct device of this new device
		 dev_t devt- The major,minor number device file to be added
		 const char* fmt - string for the device name
		 ...		-variable arguments
                 const char *name - pointer to a string for the name of this class 
output_param    :returns a pointer of  struct device datatype.
*/


	if(device_create(cl,NULL,first,NULL,"abc_driver")==NULL){
	 class_destroy(cl);
	 unregister_chrdev_region(first,1);
	 return -1;
	}
/*
function        :void cdev_init(struct cdev *cdev,struct file_operations *fops);
desc            :Before the kernel invokes the device's operations,we must allocate and register
		 one of these structures. In short use to initialize the structure.
input_param     :struct cdev *cdev - address of struct cdev's object mycdev
		 file_operations *fops - address of struct file_operations's object fops 
output_param    :none
*/

	
	cdev_init(&mycdev,&fops);
	
/*
function        :int cdev_add(struct cdev *cdev,dev_t num,unsigned int count);
desc            :This function is use to tell the kernel to add the device to system and on
		 successful addition it operations can be used by the kernel.
		 Note:Never call cdev_add unitl the driver is ready to handle operations on 
		 device.
input_param     :struct cdev *cdev - address of struct cdev's object mycdev 
		 dev_t num - first device number to which this device 
		 corresponds
		 unsigned int count - number of device numbers associated 
		 with the device.
output_param    :0 or positive value on success and -ve number on failure
*/

	if(cdev_add(&mycdev,first,1)==-1){
		device_destroy(cl,first);
		class_destroy(cl);
		unregister_chrdev_region(first,1);
	
	}

/*
function	:int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
                const char *name, void *dev)
desc		:To register an interrupt handler and enable a given interrupt line for handling
		 with the function request_irq(),which is declared in <linux/interrupt.h>

input para	:unsigned int irq:Specifies the interrupt number to allocate
		 irq_handler_t handler is a function pointer to the actual interrupt handler that
		 services interrupt.This function is invoked when OS receives interrupt
		 typedef irqreturn(*irq_handler_t)(int,void *);

		 The third parameter, flags, can be either zero or a bit mask of one or more of the
		 flags defined in <linux/interrupt.h>. The most important of these flags are:
		 0,IRQF_DISABLED(tells kernel to treat handler as a fast handler and hence the kernel 			disables all other interrupt handlers),IRQF_SAMPLE_RANDOM,
		IRQF_TIMER,IRQF_SHARED,IRQF_TRIGGER_RISING,IRQF_TRIGGER_HIGH
		
		 name of the device assiciated with the interrupt

		 dev used for shared interrupt lines.
		
output para	:0 on success and nonzero on error
*/
		// if(request_irq(IRQ_NO,irq_handler,IRQF_SHARED,"abc_driver",(void *)(irq_handler)))
	if(request_irq(IRQ_NO,irq_handler1,IRQF_SHARED,"abc_driver1",&my_dev1))		{
			printk("Interrupt handler with my_dev1 cannot be regustered\n");
			return -EIO;
		}

/*	
	if(request_irq(IRQ_NO,irq_handler2,IRQF_SHARED,"abc_driver2",&my_dev2))		{
			printk("Interrupt handler with my_dev2 cannot be regustered\n");
			return -EIO;
		}

*/
//--------------------------------------------------------------------------
	


	return 0 ;



}

//---------------------------------------------------------------------------

/*
function        :exit_func
desc            :This is called when the module is unloaded from the kernel by rmmod utility
input_param     :none
output_param    :none
*/



static void exit_func(void)
{
	//------------------------------------------------------------------	
	/*
        function name	:void cdev_del(struct cdev *dev)
        desc           	:To remove a character device from the system.
                         
        input_param     :struct cdev *dev - address of the var of type struct cdev
        output_param    :none

        */

	//------------------------------------------------------------------    
        /*
        function name   :void device_destroy(struct class* class,dev_t devt)
        desc            :removes the device which is created 
                         
        input_param     :struct class* class - pointer to the struct class
			 dev_t devt - major,minor pair with which the device was created.
        output_param    :none

        */

	 //------------------------------------------------------------------    
        /*
        function name   :void class_destroy(struct class* cls)
        desc            : 
                         
        input_param     :pointer to the struct class that is to be destroyed.
        output_param    :none

        */

	 //------------------------------------------------------------------    
        /*
        function name   :void unregister_chrdev_region(dev_t first,unsigned int count)
        desc            : To free the device numbers
                         
        input_param     :dev_t *dev - device number
			 unsigned int count - The number of device allocated
        output_param    :none

        */

//--------------------------------------------------------------------------------------------------
/*
	func		:void free_irq(unsigned int irq.void *dev)
	desc		:To unregister interrupt handler and disable the interrupt line, on driver
			 unloading
	input para	:unsigned int irq:Allocated interrupt number
			 void *dev	 :
	output para	:
*/
//--------------------------------------------------------------------------------------------------
	flush_scheduled_work();
	free_irq(IRQ_NO,&my_dev1);
//	free_irq(IRQ_NO,&my_dev2);
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

