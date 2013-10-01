KERNEL_DIR=../../linux/

obj-m += pmtlock.o

pmtlock-objs := \
    pmtlock_main.o\
    pmtlock_debugfs.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

