/*
 * jit-rules-x86.h - Rules that define the characteristics of the x86.
 *
 * Copyright (C) 2004  Southern Storm Software, Pty Ltd.
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef	_JIT_RULES_X86_H
#define	_JIT_RULES_X86_H

#ifdef	__cplusplus
extern	"C" {
#endif

/*
 * Pseudo register numbers for the x86 registers.  These are not the
 * same as the CPU instruction register numbers.  The order of these
 * values must match the order in "JIT_REG_INFO".
 */
#define	X86_REG_EAX			0
#define	X86_REG_ECX			1
#define	X86_REG_EDX			2
#define	X86_REG_EBX			3
#define	X86_REG_ESI			4
#define	X86_REG_EDI			5
#define	X86_REG_EBP			6
#define	X86_REG_ESP			7
#define	X86_REG_ST0			8
#define	X86_REG_ST1			9
#define	X86_REG_ST2			10
#define	X86_REG_ST3			11
#define	X86_REG_ST4			12
#define	X86_REG_ST5			13
#define	X86_REG_ST6			14
#define	X86_REG_ST7			15
#define	X86_REG_XMM0			16
#define	X86_REG_XMM1			17
#define	X86_REG_XMM2			18
#define	X86_REG_XMM3			19
#define	X86_REG_XMM4			20
#define	X86_REG_XMM5			21
#define	X86_REG_XMM6			22
#define	X86_REG_XMM7			23

/*
 * Information about all of the registers, in allocation order.
 */
#define	JIT_REG_X86_FLOAT	\
	(JIT_REG_FLOAT32 | JIT_REG_FLOAT64 | JIT_REG_NFLOAT)
#define	JIT_REG_INFO	\
	{"eax", 0, 2, JIT_REG_WORD | JIT_REG_LONG | JIT_REG_CALL_USED}, \
	{"ecx", 1, 3, JIT_REG_WORD | JIT_REG_LONG | JIT_REG_CALL_USED}, \
	{"edx", 2, -1, JIT_REG_WORD | JIT_REG_CALL_USED}, \
	{"ebx", 3, -1, JIT_REG_WORD | JIT_REG_GLOBAL}, \
	{"esi", 6, -1, JIT_REG_WORD | JIT_REG_GLOBAL}, \
	{"edi", 7, -1, JIT_REG_WORD | JIT_REG_GLOBAL}, \
	{"ebp", 4, -1, JIT_REG_FRAME | JIT_REG_FIXED}, \
	{"esp", 5, -1, JIT_REG_STACK_PTR | JIT_REG_FIXED | JIT_REG_CALL_USED}, \
	{"st",  0, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st1", 1, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st2", 2, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st3", 3, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st4", 4, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st5", 5, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st6", 6, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"st7", 7, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED | JIT_REG_IN_STACK}, \
	{"xmm0", 0, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED}, \
	{"xmm1", 1, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED}, \
	{"xmm2", 2, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED}, \
	{"xmm3", 3, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED}, \
	{"xmm4", 4, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED}, \
	{"xmm5", 5, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED}, \
	{"xmm6", 6, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED}, \
	{"xmm7", 7, -1, JIT_REG_X86_FLOAT | JIT_REG_CALL_USED},

#define	JIT_NUM_REGS		24
#define	JIT_NUM_GLOBAL_REGS	3

#define JIT_REG_STACK		1
#define JIT_REG_STACK_START	8
#define JIT_REG_STACK_END	15

/*
 * Define to 1 if we should always load values into registers
 * before operating on them.  i.e. the CPU does not have reg-mem
 * and mem-reg addressing modes.
 */
#define	JIT_ALWAYS_REG_REG		0

/*
 * The maximum number of bytes to allocate for the prolog.
 * This may be shortened once we know the true prolog size.
 */
#define	JIT_PROLOG_SIZE			256

/*
 * Preferred alignment for the start of functions.
 */
#define	JIT_FUNCTION_ALIGNMENT	32

/*
 * Define this to 1 if the platform allows reads and writes on
 * any byte boundary.  Define to 0 if only properly-aligned
 * memory accesses are allowed.
 */
#define	JIT_ALIGN_OVERRIDES		1

/*
 * Parameter passing rules.
 */
#define	JIT_CDECL_WORD_REG_PARAMS		{-1, -1, -1, -1}
#define	JIT_CDECL_FLOAT_REG_PARAMS		{-1, -1, -1, -1}
#define	JIT_FASTCALL_WORD_REG_PARAMS	{1, 2, -1, -1}	/* ecx, edx */
#define	JIT_FASTCALL_FLOAT_REG_PARAMS	{-1, -1, -1, -1}
#define JIT_INTERNAL_WORD_REG_PARAMS    {0, 2, 1, -1}   /* eax, edx, ecx */
#define JIT_INTERNAL_FLOAT_REG_PARAMS   {16, 17, 18, -1}   /* xmm0, xmm1, xmm2 */
#define	JIT_MAX_WORD_REG_PARAMS			3
#define	JIT_MAX_FLOAT_REG_PARAMS		3
#define	JIT_INITIAL_STACK_OFFSET		(2 * sizeof(void *))
#define	JIT_INITIAL_FRAME_SIZE			0

#ifdef	__cplusplus
};
#endif

#endif	/* _JIT_RULES_X86_H */
