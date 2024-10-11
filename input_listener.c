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

static ssize_t button_device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
	// Whatever is in msg will be placed into buffer, which will be copied into user space
	ssize_t dummy = copy_to_user(buffer, button_msg, length); // dummy will be 0 if successful
	printk(KERN_INFO "Here %i\n", buffer[0]);

	return length;
}

static ssize_t button_device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
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

//Interrupt handler. This will be called whenever there is a rising edge detected.
static irqreturn_t irq_unload(int irq,void *dev_id)
{
	msg[0] = UNLOAD_BUTTON;
	return IRQ_HANDLED;
}

static irqreturn_t irq_load(int irq,void *dev_id)
{
	msg[0] = LOAD_BUTTON;
	return IRQ_HANDLED;
}

/*
** Module Init function
*/
int init_module()   // Call the default name
{
	// register the Characted Device and obtain the major (assigned by the system)
	major = register_chrdev(0, CDEV_NAME_BUTTON, &fops);
	if (major < 0) {
		printk("Registering the character device failed with %d\n", major);
		return major;
	}
	printk("Create Char Device (node) with: sudo mknod /dev/%s c %d 0\n", CDEV_NAME_BUTTON, major);

	// Verify pins
	if(!gpio_is_valid(LOAD_BUTTON) || !gpio_is_valid(UNLOAD_BUTTON))
    {
		printk(KERN_INFO "A Button GPIO Pin is Invalid");
	}

	// Request pins
	if(gpio_request(LOAD_BUTTON, "Load Button") || gpio_request(UNLOAD_BUTTON, "Unload Button"))
    {
		
		printk(KERN_INFO "Button gpio_request failed");
	}

	// Set direction
	if(gpio_direction_input(LOAD_BUTTON) || gpio_direction_input(UNLOAD_BUTTON))
    {
		printk(KERN_INFO "Button gpio_direction failed");
	}

 	// Request the interrupt / attach handler
	//Get the IRQ number for our GPIO
	int unload_irqNumber = gpio_to_irq(UNLOAD_BUTTON);
	int load_irqNumber = gpio_to_irq(LOAD_BUTTON);

 	//Enable (Async) Rising Edge detection
	if (request_irq(unload_irqNumber,(void *)irq_unload, IRQF_TRIGGER_RISING,"Button Interrupt", NULL)) {
		pr_err("Cannot register the IRQ for Unload Button");
		gpio_free(UNLOAD_BUTTON);
	}
	if (request_irq(load_irqNumber,(void *)irq_load, IRQF_TRIGGER_RISING,"Button Interrupt", NULL)) {
		pr_err("Cannot register the IRQ for Load Button");
		gpio_free(LOAD_BUTTON);
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
	gpio_free(LOAD_BUTTON);
	gpio_free(UNLOAD_BUTTON);

	// Once unregistered, the Character Device won't be able to be accessed,even if the file /dev/YourDevName still exists.
	unregister_chrdev(major, CDEV_NAME_BUTTON);
	printk("Char Device /dev/%s unregistered.\n", CDEV_NAME_BUTTON);

	printk("*************Module Removed**********\n");
}