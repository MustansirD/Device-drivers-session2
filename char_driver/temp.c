#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
	int fd;
	char option;
	
	fd = open("/dev/abc_driver",O_RDWR);
	if(fd<0){
		printf("cannot open device file...\n");
		return 0;
	}

	while(1){
		printf("Pls enter option 1.Write 2.Read 3.Exit");
		scanf("%c",&option);
		printf("\nU have entered the following option:%c,option");
		
		switch(option){
			case '1':	printf("Enter the string to write in dri					ver:");
					scanf("%[^\t\n]s",write_buf);
					write(fd,write_buf,strlen(write_buf)+1);
					break;
					
			case '2':	printf("Data reading\n");
					read(fd,read_buf,1024);
					printf("Data = %s\n\n",read_buf);
					break;
		
			case '3':	close(fd);
					exit(1);
					break;	
					
			default:	printf("Enter a valid option");
		}

	}
	close(fd);
}



