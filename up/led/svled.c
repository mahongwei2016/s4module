#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/io.h>
#include "svled.h"
#define LEDIOMUX 0x20e0250
#define LEDIOGDIR 0x20b4004
#define LED0DAT 0x20b4000

unsigned int *led_iomux;
unsigned int *led_iogdir;
unsigned int *led_data;
int bitmap[6]={0};
int opennum=0;
struct cdev cdev;
dev_t devno;


static int led_open(struct inode *inode,struct file *filp)
{
    int num = MINOR(inode->i_rdev);
    //printk("major:%d\nminor:%d\n",MAJOR(inode->i_rdev),MINOR(inode->i_rdev));
    unsigned int gdir;
    filp->private_data=&num;
    //printk("kernel:led:%d\n",num);
	if(bitmap[num]==0)
	{
    led_iomux = ioremap(LEDIOMUX+4*num,4);
    writel(0b0101,led_iomux);
	}
    //printk("read iomux:0x%x\n",readl(led_iomux));
    if(opennum==0)
    {
    led_iogdir = ioremap(LEDIOGDIR,4);
    gdir=readl(led_iogdir);
    //printk("read iogdir:0x%x\n",gdir);
    gdir|=(1<<num);
    writel(gdir,led_iogdir);
    //printk("read iogdir:%x\n",readl(led_iogdir));
    led_data = ioremap(LED0DAT,4);   
	}
	opennum++;
    return 0;

}
int led_release(struct inode *inode,struct file *filp)
{
  int num = MINOR(inode->i_rdev);
  if(bitmap[num]==1)
  {
  iounmap((void *)led_iomux);
  }
  opennum--;
  if(opennum==0)
  {
  iounmap((void *)led_data);
  iounmap((void *)led_iogdir);
  }
  return 0;
}

static long led_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	int *num=filp->private_data;
	int num1=*num;
        unsigned int data;
	//printk("num = %d\n",num1);
	switch(cmd)
	    {
	    case LED0_ON:
		data=readl(led_data);data|=(1<<(0));writel(data,led_data);return 0;
	    case LED0_OFF:
		data=readl(led_data);data&=(~(1<<(0)));writel(data,led_data);return 0;
		case LED1_ON:
		data=readl(led_data);data|=(1<<(1));writel(data,led_data);return 0;
	    case LED1_OFF:
		data=readl(led_data);data&=(~(1<<(1)));writel(data,led_data);return 0;
		case LED2_ON:
		data=readl(led_data);data|=(1<<(2));writel(data,led_data);return 0;
	    case LED2_OFF:
		data=readl(led_data);data&=(~(1<<(2)));writel(data,led_data);return 0;
		case LED3_ON:
		data=readl(led_data);data|=(1<<(3));writel(data,led_data);return 0;
	    case LED3_OFF:
		data=readl(led_data);data&=(~(1<<(3)));writel(data,led_data);return 0;
		case LED4_ON:
		data=readl(led_data);data|=(1<<(4));writel(data,led_data);return 0;
	    case LED4_OFF:
		data=readl(led_data);data&=(~(1<<(4)));writel(data,led_data);return 0;
		case LED5_ON:
		data=readl(led_data);data|=(1<<(5));writel(data,led_data);return 0;
	    case LED5_OFF:
		data=readl(led_data);data&=(~(1<<(5)));writel(data,led_data);return 0;
	    default:
		return -EINVAL;
	    }
}
static const struct file_operations mem_fops=
{
    .open=led_open,
    .release =led_release,
    .unlocked_ioctl=led_ioctl,
};
static int memdev_init(void)
{
    printk("sv led driver,write by mhw!\n");
    cdev_init(&cdev,&mem_fops);
    printk("region %d\n",alloc_chrdev_region(&devno,0,6,"svled"));
    printk("cdev add %d\n",cdev_add(&cdev,devno,6));
    return 0;
}
static void memdev_exit(void)
{
    cdev_del(&cdev);
    unregister_chrdev_region(devno,6);
    printk("remove sv led driver!\n");
}

MODULE_LICENSE("GPL");
module_init(memdev_init);
module_exit(memdev_exit);
