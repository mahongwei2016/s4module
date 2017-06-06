obj-m := helloworld.o add.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
	cp helloworld.ko add.ko ~/tftpboot/
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp*	
