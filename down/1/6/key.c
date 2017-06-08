#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GPNCON (volatile unsigned long*)0x7f008830
#define GPNDAT (volatile unsigned long*)0x7f008834
struct work_struct *work1;
struct timer_list key_timer;
unsigned int *gpio_data;
unsigned int key_num;

void work1_func(struct work_struct *work)
{
	mod_timer(&key_timer,jiffies+HZ/10);
}

void key_timer_func(unsigned long data)
{
	unsigned int key_val;
	key_val=readl(gpio_data)&0x01;
	if(key_val==0)
	{
		printk("key1 down!\n");	
		key_num=1;
	}
	key_val=readl(gpio_data)&0x04;
	if(key_val==0)
	{
		printk("key3 down!\n");	
		key_num=3;
	}
}

irqreturn_t key_int(int irq,void  *dev_id)
{
    // 1.检测是否发生了按键中单

    // 2.清除已经发生的按键终端

    // 3.打印按键值
    //printk("1\n");
    schedule_work(work1);
    return 0;
}
void key_hw_init()
{
    unsigned int *gpio_config;
    unsigned int data;
    gpio_config=ioremap(GPNCON,4);
    data=readl(gpio_config);
	data &=~0b110011;
	data|=0b100010;
	writel(data,gpio_config);
	gpio_data=ioremap(GPNDAT,4);
	
}

int key_open(struct inode *node,struct file *filp)
{
	return 0;
}

ssize_t key_read(struct file *filp,char __user *buf,size_t size, loff_t *pos)
{
	copy_to_user(buf,&key_num,sizeof(key_num));
	return sizeof(key_num);
}

struct file_operations key_fops=
{
	.open=key_open,
	.read=key_read,
};

struct miscdevice key_miscdev=
{
	.minor=200,
	.name="key",
	.fops=&key_fops,
};

static int button_init()
{  
	printk("time:%s\n",__TIME__);
	misc_register(&key_miscdev);
	//按键初始化 
	key_hw_init();
	//注册中断处理函数 
    request_irq(IRQ_EINT(0),key_int,IRQF_TRIGGER_FALLING,"key",0);
	request_irq(IRQ_EINT(2),key_int,IRQF_TRIGGER_FALLING,"key",0);   
    //创建工作 
    work1=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(work1,work1_func); 
	//初始化定时器
	init_timer(&key_timer);
	//设置超时函数
	key_timer.function=key_timer_func; 
	//注册定时器
	add_timer(&key_timer); 
    return 0;	
}

static void button_exit()
{
    misc_deregister(&key_miscdev);
    free_irq(IRQ_EINT(0),0);
}

module_init(button_init);
module_exit(button_exit);
