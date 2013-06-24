KERNEL_DIR=../../linux-3.10-rc5/

obj-m += pmtlock.o

pmtlock-objs := \
    src/pmtlock_main.o\
    src/pmtlock_debugfs.o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean

