/*
 * jit-gen-i486-simd.h: Macros for generating Streaming SIMD Extensions code.
 *
 * Copyright (C) 2007 Free Software Foundation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

#ifndef JIT_GEN_SSE_H
#define JIT_GEN_SSE_H

#include <jit-gen-x86.h>

/*
// The XMM register numbers, for P3 and up chips
*/
typedef enum {
	XMM0 = 0,
	XMM1 = 1,
	XMM2 = 2,
	XMM3 = 3,
	XMM4 = 4,
	XMM5 = 5,
	XMM6 = 6,
	XMM7 = 7,
	XMM_NREG
} XMM_Reg_No;

/*
 * SSE, 3DNow!, SSE2, SSE3 and SSE4 instruction set
 */
#define sse_prefetcht0_mem(inst,mem)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_mem_emit((inst), (X86_ECX), (mem));		\
	} while(0)

#define sse_prefetcht0_membase(inst,basereg,disp)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_membase_emit((inst), (X86_ECX), (basereg), (disp));		\
	} while(0)

#define sse_prefetcht1_mem(inst,mem)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_mem_emit((inst), (X86_EDX), (mem));		\
	} while(0)

#define sse_prefetcht1_membase(inst,basereg,disp)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_membase_emit((inst), (X86_EDX), (basereg), (disp));		\
	} while(0)

#define sse_prefetcht2_mem(inst,mem)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_mem_emit((inst), (X86_EBX), (mem));		\
	} while(0)

#define sse_prefetcht2_membase(inst,basereg,disp)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_membase_emit((inst), (X86_EBX), (basereg), (disp));		\
	} while(0)

#define sse_prefetchnta_mem(inst,mem)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_mem_emit((inst), (X86_EAX), (mem));		\
	} while(0)

#define sse_prefetchnta_membase(inst,basereg,disp)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x18;	\
		x86_membase_emit((inst), (X86_EAX), (basereg), (disp));		\
	} while(0)

#define mmx_sfence(inst)	\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0xae;	\
		x86_reg_emit((inst), 7, 0);	\
	} while(0)

#define _3dnow_prefetchw_mem(inst,mem)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x0D;	\
		x86_mem_emit((inst), (X86_ECX), (mem));		\
	} while(0)

#define _3dnow_prefetchw_membase(inst,basereg,disp)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x0D;	\
		x86_membase_emit((inst), (X86_ECX), (basereg), (disp));		\
	} while(0)

#define _3dnow_prefetchr_mem(inst,mem)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x0D;	\
		x86_mem_emit((inst), (X86_EAX), (mem));		\
	} while(0)

#define _3dnow_prefetchr_membase(inst,basereg,disp)		\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x0D;	\
		x86_membase_emit((inst), (X86_EAX), (basereg), (disp));		\
	} while(0)

#define emit_sse_operand_reg_reg(inst,dreg,reg)	\
	do {	\
		x86_reg_emit((inst), (dreg), (reg));	\
	} while(0)

#define emit_sse_operand_reg_mem(inst,reg,mem)	\
	do {	\
		x86_mem_emit((inst), (reg), (mem));	\
	} while(0)

#define emit_sse_operand_reg_membase(inst,reg,basereg,disp)	\
	do {	\
		x86_membase_emit((inst), (reg), (basereg), (disp));	\
	} while(0)

/*
 * The SSE instruction set is highly regular, so this macro saves
 * a lot of cut&paste
 *
 * Macro parameters:
 *  * prefix: first opcode byte of the intsruction (or 0 if no prefix byte) 
 *  * opcode: last opcode byte of the instruction
 *  * dreg: either a GP or a XMM destination register
 *  * reg: either a GP or a XMM source register
 */

#define emit_sse_instruction_reg_reg(inst,prefix,opcode,dreg,reg)	\
	do {	\
		if (prefix != 0) *(inst)++ = (unsigned char)prefix;	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)opcode;	\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));		\
	} while(0)

#define emit_sse_instruction_reg_mem(inst,prefix,opcode,dreg,mem)	\
	do {	\
		if (prefix != 0) *(inst)++ = (unsigned char)prefix;	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)opcode;	\
		emit_sse_operand_reg_mem((inst), (dreg), (mem));		\
	} while(0)
	
#define emit_sse_instruction_reg_membase(inst,prefix,opcode,dreg,basereg,disp)	\
	do {	\
		if (prefix != 0) *(inst)++ = (unsigned char)prefix;	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)opcode;	\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg), (disp));		\
	} while(0)

#define emit_sse4_instruction_reg_reg(inst,opcode,dreg,reg)	\
	do {	\
		*(inst)++ = (unsigned char)0x66;	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x38;	\
		*(inst)++ = (unsigned char)opcode;	\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));		\
	} while(0)

#define emit_sse4_instruction_reg_mem(inst,opcode,dreg,mem)	\
	do {	\
		*(inst)++ = (unsigned char)0x66;	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x38;	\
		*(inst)++ = (unsigned char)opcode;	\
		emit_sse_operand_reg_mem((inst), (dreg), (mem));		\
	} while(0)
	
#define emit_sse4_instruction_reg_membase(inst,opcode,dreg,basereg,disp)	\
	do {	\
		*(inst)++ = (unsigned char)0x66;	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x38;	\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg), (disp));		\
	} while(0)

#define sse2_addsd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x58), (dreg), (reg));	\
	} while(0)

#define sse2_addsd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase(inst, (0xf2), (0x58), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_addsd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem(inst, (0xf2), (0x58), (dreg), (mem));	\
	} while(0)

#define sse_comiss_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x2f), (dreg), (reg));	\
	} while(0)
	
#define sse_comiss_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x2f), (dreg), (basereg), (disp));	\
	} while(0)
	
#define sse_comiss_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x2f), (dreg), (mem));	\
	} while(0)

#define sse2_comisd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x2f), (dreg), (reg));	\
	} while(0)

#define sse2_comisd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x2f), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_comisd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x2f), (dreg), (mem));	\
	} while(0)

#define sse2_cvtdq2pd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0xe6), (dreg), (reg));	\
	} while(0)

#define sse2_cvtdq2pd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0xe6), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtdq2pd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0xe6), (dreg), (mem));	\
	} while(0)

#define sse_cvtdq2ps_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x5b), (dreg), (reg));	\
	} while(0)

#define sse_cvtdq2ps_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x5b), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtdq2ps_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x5b), (dreg), (mem));	\
	} while(0)

#define sse2_cvtpd2dq_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0xe6), (dreg), (reg));	\
	} while(0)

#define sse2_cvtpd2dq_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0xe6), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtpd2dq_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0xe6), (dreg), (mem));	\
	} while(0)

#define sse2_cvtpd2pi_mmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x2d), (dreg), (reg));	\
	} while(0)

#define sse2_cvtpd2pi_mmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x2d), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtpd2pi_mmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x2d), (dreg), (mem));	\
	} while(0)

#define sse_cvtpd2ps_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x5a), (dreg), (reg));	\
	} while(0)

#define sse_cvtpd2ps_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x5a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtpd2ps_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x5a), (dreg), (mem));	\
	} while(0)

#define sse2_cvtpi2pd_xmreg_mmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x2a), (dreg), (reg));	\
	} while(0)

#define sse2_cvtpi2pd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x2a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtpi2pd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x2a), (dreg), (mem));	\
	} while(0)

#define sse_cvtpi2ps_xmreg_mmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x2a), (dreg), (reg));	\
	} while(0)

#define sse_cvtpi2ps_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x2a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtpi2ps_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x2a), (dreg), (mem));	\
	} while(0)

#define sse2_cvtps2dq_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x5b), (dreg), (reg));	\
	} while(0)

#define sse2_cvtps2dq_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x5b), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtps2dq_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x5b), (dreg), (mem));	\
	} while(0)

#define sse2_cvtps2pd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x5a), (dreg), (reg));	\
	} while(0)

#define sse2_cvtps2pd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x5a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtps2pd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x5a), (dreg), (mem));	\
	} while(0)

#define sse_cvtps2pi_mmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x2d), (dreg), (reg));	\
	} while(0)

#define sse_cvtps2pi_mmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x2d), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtps2pi_mmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x2d), (dreg), (mem));	\
	} while(0)

#define sse_cvtsd2si_reg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x2d), (dreg), (reg));	\
	} while(0)

#define sse_cvtsd2si_reg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0x2d), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtsd2si_reg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0x2d), (dreg), (mem));	\
	} while(0)

#define sse_cvtsd2ss_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x5a), (dreg), (reg));	\
	} while(0)

#define sse_cvtsd2ss_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0x5a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtsd2ss_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0x5a), (dreg), (mem));	\
	} while(0)

#define sse2_cvtsi2sd_xmreg_reg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x2a), (dreg), (reg));	\
	} while(0)

#define sse2_cvtsi2sd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0x2a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtsi2sd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0x2a), (dreg), (mem));	\
	} while(0)

#define sse_cvtsi2ss_xmreg_reg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x2a), (dreg), (reg));	\
	} while(0)

#define sse_cvtsi2ss_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x2a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtsi2ss_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x2a), (dreg), (mem));	\
	} while(0)

#define sse2_cvtss2sd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x5a), (dreg), (reg));	\
	} while(0)

#define sse2_cvtss2sd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x5a), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvtss2sd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x5a), (dreg), (mem));	\
	} while(0)

#define sse_cvtss2si_reg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x2d), (dreg), (reg));	\
	} while(0)

#define sse_cvtss2si_reg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x2d), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvtss2si_reg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x2d), (dreg), (mem));	\
	} while(0)

#define sse2_cvttpd2pi_mmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x2c), (dreg), (reg));	\
	} while(0)

#define sse2_cvttpd2pi_mmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x2c), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvttpd2pi_mmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x2c), (dreg), (mem));	\
	} while(0)

#define sse2_cvttpd2dq_mmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0xe6), (dreg), (reg));	\
	} while(0)

#define sse2_cvttpd2dq_mmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0xe6), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvttpd2dq_mmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0xe6), (dreg), (mem));	\
	} while(0)

#define sse2_cvttps2dq_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x5b), (dreg), (reg));	\
	} while(0)

#define sse2_cvttps2dq_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x5b), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvttps2dq_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x5b), (dreg), (mem));	\
	} while(0)

#define sse_cvttps2pi_mmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x2c), (dreg), (reg));	\
	} while(0)

#define sse_cvttps2pi_mmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x2c), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvttps2pi_mmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x2c), (dreg), (mem));	\
	} while(0)

#define sse2_cvttsd2si_reg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x2c), (dreg), (reg));	\
	} while(0)

#define sse2_cvttsd2si_reg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0x2c), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_cvttsd2si_reg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0x2c), (dreg), (mem));	\
	} while(0)

#define sse_cvttss2si_reg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x2c), (dreg), (reg));	\
	} while(0)

#define sse_cvttss2si_reg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x2c), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_cvttss2si_reg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x2c), (dreg), (mem));	\
	} while(0)

#define sse_fxrstor_mem(inst,mem)	\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_mem((inst), (X86_ECX), (mem));	\
	} while(0)

#define sse_fxrstor_membase(inst,basereg,disp)		\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_membase((inst), (X86_ECX), (basereg), (disp));	\
	} while(0)

#define sse_fxsave_mem(inst,mem)		\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_mem((inst), (X86_EAX), (mem));	\
	} while(0)

#define sse_fxsave_membase(inst,basereg,disp)		\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_membase((inst), (X86_EAX), (basereg), (disp));	\
	} while(0)

#define sse_ldmxcsr_mem(inst,mem)	\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_mem((inst), (X86_EDX), (mem));	\
	} while(0)

#define sse_ldmxcsr_membase(inst,basereg,disp)	\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_membase((inst), (X86_EDX), (basereg), (disp));	\
	} while(0)

#define sse2_maskmovdqu_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0xf7), (dreg), (reg));	\
	} while(0)
#define sse3_monitor(inst)	\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x01;	\
		*(inst)++ = (unsigned char)0xc8;	\
	} while(0)

#define sse2_movapd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x28), (dreg), (reg));	\
	} while(0)

#define sse2_movapd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x28), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movapd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x28), (dreg), (mem));	\
	} while(0)

#define sse2_movapd_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x29), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movapd_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x29), (reg), (mem));	\
	} while(0)

#define sse_movaps_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x28), (dreg), (reg));	\
	} while(0)

#define sse_movaps_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x28), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_movaps_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x28), (dreg), (mem));	\
	} while(0)

#define sse_movaps_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x29), (reg), (basereg), (disp));	\
	} while(0)

#define sse_movaps_mem_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x29), (reg), (mem));	\
	} while(0)

#define sse2_movd_xmreg_reg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x6e), (dreg), (reg));	\
	} while(0)

#define sse2_movd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x6e), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x6e), (dreg), (mem));	\
	} while(0)

#define sse2_movd_reg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x7e), (reg), (dreg));	\
	} while(0)

#define sse2_movd_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x7e), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movd_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x7e), (reg), (mem));	\
	} while(0)

#define sse3_movddup_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x12), (dreg), (dreg));	\
	} while(0)

#define sse3_movddup_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0x12), (dreg), (basereg), (disp));	\
	} while(0)

#define sse3_movddup_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0x12), (dreg), (mem));	\
	} while(0)

#define sse2_movdqa_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x6f), (dreg), (reg));	\
	} while(0)

#define sse2_movdqa_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x6f), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movdqa_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x6f), (dreg), (mem));	\
	} while(0)

#define sse2_movdqa_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x7f), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movdqa_mem_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x7f), (reg), (mem));	\
	} while(0)

#define sse2_movdqu_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x6f), (dreg), (reg));	\
	} while(0)

#define sse2_movdqu_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x6f), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movdqu_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x6f), (dreg), (mem);	\
	} while(0)

#define sse2_movdqu_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x7f), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movdqu_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x7f), (reg), (mem));	\
	} while(0)

#define sse2_movdq2q_mmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0xd6), (dreg), (reg));	\
	} while(0)

#define sse_movhlps_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x12), (dreg), (reg));	\
	} while(0)

#define sse2_movhpd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x16), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movhpd_xmreg_mem(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x16), (dreg), (mem));	\
	} while(0)

#define sse2_movhpd_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x17), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movhpd_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x17), (reg), (mem));	\
	} while(0)

#define sse_movhps_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x16), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_movhps_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x16), (dreg), (mem));	\
	} while(0)

#define sse_movhps_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x17), (reg), (basereg), (disp));	\
	} while(0)

#define sse_movhps_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x17), (reg), (mem));	\
	} while(0)

#define sse_movlhps_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x16), (dreg), (reg));	\
	} while(0)

#define sse2_movlpd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x12), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movlpd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x12), (dreg), (mem));	\
	} while(0)

#define sse2_movlpd_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x13), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movlpd_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x13), (reg), (mem));	\
	} while(0)

#define sse_movlps_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x12), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_movlps_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x12), (dreg), (mem));	\
	} while(0)

#define sse_movlps_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x13), (reg), (basereg), (disp));	\
	} while(0)

#define sse_movlps_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x13), (reg), (mem));	\
	} while(0)

#define sse2_movmskpd_reg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x50), (dreg), (reg));	\
	} while(0)

#define sse_movmskps_reg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x50), (dreg), (reg));	\
	} while(0)

#define sse2_movntdq_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0xe7), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movntdq_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0xe7), (reg), (mem));	\
	} while(0)

#define sse2_movnti_membase_reg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_basereg((inst), (0x0), (0xc3), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movnti_mem_reg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0xc3), (reg), (mem));	\
	} while(0)

#define sse2_movntpd_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x2b), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movntpd_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x2b), (reg), (mem));	\
	} while(0)

#define sse_movntps_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x2b), (reg), (basereg), (disp));	\
	} while(0)

#define sse_movntps_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x2b), (reg), (mem));	\
	} while(0)

#define sse3_movshdup_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x16), (dreg), (reg));	\
	} while(0)

#define sse3_movshdup_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x16), (dreg), (basereg), (disp));	\
	} while(0)

#define sse3_movshdup_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x16), (dreg), (mem));	\
	} while(0)

#define sse3_movsldup_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x12), (dreg), (reg));	\
	} while(0)

#define sse3_movsldup_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x12), (dreg), (basereg), (disp));	\
	} while(0)

#define sse3_movsldup_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x12), (dreg), (mem));	\
	} while(0)

#define sse2_movq_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x7e), (dreg), (reg));	\
	} while(0)

#define sse2_movq_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x7e), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movq_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x7e), (dreg), (mem));	\
	} while(0)

#define sse2_movq_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0xd6), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movq_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0xd6), (reg), (mem));	\
	} while(0)

#define sse2_movq2dq_xmreg_mmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0xd6), (dreg), (reg));	\
	} while(0)

#define sse2_movsd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x10), (dreg), (reg));	\
	} while(0)

#define sse2_movsd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0x10), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_movsd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0x10), (dreg), (mem));	\
	} while(0)

#define sse2_movsd_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf2), (0x11), (reg), (basereg), (disp));	\
	} while(0)

#define sse2_movsd_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf2), (0x11), (reg), (mem));	\
	} while(0)

#define sse_movss_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x10), (dreg), (reg));	\
	} while(0)

#define sse_movss_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x10), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_movss_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x10), (dreg), (mem));	\
	} while(0)

#define sse_movss_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0xf3), (0x11), (reg), (basereg), (disp));	\
	} while(0)

#define sse_movss_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0xf3), (0x11), (reg), (mem));	\
	} while(0)

#define sse_movupd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x10), (dreg), (reg));	\
	} while(0)

#define sse_movupd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x10), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_movupd_xmreg_mem(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x10), (dreg), (mem));	\
	} while(0)

#define sse_movupd_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0x11), (reg), (basereg), (disp));	\
	} while(0)

#define sse_movupd_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0x11), (reg), (mem));	\
	} while(0)

#define sse_movups_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x10), (dreg), (reg));	\
	} while(0)

#define sse_movups_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x10), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_movups_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x10), (dreg), (mem));	\
	} while(0)

#define sse_movups_membase_xmreg(inst,basereg,disp,reg)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x0), (0x11), (reg), (basereg), (disp));	\
	} while(0)

#define sse_movups_mem_xmreg(inst,mem,reg)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x0), (0x11), (reg), (mem));	\
	} while(0)

#define sse3_mwait(inst)	\
	do {	\
		*(inst)++ = (unsigned char)0x0f;	\
		*(inst)++ = (unsigned char)0x01;	\
		*(inst)++ = (unsigned char)0xc9;	\
	} while(0)

#define sse_pextrw_reg_mmreg_imm8(inst,dreg,reg,imm)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0xc5), (dreg), (reg));	\
		*(inst)++ = (unsigned char)imm;		\
	} while(0)

#define sse_pextrw_reg_xmreg_imm8(inst,dreg,reg,imm)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0xc5), (dreg), (reg));	\
		*(inst)++ = (unsigned char)imm;		\
	} while(0)

#define sse_pinsrw_xmreg_reg_imm8(inst,dreg,reg,imm)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0xc4), (dreg), (reg));	\
		*(inst)++ = (unsigned char)((int)(imm) & 0xff);		\
	} while(0)

#define sse_pinsrw_xmreg_membase_imm8(inst,dreg,basereg,disp,imm)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0xc4), (dreg), (basereg),(disp));	\
		*(inst)++ = (unsigned char)((int)(imm) & 0xff);		\
	} while(0)

#define sse_pinsrw_xmreg_mem_imm8(inst,dreg,mem,imm)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0xc4), (dreg), (mem));	\
		*(inst)++ = (unsigned char)((int)(imm) & 0xff);		\
	} while(0)

#define sse2_pshufd_xmreg_xmreg_imm8(inst,dreg,reg,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshufd_xmreg_membase_imm8(inst,dreg,basereg,disp,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg),(disp));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshufd_xmreg_mem_imm8(inst,dreg,mem,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_mem((inst), (dreg), (basereg),(disp));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshufhwd_xmreg_xmreg_imm8(inst,dreg,reg,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0xf3;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshufhwd_xmreg_membase_imm8(inst,dreg,basereg,disp,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0xf3;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg),(disp));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshufhwd_xmreg_mem_imm8(inst,dreg,mem,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0xf3;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_mem((inst), (dreg), (mem));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshuflw_xmreg_xmreg_imm8(inst,dreg,reg,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0xf2;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshuflw_xmreg_membase_imm8(inst,dreg,basereg,disp,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0xf2;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg), (disp));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pshuflw_xmreg_mem_imm8(inst,dreg,mem,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0xf2;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x70;		\
		emit_sse_operand_reg_mem((inst), (dreg), (mem));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_pslldq_xmreg_imm8(inst,dreg,mode)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x73;		\
		emit_sse_operand_reg_reg((inst), (dreg), (XMM7));	\
		*(inst) ++ = (unsigned char)((int)(mode) & 0xff);	\
	} while(0)

#define sse2_psllw_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf1;		\
		emit_sse_operand_reg_membase((inst), (dreg), (reg));		\
	} while(0)

#define sse2_psllw_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf1;		\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg), (disp));		\
	} while(0)

#define sse2_psllw_xmreg_mem(inst,dreg,mem)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf1;		\
		emit_sse_operand_reg_mem((inst), (dreg), (mem));		\
	} while(0)

#define sse2_psllw_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x71;		\
		emit_sse_operand_reg_reg((inst), (XMM6), (dreg));		\
		*(inst) ++ = (unsigned char)((int)(shift) & 0xff);	\
	} while(0)

#define sse2_pslld_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf2;		\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));		\
	} while(0)

#define sse2_pslld_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf2;		\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg),(disp));		\
	} while(0)

#define sse2_pslld_xmreg_mem(inst,dreg,mem)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf2;		\
		emit_sse_operand_reg_mem((inst), (dreg), (mem));		\
	} while(0)

#define sse2_pslld_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x72;		\
		emit_sse_operand_reg_reg((inst), (XMM6), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psllq_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf3;		\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));		\
	} while(0)

#define sse2_psllq_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf3;		\
		emit_sse_operand_reg_membase((inst), (dreg), (basereg), (disp));		\
	} while(0)

#define sse2_psllq_xmreg_mem(inst,dreg,mem)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0xf3;		\
		emit_sse_operand_reg_mem((inst), (dreg), (mem));		\
	} while(0)

#define sse2_psllq_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x73;		\
		emit_sse_operand_reg_reg((inst), (XMM6), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psraw_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0xe1), (dreg), (reg));	\
	} while(0)

#define sse2_psraw_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase((inst), (0x66), (0xe1), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_psraw_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem((inst), (0x66), (0xe1), (dreg), (mem));	\
	} while(0)

#define sse2_psraw_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x71;		\
		emit_sse_operand_reg_reg((inst), (XMM4), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psrad_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x72;		\
		emit_sse_operand_reg_reg((inst), (XMM4), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)


#define sse2_psraw_mmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x71;		\
		emit_sse_operand_reg_reg((inst), (XMM4), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psrad_mmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x72;		\
		emit_sse_operand_reg_reg((inst), (XMM4), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psrldq_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x73;		\
		emit_sse_operand_reg_reg((inst), (XMM3), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psrlw_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x71;		\
		emit_sse_operand_reg_reg((inst), (XMM2), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psrld_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x72;		\
		emit_sse_operand_reg_reg((inst), (XMM2), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_psrlq_xmreg_imm8(inst,dreg,shift)	\
	do {	\
		*(inst) ++ = (unsigned char)0x66;		\
		*(inst) ++ = (unsigned char)0x0f;		\
		*(inst) ++ = (unsigned char)0x73;		\
		emit_sse_operand_reg_reg((inst), (XMM2), (dreg));		\
		*(inst) ++ = (unsigned char)(int)((shift) & 0xff);	\
	} while(0)

#define sse2_shufpd_xmreg_xmreg_imm8(inst,dreg,reg,imm)	\
	do {	\
		emit_sse_instruction_reg_reg(inst, (0x66), (0xc6), (dreg), (reg));	\
		*(inst)++ = (unsigned char)imm;		\
	} while(0)

#define sse2_shufpd_xmreg_membase_imm8(inst,dreg,basereg,disp,imm)	\
	do {	\
		emit_sse_instruction_reg_membase(inst, (0x66), (0xc6), (dreg), (basereg), (disp));	\
		*(inst) ++ = (unsigned char)(int)((imm) & 0xff);	\
	} while(0)

#define sse2_shufpd_xmreg_mem_imm8(inst,dreg,mem,imm)	\
	do {	\
		emit_sse_instruction_reg_mem(inst, (0x66), (0xc6), (dreg), (mem));	\
		*(inst) ++ = (unsigned char)(int)((imm) & 0xff);	\
	} while(0)

#define sse_shufps_xmreg_xmreg_imm8(inst,dreg,reg,imm)	\
	do {	\
		emit_sse_instruction_reg_reg(inst, (0x0), (0xc6), (dreg), (reg));	\
		*(inst) ++ = (unsigned char)(int)((imm) & 0xff);	\
	} while(0)

#define sse_shufps_xmreg_membase_imm8(inst,dreg,basereg,disp,imm)	\
	do {	\
		emit_sse_instruction_reg_membase(inst, (0x0), (0xc6), (dreg), (basereg), (disp));	\
		*(inst) ++ = (unsigned char)(int)((imm) & 0xff);	\
	} while(0)

#define sse_shufps_xmreg_mem_imm8(inst,dreg,mem,imm)	\
	do {	\
		emit_sse_instruction_reg_mem(inst, (0x0), (0xc6), (dreg), (mem);	\
		*(inst) ++ = (unsigned char)(int)((imm) & 0xff);	\
	} while(0)

#define sse_stmxcsr_mem(inst,mem)	\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_mem((inst), (X86_EBX), (mem));	\
	} while(0)

#define sse_stmxcsr_membase(inst,basereg,disp)	\
	do {	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0xae;	\
		emit_sse_operand_reg_membase((inst), (X86_EBX), (basereg), (disp));	\
	} while(0)

#define sse2_subsd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf2), (0x5c), (dreg), (reg));	\
	} while(0)

#define sse2_subsd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase(inst, (0xf2), (0x5c), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_subsd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem(inst, (0xf2), (0x5c), (dreg), (mem));	\
	} while(0)

#define sse_subss_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0xf3), (0x5c), (dreg), (reg));	\
	} while(0)

#define sse_subss_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase(inst, (0xf3), (0x5c), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_subss_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem(inst, (0xf3), (0x5c), (dreg), (mem));	\
	} while(0)

#define sse2_ucomisd_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x66), (0x2e), (dreg), (reg));	\
	} while(0)

#define sse2_ucomisd_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase(inst, (0x66), (0x2e), (dreg), (basereg), (disp));	\
	} while(0)

#define sse2_ucomisd_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem(inst, (0x66), (0x2e), (dreg), (mem));	\
	} while(0)

#define sse_ucomiss_xmreg_xmreg(inst,dreg,reg)	\
	do {	\
		emit_sse_instruction_reg_reg((inst), (0x0), (0x2e), (dreg), (reg));	\
	} while(0)

#define sse_ucomiss_xmreg_membase(inst,dreg,basereg,disp)	\
	do {	\
		emit_sse_instruction_reg_membase(inst, (0x0), (0x2e), (dreg), (basereg), (disp));	\
	} while(0)

#define sse_ucomiss_xmreg_mem(inst,dreg,mem)	\
	do {	\
		emit_sse_instruction_reg_mem(inst, (0x0), (0x2e), (dreg), (mem));	\
	} while(0)

#define sse4_palignr_xmreg_xmreg(inst,dreg,reg)		\
	do {	\
		*(inst) ++ = (unsigned char)0x66;	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0x3a;	\
		*(inst) ++ = (unsigned char)0x0f;	\
		emit_sse_operand_reg_reg((inst), (dreg), (reg));	\
	} while(0)

#define sse4_palignr_xmreg_membase(inst,dreg,basereg,disp)		\
	do {	\
		*(inst) ++ = (unsigned char)0x66;	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0x3a;	\
		*(inst) ++ = (unsigned char)0x0f;	\
		emit_sse_operand_reg_membase(inst, (dreg), (basereg), (disp));	\
	} while(0)

#define sse4_palignr_xmreg_mem(inst,dreg,mem)		\
	do {	\
		*(inst) ++ = (unsigned char)0x66;	\
		*(inst) ++ = (unsigned char)0x0f;	\
		*(inst) ++ = (unsigned char)0x3a;	\
		*(inst) ++ = (unsigned char)0x0f;	\
		emit_sse_operand_reg_mem(inst, (dreg), (mem));	\
	} while(0)

#endif /* JIT_GEN_SSE_H */
