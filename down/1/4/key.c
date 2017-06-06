#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/slab.h>

#define GPNCON (volatile unsigned long*)0x7f008830
struct work_struct *work1;

void work1_func(struct work_struct *work)
{
	printk("key down!\n");
}

irqreturn_t key_int(int irq,void  *dev_id)
{
    // 1.检测是否发生了按键中单

    // 2.清除已经发生的按键终端

    // 3.打印按键值
    printk("1\n");
    schedule_work(work1);
    return 0;
}
void key_hw_init()
{
    unsigned int *gpio_config;
    unsigned int data;
    gpio_config=ioremap(GPNCON,4);
    data=readl(gpio_config);
	data &=~0b11;
	data|=0b10;
	writel(data,gpio_config);
	
}

int key_open(struct inode *node,struct file *filp)
{
	return 0;
}

struct file_operations key_fops=
{
	.open=key_open,
};

struct miscdevice key_miscdev=
{
	.minor=200,
	.name="key",
	.fops=&key_fops,
};

static int button_init()
{ 
	misc_register(&key_miscdev);
	key_hw_init();
    request_irq(IRQ_EINT(0),key_int,IRQF_TRIGGER_FALLING,"key",0);  
    work1=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(work1,work1_func); 
    return 0;	
}

static void button_exit()
{
    misc_deregister(&key_miscdev);
    free_irq(IRQ_EINT(0),0);
}

module_init(button_init);
module_exit(button_exit);
