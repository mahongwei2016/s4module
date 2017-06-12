obj-m := bus.o driver.o device.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
	cp bus.ko driver.ko device.ko ~/tftpboot/
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp*
