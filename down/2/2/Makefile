obj-m := key_device.o key_driver.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
	cp key_device.ko key_driver.ko ~/tftpboot
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp*
