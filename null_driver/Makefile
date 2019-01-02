obj-m :=ex01_null_driver.o
obj-m +=ex02_null_driver.o
obj-m +=ex03_null_driver.o

all:
	make	-C	/lib/modules/$(shell uname -r)/build	M=$(PWD)	modules

clean:
	make	-C	/lib/modules/$(shell uname -r)/build	M=$(PWD)	clean

