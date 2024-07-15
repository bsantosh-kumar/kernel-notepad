MODULE_NAME:=clipboard-history
obj-m := ${MODULE_NAME}.o
all:
	make -C /lib/modules/$(shell uname -r)/build M='$(PWD)' modules
	sudo insmod clipboard-history.ko
	sudo mknod -m 666 /dev/char_dev c 245 0
	gcc interface.c -o interface.out
clean:
	sudo rm -f /dev/char_dev interface.out 
	sudo rmmod clipboard-history
	make -C /lib/modules/$(shell uname -r)/build M='$(PWD)' clean