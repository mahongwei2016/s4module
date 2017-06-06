obj-m := mylist.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
	cp mylist.ko ~/tftpboot/
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp*
