/*
 * Board specific setup info
 *
 * (C) Copyright 2004-2006
 * Texas Instruments, <www.ti.com>
 * Richard Woodruff <r-woodruff2@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* DPLL(1-4) PARAM TABLES */
/* Each of the tables has M, N, FREQSEL, M2 values defined for nominal
 * OPP (1.2V). The fields are defined according to dpll_param struct(clock.c).
 * The values are defined for all possible sysclk and for ES1 and ES2.
 */

mpu_dpll_param:
/* 12MHz */
/* ES1 */
.word 0x0FE,0x07,0x05,0x01
/* ES2 */
.word 0x0FA,0x05,0x07,0x01
/* 3410 */
.word 0x085,0x05,0x07,0x01

/* 13MHz */
/* ES1 */
.word 0x17D,0x0C,0x03,0x01
/* ES2 */
.word 0x1F4,0x0C,0x03,0x01
/* 3410 */
.word 0x10A,0x0C,0x03,0x01

/* 19.2MHz */
/* ES1 */
.word 0x179,0x12,0x04,0x01
/* ES2 */
.word 0x271,0x17,0x03,0x01
/* 3410 */
.word 0x14C,0x17,0x03,0x01

/* 26MHz */
/* ES1 */
.word 0x17D,0x19,0x03,0x01
/* ES2 */
.word 0x0FA,0x0C,0x07,0x01
/* 3410 */
.word 0x085,0x0C,0x07,0x01

/* 38.4MHz */
/* ES1 */
.word 0x1FA,0x32,0x03,0x01
/* ES2 */
.word 0x271,0x2F,0x03,0x01
/* 3410 */
.word 0x14C,0x2F,0x03,0x01


iva_dpll_param:
/* 12MHz */
/* ES1 */
.word 0x07D,0x05,0x07,0x01
/* ES2 */
.word 0x0B4,0x05,0x07,0x01
/* 3410 */
.word 0x085,0x05,0x07,0x01

/* 13MHz */
/* ES1 */
.word 0x0FA,0x0C,0x03,0x01
/* ES2 */
.word 0x168,0x0C,0x03,0x01
/* 3410 */
.word 0x10A,0x0C,0x03,0x01

/* 19.2MHz */
/* ES1 */
.word 0x082,0x09,0x07,0x01
/* ES2 */
.word 0x0E1,0x0B,0x06,0x01
/* 3410 */
.word 0x14C,0x17,0x03,0x01

/* 26MHz */
/* ES1 */
.word 0x07D,0x0C,0x07,0x01
/* ES2 */
.word 0x0B4,0x0C,0x07,0x01
/* 3410 */
.word 0x085,0x0C,0x07,0x01

/* 38.4MHz */
/* ES1 */
.word 0x13F,0x30,0x03,0x01
/* ES2 */
.word 0x0E1,0x17,0x06,0x01
/* 3410 */
.word 0x14C,0x2F,0x03,0x01


/* Core DPLL targets for L3 at 166 & L133 */
core_dpll_param:
/* 12MHz */
/* ES1 */
.word M_12_ES1,M_12_ES1,FSL_12_ES1,M2_12_ES1
/* ES2 */
.word M_12,N_12,FSEL_12,M2_12
/* 3410 */
.word M_12,N_12,FSEL_12,M2_12

/* 13MHz */
/* ES1 */
.word M_13_ES1,N_13_ES1,FSL_13_ES1,M2_13_ES1
/* ES2 */
.word M_13,N_13,FSEL_13,M2_13
/* 3410 */
.word M_13,N_13,FSEL_13,M2_13

/* 19.2MHz */
/* ES1 */
.word M_19p2_ES1,N_19p2_ES1,FSL_19p2_ES1,M2_19p2_ES1
/* ES2 */
.word M_19p2,N_19p2,FSEL_19p2,M2_19p2
/* 3410 */
.word M_19p2,N_19p2,FSEL_19p2,M2_19p2

/* 26MHz */
/* ES1 */
.word M_26_ES1,N_26_ES1,FSL_26_ES1,M2_26_ES1
/* ES2 */
.word M_26,N_26,FSEL_26,M2_26
/* 3410 */
.word M_26,N_26,FSEL_26,M2_26

/* 38.4MHz */
/* ES1 */
.word M_38p4_ES1,N_38p4_ES1,FSL_38p4_ES1,M2_38p4_ES1
/* ES2 */
.word M_38p4,N_38p4,FSEL_38p4,M2_38p4
/* 3410 */
.word M_38p4,N_38p4,FSEL_38p4,M2_38p4

/* PER DPLL values are same for both ES1 and ES2 */
per_dpll_param:
/* 12MHz */
.word 0xD8,0x05,0x07,0x09

/* 13MHz */
.word 0x1B0,0x0C,0x03,0x09

/* 19.2MHz */
.word 0xE1,0x09,0x07,0x09

/* 26MHz */
.word 0xD8,0x0C,0x07,0x09

/* 38.4MHz */
.word 0xE1,0x13,0x07,0x09

