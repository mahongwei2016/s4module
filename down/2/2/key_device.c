#include<linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

MODULE_LICENSE("GPL");

#define GPNCON 0x7f008830

struct resource key_resource[] = {
	[0] = {
		.start = GPNCON,	
		.end = GPNCON + 8,
		.flags = IORESOURCE_MEM, 
	},
	[1] = {
		.start = IRQ_EINT(0),
		.end = IRQ_EINT(2),
		.flags = IORESOURCE_IRQ,	
	},
};

struct platform_device key_device={
	.name = "my-key",
	.id = 0,
	.num_resources = 2,
	.resource = key_resource,
};

int keydev_init(void)
{
	platform_device_register(&key_device);
	return 0;
}

void keydev_exit(void)
{
	platform_device_unregister(&key_device);
}

module_init(keydev_init);
module_exit(keydev_exit); 
