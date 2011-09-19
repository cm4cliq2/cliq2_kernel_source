/*
 * Copyright (C) 2009 Motorola, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307, USA
 */
#ifndef _LINUX_TCMD_DRIVER_H__
#define _LINUX_TCMD_DRIVER_H__

#define FOPS_TCMD_NAME "tcmd_driver"

#define TCMD_IO 0x0a
#define TCMD_IOCTL_INT_MASK _IOW(TCMD_IO, 0x01, char)

enum tcmd_int_mask {
	TCMD_INT_MASK_ISL29030 = 0,
	TCMD_GPIO_INT_MAX_NUM
};

#define TCMD_GPIO_NAME_MAX_LEN 80
static char tcmd_int_mask_devtree_gpio_name[TCMD_GPIO_INT_MAX_NUM][TCMD_GPIO_NAME_MAX_LEN] = {"als_int"};

struct tcmd_gpio_int_mask_arg {
	unsigned char mask;
	unsigned char tcmd_gpio_no;
};
#define TCMD_MASK_INT_FLAG 1
#endif /* _LINUX_TCMD_DRIVER_H__ */
