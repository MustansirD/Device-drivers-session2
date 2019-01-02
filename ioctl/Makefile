obj-m :=ex02_driver.o

all:
	make	-C	/lib/modules/$(shell uname -r)/build	M=$(PWD)	modules
	gcc ex02_app.c -o ex02_app

clean:
	make	-C	/lib/modules/$(shell uname -r)/build	M=$(PWD)	clean
	rm ex02_app


