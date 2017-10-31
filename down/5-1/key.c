#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/input.h>

#define GPNCON (volatile unsigned long*)0x7f008830
#define GPNDAT (volatile unsigned long*)0x7f008834
struct work_struct *work1;
struct timer_list key_timer;
wait_queue_head_t key_q;
unsigned int *gpio_data;
unsigned int key_num=0;
struct input_dev *button_dev;

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
		input_report_key(button_dev,KEY_1,1);
	}
	key_val=readl(gpio_data)&0x04;
	if(key_val==0)
	{
		printk("key3 down!\n");	
		key_num=3;
		input_report_key(button_dev,KEY_1,0);
	}
	input_sync(button_dev);
}

irqreturn_t key_int(int irq,void  *dev_id)
{
    // 1.检测是否发生了按键中单

    // 2.清除已经发生的按键终端

    // 3.打印按键值
    //printk("1\n");
    schedule_work(work1);
    return IRQ_HANDLED;
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

static int button_init()
{  
	int ret;
	/*分配输入型设备结构*/
	button_dev=input_allocate_device();
	/*申明所支持的事件类型*/
	set_bit(EV_KEY,button_dev->evbit);
	/*申明可能上报的键编号*/
	set_bit(KEY_1,button_dev->keybit);
	set_bit(KEY_3,button_dev->keybit);
	/*注册输入型设备*/
	input_register_device(button_dev);
	//注册中断处理函数
	request_irq(IRQ_EINT(0),key_int,IRQF_TRIGGER_FALLING,"key",(void *)1);
	request_irq(IRQ_EINT(2),key_int,IRQF_TRIGGER_FALLING,"key",(void *)3);
	//硬件初始化
	key_hw_init();
	//创建工作
	work1=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(work1,work1_func);
	//注册一个定时器
	init_timer(&key_timer);
	key_timer.function=key_timer_func;
    return 0;	
}

static void button_exit()
{
    input_unregister_device(button_dev);
}

module_init(button_init);
module_exit(button_exit);
