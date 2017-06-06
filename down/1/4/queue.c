#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL"); 

struct workqueue_struct *my_wq;
struct work_struct *work1;
struct work_struct *work2;

void work1_func(struct work_struct *work)
{
	printk("this is work1->\n");
}

void work2_func(struct work_struct *work)
{
	printk("this is work2->\n");
}

int init_que()
{
	//创建工作队列
	//my_wq=create_workqueue("my_que"); 
	//创建工作
	work1=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(work1,work1_func); 
	work2=kmalloc(sizeof(struct work_struct),GFP_KERNEL);
	INIT_WORK(work2,work2_func); 
	//挂载工作
	//queue_work(my_wq,work1);
	//queue_work(my_wq,work2);
	schedule_work(work1);
	schedule_work(work2);
	
	return 0;
}

void clean_que()
{
	
}

module_init(init_que);
module_exit(clean_que);
