#include <stdio.h>
#include <stdlib.h>
int main()
{
	int fd;
	int key_num;
	//���豸 
	fd=open("/dev/6410key",0);
	if(fd<0)
	{
		printf("open device fail!\n");
		exit(-1);
	}
	//2.��ȡ�豸
	read(fd,&key_num,4);
	printf("key is %d\n",key_num);
	//3.�ر��豸
	close(fd); 
} 
