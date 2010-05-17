/*
 *  A module for creating rebootless custom image boot support.
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
 *  along with 2ndboot-ng.  If not, see <http://www.gnu.org/licenses/>.
 * 
*/

#ifdef __KERNEL__
#include <asm/uaccess.h>
#include <asm/cacheflush.h>
#endif

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/delay.h>

#include "xboot.h"

#ifdef __PLAT_TI_OMAP3430__
#include <mach/omap34xx.h>
#endif

#define CTRL_DEVNAME "xbootctrl"

#define TTBL_NORMAL_MAPPING (PMD_TYPE_SECT | PMD_SECT_AP_WRITE | PMD_SECT_WB)
#define TTBL_DEVICE_MAPPING (PMD_TYPE_SECT | PMD_SECT_AP_WRITE | PMD_SECT_UNCACHED)

struct cdev *xboot_cdev;
dev_t xboot_dev;

/* This function do jump and execute code of *function
 * For this function we need use init function in bootloader */ 
int __attribute__((__naked__)) do_branch(void *bootlist, uint32_t bootsize, uint32_t new_ttbl, void *func) {
	__asm__ volatile (
		"bx r3\n"
	);
	return 0;
}

/* This function do remapping from virtual memory address to physical */
static void tlb_map(uint32_t *table, uint32_t phys, uint32_t virt, size_t sects, uint32_t flags) {
	uint32_t physbase, virtbase;

	physbase = phys >> 20;
	virtbase = virt >> 20;
	while (sects-- > 0) {
		table[virtbase++] = (physbase++ << 20) | flags;
	}
}

/* In this function we need to change all defined constants from mach/plat/mxc
 * to defines from mach/plat-omap/include/omap34xx.h
 * See http://docs.blackfin.uclinux.org/doku.php?id=linux-kernel:drivers:example_on-chip_sram
 */
void build_tlb_table(uint32_t *table) {
	memset(table, 0, 4*4096);
	tlb_map(table, PHYS_OFFSET, PHYS_OFFSET, 64, TTBL_NORMAL_MAPPING);
	tlb_map(table, PHYS_OFFSET, PAGE_OFFSET, 64, TTBL_NORMAL_MAPPING);

/* Hardware specific mapping */
#ifdef __PLAT_TI_OMAP3430__
	tlb_map(table, L2CC_BASE_ADDR, L2CC_BASE_ADDR, 1, TTBL_DEVICE_MAPPING);
#endif
/* End of hardware specific mapping */

}

/* This function point exec stack on entry point of bootloader image in memory
 * and branch to this point 
 */
int xboot_boot(int handle) {
	bootfunc_t boot_entry;
	uint32_t bootsize, listsize;
	void *bootlist;
	uint32_t ttlb_mem, *tlb_table;

/* create aligned tlb_table */
	ttlb_mem = get_high_pages(2);
	if (ttlb_mem == 0) {
		printk("Failed to allocate new TLB table\n");
		return -ENOMEM;
	}
	if (ttlb_mem & 0x3fff) { 						
		tlb_table = (uint32_t*)(((ttlb_mem >> 14) + 1) << 14); 
		printk("unaligned TLB table\n");
		free_high_pages((void*)ttlb_mem, 2);
		return -EINVAL;
	} else {
		tlb_table = (uint32_t*)ttlb_mem;
	}
	build_tlb_table(tlb_table);
/* end of creating tlb_table */

	boot_entry = get_bootentry(&bootsize, handle);
	if (boot_entry == NULL) {
		return -EINVAL;
	}
	bootlist = get_bootlist(&listsize, handle);
	if (bootlist == NULL) {
		return -ENOMEM;
	}
        
/* Boot block - disable everything and jump! */
        preempt_disable();
	local_irq_disable();
	local_fiq_disable();
	do_branch(bootlist, listsize, virt_to_phys(tlb_table), boot_entry);
/* End of boot block */

	return -EBUSY;
}

/* Some ioctrl functions for user-kernel communicating by char device
 * Can be replaced by other mechanisms */
static int xbootctrl_open(struct inode *inode, struct file *file);
static int xbootctrl_release(struct inode *inode, struct file *file);
static int xbootctrl_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
static int xbootctrl_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);

static struct file_operations xbootctrl_ops = {
	.owner		=	THIS_MODULE,
	.open		=	xbootctrl_open,
	.release	=	xbootctrl_release,
	.ioctl		=	xbootctrl_ioctl,
	.write		=	xbootctrl_write,

};

static int xbootctrl_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO CTRL_DEVNAME ": `open' stub called\n");
	return 0;
}

static int xbootctrl_release(struct inode *inode, struct file *file) {
	printk(KERN_INFO CTRL_DEVNAME ": `release' stub called\n");
	return 0;
}

static int xbootctrl_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg) {
	int ret;
	int handle;
	struct xboot_buffer_req buf_req;

	ret = 0;
	switch (cmd) {
		case XBOOT_ALLOCATE_BUFFER:
			if (copy_from_user((void*)&buf_req, (void*)arg, sizeof(struct xboot_buffer_req)) != 0) {
				printk(KERN_WARNING CTRL_DEVNAME ": failed to copy buffer request\n");
				return -EINVAL;
			}
			ret = allocate_buffer(buf_req.tag, buf_req.type, buf_req.attrs, buf_req.size, buf_req.rest);
			break;
		case XBOOT_FREE_BUFFER:
			handle = (int)arg;
			ret = free_buffer(handle);
			break;
		case XBOOT_SELECT_BUFFER:
			handle = (int)arg;
			ret = select_buffer(handle);
			if (ret >= 0) {
				file->f_pos = 0;
			}
			break;
		case XBOOT_BOOT:
			handle = (int)arg;
			ret = xboot_boot(handle);
			break;
		default:
			printk(KERN_INFO CTRL_DEVNAME ": unknown ioctl\n");
			ret = -EINVAL;
			break;
	}
	return ret;
}

static int xbootctrl_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	return buffer_append_userdata(buf, count, ppos);
}

/* Main module init function */
static int __init xboot_init(void)
{
	int ret;
        
        printk(KERN_INFO CTRL_DEVNAME ": Initializing 2ndboot-ng module...\n");
        udelay(30000);
	alloc_chrdev_region(&xboot_dev, 0, 1, CTRL_DEVNAME);
        xboot_cdev = cdev_alloc();
        cdev_init(xboot_cdev, &xbootctrl_ops);
	ret = cdev_add(xboot_cdev, xboot_dev, 1);
	printk(KERN_INFO CTRL_DEVNAME ":  Successfully registered dev\n");
	return 0;
}

/* Main module exit function */
static void __exit xboot_exit(void) {
	cdev_del(xboot_cdev);
	unregister_chrdev_region(xboot_dev, 1);
	return;
}

module_init(xboot_init);
module_exit(xboot_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("2ndboot-ng kernel module");
MODULE_AUTHOR("XVilka");
