/* Automatically generated from ./jite-rules-i486-math.lir - DO NOT EDIT */
/*
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

#if defined(JIT_INCLUDE_RULES)

case JIT_OP_FTAN:
case JIT_OP_DTAN:
{
	switch(state)
	{
	default:
	{
        inst = masm_mov_st0_value(inst, insn->value1);
        x86_fld1(inst);
        x86_fpatan(inst);
        x86_fldz(inst);
        x86_fp_op_reg(inst, X86_FADD, 1, 1);
        inst = masm_mov_value_st0(inst, insn->dest);
    }
	break;
	}
}
break;

case JIT_OP_FCOS:
case JIT_OP_DCOS:
{
	switch(state)
	{
	default:
	{
        inst = masm_mov_st0_value(inst, insn->value1);
        x86_fcos(inst);
        x86_fldz(inst);
        x86_fp_op_reg(inst, X86_FADD, 1, 1);
        inst = masm_mov_value_st0(inst, insn->dest);
    }
	break;
	}
}
break;

case JIT_OP_FSIN:
case JIT_OP_DSIN:
{
	switch(state)
	{
	default:
	{
        inst = masm_mov_st0_value(inst, insn->value1);
        x86_fsin(inst);
        x86_fldz(inst);
        x86_fp_op_reg(inst, X86_FADD, 1, 1);
        inst = masm_mov_value_st0(inst, insn->dest);
    }
	break;
	}
}
break;

case JIT_OP_FSQRT:
case JIT_OP_DSQRT:
{
	switch(state)
	{
	case 0x6:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code1), (machine_code2), param[0], X86_ESP, -32);
    }
	break;
	case 0x7:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_membase_imm(inst, X86_EBP, param[0], param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[0]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x0:
	{
        emit_sse_instruction_reg_reg(inst, (machine_code1), (machine_code2), param[0], param[1]);
    }
	break;
	case 0x3:
	{
        emit_sse_instruction_reg_membase(inst, (machine_code1), (machine_code2), param[0], X86_EBP, param[1]);
    }
	break;
	case 0x1:
	{
        find_one_xmm_reg(inst);
        emit_sse_instruction_reg_reg(inst, (machine_code1), (machine_code2), xmmreg1, param[1]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x4:
	{
        find_one_xmm_reg(inst);
        emit_sse_instruction_reg_membase(inst, (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[1]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	}
}
break;

case JIT_OP_IABS:
{
	switch(state)
	{
	case 0x7:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], abs(param[1]), 4);
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_membase_reg(inst, X86_ADD, X86_EBP, param[0], gpreg1);
        x86_alu_membase_reg(inst, X86_XOR, X86_EBP, param[0], gpreg1);
        
    }
	break;
	case 0x1:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        find_one_gp_reg(inst);
        if(gpreg1 != param[1]) x86_mov_reg_reg(inst, gpreg1, param[1], 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_membase_reg(inst, X86_ADD, X86_EBP, param[0], gpreg1);
        x86_alu_membase_reg(inst, X86_XOR, X86_EBP, param[0], gpreg1);
        
    }
	break;
	case 0x6:
	{
        x86_mov_reg_imm(inst, param[0], abs(param[1]));
    }
	break;
	case 0x3:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        find_one_gp_reg(inst);
        x86_mov_reg_reg(inst, gpreg1, param[0], 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_reg_reg(inst, X86_ADD, param[0], gpreg1);
        x86_alu_reg_reg(inst, X86_XOR, param[0], gpreg1);
        
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
        find_one_gp_reg(inst);
        if(gpreg1 != param[1]) x86_mov_reg_reg(inst, gpreg1, param[1], 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_reg_reg(inst, X86_ADD, param[0], gpreg1);
        x86_alu_reg_reg(inst, X86_XOR, param[0], gpreg1);
        
    }
	break;
	}
}
break;

case JIT_OP_LABS:
{
	switch(state)
	{
	case 0x7:
	{
        jit_long value = ((jit_long*)(param[1]))[0];
        value = labs(value);
        inst = masm_mov_membase_imm(inst, X86_EBP, param[0], (jit_nint)(&value), sourceType);
    }
	break;
	case 0x6:
	{
        jit_long value = ((jit_long*)(param[1]))[0];
        value = labs(value);
        inst = masm_mov_reg_imm(inst, param[0], (jit_nint)(&value), sourceType);
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_membase_reg(inst, X86_XOR, X86_EBP, param[0], gpreg1);
        x86_alu_membase_reg(inst, X86_XOR, X86_EBP, param[0] + 4, gpreg1);
        x86_alu_membase_reg(inst, X86_SUB, X86_EBP, param[0], gpreg1);
        x86_alu_membase_reg(inst, X86_SBB, X86_EBP, param[0] + 4, gpreg1);
        
    }
	break;
	case 0x1:
	{
        find_one_gp_reg(inst);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        x86_mov_reg_reg(inst, gpreg1, jite_register_pair(param[1]), 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_membase_reg(inst, X86_XOR, X86_EBP, param[0], gpreg1);
        x86_alu_membase_reg(inst, X86_XOR, X86_EBP, param[0] + 4, gpreg1);
        x86_alu_membase_reg(inst, X86_SUB, X86_EBP, param[0], gpreg1);
        x86_alu_membase_reg(inst, X86_SBB, X86_EBP, param[0] + 4, gpreg1);
        
    }
	break;
	case 0x3:
	{
        find_one_gp_reg_cond2(inst, param[0], jite_register_pair(param[0]));
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        x86_mov_reg_reg(inst, gpreg1, jite_register_pair(param[1]), 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_reg_reg(inst, X86_XOR, param[0], gpreg1);
        x86_alu_reg_reg(inst, X86_XOR, jite_register_pair(param[0]), gpreg1);
        x86_alu_reg_reg(inst, X86_SUB, param[0], gpreg1);
        x86_alu_reg_reg(inst, X86_SBB, jite_register_pair(param[0]), gpreg1);
        
    }
	break;
	case 0x0:
	{
        find_one_gp_reg_cond2(inst, param[0], jite_register_pair(param[0]));
        inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
        x86_mov_reg_reg(inst, gpreg1, jite_register_pair(param[1]), 4);
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_alu_reg_reg(inst, X86_XOR, param[0], gpreg1);
        x86_alu_reg_reg(inst, X86_XOR, jite_register_pair(param[0]), gpreg1);
        x86_alu_reg_reg(inst, X86_SUB, param[0], gpreg1);
        x86_alu_reg_reg(inst, X86_SBB, jite_register_pair(param[0]), gpreg1);
        
    }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_FTAN:
case JIT_OP_DTAN:
case JIT_OP_FCOS:
case JIT_OP_DCOS:
case JIT_OP_FSIN:
case JIT_OP_DSIN:
case JIT_OP_FSQRT:
case JIT_OP_DSQRT:
case JIT_OP_IABS:
case JIT_OP_LABS:
	return 1;

#endif
