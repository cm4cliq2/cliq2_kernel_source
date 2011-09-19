/*
 *	tcmd_driver.c
 *
 * Copyright (c) 2010 Motorola
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>
#include <linux/gpio_mapping.h>
#include <linux/tcmd_driver.h>

static int tcmd_misc_open(struct inode *inode, struct file *file)
{
	int err;

	printk(KERN_INFO "Driver for moto tcmd related part - open.\n");

	err = nonseekable_open(inode, file);
	if (err < 0)
		return err;

	return 0;
}

static int tcmd_interrupt_mask(u8 mask, int irq)
{
	if (mask) {
		printk(KERN_INFO "tcmd_interrupt_mask: %d.", irq);
		disable_irq(irq);
	} else {
		printk(KERN_INFO "tcmd_interrupt_unmask: %d.", irq);
		enable_irq(irq);
	}

	return 0;
}


static int tcmd_misc_ioctl(struct inode *inode, struct file *file,
							unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int irq_gpio = 0;
	char *gpio_name_devtree;
	struct tcmd_gpio_int_mask_arg gpio_int_mask_arg;

	printk(KERN_INFO "Driver for moto tcmd related part - ioctl.\n");

	switch (cmd) {
	case TCMD_IOCTL_INT_MASK:
		if (copy_from_user(&gpio_int_mask_arg, argp, 2))
			return -EFAULT;
		if (gpio_int_mask_arg.tcmd_gpio_no >= TCMD_GPIO_INT_MAX_NUM)
			return -EINVAL;

		gpio_name_devtree = (char *)(&tcmd_int_mask_devtree_gpio_name + gpio_int_mask_arg.tcmd_gpio_no * TCMD_GPIO_NAME_MAX_LEN);
		irq_gpio = get_gpio_by_name(gpio_name_devtree);
		if (irq_gpio > 0)
			tcmd_interrupt_mask(gpio_int_mask_arg.mask, gpio_to_irq(irq_gpio));
		else
			return -EFAULT;
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static const struct file_operations tcmd_misc_fops = {
	.owner = THIS_MODULE,
	.open = tcmd_misc_open,
	.ioctl = tcmd_misc_ioctl,
};

static struct miscdevice tcmd_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = FOPS_TCMD_NAME,
	.fops = &tcmd_misc_fops,
};

static int __init tcmd_init(void)
{
	int error = 0;

	printk(KERN_INFO "Driver for moto tcmd related part - init.\n");

	error = misc_register(&tcmd_misc_device);
	if (error < 0) {
		pr_err("%s: tcmd misc register failed!\n", __func__);
		goto error_misc_register_failed;
	}
	return 0;

error_misc_register_failed:
	return error;
}

static void __exit tcmd_exit(void)
{
	misc_deregister(&tcmd_misc_device);
}

module_init(tcmd_init);
module_exit(tcmd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Motorola");
MODULE_DESCRIPTION("Driver for moto tcmd related part");
