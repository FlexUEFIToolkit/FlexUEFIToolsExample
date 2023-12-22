CC = x86_64-linux-gnu-gcc-11

ifneq ($(KERNELRELEASE),)
	obj-m := FlexUEFIToolkitK.o
else
	KERNELDIR ?= /lib/modules/6.2.0/build/
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules CC=${CC}
endif
.PHONY:clean
clean:
	-rm *.mod.c *.o *.order *.symvers *.ko

