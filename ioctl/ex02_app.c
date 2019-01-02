#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/ioctl.h>

#include "ex02_header.h"

void get_vars(int fd)
{
	query_arg_t q;
	
	if(ioctl(fd,QUERY_GET_VARIABLES,&q) == -1)
	{
		printf("error for get variables\n");
		perror("query_apps ioctl get");
	}
	else
	{
		printf("Status = %d\n",q.status);
		printf("Dignity = %d\n",q.dignity);
		printf("ego = %d\n",q.ego);		
	}
		
}

void clr_vars(int fd)
{
	if(ioctl(fd,QUERY_CLR_VARIABLES)==-1)
	{	
		printf("error for clear variables");
		perror("query_apps ioctl clr");
	}
}
	
void set_vars(int fd)
{
	int v;
	query_arg_t q;
	
	printf("Enter status:");	
	scanf("%d",&v);	
	getchar();	
	q.status = v;

	printf("Enter dignity:");
        scanf("%d",&v);
        getchar();
        q.dignity = v;

	printf("Enter ego:");
        scanf("%d",&v);
        getchar();
        q.ego = v;

	if(ioctl(fd,QUERY_SET_VARIABLES,&q) == -1)
	{
		printf("error set variables\n");
		perror("query apps ioctl set");
	}

}


int main()
{
	int fd;
	char option;
	
	fd = open("/dev/my_ioctl_driver1",O_RDWR);
	if(fd<0)
	printf("ERROR:%d\n",fd);
	else
	printf("File descriptor = %d\n",fd);
//	while(1)
//	{
		printf("Pls enter option 1.GET_VARIABLES 2.CLR_VARIABLES 3.SET VARIABLES");
		scanf(" %c",&option);
		getchar();
		printf("\noption =%c\n",option);
				
		switch(option)
		{
			case '1':	
					printf("In case 1\n");
					get_vars(fd);
					break;
			case '2':	
					printf("In case 2\n");
					clr_vars(fd);
					break; 
						
			case '3':	
					printf("In case 3\n");
					set_vars(fd);
					break;
			default:	printf("Invalid\n");
		}
		close(fd);
//	}
	return 0;
}
