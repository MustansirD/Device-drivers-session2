/* Necessary includes for device drivers*/

#include<linux/init.h>
#include<linux/config.h>
/*#include</usr/src/linux-headers-4.15.0-29-generic/include/config.h>*/
#include<linux/module.h>
#include<linux/kernel.h>	/*printk()*/
#include<linux/slab.h>		/*kmalloc()*/
#include<linux/fs.h>		/*everything...*/
#include<linux/errno.h>		/* error codes*/
#include<linux/types.h>		/*size_t*/
#include<linux/proc_fs.h>	
#include<linux/fcntl.h>		/*O_ACCMODE*/
/*#include<asm/system.h>*/		/*cli(),*_flag*/
#include<asm/uaccess.h>		/*copy_from/to_user*/

MODULE_LICENSE("Dual BSD/GPL");

/*Declaration of memory.c functions*/

int memory_open(struct inode *inode,struct file *flip);
int memoy_release(struct inode *inode,struct file *flip);
ssize_t memory_read(struct file *flip,char *buf,size_t count,loff_t *f_pos);
ssize_t memory_write(struct file *flip,char *buf,size_t count,loff_t *f_pos);
void memory_exit(void);
int memory_init(void);

/*structure that declares usual file*/
/*access functions*/

struct file_operations memory_fops = {read: memory_read, write:memory_write,open:memory_open,release:memory_release};

/*Declarations of init and exit functions*/

module_init(memory_init);
module_exit(memory_exit);

/*Global variables of the driver*/

/*Major number*/

int memory_major = 60;

/*Buffer to store data*/
char *memory_buffer;	/*pointer to a region in memory*/

/*Definition of functions */
/*In order to link driver file with its corresponding /dev file in kernel space
,the register_chrdev function is used.It is called with three arguments: major number , a string of characters showing the module name, and a file operations structure which links the call with the file functions it defines */

/*--------------------------------------------------------------------------*/

int memory_init(void){

int result;

/*registering a device*/

result = register_chrdev(memory_major,"memory",&memory_fops);
	if(result<0){
	printk("<1>memory:cannot obtain major number %d\n",memory_major);
	return result;


	}



/*Allocating memory for the buffer*/

memory_buffer = kmalloc(1,GFP_KERNEL);

/*kmalloc() function is use for memory allocation of the buffer in the device driver which resides in kernel space.*/

if(!memory_buffer){

result = -ENOMEN;
goto fail;

}

memset(memory_buffer,0,1);

printk("<1>Inserting memory module\n");

return 0;

fail:
	memory_exit();
	return result;

/*If registering the major number or allocatng the memory fails,the module comes under fail label*/

}

/*--------------------------------------------------------------------------*/

void memory_exit(void){
/* Freeing the major number*/

unregistering_chrdev(memory_major,"memory");

/*Freeing buffer memory*/
if(memory_buffer){
kfree(memory_buffer);
}

printk("<1>Removing memory module\n");

}
/*The buffer memory is also freed in this function,in order to leave a clean kernel when removing the device drivers*/

/*---------------------------------------------------------------------------*/

int memory_open(struct inode *inode,struct file *filp){
/*success*/

return 0;

}

/*---------------------------------------------------------------------------*/

int memory_release(struct inode *inode,struct file *filp){
/*success*/

return 0;

}

/*----------------------------------------------------------------------------*/

ssize_t memory_read(struct file *filp,char *buf,size_t count,loff_t *f_pos){


/*Transferring data to user space*/

copy_to_user(buf,memory_buffer,1);

/*Changing reading positions as best suits */

	if(*f_pos == 0){
	*f_pos = *f_pos+1;
	return 1;
	}
	else{
	return 0;
	}

}

/*---------------------------------------------------------------------------*/

ssize_t memory_write(struct file *filp,char *buf,size_t count,loff_t *f_pos){

char *tmp;
tmp = buf+count-1;
copy_from_user(memory_buffer,tmp,1);
return 1;

}


