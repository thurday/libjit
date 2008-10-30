/* Automatically generated from ./jite-rules-i486-branch.lir - DO NOT EDIT */
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

case JIT_OP_BR:
{
	switch(state)
	{
	default:
	{
        inst = output_branch(func, inst, 0xEB /* jmp */, insn);
    }
	break;
	}
}
break;

case JIT_OP_BR_IFALSE:
case JIT_OP_BR_ITRUE:
{
	switch(state)
	{
	case 0x2:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_alu_reg_reg(inst, X86_OR, gpreg1, gpreg1);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x1:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], 0);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x0:
	{
        x86_alu_reg_reg(inst, X86_OR, param[0], param[0]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	}
}
break;

case JIT_OP_BR_IEQ:
case JIT_OP_BR_INE:
{
	switch(state)
	{
	case 0x8:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_alu_reg_imm(inst, X86_CMP, gpreg1, param[1]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x5:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], param[0]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x2:
	{
        if(param[0] != 0)
        {
            x86_alu_reg_imm(inst, X86_CMP, param[1], param[0]);
            inst = output_branch(func, inst, machine_code1, insn);
        }
        else
        {
            x86_alu_reg_reg(inst, X86_OR, param[1], param[1]);
            inst = output_branch(func, inst, machine_code1, insn);
        }
    }
	break;
	case 0x7:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[0], 4);
        x86_alu_reg_membase(inst, X86_CMP, gpreg1, X86_EBP, param[1]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x1:
	{
        x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x6:
	{
        if(param[1] != 0)
        {
            x86_alu_reg_imm(inst, X86_CMP, param[0], param[1]);
            inst = output_branch(func, inst, machine_code1, insn);
        }
        else
        {
            x86_alu_reg_reg(inst, X86_OR, param[0], param[0]);
            inst = output_branch(func, inst, machine_code1, insn);
        }
    }
	break;
	case 0x3:
	{
        x86_alu_reg_membase(inst, X86_CMP, param[0], X86_EBP, param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x0:
	{
        x86_alu_reg_reg(inst, X86_CMP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	}
}
break;

case JIT_OP_BR_ILT:
case JIT_OP_BR_ILT_UN:
case JIT_OP_BR_ILE:
case JIT_OP_BR_ILE_UN:
case JIT_OP_BR_IGT:
case JIT_OP_BR_IGT_UN:
case JIT_OP_BR_IGE:
case JIT_OP_BR_IGE_UN:
{
	switch(state)
	{
	case 0x8:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_alu_reg_imm(inst, X86_CMP, gpreg1, param[1]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x5:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], param[0]);
        inst = output_branch(func, inst, machine_code2, insn);
    }
	break;
	case 0x2:
	{
        x86_alu_reg_imm(inst, X86_CMP, param[1], param[0]);
        inst = output_branch(func, inst, machine_code2, insn);
    }
	break;
	case 0x7:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[0], 4);
        x86_alu_reg_membase(inst, X86_CMP, gpreg1, X86_EBP, param[1]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x1:
	{
        x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x6:
	{
        x86_alu_reg_imm(inst, X86_CMP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x3:
	{
        x86_alu_reg_membase(inst, X86_CMP, param[0], X86_EBP, param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x0:
	{
        x86_alu_reg_reg(inst, X86_CMP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	}
}
break;

case JIT_OP_IEQ:
case JIT_OP_INE:
case JIT_OP_ILT:
case JIT_OP_ILT_UN:
case JIT_OP_ILE:
case JIT_OP_ILE_UN:
case JIT_OP_IGT:
case JIT_OP_IGT_UN:
case JIT_OP_IGE:
case JIT_OP_IGE_UN:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, param[1]);
        x86_alu_reg_imm(inst, X86_CMP, gpreg1, param[2]);
        setcc_reg(inst, gpreg1, machine_code1, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0x10:
	{
        find_one_gp_reg(inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], param[1]);
        setcc_reg(inst, gpreg1, machine_code2, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0x7:
	{
        find_one_gp_reg(inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], param[1]);
        setcc_reg(inst, gpreg1, machine_code2, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0x16:
	{
        find_one_gp_reg(inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], param[2]);
        setcc_reg(inst, gpreg1, machine_code1, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0xd:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_alu_reg_membase(inst, X86_CMP, gpreg1, X86_EBP, param[2]);
        setcc_reg(inst, gpreg1, machine_code1, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[1], param[2]);
        setcc_reg(inst, gpreg1, machine_code1, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0x13:
	{
        find_one_gp_reg(inst);
        x86_alu_reg_imm(inst, X86_CMP, param[1], param[2]);
        setcc_reg(inst, gpreg1, machine_code1, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0xa:
	{
        find_one_gp_reg(inst);
        x86_alu_reg_membase(inst, X86_CMP, param[1], X86_EBP, param[2]);
        setcc_reg(inst, gpreg1, machine_code1, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0x1:
	{
        find_one_gp_reg(inst);
        x86_alu_reg_reg(inst, X86_CMP, param[1], param[2]);
        setcc_reg(inst, gpreg1, machine_code1, machine_code3);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        release_one_gp_reg(inst);
    }
	break;
	case 0x18:
	{
        x86_mov_reg_imm(inst, param[0], param[1]);
        x86_alu_reg_imm(inst, X86_CMP, param[0], param[2]);
        setcc_reg(inst, param[0], machine_code1, machine_code3);
    }
	break;
	case 0xf:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], param[1]);
        setcc_reg(inst, param[0], machine_code2, machine_code3);
    }
	break;
	case 0x6:
	{
        x86_alu_reg_imm(inst, X86_CMP, param[2], param[1]);
        setcc_reg(inst, param[0], machine_code2, machine_code3);
    }
	break;
	case 0x15:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], param[2]);
        setcc_reg(inst, param[0], machine_code1, machine_code3);
    }
	break;
	case 0xc:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        x86_alu_reg_membase(inst, X86_CMP, param[0], X86_EBP, param[2]);
        setcc_reg(inst, param[0], machine_code1, machine_code3);
    }
	break;
	case 0x3:
	{
        x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[1], param[2]);
        setcc_reg(inst, param[0], machine_code1, machine_code3);
    }
	break;
	case 0x12:
	{
        x86_alu_reg_imm(inst, X86_CMP, param[1], param[2]);
        setcc_reg(inst, param[0], machine_code1, machine_code3);
    }
	break;
	case 0x9:
	{
        x86_alu_reg_membase(inst, X86_CMP, param[1], X86_EBP, param[2]);
        setcc_reg(inst, param[0], machine_code1, machine_code3);
    }
	break;
	case 0x0:
	{
        x86_alu_reg_reg(inst, X86_CMP, param[1], param[2]);
        setcc_reg(inst, param[0], machine_code1, machine_code3);
    }
	break;
	}
}
break;

case JIT_OP_BR_LFALSE:
case JIT_OP_BR_LTRUE:
{
	switch(state)
	{
	case 0x2:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, ((jit_nint *)(param[0]))[0]);
        x86_alu_reg_imm(inst, X86_OR, gpreg1, ((jit_nint *)(param[0]))[1]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x1:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[0] + 4, 4);
        x86_alu_reg_membase(inst, X86_OR, gpreg1, X86_EBP, param[0]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x0:
	{
        x86_alu_reg_reg(inst, X86_OR, param[0], jite_register_pair(param[0]));
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	}
}
break;

case JIT_OP_BR_LEQ:
{
	switch(state)
	{
	case 0x8:
	{
        jit_long value1 = *((jit_long *)(param[0]));
        jit_long value2 = *((jit_long *)(param[1]));
        if(value1 == value2)
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x5:
	{
        unsigned char *patch1, *patch2;
        jit_uint value1 = ((jit_nint *)(param[0]))[0];
        jit_int value2 = ((jit_nint *)(param[0]))[1];
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], value1);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 0);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1] + 4, value2);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x2:
	{
        unsigned char *patch1, *patch2;
        jit_uint value1 = ((jit_nint *)(param[0]))[0];
        jit_int value2 = ((jit_nint *)(param[0]))[1];
        x86_alu_reg_imm(inst, X86_CMP, param[1], value1);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 0);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_reg_imm(inst, X86_CMP, jite_register_pair(param[1]), value2);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x7:
	{
        unsigned char *patch1, *patch2;
        jit_int value1 = ((jit_nint *)(param[1]))[0];
        jit_int value2 = ((jit_nint *)(param[1]))[1];
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], value1);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 0);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0] + 4, value2);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x6:
	{
        unsigned char *patch1, *patch2;
        jit_int value1 = ((jit_nint *)(param[1]))[0];
        jit_int value2 = ((jit_nint *)(param[1]))[1];
        x86_alu_reg_imm(inst, X86_CMP, param[0], value1);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 0);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_reg_imm(inst, X86_CMP, jite_register_pair(param[0]), value2);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            unsigned char *patch1, *patch2;
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[0], gpreg1);
            release_one_gp_reg(inst);
            patch1 = inst;
            x86_branch8(inst, X86_CC_EQ, 0, 0);
            patch2 = inst;
            x86_jump8(inst, 0);
            x86_patch(patch1, inst);
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
            x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[0] + 4, gpreg1);
            release_one_gp_reg(inst);
            inst = output_branch(func, inst, 0x74 /* eq */, insn);
            x86_patch(patch2, inst);
        }
        else
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            unsigned char *patch1, *patch2;
            x86_alu_reg_reg(inst, X86_CMP, param[0], param[1]);
            patch1 = inst;
            x86_branch8(inst, X86_CC_EQ, 0, 0);
            patch2 = inst;
            x86_jump8(inst, 0);
            x86_patch(patch1, inst);
            x86_alu_reg_reg(inst, X86_CMP, jite_register_pair(param[0]), jite_register_pair(param[1]));
            inst = output_branch(func, inst, 0x74 /* eq */, insn);
            x86_patch(patch2, inst);
        }
        else
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	}
}
break;

case JIT_OP_BR_LNE:
{
	switch(state)
	{
	case 0x8:
	{
        jit_long value1 = *((jit_long *)(param[0]));
        jit_long value2 = *((jit_long *)(param[1]));
        if(value1 != value2)
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x5:
	{
        jit_nint value1 = ((jit_nint *)(param[0]))[0];
        jit_nint value2 = ((jit_nint *)(param[0]))[1];
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], value1);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1] + 4, value2);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
    }
	break;
	case 0x2:
	{
        jit_nint value1 = ((jit_nint *)(param[0]))[0];
        jit_nint value2 = ((jit_nint *)(param[0]))[1];
        x86_alu_reg_imm(inst, X86_CMP, param[1], value1);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
        x86_alu_reg_imm(inst, X86_CMP, jite_register_pair(param[1]), value2);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
    }
	break;
	case 0x7:
	{
        jit_nint value1 = ((jit_nint *)(param[1]))[0];
        jit_nint value2 = ((jit_nint *)(param[1]))[1];
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], value1);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0] + 4, value2);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
    }
	break;
	case 0x6:
	{
        jit_nint value1 = ((jit_nint *)(param[1]))[0];
        jit_nint value2 = ((jit_nint *)(param[1]))[1];
        x86_alu_reg_imm(inst, X86_CMP, param[0], value1);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
        x86_alu_reg_imm(inst, X86_CMP, jite_register_pair(param[0]), value2);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[0], gpreg1);
            release_one_gp_reg(inst);
            inst = output_branch(func, inst, 0x75 /* neq */, insn);
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
            x86_alu_membase_reg(inst, X86_CMP, X86_EBP, param[0] + 4, gpreg1);
            release_one_gp_reg(inst);
            inst = output_branch(func, inst, 0x75 /* neq */, insn);
        }
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            x86_alu_reg_reg(inst, X86_CMP, param[0], param[1]);
            release_one_gp_reg(inst);
            inst = output_branch(func, inst, 0x75 /* neq */, insn);
            x86_alu_reg_reg(inst, X86_CMP, jite_register_pair(param[0]), jite_register_pair(param[1]));
            inst = output_branch(func, inst, 0x75 /* neq */, insn);
        }
    }
	break;
	}
}
break;

case JIT_OP_BR_LLT:
case JIT_OP_BR_LLT_UN:
case JIT_OP_BR_LLE:
case JIT_OP_BR_LLE_UN:
case JIT_OP_BR_LGT:
case JIT_OP_BR_LGT_UN:
case JIT_OP_BR_LGE:
case JIT_OP_BR_LGE_UN:
{
	switch(state)
	{
	case 0x8:
	{
        unsigned char *patch1, *patch2;
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, ((jit_nint *)(param[0]))[1]);
        x86_alu_reg_imm(inst, X86_CMP, gpreg1, ((jit_nint *)(param[1]))[1]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 1);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, ((jit_nint *)(param[0]))[0]);
        x86_alu_reg_imm(inst, X86_CMP, gpreg1, ((jit_nint *)(param[1]))[0]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code2, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x5:
	{
        unsigned char *patch1, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1] + 4, ((jit_nint *)(param[0]))[1]);
        inst = output_branch(func, inst, machine_code3, insn);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 1);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], ((jit_nint *)(param[0]))[0]);
        inst = output_branch(func, inst, machine_code4, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x2:
	{
        unsigned char *patch1, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, jite_register_pair(param[1]), ((jit_nint *)(param[0]))[1]);
        inst = output_branch(func, inst, machine_code3, insn);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 1);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[1], ((jit_nint *)(param[0]))[0]);
        inst = output_branch(func, inst, machine_code4, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x7:
	{
        unsigned char *patch1, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0] + 4, ((jit_nint *)(param[1]))[1]);
        inst = output_branch(func, inst, machine_code1, insn);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 1);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], ((jit_nint *)(param[1]))[0]);
        inst = output_branch(func, inst, machine_code2, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x6:
	{
        unsigned char *patch1, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, jite_register_pair(param[0]), ((jit_nint *)(param[1]))[1]);
        inst = output_branch(func, inst, machine_code1, insn);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 1);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[0], ((jit_nint *)(param[1]))[0]);
        inst = output_branch(func, inst, machine_code2, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x4:
	{
        unsigned char *patch1, *patch2;
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[0] + 4, 4);
        x86_alu_reg_membase(inst, X86_CMP, gpreg1, X86_EBP, param[1] + 4);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 1);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[0], 4);
        x86_alu_reg_membase(inst, X86_CMP, gpreg1, X86_EBP, param[1]);
        release_one_gp_reg(inst);
        inst = output_branch(func, inst, machine_code2, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x0:
	{
        unsigned char *patch1, *patch2;
        x86_alu_reg_reg(inst, X86_CMP, jite_register_pair(param[0]), jite_register_pair(param[1]));
        inst = output_branch(func, inst, machine_code1, insn);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 1);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_reg_reg(inst, X86_CMP, param[0], param[1]);
        inst = output_branch(func, inst, machine_code2, insn);
        x86_patch(patch2, inst);
    }
	break;
	}
}
break;

case JIT_OP_DEQ:
case JIT_OP_DEQ_INV:
case JIT_OP_DNE:
case JIT_OP_DNE_INV:
case JIT_OP_DLT:
case JIT_OP_DLT_INV:
case JIT_OP_DLE:
case JIT_OP_DLE_INV:
case JIT_OP_DGT:
case JIT_OP_DGT_INV:
case JIT_OP_DGE:
case JIT_OP_DGE_INV:
case JIT_OP_FEQ:
case JIT_OP_FEQ_INV:
case JIT_OP_FNE:
case JIT_OP_FNE_INV:
case JIT_OP_FLT:
case JIT_OP_FLT_INV:
case JIT_OP_FLE:
case JIT_OP_FLE_INV:
case JIT_OP_FGT:
case JIT_OP_FGT_INV:
case JIT_OP_FGE:
case JIT_OP_FGE_INV:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_ESP, -32);
        release_one_xmm_reg(inst);
        setcc_membase(inst, X86_EBP, param[0], machine_code1, 0);
    }
	break;
	case 0x10:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_EBP, param[2]);
        release_one_xmm_reg(inst);
        setcc_membase(inst, X86_EBP, param[0], machine_code1, 0);
    }
	break;
	case 0x7:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[2], X86_ESP, -16);
        setcc_membase(inst, X86_EBP, param[0], machine_code2, 0);
    }
	break;
	case 0x16:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_ESP, -32);
        release_one_xmm_reg(inst);
        setcc_membase(inst, X86_EBP, param[0], machine_code1, 0);
    }
	break;
	case 0xd:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_EBP, param[2]);
        release_one_xmm_reg(inst);
        setcc_membase(inst, X86_EBP, param[0], machine_code1, 0);
    }
	break;
	case 0x4:
	{
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[2], X86_EBP, param[1]);
        setcc_membase(inst, X86_EBP, param[0], machine_code2, 0);
    }
	break;
	case 0x13:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[1], X86_ESP, -16);
        setcc_membase(inst, X86_EBP, param[0], machine_code1, 0);
    }
	break;
	case 0xa:
	{
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[1], X86_EBP, param[2]);
        setcc_membase(inst, X86_EBP, param[0], machine_code1, 0);
    }
	break;
	case 0x1:
	{
        emit_sse_instruction_reg_reg(inst, (machine_code3), (machine_code4), param[1], param[2]);
        setcc_membase(inst, X86_EBP, param[0], machine_code1, 0);
    }
	break;
	case 0x18:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_ESP, -32);
        release_one_xmm_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
        setcc_reg(inst, param[0], machine_code1, 0);
    }
	break;
	case 0xf:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_EBP, param[2]);
        release_one_xmm_reg(inst);
        setcc_reg(inst, param[0], machine_code1, 0);
    }
	break;
	case 0x6:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[2], X86_ESP, -16);
        setcc_reg(inst, param[0], machine_code2, 0);
    }
	break;
	case 0x15:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_ESP, -32);
        release_one_xmm_reg(inst);
        setcc_reg(inst, param[0], machine_code1, 0);
    }
	break;
	case 0xc:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_EBP, param[2]);
        release_one_xmm_reg(inst);
        setcc_reg(inst, param[0], machine_code1, 0);
    }
	break;
	case 0x3:
	{
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[2], X86_EBP, param[1]);
        setcc_reg(inst, param[0], machine_code2, 0);
    }
	break;
	case 0x12:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[1], X86_ESP, -16);
        setcc_reg(inst, param[0], machine_code1, 0);
    }
	break;
	case 0x9:
	{
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[1], X86_EBP, param[2]);
        setcc_reg(inst, param[0], machine_code1, 0);
    }
	break;
	case 0x0:
	{
        emit_sse_instruction_reg_reg(inst, (machine_code3), (machine_code4), param[1], param[2]);
        setcc_reg(inst, param[0], machine_code1, 0);
    }
	break;
	}
}
break;

case JIT_OP_BR_DEQ:
case JIT_OP_BR_DEQ_INV:
{
	switch(state)
	{
	case 0x8:
	{
        jit_float64 value1 = *((jit_float64 *)(param[0]));
        jit_float64 value2 = *((jit_float64 *)(param[1]));
        if(value1 == value2)
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x5:
	{
        unsigned char *patch1, *patch2;
        jit_int value1 = ((jit_int *)(param[0]))[0];
        jit_int value2 = ((jit_int *)(param[0]))[1];
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], value1);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 0);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1] + 4, value2);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x2:
	{
        x86_mov_membase_imm(inst, X86_ESP, -16, ((jit_uint *)(param[0]))[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -12, ((jit_uint *)(param[0]))[1], 4);
        sse2_comisd_xmreg_membase(inst, param[1], X86_ESP, -16);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x7:
	{
        unsigned char *patch1, *patch2;
        jit_int value1 = ((jit_int *)(param[1]))[0];
        jit_int value2 = ((jit_int *)(param[1]))[1];
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], value1);
        patch1 = inst;
        x86_branch8(inst, X86_CC_EQ, 0, 0);
        patch2 = inst;
        x86_jump8(inst, 0);
        x86_patch(patch1, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0] + 4, value2);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
        x86_patch(patch2, inst);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[0]);
            sse2_comisd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            release_one_xmm_reg(inst);
            inst = output_branch(func, inst, 0x74 /* eq */, insn);
        }
        else
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x1:
	{
        sse2_comisd_xmreg_membase(inst, param[1], X86_EBP, param[0]);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x6:
	{
        x86_mov_membase_imm(inst, X86_ESP, -16, ((jit_uint *)(param[1]))[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -12, ((jit_uint *)(param[1]))[1], 4);
        sse2_comisd_xmreg_membase(inst, param[0], X86_ESP, -16);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x3:
	{
        sse2_comisd_xmreg_membase(inst, param[0], X86_EBP, param[1]);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            sse2_comisd_xmreg_xmreg(inst, param[0], param[1]);
            inst = output_branch(func, inst, 0x74 /* eq */, insn);
        }
        else
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	}
}
break;

case JIT_OP_BR_DNE:
case JIT_OP_BR_DNE_INV:
{
	switch(state)
	{
	case 0x8:
	{
        jit_float64 value1 = *((jit_float64 *)(param[0]));
        jit_float64 value2 = *((jit_float64 *)(param[1]));
        if(value1 != value2)
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x5:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], ((jit_nint *)(param[0]))[0]);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1] + 4, ((jit_nint *)(param[0]))[1]);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
    }
	break;
	case 0x2:
	{
        x86_mov_membase_imm(inst, X86_ESP, -16, ((jit_uint *)(param[0]))[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -12, ((jit_uint *)(param[0]))[1], 4);
        sse2_comisd_xmreg_membase(inst, param[1], X86_ESP, -16);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x7:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], ((jit_nint *)(param[1]))[0]);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0] + 4, ((jit_nint *)(param[1]))[1]);
        inst = output_branch(func, inst, 0x75 /* neq */, insn);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[0]);
            sse2_comisd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            release_one_xmm_reg(inst);
            inst = output_branch(func, inst, 0x75 /* ne */, insn);
        }
    }
	break;
	case 0x1:
	{
        sse2_comisd_xmreg_membase(inst, param[1], X86_EBP, param[0]);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x6:
	{
        x86_mov_membase_imm(inst, X86_ESP, -16, ((jit_uint *)(param[1]))[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -12, ((jit_uint *)(param[1]))[1], 4);
        sse2_comisd_xmreg_membase(inst, param[0], X86_ESP, -16);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x3:
	{
        sse2_comisd_xmreg_membase(inst, param[0], X86_EBP, param[1]);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            sse2_comisd_xmreg_xmreg(inst, param[0], param[1]);
            inst = output_branch(func, inst, 0x75 /* ne */, insn);
        }
    }
	break;
	}
}
break;

case JIT_OP_BR_DLT:
case JIT_OP_BR_DLT_INV:
case JIT_OP_BR_DLE:
case JIT_OP_BR_DLE_INV:
case JIT_OP_BR_DGT:
case JIT_OP_BR_DGT_INV:
case JIT_OP_BR_DGE:
case JIT_OP_BR_DGE_INV:
case JIT_OP_BR_FLT:
case JIT_OP_BR_FLT_INV:
case JIT_OP_BR_FLE:
case JIT_OP_BR_FLE_INV:
case JIT_OP_BR_FGT:
case JIT_OP_BR_FGT_INV:
case JIT_OP_BR_FGE:
case JIT_OP_BR_FGE_INV:
{
	switch(state)
	{
	case 0x8:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[0], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_ESP, -32);
        release_one_xmm_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x5:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[0], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_EBP, param[1]);
        release_one_xmm_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x2:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[0], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[1], X86_ESP, -16);
        inst = output_branch(func, inst, machine_code2, insn);
    }
	break;
	case 0x7:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[0], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_ESP, -32);
        release_one_xmm_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x4:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[0], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), xmmreg1, X86_EBP, param[1]);
        release_one_xmm_reg(inst);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x1:
	{
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[1], X86_EBP, param[0]);
        inst = output_branch(func, inst, machine_code2, insn);
    }
	break;
	case 0x6:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[1], sourceType);
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[0], X86_ESP, -16);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x3:
	{
        emit_sse_instruction_reg_membase(inst, (machine_code3), (machine_code4), param[0], X86_EBP, param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	case 0x0:
	{
        emit_sse_instruction_reg_reg(inst, (machine_code3), (machine_code4), param[0], param[1]);
        inst = output_branch(func, inst, machine_code1, insn);
    }
	break;
	}
}
break;

case JIT_OP_BR_FEQ:
case JIT_OP_BR_FEQ_INV:
{
	switch(state)
	{
	case 0x8:
	{
        jit_float32 value1 = *((jit_float32 *)(param[0]));
        jit_float32 value2 = *((jit_float32 *)(param[1]));

        if(value1 == value2)
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x5:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], ((jit_uint *)(param[0]))[0]);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x2:
	{
        x86_mov_membase_imm(inst, X86_ESP, -16, ((jit_uint *)(param[0]))[0], 4);
        sse_comiss_xmreg_membase(inst, param[1], X86_ESP, -16);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x7:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], ((jit_uint *)(param[1]))[0]);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_xmm_reg(inst);
            sse_movss_xmreg_membase(inst, xmmreg1, X86_EBP, param[0]);
            sse_comiss_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            release_one_xmm_reg(inst);
            inst = output_branch(func, inst, 0x74 /* eq */, insn);
        }
        else
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x1:
	{
        sse_comiss_xmreg_membase(inst, param[1], X86_EBP, param[0]);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x6:
	{
        x86_mov_membase_imm(inst, X86_ESP, -16, ((jit_uint *)(param[1]))[0], 4);
        sse_comiss_xmreg_membase(inst, param[0], X86_ESP, -16);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x3:
	{
        sse_comiss_xmreg_membase(inst, param[0], X86_EBP, param[1]);
        inst = output_branch(func, inst, 0x74 /* eq */, insn);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            sse_comiss_xmreg_xmreg(inst, param[0], param[1]);
            inst = output_branch(func, inst, 0x74 /* eq */, insn);
        }
        else
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	}
}
break;

case JIT_OP_BR_FNE:
case JIT_OP_BR_FNE_INV:
{
	switch(state)
	{
	case 0x8:
	{
        jit_float32 value1 = *((jit_float32 *)(param[0]));
        jit_float32 value2 = *((jit_float32 *)(param[1]));
        if(value1 != value2)
        {
            inst = output_branch(func, inst, 0xEB /* jmp */, insn);
        }
    }
	break;
	case 0x5:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], ((jit_uint *)(param[0]))[0]);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x2:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[0], sourceType);
        sse_comiss_xmreg_membase(inst, param[1], X86_ESP, -16);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x7:
	{
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], ((jit_uint *)(param[1]))[0]);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_xmm_reg(inst);
            sse_movss_xmreg_membase(inst, xmmreg1, X86_EBP, param[0]);
            sse_comiss_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            release_one_xmm_reg(inst);
            inst = output_branch(func, inst, 0x75 /* ne */, insn);
        }
    }
	break;
	case 0x1:
	{
        sse_comiss_xmreg_membase(inst, param[1], X86_EBP, param[0]);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x6:
	{
        x86_mov_membase_imm(inst, X86_ESP, -16, ((jit_uint *)(param[1]))[0], 4);
        sse_comiss_xmreg_membase(inst, param[0], X86_ESP, -16);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x3:
	{
        sse_comiss_xmreg_membase(inst, param[0], X86_EBP, param[1]);
        inst = output_branch(func, inst, 0x75 /* ne */, insn);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            sse_comiss_xmreg_xmreg(inst, param[0], param[1]);
            inst = output_branch(func, inst, 0x74 /* ne */, insn);
        }
    }
	break;
	}
}
break;

case JIT_OP_CHECK_NULL:
{
	switch(state)
	{
	case 0x2:
	{
        if(param[0] == 0)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_NULL_REFERENCE);
        }
    }
	break;
	case 0x1:
	{
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[0], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_NULL_REFERENCE);
        x86_patch(patch, inst);
    }
	break;
	case 0x0:
	{
        unsigned char *patch;
        x86_alu_reg_reg(inst, X86_OR, param[0], param[0]);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_NULL_REFERENCE);
        x86_patch(patch, inst);
    }
	break;
	}
}
break;

case JIT_OP_JUMP_TABLE:
{
	switch(state)
	{
	case 0x18:
	{
        unsigned char *patch_jump_table;
        unsigned char *patch_fall_through;
        int index;
        jit_label_t *labels;
        jit_nint num_labels;
        jit_block_t block;

        labels = (jit_label_t *) param[1];
        num_labels = param[2];

        x86_alu_reg_imm(inst, X86_CMP, param[0], num_labels);
        patch_fall_through = inst;
        x86_branch32(inst, X86_CC_AE, 0, 0);

        if(func->builder->position_independent)
        {
            /* TODO */
        }
        else
        {
            patch_jump_table = inst;
            x86_jump_memindex(inst, X86_NOBASEREG, 0, param[0], 2);
            while(((jit_nint) inst & (sizeof(void*) - 1)) != 0)
            {
                x86_nop(inst);
            }

            // displacement goes after opcode. ModR/M, and SIB bytes
            *((void **)(patch_jump_table + 3)) = inst;
        }

        for(index = 0; index < num_labels; index++)
        {
            block = jit_block_from_label(func, labels[index]);
            if(!block)
            {
                return;
            }

            if(func->builder->position_independent)
            {
                /* TODO */
            }
            else
            {
                if(block->address)
                {
                    x86_imm_emit32(inst, block->address);
                }
                else
                {
                    /* Output a placeholder and record on the block's fixup list */
                    x86_imm_emit32(inst, (int)(block->fixup_absolute_list));
                    block->fixup_absolute_list = (void *)(inst - 4);
                }
            }
        }

        x86_patch(patch_fall_through, inst);
    }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_BR:
case JIT_OP_BR_IFALSE:
case JIT_OP_BR_ITRUE:
case JIT_OP_BR_IEQ:
case JIT_OP_BR_INE:
case JIT_OP_BR_ILT:
case JIT_OP_BR_ILT_UN:
case JIT_OP_BR_ILE:
case JIT_OP_BR_ILE_UN:
case JIT_OP_BR_IGT:
case JIT_OP_BR_IGT_UN:
case JIT_OP_BR_IGE:
case JIT_OP_BR_IGE_UN:
case JIT_OP_IEQ:
case JIT_OP_INE:
case JIT_OP_ILT:
case JIT_OP_ILT_UN:
case JIT_OP_ILE:
case JIT_OP_ILE_UN:
case JIT_OP_IGT:
case JIT_OP_IGT_UN:
case JIT_OP_IGE:
case JIT_OP_IGE_UN:
case JIT_OP_BR_LFALSE:
case JIT_OP_BR_LTRUE:
case JIT_OP_BR_LEQ:
case JIT_OP_BR_LNE:
case JIT_OP_BR_LLT:
case JIT_OP_BR_LLT_UN:
case JIT_OP_BR_LLE:
case JIT_OP_BR_LLE_UN:
case JIT_OP_BR_LGT:
case JIT_OP_BR_LGT_UN:
case JIT_OP_BR_LGE:
case JIT_OP_BR_LGE_UN:
case JIT_OP_DEQ:
case JIT_OP_DEQ_INV:
case JIT_OP_DNE:
case JIT_OP_DNE_INV:
case JIT_OP_DLT:
case JIT_OP_DLT_INV:
case JIT_OP_DLE:
case JIT_OP_DLE_INV:
case JIT_OP_DGT:
case JIT_OP_DGT_INV:
case JIT_OP_DGE:
case JIT_OP_DGE_INV:
case JIT_OP_FEQ:
case JIT_OP_FEQ_INV:
case JIT_OP_FNE:
case JIT_OP_FNE_INV:
case JIT_OP_FLT:
case JIT_OP_FLT_INV:
case JIT_OP_FLE:
case JIT_OP_FLE_INV:
case JIT_OP_FGT:
case JIT_OP_FGT_INV:
case JIT_OP_FGE:
case JIT_OP_FGE_INV:
case JIT_OP_BR_DEQ:
case JIT_OP_BR_DEQ_INV:
case JIT_OP_BR_DNE:
case JIT_OP_BR_DNE_INV:
case JIT_OP_BR_DLT:
case JIT_OP_BR_DLT_INV:
case JIT_OP_BR_DLE:
case JIT_OP_BR_DLE_INV:
case JIT_OP_BR_DGT:
case JIT_OP_BR_DGT_INV:
case JIT_OP_BR_DGE:
case JIT_OP_BR_DGE_INV:
case JIT_OP_BR_FLT:
case JIT_OP_BR_FLT_INV:
case JIT_OP_BR_FLE:
case JIT_OP_BR_FLE_INV:
case JIT_OP_BR_FGT:
case JIT_OP_BR_FGT_INV:
case JIT_OP_BR_FGE:
case JIT_OP_BR_FGE_INV:
case JIT_OP_BR_FEQ:
case JIT_OP_BR_FEQ_INV:
case JIT_OP_BR_FNE:
case JIT_OP_BR_FNE_INV:
case JIT_OP_CHECK_NULL:
case JIT_OP_JUMP_TABLE:
	return 1;

#endif
