obj-m := queue.o key.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
	cp queue.ko key.ko ~/tftpboot/
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp*
