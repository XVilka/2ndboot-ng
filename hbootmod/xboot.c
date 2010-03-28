/*
 *  A testing zone for creating rebootless custom image boot support.
 *
 *  Copyright (C) 2010 XVilka <xvilka at gmail.com>
 *
 *  Inspired by 2ndboot by dimich: http://hg.ezxdev.org/2ndboot/
 *
 *  This file is part of 2ndboot-ng.
 *
 *  2ndboot-ng is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Foobar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *   along with 2ndboot-ng.  If not, see <http://www.gnu.org/licenses/>.
 *
*/

#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <mach/omap34xx.h>

#define CODE 0

static int xprint(void) {
	printk("Jumped to code\n");
	return 0;
}

int __attribute__((__naked__)) boot_image(void) {
	__asm__ volatile (
		"mov r3, #0\n"
	);
	return 0;
}

int __attribute__((__naked__)) do_branch(void *bootlist, uint32_t bootsize, uint32_t new_ttbl, void *func) {
	__asm__ volatile (
		"bx r3\n"
	);
	return 0;
}

static int xboot(void) {
	preempt_disable();
	local_irq_disable();
	local_fiq_disable();
	do_branch(NULL, 1, CODE, &xprint);
	while (1);
	return -EBUSY;
}

int __init xboot_init(void) {
        xboot();
	return 0;
}

void __exit xboot_exit(void) {
	return;
}

module_init(xboot_init);
module_exit(xboot_exit);
