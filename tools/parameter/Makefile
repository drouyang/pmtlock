obj-m += pmtlock.o

pmtlock-objs := \
    pmtlock_main.o\
    pmtlock_debugfs.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install:
	sudo insmod pmtlock.ko 
remove:
	sudo rmmod pmtlock
cat:
	cat /sys/kernel/debug/pmtlock/pmtlock_timeout_shift

