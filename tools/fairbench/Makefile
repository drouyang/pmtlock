obj-m += fairbench.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install: all
	sudo insmod fairness.ko
remove:
	sudo rmmod fairness
