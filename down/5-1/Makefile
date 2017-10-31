obj-m :=  key.o
KDIR := ${kernel}
all:
	make -C $(KDIR) M=$(PWD) modules
	$(CC) -g -static -o key_app key_app.c
	cp  key.ko key_app  /tftpboot/
	cp  key.ko key_app  /tftpboot/rootfs/modules
clean:
	rm -fr *.o *.ko *.order *.symvers *.mod.c .*.cmd .tmp* key_app
