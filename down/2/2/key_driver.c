#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>

MODULE_LICENSE("GPL");
struct work_struct *work1;
struct timer_list key_timer;
wait_queue_head_t key_q;
unsigned int *key_base;
unsigned int key_num=0;
struct resource *res_irq;
struct resource *res_mem;

void work1_func(struct work_struct *work)
{
	mod_timer(&key_timer,jiffies+HZ/10);
}

void key_timer_func(unsigned long data)
{
	unsigned int key_val;
	key_val=readl(key_base+1)&0x01;
	if(key_val==0)
	{
		printk("key1 down!\n");	
		key_num=1;
	}
	key_val=readl(key_base+1)&0x04;
	if(key_val==0)
	{
		printk("key3 down!\n");	
		key_num=3;
	}
	wake_up(&key_q);
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
    unsigned int data;
    
    data=readl(key_base);
	data &=~0b110011;
	data|=0b100010;
	writel(data,key_base);
}

int key_open(struct inode *node,struct file *filp)
{
	return 0;
}

ssize_t key_read(struct file *filp,char __user *buf,size_t size, loff_t *pos)
{
	wait_event(key_q,key_num); 
	copy_to_user(buf,&key_num,sizeof(key_num));
	key_num=0;
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

int __devinit key_probe(struct platform_device *pdev)
{
	int ret;
	int size;
	ret=misc_register(&key_miscdev);
	if(ret!=0)
		printk("register fail!");
	//按键初始化 
	res_mem=platform_get_resource(pdev,IORESOURCE_MEM,0);
	size=res_mem->end-res_mem->start+1;
	key_base = ioremap(res_mem->start,size);
	key_hw_init();
	//注册中断处理函数 
	res_irq=platform_get_resource(pdev,IORESOURCE_IRQ,0);
    request_irq(res_irq->start,key_int,IRQF_TRIGGER_FALLING,"key",0);
	request_irq(res_irq->end,key_int,IRQF_TRIGGER_FALLING,"key",0);   
    //创建工作 
    work1=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(work1,work1_func); 
	//初始化定时器
	init_timer(&key_timer);
	//设置超时函数
	key_timer.function=key_timer_func; 
	//注册定时器
	add_timer(&key_timer); 
	//初始化等待队列
	init_waitqueue_head(&key_q); 
	return 0;
}

int __devexit key_remove(struct platform_device *pdev)
{
	misc_deregister(&key_miscdev);
    free_irq(IRQ_EINT(0),0);
} 


struct platform_driver key_driver = {
	.probe = key_probe,
	.remove = key_remove,
	.driver={
		.name="my-key",
		.owner = THIS_MODULE,
	},
	
};

static int button_init()
{  
	printk("time:%s\n",__TIME__);
	int ret=platform_driver_register(&key_driver);
	
    return ret;	
}

static void button_exit()
{
	platform_driver_unregister(&key_driver);
    
}

module_init(button_init);
module_exit(button_exit);
