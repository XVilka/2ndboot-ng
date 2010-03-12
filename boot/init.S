#include "buffers.h"
#include "mxc91231.h"
.extern buffers_list
.extern font_8x8
.align 4

.macro barrier, reg
	mcr p15, 0, \reg, c7, c10, 4
.endm
.macro cpwait, reg
	mrc p15, 0, \reg, c2, c0, 0
	mov \reg, \reg
	sub pc, pc, #4
.endm
.macro flush_tlb, reg
	mov \reg, #0
	mcr p15, 0, \reg, c8, c5, 0
	mcr p15, 0, \reg, c8, c6, 0
	cpwait \reg
.endm

/* Now we are running with MMU enabled _and_ phys != virt!
 * r0 - pointer to set of buffers.
 * r1 - count of buffers.
 * r2 - pointer to new trnaslation table.
 */
	cpsid   ifa
	sub	r14, r15, #0xc         @ virt addr of entry point
	sub	r14, r14, #0x30000000

	mov	r3, #0

	mcr	p15, 0, r3, c2, c0, 2  @ load new translation table
	mcr	p15, 0, r2, c2, c0, 0
	cpwait	r3
	flush_tlb r2
	
	sub	r2, pc, #0             @ now we are switching to physical adress for smooth mmu turnoff.
	sub	pc, r2, #0x30000000

	b	__with_mmu             @ at that place, pc is physical.

__reloc_start__:

p_buffers_list:
	.word	buffers_list
p_font_8x8:
	.word	font_8x8

__reloc_end__:

fixup_got:
	add     r1, r1, r0
	add     r2, r2, r0
fixup_loop:
	cmp     r1, r2
	beq     fixup_end
	ldr     r3, [r1]
	add     r3, r3, r0
	str     r3, [r1], #4
	b       fixup_loop
fixup_end:
	bx      lr

clean_bss:
	ldr     r1, bss_start
	ldr     r2, bss_end
	add     r1, r1, r0
	add     r2, r2, r0
	mov     r3, #0
clean_loop:
	cmp     r1, r2
	beq     clean_end
	str     r3, [r1], #4
	b       clean_loop
clean_end:
	bx      lr

__with_mmu:
	ldr	sp, sp_svc
	mov	r5, r0
	mov	r6, r1
	mov	r0, r14

	mov	r1, #__reloc_start__
	mov	r2, #__reloc_end__
	bl	fixup_got
	ldr	r1, got_start
	ldr	r2, got_end
	bl	fixup_got
	bl	clean_bss
	
	bl	font_init
	ldr	r0, p_font_8x8
	bl	console_init
	bl	gpt_init
	bl	nfc_init
__buffers_loop:
	subs	r6, r6, #1
	bmi	__buffers_end
	ldr	r4, [r5], #4
	ldrb	r0, [r4]
	bl	find_tag
	cmp	r0, #0
	movne	r1, r4
	blne	proccess_buffer
	b	__buffers_loop
__buffers_end:
	b	_start

find_tag:
	cmp	r0, #IMG_LAST_TAG
	ldrls	r1, p_buffers_list
	addls	r1, r1, r0, lsl #5     @ each entry is 8-words long
	movhi	r1, #0
	mov	r0, r1
	bx	lr

/*
   r0 - destination
   r1 - buffer handle

   returns b_stat_
*/
unpack_buffer:
	stmfd	sp!, {r4,lr}
	ldmia	r1, {r2-r3}
	ldr	r1, [r1, #0xc]
	uxtb	r2, r2, ror #16
	cmp	r2, #2                 @ max type
	movhi	r0, #B_STAT_ERROR
	bhi	__final
	mov	r4, r2
	mov	r2, r3
	ldr	r4, [pc, r4, lsl #2]
	add	pc, pc, r4
__jtable:
	.word	__plain - __jtable - 0x4
	.word	__scatt - __jtable - 0x4
	.word	__nand  - __jtable - 0x4
__plain:
	bl	unpack_plain
	b	__final
__scatt:
	bl	unpack_scattered
	b	__final
__nand:
	bl	unpack_nand
__final:
	ldmfd	sp!, {r4,pc}

proccess_buffer:
	stmfd	sp!, {r4,lr}

	ldmia	r1, {r2-r4}
	stmia	r0, {r2-r4}
	add	r4, r1, #0xc
	str	r4, [r0, #0xc]         @ non-abstract part
	ldr	r4, [r0, #0x18]        @ attrs
	tst     r4, #IMG_ATTR_LAZY
	ldmnefd	sp!, {r4,pc}
	ldr	r4, [r0, #0x14]        @ maxsize
	cmp	r3, r4
	mov	r4, r0
	movhi	r0, #B_STAT_OVERFLOW
	ldrls	r0, [r4, #0x10]
	movls	r1, r4
	blls	unpack_buffer
	strb	r0, [r4, #1]           @ state
	ldmfd	sp!, {r4,pc}

memcpy:
	stmfd	sp!, {r4-r10,lr}
__dqwords_loop:
	cmp	r2, #0x20
	blo	__dqwords_done
	ldmia	r1!, {r3-r10}
	stmia	r0!, {r3-r10}
	subs	r2, r2, #0x20
	beq	__memcpy_done
	b	__dqwords_loop
__dqwords_done:	
	movs	r2, r2, lsl #28
	ldmcsia	r1!, {r3-r6}
	stmcsia	r0!, {r3-r6}
	ldmmiia	r1!, {r3-r4}
	stmmiia	r0!, {r3-r4}

	movs	r2, r2, lsl #2
	ldrcs	r3, [r1], #4
	strcs	r3, [r0], #4
	ldrmih	r3, [r1], #2
	strmih	r3, [r0], #2
	cmp	r2, #0
	ldrneb	r3, [r1], #1
	strneb	r3, [r0], #1
__memcpy_done:
	ldmfd	sp!, {r4-r10,pc}

unpack_scattered:
	stmfd	sp!, {r4-r12,lr}
	cmp	r2, #0
	movlt	r0, #B_STAT_OVERFLOW
	moveq	r0, #B_STAT_NONE
	ble	__scat_final
	ldr	r3, [r1]               @ chunk size
	add	r1, r1, #8
__chunks_loop:
	subs	r2, r2, r3
	bmi	__copy_done
	ldr	r4, [r1], #4           @ new chunk
	mov	r5, r3                 @ chunk size
__copy_loop:
	ldmia	r4!, {r6-r12,lr}
	stmia	r0!, {r6-r12,lr}
	subs 	r5, r5, #0x20
	bne	__copy_loop
	b	__chunks_loop
__copy_done:
	adds	r2, r2, r3
	ldrne	r1, [r1]
	blne	memcpy
	mov	r0, #B_STAT_COMPLETED
__scat_final:
	ldmfd	sp!, {r4-r12,pc}

unpack_plain:
	stmfd	sp!, {lr}
	sub	sp, sp, #4
	ldr	r1, [r1]
	bl	memcpy
	mov	r0, #B_STAT_COMPLETED
	add	sp, sp, #4
	ldmfd	sp!, {pc}

unpack_nand:
	stmfd	sp!, {r4, lr}
	ldr	r1, [r1]
	mov	r4, r2
	bl	nfc_read_data
	cmp	r4, r0
	movne	r0, #B_STAT_ERROR
	moveq	r0, #B_STAT_COMPLETED
	ldmfd	sp!, {r4, pc}
	
_start:
	b	__main

got_start:
	.word	__got_start__
got_end:
	.word	__got_end__
bss_start:
	.word	__bss_start__
bss_end:
	.word	__bss_end__
sp_svc:
	.word	0x92000000
sp_sys:
	.word	0x91f80000
sp_und:
	.word	0x91f7f000
sp_abt:
	.word	0x91f7e000
sp_fiq:
	.word	0x91f7d000
sp_irq:
	.word	0x91f7c000

__main:
	cps     #0x1f @ system
	ldr     sp, sp_sys
	cps     #0x1b @ undef
	ldr     sp, sp_und
	cps     #0x17 @ abort
	ldr     sp, sp_abt
	cps     #0x11  @ fiq
	ldr     sp, sp_fiq
	cps     #0x12  @ irq
	ldr     sp, sp_irq
	cps     #0x13  @ supervisor

	bl	main
	b	__forever

/* r0 - PA of start
   r1 - PA of end 
*/
caches_flush_lines:
	mov	r2, #0
	
	barrier	r2
	tst	r1, #(L1_LINE_LENGTH-1)
	addeq	r2, r1, #L1_LINE_LENGTH
	movne	r2, r1
	mcrr	p15, 0, r2, r0, c14    @ clean and invalidate L1DC range
	mov	r2, #L2CC_BASE_ADDR
	add	r2, r2, #0x7f0         @ clean and invalidate L2C line by PA

	bic	r0, r0, #(L2_LINE_LENGTH-1)
__l2_flush_range:
	cmp	r0, r1
	strlo	r0, [r2]
	addlo	r0, r0, #L2_LINE_LENGTH
	blo	__l2_flush_range
	
	bx	lr

/* Promises not to touch r0-r3 */
__turn_off_mmu:
	mov	r4, #0

	barrier r4
	mcr     p15, 0, r4, c13, c0, 0 @ set zero PID
	mcr     p15, 0, r4, c7, c10, 0 @ clean D cache
	mov	r5, #L2CC_BASE_ADDR

	ldr	r6, [r5, #0x100]       @ disable L2 cache
	bic	r6, r6, #0x1
	str	r6, [r5, #0x100]

	mov	r6, #0xff
	str	r6, [r5, #0x7fc]       @ request L2 cache clean and invalidation
__l2_flush_whole:
	ldr	r6, [r5, #0x7fc]       @ and wait for it.
	cmp	r6, #0
	bne	__l2_flush_whole

	mcr     p15, 0, r4, c7, c5,  0 @ invalidate I cache

	mrc     p15, 0, r5, c1, c0, 0
	bic     r5, r5, #0x0005        @ -dcache & -mmu
	mcr     p15, 0, r5, c1, c0, 0

	mcr     p15, 0, r4, c7, c5, 4  @ flush prefetch buffer
	cpwait 	r4

	mcr     p15, 0, r4, c7, c5, 0  @ invalidate I cache
	mcr     p15, 0, r4, c7, c6, 0  @ invalidate D cache
	mcr     p15, 0, r4, c8, c5, 0  @ invalidate ITLB
	mcr     p15, 0, r4, c8, c6, 0  @ invalidate DTLB
	
	bx	lr

/* r0 - image base addr
   r1 - arch
   r2 - atag list
*/
	
jump_to_linux:
	bl	__turn_off_mmu
	mov	r3, r0
	mov	r0, #0
	bx	r3
__forever:
	b	__forever

.global _start
.global jump_to_linux
.global unpack_buffer
.global caches_flush_lines
