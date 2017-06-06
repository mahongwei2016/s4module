obj-m := helloworld.o add.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp*	
