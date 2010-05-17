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
#endif

#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>

#include "xboot.h"
#include "crc32.h"

#define CHUNK_SIZE_ORDER 0
#define PAGES_PER_CHUNK (1 << CHUNK_SIZE_ORDER)
#define CHUNK_SIZE (PAGE_SIZE*PAGES_PER_CHUNK)

#define ABSTRACT(p) ((struct abstract_buffer*)p)
#define BUFFER_TAG(p) (ABSTRACT(p)->tag)
#define BUFFER_STATE(p) (ABSTRACT(p)->state)
#define BUFFER_TYPE(p) (ABSTRACT(p)->type)
#define BUFFER_ATTRS(p) (ABSTRACT(p)->attrs)
#define BUFFER_SIZE(p) (ABSTRACT(p)->size)
#define BUFFER_CHECKSUM(p) (ABSTRACT(p)->checksum)

/* Functions to allocating high mem */
uint32_t get_high_pages(unsigned int order)
{
    return (uint32_t)page_address(alloc_pages(GFP_KERNEL | __GFP_HIGHMEM | __GFP_HIGH, order));
}

void free_high_pages(void *p, unsigned int order)
{
    free_pages((unsigned long)p, order);
}
/* End of functions to allocating high mem */

/* Working with boot parameters */
bootfunc_t get_bootentry(uint32_t *bootsize, int handle)
{
    bootfunc_t ret;

    buf = get_buffer(handle);
    ret = (bootfunc_t)buf->container.plain->data;
    *bootsize = BUFFER_SIZE(buf->container.plain);
    put_buffer(buf);
    return ret;
}

void *get_bootlist(uint32_t *listsize, int handle)
{
    uint32_t *list;
    int i, j = 0;

    spin_lock(&buffers.ctx_lock);
    list = (uint32_t*)buffers.bootlist;
    for (i = 0; i < buffers.used; ++i) {
        struct generic_buffer *buf;
        buf = &buffers.bufs[i];
        if (handle != i && buf->container.generic != NULL) {
            if (BUFFER_ATTRS(buf->container.abstract) & B_ATTR_VERIFY) {
                crc32_final(&BUFFER_CHECKSUM(buf->container.abstract));
            }
            BUFFER_STATE(buf->container.abstract) = B_STAT_CREATED;
            list[j] = (uint32_t)buf->container.generic;
            j++;
        }
    }
    *listsize = j;
    spin_unlock(&buffers.ctx_lock);
    return (void*)list;
}
/* End of working with boot parameters */


int buffer_append_userdata(const char __user *data, size_t size, loff_t *ppos)
{
    struct generic_buffer *buf;
    int ret;
    
    buf = get_buffer(buffers.handle);
    if (buf == NULL) {
        return -EINVAL;
    }
    switch (BUFFER_TYPE(buf->container.abstract)) {
        case B_TYPE_PLAIN:
            ret = append_plain_buffer(buf->container.plain, data, size, *ppos);
            break;
        case B_TYPE_SCATTERED:
            ret = append_scattered_buffer(buf->container.scattered, data, size, *ppos);
            break;
        default:
            ret = -EINVAL;
            break;
    }
    put_buffer(buf);
    if (ret > 0) {
        *ppos += ret;
    }
    return ret;
}



