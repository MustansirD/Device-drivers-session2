#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include<linux/ioctl.h>	//Macros for IOCTL commands

typedef struct
{
   int status,dignity,ego;
}query_arg_t;

/*
IOCTL Commands

#define "ioctl_cmd" _IOX('magic number',command number,argument)

magic number is a unique number

_IO -icotl with no para
_IOW - an ioctl with write para
_IOR - an ioctl with read para
_IOWR - an ioctl with write and read para

*/

#define QUERY_GET_VARIABLES _IOR('q',1,query_arg_t*)
#define QUERY_CLR_VARIABLES _IO('q',2)
#define QUERY_SET_VARIABLES _IOW('q',3,query_arg_t*)

#endif


