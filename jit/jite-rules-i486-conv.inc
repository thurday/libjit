/* Automatically generated from ./jite-rules-i486-conv.lir - DO NOT EDIT */
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

case JIT_OP_TRUNC_SBYTE:
{
	switch(state)
	{
	case 0x0:
	{
        if(param[1] == X86_EAX || param[1] == X86_EDX || param[1] == X86_ECX || param[1] == X86_EBX)
        {
            x86_widen_reg(inst, param[0], param[1], 1, 0);
        }
        else if(param[0] == X86_EAX || param[0] == X86_EDX || param[0] == X86_ECX || param[0] == X86_EBX)
        {
            x86_mov_reg_reg(inst, param[0], param[1], 4);
            x86_widen_reg(inst, param[0], param[0], 1, 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_widen_reg(inst, param[0], gpreg1, 1, 0);
            
        }
    }
	break;
	case 0x3:
	{
        x86_widen_membase(inst, param[0], X86_EBP, param[1], 1, 0);
    }
	break;
	case 0x6:
	{
        if(param[0] == X86_EAX || param[0] == X86_EDX || param[0] == X86_ECX || param[0] == X86_EBX)
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_widen_reg(inst, param[0], param[0], 1, 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_widen_reg(inst, param[0], gpreg1, 1, 0);
            
        }
    }
	break;
	case 0x1:
	{
        if(param[1] == X86_EAX || param[1] == X86_EDX || param[1] == X86_ECX || param[1] == X86_EBX)
        {
            find_one_gp_reg(inst);
            x86_widen_reg(inst, gpreg1, param[1], 1, 0);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_widen_reg(inst, gpreg1, gpreg1, 1, 0);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x4:
	{
        find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
        x86_widen_membase(inst, gpreg1, X86_EBP, param[1], 1, 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
        x86_mov_reg_imm(inst, gpreg1, param[1]);
        x86_widen_reg(inst, gpreg1, gpreg1, 1, 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	}
}
break;

case JIT_OP_TRUNC_UBYTE:
{
	switch(state)
	{
	case 0x0:
	{
        if(param[1] == X86_EAX || param[1] == X86_EDX || param[1] == X86_ECX || param[1] == X86_EBX)
        {
            x86_widen_reg(inst, param[0], param[1], 0, 0);
        }
        else if(param[0] == X86_EAX || param[0] == X86_EDX || param[0] == X86_ECX || param[0] == X86_EBX)
        {
            x86_mov_reg_reg(inst, param[0], param[1], 4);
            x86_widen_reg(inst, param[0], param[0], 0, 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_widen_reg(inst, param[0], gpreg1, 0, 0);
            
        }
    }
	break;
	case 0x3:
	{
        x86_widen_membase(inst, param[0], X86_EBP, param[1], 0, 0);
    }
	break;
	case 0x6:
	{
        if(param[0] == X86_EAX || param[0] == X86_EDX || param[0] == X86_ECX || param[0] == X86_EBX)
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_widen_reg(inst, param[0], param[0], 0, 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_widen_reg(inst, param[0], gpreg1, 0, 0);
            
        }
    }
	break;
	case 0x1:
	{
        if(param[1] == X86_EAX || param[1] == X86_EDX || param[1] == X86_ECX || param[1] == X86_EBX)
        {
            find_one_gp_reg(inst);
            x86_widen_reg(inst, gpreg1, param[1], 0, 0);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_widen_reg(inst, gpreg1, gpreg1, 0, 0);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x4:
	{
        find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
        x86_widen_membase(inst, gpreg1, X86_EBP, param[1], 0, 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
        x86_mov_reg_imm(inst, gpreg1, param[1]);
        x86_widen_reg(inst, gpreg1, gpreg1, 0, 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	}
}
break;

case JIT_OP_TRUNC_USHORT:
{
	switch(state)
	{
	case 0x0:
	{
        x86_widen_reg(inst, param[0], param[1], 0, 1);
    }
	break;
	case 0x3:
	{
        x86_widen_membase(inst, param[0], X86_EBP, param[1], 0, 1);
    }
	break;
	case 0x6:
	{
        x86_mov_reg_imm(inst, param[0], param[1]);
        x86_widen_reg(inst, param[0], param[0], 0, 1);
    }
	break;
	case 0x1:
	{
        find_one_gp_reg(inst);
        x86_widen_reg(inst, gpreg1, param[1], 0, 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_widen_membase(inst, gpreg1, X86_EBP, param[1], 0, 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, param[1]);
        x86_widen_reg(inst, gpreg1, gpreg1, 0, 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	}
}
break;

case JIT_OP_TRUNC_SHORT:
{
	switch(state)
	{
	case 0x0:
	{
        x86_widen_reg(inst, param[0], param[1], 1, 1);
    }
	break;
	case 0x3:
	{
        x86_widen_membase(inst, param[0], X86_EBP, param[1], 1, 1);
    }
	break;
	case 0x6:
	{
        x86_mov_reg_imm(inst, param[0], param[1]);
        x86_widen_reg(inst, param[0], param[0], 1, 1);
    }
	break;
	case 0x1:
	{
        find_one_gp_reg(inst);
        x86_widen_reg(inst, gpreg1, param[1], 1, 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_widen_membase(inst, gpreg1, X86_EBP, param[1], 1, 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, param[1]);
        x86_widen_reg(inst, gpreg1, gpreg1, 1, 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	}
}
break;

case JIT_OP_CHECK_SBYTE:
{
	switch(state)
	{
	case 0x0:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[1], -128);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_reg_imm(inst, X86_CMP, param[1], 127);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
    }
	break;
	case 0x3:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], -128);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 127);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
    }
	break;
	case 0x6:
	{
        if(param[1] < -128 || param[1] > 127)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_reg_imm(inst, param[0], param[1]);
    }
	break;
	case 0x1:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[1], -128);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_reg_imm(inst, X86_CMP, param[1], 127);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	case 0x4:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], -128);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 127);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x7:
	{
        if(param[1] < -128 || param[1] > 127)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	}
}
break;

case JIT_OP_CHECK_UBYTE:
{
	switch(state)
	{
	case 0x0:
	{
        unsigned char *patch1;
        x86_alu_reg_imm(inst, X86_CMP, param[1], 256);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
    }
	break;
	case 0x3:
	{
        unsigned char *patch1;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 256);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
    }
	break;
	case 0x6:
	{
        if(param[1] >= 256)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_reg_imm(inst, param[0], param[1]);
    }
	break;
	case 0x1:
	{
        unsigned char *patch1;
        x86_alu_reg_imm(inst, X86_CMP, param[1], 256);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	case 0x4:
	{
        unsigned char *patch1;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 256);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        if(param[1] >= 256)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	}
}
break;

case JIT_OP_CHECK_SHORT:
{
	switch(state)
	{
	case 0x0:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[1], -32768);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_reg_imm(inst, X86_CMP, param[1], 32767);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
    }
	break;
	case 0x3:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], -32768);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 32767);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
    }
	break;
	case 0x6:
	{
        if(param[1] < -32768 || param[1] > 32767)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_reg_imm(inst, param[0], param[1]);
    }
	break;
	case 0x1:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[1], -32768);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_reg_imm(inst, X86_CMP, param[1], 32767);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	case 0x4:
	{
        unsigned char *patch1;
        unsigned char *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], -32768);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 1);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 32767);
        patch2 = inst;
        x86_branch8(inst, X86_CC_LE, 0, 1);
        x86_patch(patch1, inst);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch2, inst);
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x7:
	{
        if(param[1] < -32768 || param[1] > 32767)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	}
}
break;

case JIT_OP_CHECK_USHORT:
{
	switch(state)
	{
	case 0x0:
	{
        unsigned char *patch1;
        x86_alu_reg_imm(inst, X86_CMP, param[1], 65536);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
    }
	break;
	case 0x3:
	{
        unsigned char *patch1;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 65536);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
    }
	break;
	case 0x6:
	{
        if(param[1] >= 65536)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_reg_imm(inst, param[0], param[1]);
    }
	break;
	case 0x1:
	{
        unsigned char *patch1;
        x86_alu_reg_imm(inst, X86_CMP, param[1], 65536);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	case 0x4:
	{
        unsigned char *patch1;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 65536);
        patch1 = inst;
        x86_branch8(inst, X86_CC_LT, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x7:
	{
        if(param[1] >= 65536)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	}
}
break;

case JIT_OP_CHECK_INT:
case JIT_OP_CHECK_UINT:
{
	switch(state)
	{
	case 0x0:
	{
        unsigned char *patch1;
        x86_alu_reg_imm(inst, X86_CMP, param[1], 0);
        patch1 = inst;
        x86_branch8(inst, X86_CC_GE, 0, 1);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
    }
	break;
	case 0x3:
	{
        unsigned char *patch1;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 0);
        patch1 = inst;
        x86_branch8(inst, X86_CC_GE, 0, 1);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
    }
	break;
	case 0x6:
	{
        if(param[1] < 0)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_reg_imm(inst, param[0], param[1]);
    }
	break;
	case 0x1:
	{
        unsigned char *patch1;
        x86_alu_reg_imm(inst, X86_CMP, param[1], 0);
        patch1 = inst;
        x86_branch8(inst, X86_CC_GE, 0, 1);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	case 0x4:
	{
        unsigned char *patch1;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], 0);
        patch1 = inst;
        x86_branch8(inst, X86_CC_GE, 0, 1);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        x86_patch(patch1, inst);
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x7:
	{
        if(param[1] < 0)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_OVERFLOW);
        }
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	}
}
break;

case JIT_OP_LOW_WORD:
{
	switch(state)
	{
	case 0x6:
	{
        jit_uint value = ((jit_uint *)(param[1]))[0];
        x86_mov_reg_imm(inst, param[0], value);
    }
	break;
	case 0x7:
	{
        jit_uint value = ((jit_uint *)(param[1]))[0];
        x86_mov_membase_imm(inst, X86_EBP, param[0], value, 4);
    }
	break;
	case 0x3:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
    }
	break;
	case 0x0:
	{
        x86_mov_reg_reg(inst, param[0], param[1], 4);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x1:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
    }
	break;
	}
}
break;

case JIT_OP_EXPAND_INT:
{
	switch(state)
	{
	case 0x7:
	{
        jit_int value = (jit_int)(param[1]);
        x86_mov_membase_imm(inst, X86_EBP, param[0], value, 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, value, 4);
        x86_shift_membase_imm(inst, X86_SAR, X86_EBP, param[0] + 4, 31);
    }
	break;
	case 0x1:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, param[1], 4);
        x86_shift_membase_imm(inst, X86_SAR, X86_EBP, param[0] + 4, 31);
    }
	break;
	case 0x4:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        if(param[0] != param[1])
        {
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        }
        x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        
    }
	break;
	case 0x6:
	{
        x86_mov_reg_imm(inst, param[0], param[1]);
        x86_mov_reg_imm(inst, jite_register_pair(param[0]), param[1]);
        x86_shift_reg_imm(inst, X86_SAR, jite_register_pair(param[0]), 31);
    }
	break;
	case 0x3:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        x86_mov_reg_reg(inst, jite_register_pair(param[0]), param[0], 4);
        x86_shift_reg_imm(inst, X86_SAR, jite_register_pair(param[0]), 31);
    }
	break;
	case 0x0:
	{
        x86_mov_reg_reg(inst, param[0], param[1], 4);
        x86_mov_reg_reg(inst, jite_register_pair(param[0]), param[1], 4);
        x86_shift_reg_imm(inst, X86_SAR, jite_register_pair(param[0]), 31);
    }
	break;
	}
}
break;

case JIT_OP_EXPAND_UINT:
{
	switch(state)
	{
	case 0x7:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, 0, 4);
    }
	break;
	case 0x1:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, 0, 4);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, 0, 4);
    }
	break;
	case 0x6:
	{
        x86_mov_reg_imm(inst, param[0], param[1]);
        x86_mov_reg_imm(inst, jite_register_pair(param[0]), 0);
    }
	break;
	case 0x3:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        x86_mov_reg_imm(inst, jite_register_pair(param[0]), 0);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
        x86_mov_reg_imm(inst, jite_register_pair(param[0]), 0);
    }
	break;
	}
}
break;

case JIT_OP_NFLOAT_TO_FLOAT32:
{
	switch(state)
	{
	case 0x6:
	{
        jit_float32 value = (jit_float32)((jit_nfloat *)(param[1]))[0];
        jit_float32 *value_ptr = &value;
        jit_uint *ptr = (jit_uint*)value_ptr;
        x86_mov_membase_imm(inst, X86_ESP, -8, ptr[0], 4);
        sse_movss_xmreg_membase(inst, param[0], X86_ESP, -8);
    }
	break;
	case 0x3:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_ESP, -8, 0, 1);
            sse_movss_xmreg_membase(inst, param[0], X86_ESP, -8);
        }
        else
        {
            sse_cvtsd2ss_xmreg_membase(inst, param[0], X86_EBP, param[1]);
        }
    }
	break;
	case 0x0:
	{
        sse_cvtsd2ss_xmreg_xmreg(inst, param[0], param[1]);
    }
	break;
	case 0x7:
	{
        jit_float32 value = (jit_float32)((jit_nfloat *)(param[1]))[0];
        jit_float32 *value_ptr = &value;
        jit_uint *ptr = (jit_uint*)value_ptr;
        x86_mov_membase_imm(inst, X86_EBP, param[0], ptr[0], 4);
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_EBP, param[0], 0, 1);
        }
        else
        {
                find_one_xmm_reg(inst);
                sse_cvtsd2ss_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
                    sse_movss_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	case 0x1:
	{
        find_one_xmm_reg(inst);
        sse_cvtsd2ss_xmreg_xmreg(inst, xmmreg1, param[1]);
            sse_movss_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
        
    }
	break;
	}
}
break;

case JIT_OP_NFLOAT_TO_FLOAT64:
{
	switch(state)
	{
	case 0x6:
	{
        jit_float64 value = (jit_float64)((jit_nfloat *)(param[1]))[0];
        jit_float64 *value_ptr = &value;
        jit_uint *ptr = (jit_uint*)value_ptr;
        x86_mov_membase_imm(inst, X86_ESP, -8, ptr[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -4, ptr[1], 4);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -8);
    }
	break;
	case 0x3:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_ESP, -8, 1, 1);
            sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -8);
        }
        else
        {
            sse2_movsd_xmreg_membase(inst, param[0], X86_EBP, param[1]);
        }
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1]) sse2_movsd_xmreg_xmreg(inst, param[0], param[1]);
    }
	break;
	case 0x7:
	{
        jit_float64 value = (jit_float64)((jit_nfloat *)(param[1]))[0];
        jit_float64 *value_ptr = &value;
        jit_uint *ptr = (jit_uint*)value_ptr;
        x86_mov_membase_imm(inst, X86_EBP, param[0], ptr[0], 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, ptr[1], 4);
    }
	break;
	case 0x4:
	{
            if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_EBP, param[0], 1, 1);
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	case 0x1:
	{
        sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], param[1]);
    }
	break;
	}
}
break;

case JIT_OP_FLOAT32_TO_NFLOAT:
{
	switch(state)
	{
	case 0x6:
	{
        jit_float64 value = (jit_float64)((jit_float32 *)(param[1]))[0];
        jit_float64 *value_ptr = &value;
        jit_uint *ptr = (jit_uint*)value_ptr;
        x86_mov_membase_imm(inst, X86_ESP, -8, ptr[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -4, ptr[1], 4);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -8);
    }
	break;
	case 0x3:
	{
        sse2_cvtss2sd_xmreg_membase(inst, param[0], X86_EBP, param[1]);
    }
	break;
	case 0x0:
	{
        sse2_cvtss2sd_xmreg_xmreg(inst, param[0], param[1]);
    }
	break;
	case 0x7:
	{
        jit_nfloat value = (jit_nfloat)((jit_float32 *)(param[1]))[0];
        jit_nfloat *value_ptr = &value;
        jit_uint *ptr = (jit_uint*)value_ptr;
        x86_mov_membase_imm(inst, X86_EBP, param[0], ptr[0], 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, ptr[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, ptr[2], 4);
        }
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld_membase(inst, X86_EBP, param[1], 0);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_cvtss2sd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	case 0x1:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            sse_movss_membase_xmreg(inst, X86_ESP, -8, param[1]);
            x86_fld_membase(inst, X86_ESP, -8, 0);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_cvtss2sd_xmreg_xmreg(inst, xmmreg1, param[1]);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	}
}
break;

case JIT_OP_FLOAT64_TO_NFLOAT:
{
	switch(state)
	{
	case 0x6:
	{
        jit_nfloat value = (jit_nfloat)((jit_float64 *)(param[1]))[0];
        jit_nfloat *value_ptr = &value;
        jit_uint *ptr = (jit_uint*)value_ptr;
        x86_mov_membase_imm(inst, X86_ESP, -8, ptr[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -4, ptr[1], 4);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -8);
    }
	break;
	case 0x3:
	{
        sse2_movsd_xmreg_membase(inst, param[0], X86_EBP, param[1]);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1]) sse2_movsd_xmreg_xmreg(inst, param[0], param[1]);
    }
	break;
	case 0x7:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            jit_nfloat value = (jit_nfloat)((jit_float64 *)(param[1]))[0];
            jit_nfloat *value_ptr = &value;
            jit_uint *ptr = (jit_uint*)value_ptr;
            x86_mov_membase_imm(inst, X86_EBP, param[0], ptr[0], 4);
            x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, ptr[1], 4);
            x86_mov_membase_imm(inst, X86_EBP, param[0] + 8, ptr[2], 4);
        }
        else
        {
            jit_float64 value = (jit_float64)((jit_float64 *)(param[1]))[0];
            jit_float64 *value_ptr = &value;
            jit_uint *ptr = (jit_uint*)value_ptr;
            x86_mov_membase_imm(inst, X86_EBP, param[0], ptr[0], 4);
            x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, ptr[1], 4);
        }

    }
	break;
	case 0x4:
	{
            if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld_membase(inst, X86_EBP, param[1], 1);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	case 0x1:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            sse2_movsd_membase_xmreg(inst, X86_ESP, -8, param[1]);
            x86_fld_membase(inst, X86_ESP, -8, 1);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], param[1]);
        }
    }
	break;
	}
}
break;

case JIT_OP_INT_TO_NFLOAT:
{
	switch(state)
	{
	case 0x6:
	{
        jit_nfloat float_value = (jit_nfloat)((jit_int)param[1]);
        void *ptr = &float_value;
        jit_uint *ptr_u = (jit_uint*)ptr;
        x86_mov_membase_imm(inst, X86_ESP, -8, ptr_u[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -4, ptr_u[1], 4);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -8);
    }
	break;
	case 0x3:
	{
        sse2_cvtsi2sd_xmreg_membase(inst, param[0], X86_EBP, param[1]);
    }
	break;
	case 0x0:
	{
        sse2_cvtsi2sd_xmreg_reg(inst, param[0], param[1]);
    }
	break;
	case 0x7:
	{
        jit_nfloat float_value = (jit_nfloat)((jit_int)param[1]);
        void *ptr = &float_value;
        jit_uint *ptr_u = (jit_uint*)ptr;
        x86_mov_membase_imm(inst, X86_EBP, param[0], ptr_u[0], 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, ptr_u[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_membase_imm(inst, X86_EBP, param[0] + 8, ptr_u[2], 4);
        }
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fild_membase(inst, X86_EBP, param[1], 0);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_cvtsi2sd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	case 0x1:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_membase_reg(inst, X86_ESP, -8, param[1], 4);
            x86_fild_membase(inst, X86_ESP, -8, 0);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_cvtsi2sd_xmreg_reg(inst, xmmreg1, param[1]);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	}
}
break;

case JIT_OP_NFLOAT_TO_INT:
{
	switch(state)
	{
	case 0x6:
	{
        jit_nfloat float_value = ((jit_nfloat *)(param[1]))[0];
        jit_int int_value = (jit_int)(float_value);
        x86_mov_reg_imm(inst, param[0], int_value);
    }
	break;
	case 0x3:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_ESP, -8, 1, 1);
            sse2_cvttsd2si_reg_membase(inst, param[0], X86_ESP, -8);
        }
        else
        {
            sse2_cvttsd2si_reg_membase(inst, param[0], X86_EBP, param[1]);
        }
    }
	break;
	case 0x0:
	{
        sse2_cvttsd2si_reg_xmreg(inst, param[0], param[1]);
    }
	break;
	case 0x7:
	{
        jit_nfloat float_value = ((jit_nfloat *)(param[1]))[0];
        jit_int int_value = (jit_int)(float_value);
        x86_mov_membase_imm(inst, X86_EBP, param[0], int_value, 4);
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_ESP, -32, 1, 1);
            find_one_gp_reg(inst);
            sse2_cvttsd2si_reg_membase(inst, gpreg1, X86_ESP, -32);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            find_one_gp_reg(inst);
            sse2_cvttsd2si_reg_membase(inst, gpreg1, X86_EBP, param[1]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x1:
	{
            find_one_gp_reg(inst);
        sse2_cvttsd2si_reg_xmreg(inst, gpreg1, param[1]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	}
}
break;

case JIT_OP_UINT_TO_NFLOAT:
{
	switch(state)
	{
	case 0x6:
	{
        jit_float64 float_value = (jit_float64)((jit_uint)param[1]);
        void *ptr = &float_value;
        jit_uint *ptr_u = (jit_uint*)ptr;
        x86_mov_membase_imm(inst, X86_ESP, -8, ptr_u[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -4, ptr_u[1], 4);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -8);
    }
	break;
	case 0x3:
	{
        unsigned int value = 0x43300000;
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_membase_imm(inst, X86_ESP, -28, value, 4);
        x86_mov_membase_reg(inst, X86_ESP, -32, gpreg1, 4);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -32);
        x86_mov_membase_imm(inst, X86_ESP, -32, 0, 4);
        sse2_subsd_xmreg_membase(inst, param[0], X86_ESP, -32);
        
    }
	break;
	case 0x0:
	{
        unsigned int value = 0x43300000;
        x86_mov_membase_imm(inst, X86_ESP, -4, value, 4);
        x86_mov_membase_reg(inst, X86_ESP, -8, param[1], 4);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -8);
        x86_mov_membase_imm(inst, X86_ESP, -8, 0, 4);
        sse2_subsd_xmreg_membase(inst, param[0], X86_ESP, -8);
    }
	break;
	case 0x7:
	{
        jit_nfloat float_value = (jit_nfloat)((jit_uint)param[1]);
        void *ptr = &float_value;
        jit_uint *ptr_u = (jit_uint*)ptr;
        x86_mov_membase_imm(inst, X86_EBP, param[0], ptr_u[0], 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, ptr_u[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_membase_imm(inst, X86_EBP, param[0] + 8, ptr_u[2], 4);
        }
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_push_imm(inst, 0);
            x86_push_membase(inst, X86_EBP, param[1]);
            x86_fild_membase(inst, X86_ESP, 0, 1);
            x86_alu_reg_imm(inst, X86_ADD, X86_ESP, sizeof(jit_long));
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            unsigned int value = 0x43300000;
                find_one_gp_reg(inst);
                find_one_xmm_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_imm(inst, X86_ESP, -44, value, 4);
            x86_mov_membase_reg(inst, X86_ESP, -48, gpreg1, 4);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_ESP, -48);
            x86_mov_membase_imm(inst, X86_ESP, -48, 0, 4);
            sse2_subsd_xmreg_membase(inst, xmmreg1, X86_ESP, -48);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
            
        }
    }
	break;
	case 0x1:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_push_imm(inst, 0);
            x86_push_reg(inst, param[1]);
            x86_fild_membase(inst, X86_ESP, 0, 1);
            x86_alu_reg_imm(inst, X86_ADD, X86_ESP, sizeof(jit_long));
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            find_one_xmm_reg(inst);
            unsigned int value = 0x43300000;
            x86_mov_membase_imm(inst, X86_EBP, param[0] + 4, value, 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[0]);
            x86_mov_membase_imm(inst, X86_EBP, param[0], 0, 4);
            sse2_subsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[0]);
            sse2_movsd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	}
}
break;

case JIT_OP_NFLOAT_TO_UINT:
{
	switch(state)
	{
	case 0x6:
	{
        jit_nfloat float_value = ((jit_nfloat *)(param[1]))[0];
        jit_uint uint_value = (jit_uint)(float_value);
        x86_mov_reg_imm(inst, param[0], uint_value);
    }
	break;
	case 0x3:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {            
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_ESP, -32, 1, 1);
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
            unsigned int value = 0x43300000;
            x86_mov_membase_imm(inst, X86_ESP, -28, value, 4);
            x86_mov_membase_imm(inst, X86_ESP, -32, 0, 4);
            sse2_addsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
            sse2_movd_reg_xmreg(inst, gpreg1, xmmreg1);
                    
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            unsigned int value = 0x43300000;
            x86_mov_membase_imm(inst, X86_ESP, -28, value, 4);
                x86_mov_membase_imm(inst, X86_ESP, -32, 0, 4);
            sse2_addsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
            sse2_movd_reg_xmreg(inst, gpreg1, xmmreg1);
            
        }
    }
	break;
	case 0x0:
	{
        find_one_xmm_reg(inst);
        if(param[1] != xmmreg1) sse2_movsd_xmreg_xmreg(inst, xmmreg1, param[1]);
        unsigned int value = 0x43300000;
        x86_mov_membase_imm(inst, X86_ESP, -28, value, 4);
        x86_mov_membase_imm(inst, X86_ESP, -32, 0, 4);
        sse2_addsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
        sse2_movd_reg_xmreg(inst, param[0], xmmreg1);
        
    }
	break;
	case 0x7:
	{
        jit_float64 float_value = ((jit_float64 *)(param[1]))[0];
        jit_uint uint_value = (jit_uint)(float_value);
        x86_mov_membase_imm(inst, X86_EBP, param[0], uint_value, 4);
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
            x86_fst_membase(inst, X86_ESP, -32, 1, 1);    
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
            unsigned int value = 0x43300000;
            x86_mov_membase_imm(inst, X86_ESP, -28, value, 4);
            x86_mov_membase_imm(inst, X86_ESP, -32, 0, 4);
            sse2_addsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
            sse2_movd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
                        
        }
        else
        {
            find_one_xmm_reg(inst);
            sse2_movsd_xmreg_membase(inst, xmmreg1, X86_EBP, param[1]);
            unsigned int value = 0x43300000;
            x86_mov_membase_imm(inst, X86_ESP, -28, value, 4);
            x86_mov_membase_imm(inst, X86_ESP, -32, 0, 4);
            sse2_addsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
            sse2_movd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
            
        }
    }
	break;
	case 0x1:
	{
        find_one_xmm_reg(inst);
        sse2_movsd_xmreg_xmreg(inst, xmmreg1, param[1]);
        unsigned int value = 0x43300000;
        x86_mov_membase_imm(inst, X86_ESP, -28, value, 4);
        x86_mov_membase_imm(inst, X86_ESP, -32, 0, 4);
        sse2_addsd_xmreg_membase(inst, xmmreg1, X86_ESP, -32);
        sse2_movd_membase_xmreg(inst, X86_EBP, param[0], xmmreg1);
        
    }
	break;
	}
}
break;

case JIT_OP_LONG_TO_NFLOAT:
{
	switch(state)
	{
	case 0x6:
	{
        jit_long long_value = *((jit_long*)(param[1]));
        jit_nfloat float_value = (jit_nfloat)(long_value);
        inst = masm_mov_reg_imm(inst, param[0], (jit_nint)(&float_value), jit_value_get_type(dest));
    }
	break;
	case 0x3:
	{
        x86_fild_membase(inst, X86_EBP, param[1], 1);
        x86_fst_membase(inst, X86_ESP, -16, 1, 1);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -16);
    }
	break;
	case 0x0:
	{
        inst = masm_mov_membase_reg(inst, X86_ESP, -16, param[1], sourceType);
        x86_fild_membase(inst, X86_ESP, -16, 1);
        x86_fst_membase(inst, X86_ESP, -16, 1, 1);
        sse2_movsd_xmreg_membase(inst, param[0], X86_ESP, -16);
    }
	break;
	case 0x7:
	{
        jit_long long_value = *((jit_long*)(param[1]));
        jit_nfloat float_value = (jit_nfloat)long_value;
        inst = masm_mov_membase_imm(inst, X86_EBP, param[0], (jit_nint)(&float_value), jit_value_get_type(dest));
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fild_membase(inst, X86_EBP, param[1], 1);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            x86_fild_membase(inst, X86_EBP, param[1], 1);
            x86_fst_membase(inst, X86_EBP, param[0], 1, 1);
        }
    }
	break;
	case 0x1:
	{
        inst = masm_mov_membase_reg(inst, X86_ESP, -16, param[1], sourceType);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fild_membase(inst, X86_ESP, -16, 1);
            x86_fst80_membase(inst, X86_EBP, param[0]);
        }
        else
        {
            x86_fild_membase(inst, X86_ESP, -16, 1);
            x86_fst_membase(inst, X86_EBP, param[0], 1, 1);
        }
    }
	break;
	}
}
break;

case JIT_OP_NFLOAT_TO_LONG:
{
	switch(state)
	{
	case 0x7:
	{
        jit_nfloat float_value = *((jit_nfloat*)(param[1]));
        jit_long long_value = (jit_long)(float_value);
        inst = masm_mov_membase_imm(inst, X86_EBP, param[0], (jit_nint)(&long_value), jit_value_get_type(dest));
    }
	break;
	case 0x6:
	{
        jit_nfloat float_value = *((jit_nfloat*)(param[1]));
        jit_long long_value = (jit_long)(float_value);
        inst = masm_mov_reg_imm(inst, param[0], (jit_nint)(&long_value), jit_value_get_type(dest));
    }
	break;
	case 0x4:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
        }
        else
        {
            x86_fld_membase(inst, X86_EBP, param[1], 1);
        }
        x86_fist_pop_membase(inst, X86_EBP, param[0], 1);
    }
	break;
	case 0x3:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_fld80_membase(inst, X86_EBP, param[1]);
        }
        else
        {
            x86_fld_membase(inst, X86_EBP, param[1], 1);
        }
        x86_fist_pop_membase(inst, X86_ESP, -16, 1);
        inst = masm_mov_reg_membase(inst, param[0], X86_ESP, -16, jit_value_get_type(dest));
    }
	break;
	case 0x1:
	{
        sse2_movsd_membase_xmreg(inst, X86_ESP, -16, param[1]);
        x86_fld_membase(inst, X86_ESP, -16, 1);
        x86_fist_pop_membase(inst, X86_EBP, param[0], 1);
    }
	break;
	case 0x0:
	{
        sse2_movsd_membase_xmreg(inst, X86_ESP, -16, param[1]);
        x86_fld_membase(inst, X86_ESP, -16, 1);
        x86_fist_pop_membase(inst, X86_ESP, -16, 1);
        inst = masm_mov_reg_membase(inst, param[0], X86_ESP, -16, jit_value_get_type(dest));
    }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_TRUNC_SBYTE:
case JIT_OP_TRUNC_UBYTE:
case JIT_OP_TRUNC_USHORT:
case JIT_OP_TRUNC_SHORT:
case JIT_OP_CHECK_SBYTE:
case JIT_OP_CHECK_UBYTE:
case JIT_OP_CHECK_SHORT:
case JIT_OP_CHECK_USHORT:
case JIT_OP_CHECK_INT:
case JIT_OP_CHECK_UINT:
case JIT_OP_LOW_WORD:
case JIT_OP_EXPAND_INT:
case JIT_OP_EXPAND_UINT:
case JIT_OP_NFLOAT_TO_FLOAT32:
case JIT_OP_NFLOAT_TO_FLOAT64:
case JIT_OP_FLOAT32_TO_NFLOAT:
case JIT_OP_FLOAT64_TO_NFLOAT:
case JIT_OP_INT_TO_NFLOAT:
case JIT_OP_NFLOAT_TO_INT:
case JIT_OP_UINT_TO_NFLOAT:
case JIT_OP_NFLOAT_TO_UINT:
case JIT_OP_LONG_TO_NFLOAT:
case JIT_OP_NFLOAT_TO_LONG:
	return 1;

#endif
