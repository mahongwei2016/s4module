obj-m :=  key.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
	cp  key.ko ~/tftpboot/
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp*
