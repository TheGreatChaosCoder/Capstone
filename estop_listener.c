/**
 * @file input_listener.c
 * @author Connor Johnson
 * @brief Inplements interrupts for button inputs 
 *        in kernel space.
 * @version 0.1
 * @date 2024-09-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/jiffies.h>
#include "IO_Constants.h"

MODULE_LICENSE("GPL");

#define MSG_SIZE 5

static char button_msg[MSG_SIZE];
static int major;

static ssize_t estop_device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
	// Whatever is in msg will be placed into buffer, which will be copied into user space
	ssize_t dummy = copy_to_user(buffer, button_msg, length); // dummy will be 0 if successful
	printk(KERN_INFO "Here %i\n", buffer[0]);

	return length;
}

// structure needed when registering the Character Device. Members are the callback
// functions when the device is read from or written to.
static struct file_operations fops = {
	.read = device_read,
};

//Interrupt handlers
static irqreturn_t irq_estop_up(int irq,void *dev_id)
{
	msg[0] = SOFT_ESTOP;
	return IRQ_HANDLED;
}

static irqreturn_t irq_estop_down(int irq,void *dev_id)
{
	msg[0] = !SOFT_ESTOP;
	return IRQ_HANDLED;
}


/*
** Module Init function
*/
int init_module()   // Call the default name
{
	// register the Characted Device and obtain the major (assigned by the system)
	major = register_chrdev(0, CDEV_NAME_ESTOP, &fops);
	if (major < 0) {
		printk("Registering the character device failed with %d\n", major);
		return major;
	}
	printk("Create Char Device (node) with: sudo mknod /dev/%s c %d 0\n", CDEV_NAME_ESTOP, major);

	// Verify pins
	if(!gpio_is_valid(SOFT_ESTOP))
    {
		printk(KERN_INFO "EStop Button GPIO Pin is Invalid");
	}

	// Request pins
	if(gpio_request(SOFT_ESTOP, "EStop Button"))
    {
		
		printk(KERN_INFO "EStop Button gpio_request failed");
	}

	// Set direction
	if(gpio_direction_input(SOFT_ESTOP))
    {
		printk(KERN_INFO "EStop Button gpio_direction failed");
	}

 	// Request the interrupt / attach handler
	//Get the IRQ number for our GPIO
    int estop_irqNumber = gpio_to_irq(SOFT_ESTOP);

 	//Enable Async Edge detection
    if (request_irq(estop_irqNumber,(void *)irq_estop_up, IRQF_TRIGGER_RISING,"Button Interrupt", NULL)) {
		pr_err("Cannot register the IRQ for EStop Button, Rising Edge");
		gpio_free(SOFT_ESTOP);
	}
    if (request_irq(estop_irqNumber,(void *)irq_estop_down, IRQF_TRIGGER_FALLING,"Button Interrupt", NULL)) {
		pr_err("Cannot register the IRQ for EStop Button, Falling Edge");
		gpio_free(SOFT_ESTOP);
	}
	
	pr_info("Module Inserted!!!\n");
	return 0;

}

/*
** Module exit function
*/
void cleanup_module()
{
	// Free the Pins
    gpio_free(SOFT_ESTOP)

	// Once unregistered, the Character Device won't be able to be accessed,even if the file /dev/YourDevName still exists.
	unregister_chrdev(major, CDEV_NAME_ESTOP);
	printk("Char Device /dev/%s unregistered.\n", CDEV_NAME_ESTOP);

	printk("*************Module Removed**********\n");
}