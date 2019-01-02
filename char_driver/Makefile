obj-m :=ex01_char_driver.o

all:
	make	-C	/lib/modules/$(shell uname -r)/build	M=$(PWD)	modules
	gcc -o test_app test_app.c

clean:
	make	-C	/lib/modules/$(shell uname -r)/build	M=$(PWD)	clean
	rm test_app

