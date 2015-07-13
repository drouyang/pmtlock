obj-m += fairbench.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install: all
	sudo insmod fairbench.ko
remove:
	sudo rmmod fairbench
