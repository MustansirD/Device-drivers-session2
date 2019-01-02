/*
fileName	ex01_char_driver.c
author	:	Mustansir
teamLead:	Rajesh Dommaraju
iDetails	:	
		
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


//----------------------------------------------------------------------------

/*Local Macros*/

#define GPL_LICENSE		"GPL"
#define DRIVER_MODULE_AUTHOR	"SPANIDEA"
#define DRIVER_MODULE_DESC	"character device driver"
#define DRIVER_MODULE_VERSION	"1.0.0"
#define DRIVER_NAME		"ex01_char_driver"

#define mem_size 1024
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
uint8_t *kernel_buffer;


//---------------------------------------------------------------------------

/*Local variables*/

//---------------------------------------------------------------------------

/*Local Functions prototype*/

static int my_open(struct inode *i,struct file *f);
static int my_close(struct inode *i,struct file *f);
static ssize_t my_read(struct file *f,char __user *buf,size_t len,loff_t *off);
static ssize_t my_write(struct file *f,const char __user *buf,size_t len,loff_t *off);


static struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
	.read = my_read,
        .write = my_write
};

//The above initialization of structure members is known as tagged assignment

static int init_func(void);
static void exit_func(void);

//--------------------------------------------------------------------------
/*
function        :int (*open)(struct inode *inode,struct file *filp)
		 Note: The above instructn is for function pointer declared under the 
		file_operations struct in linux/fs.h
desc            :The open method is provided for a driver to do any initialization in 
		 preparation for later operations
		 -check for device specific errors(device not ready or similar hardware problems)
		 -Initialize the device if it opened for thee first time
		 -Update the f_op pointer if necessary??????
		 -Allocate and fill any data structure to be put in filp->private data...????
input_param     :struct inode *inode - This parameter has information related to the cdev 
		 structure and major,minor as well 
		 struct file *filp - 
output_param    :
*/

static int my_open(struct inode *i,struct file *f){

	printk(KERN_INFO "Driver: open()\n");
	printk(KERN_INFO "The process is \"%s\" (pid %i)\n",current->comm,current->pid);
	
	
/*------------------------------------------------------------------------------
function        :void *kmalloc(size_t size,int flags) linux/slab.h
desc            :Allocates  size byte of memory
input_param     :size_t size : Bytes of memory required
		 gfp_t flags : The type of memory to be allocated
		 GFP_KERNEL  : Allocate normal kernel ram.
output_param    :returns a pointer to memory allocated or NULL
*/


	if((kernel_buffer = kmalloc(mem_size,GFP_KERNEL))==0){
		printk(KERN_INFO "Caannot allocate memory in kernel\n");
		return -1;
	}
	printk(KERN_INFO "Driver: open()\n");
	return 0;
}

//----------------------------------------------------------------------------

/*
function        :int release(struct inode *inode,struct file *filp)
desc            :Deallocate anything that open allocated in filp->private_data...?????
input_param     :struct inode *inode - This parameter has information related to the cdev 
                 structure and major,minor as well 
                 struct file *filp - 
output_param    :
*/

static int my_close(struct inode *i,struct file *f){
/*------------------------------------------------------------------------------
function        :void kfree(void *ptr)---- linux/slab.h
desc            :Allocated memory gets free 
input_param     :
output_param    :none
*/
	
	
	kfree(kernel_buffer);
	printk(KERN_INFO "Driver:close()\n");
	return 0;
}

//---------------------------------------------------------------------------

/*
function        :ssize_t read(struct file *filp,char __user *buff,size_t count,loff_t *offp)
desc            :
input_param     :struct file *filp	-file pointer
		 char __user *buff	-points to the user buffer where the newly read data 
					 should be placed.
		 size_t count		-size of requested data transfer
		 loff_t *offp		-pointer to a long offset type object that indicates
					 the file position the user is accessing
output_param    :The return value is signed size type
		 - If the 
*/


static ssize_t my_read(struct file *f,char __user *buf,size_t len,loff_t *off){
	
/*
function        :unsigned long copy_to_user(const void _user *to,const void *from,unsigned long n);
desc            :copy data from kernel space to user space
input_param     :const void _user * -Destn address in usr space( a pointer to a const)
		 const void *from - Src address in kernel space
		 unsigned long n  - amount of memory still to be copied
output_param    :negative value if an error occurs and a return value greater then or equal to 
		 0,instead tells the calling program how many bytes have been successfully 
		 transferred(More information on page number 66 of linux devicd driver 3 edition)
		 
*/
/*
	To print the parameters passed by the user spave program
*/		
	int pos;
	printk(KERN_INFO "The number of of bytes to be transferred=%zu\n",len);
	int num_of_bytes;
	num_of_bytes = copy_to_user(buf,kernel_buffer,mem_size);
	if(num_of_bytes<0)
	{
		num_of_bytes = -EFAULT;
	}
	printk(KERN_INFO "Driver:read()\n");
	return num_of_bytes;
}

//----------------------------------------------------------------------------

/*
function        :ssize_t write(struct file *filp,const char __user *buff,size_t count,loff_t *offp);
desc            :copying data from application to device file
input_param     :struct file *filp - file pointer	
		 const char __user *buff - points to the user buffer holding data to be written 
		 size_t count - size of requested data transfer
		 loff_t *offp - pointer to a long offset type object that indicates the file 
		 position user is accessing.
output_param    :signed size type.
*/


static ssize_t my_write(struct file *f,const char __user *buf,size_t len,loff_t *off){

/*
function        :unsigned long copy_from_user(void *to,const void __user *from,
		 unsigned long n);
desc            :copy data from user space to kernel space
input_param     :void *to	- Destn add in kernel space
		 const void __user *from - src addressin user space
		 unsigned long n - Number of bytes to copy
output_param    :returns number of bytes that could not be copied
*/	
	int retval;
	printk("Curent position of file before copying buffer:%u",f->f_pos);
        retval = copy_from_user(kernel_buffer,buf,len);
	if(retval<0){
	retval = -EFAULT;
	}
	f->f_pos = len+1;
	printk("Position of file after writing to it=%u",f->f_pos);
	printk(KERN_INFO "Driver:write()\n");
        return retval;
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

