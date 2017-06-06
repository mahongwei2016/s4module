#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/io.h>

irqreturn_t key_int(int irq,void  *dev_id)
{
    // 1.检测是否发生了按键中单

    // 2.清除已经发生的按键终端

    // 3.打印按键值
    printk("key down!\n");
    return 0;
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
    request_irq(32,key_int,IRQF_SHARED,"key",0);   
}

static void button_exit()
{
    misc_deregister(&key_miscdev);
    free_irq(32,0);
}

module_init(button_init);
module_exit(button_exit);
