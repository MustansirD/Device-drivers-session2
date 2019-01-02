#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h> 
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/errno.h>
#include<linux/uaccess.h>

#include"ex02_header.h"

#define FIRST_MINOR 0
#define MINOR_CNT 1

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
 int status = 1,dignity = 3,ego = 5;

static int my_open(struct inode* i,struct file* f)
{
        printk(KERN_INFO "in open\n");
	return 0;
}

static int my_close(struct inode* i,struct file* f)
{
        printk(KERN_INFO "in close\n");
        return 0;
}

/*

func	:
desc	:
para	:struct file* - pointer to the file which is passed from application
	cmd - ioctl command passed from user application
	arg - arguments passed to and from the user space
	
ret	:
*/

static long my_ioctl(struct file* f,unsigned int cmd,unsigned long arg)
{
	query_arg_t q;
	
	switch(cmd)
	{
		case QUERY_GET_VARIABLES:
			q.status = status;
			q.dignity = dignity;
			q.ego = ego;
			
			if(copy_to_user((query_arg_t*)arg,&q,sizeof(query_arg_t)))
			{
				return -EACCES;
			}
			break;
		case QUERY_CLR_VARIABLES:
			status = 0;
			dignity = 0;
			ego = 0;
			break;
		case QUERY_SET_VARIABLES:
			if(copy_from_user(&q,(query_arg_t*)arg,sizeof(query_arg_t)))
			{
				return -EACCES;
			}
			status  = q.status;
			dignity = q.dignity;
			ego = q.ego;
			break;	
		default:
			return -EINVAL;
	
	}
	
	return 0;
}

static struct file_operations query_fops = {.owner = THIS_MODULE,.open = my_open,.release = my_close,						.unlocked_ioctl = my_ioctl};


static int init_func(void)
{
	printk(KERN_ALERT "In the init function of ex02.c for ioctl\n");
	
	if(alloc_chrdev_region(&dev,FIRST_MINOR,MINOR_CNT,"my_ioctl_driver")<0)
	{
		printk(KERN_ALERT "Registeration for major and minor number failed");
		return -1;
	}

	cl = class_create(THIS_MODULE,"my_ioctl_driver");
	if(cl == NULL)
	{
		unregister_chrdev_region(dev,1);
		return -1;
	}
	
	if(device_create(cl,NULL,dev,NULL,"my_ioctl_driver1")==NULL)
	{
		class_destroy(cl);
		unregister_chrdev_region(dev,1);
		return -1;
	}

	cdev_init(&c_dev,&query_fops);

	if(cdev_add(&c_dev,dev,1)==-1)
	{
		device_destroy(cl,dev);
		class_destroy(cl);	
		unregister_chrdev_region(dev,1);
	}
	
	return 0;
}

static void exit_func(void)
{
	cdev_del(&c_dev);
	device_destroy(cl,dev);
	class_destroy(cl);
	unregister_chrdev_region(dev,1);
	
	printk(KERN_ALERT "In the exit function of ex02\n");
}

module_init(init_func);
module_exit(exit_func);
MODULE_LICENSE("GPL");



