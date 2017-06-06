#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
struct score
{
    int num;
    int english;
    int math;
    struct list_head list;
};
struct list_head head,*pos;
struct score stu1,stu2,stu3,*tmp;
int mylist_init()
{
	INIT_LIST_HEAD(&head);
	stu1.num=1;
	stu1.english=90;
	stu1.math=98;
	list_add(&stu1.list,&head);
	stu2.num=2;
	stu2.english=91;
	stu2.math=91;
	list_add(&stu2.list,&head);
	stu3.num=3;
	stu3.english=70;
	stu3.math=78;
	list_add(&stu3.list,&head);
	list_for_each(pos,&head)
	{
		tmp=list_entry(pos,struct score,list);
		printk("No:%d,english:%d,math:%d\n",tmp->num,tmp->english,tmp->math);	
	}
	
    return 0;
}

void mylist_exit()
{

}
module_init(mylist_init);
module_exit(mylist_exit);
