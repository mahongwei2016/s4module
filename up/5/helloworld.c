#include <linux/init.h>
#include <linux/module.h>
MODULE_AUTHOR("MHW");
MODULE_LICENSE("GPL");
int a=3;
module_param(a,int,S_IRUGO|S_IWUSR);
extern int add(int a,int b);
static int hello_init(void)
{
    printk(KERN_WARNING"hello world\n");
    printk(KERN_WARNING"a=%d\n",a);
    printk(KERN_WARNING"a+10=%d\n",add(a,10));
    return 0;
}
static void hello_exit(void)
{
    printk(KERN_WARNING"Goodbye,world!");
}
module_init(hello_init);
module_exit(hello_exit);
