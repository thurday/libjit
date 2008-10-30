/* Automatically generated from ./jite-rules-i486-arith.lir - DO NOT EDIT */
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

case JIT_OP_IADD:
case JIT_OP_IOR:
case JIT_OP_IAND:
case JIT_OP_IXOR:
case JIT_OP_ADD_RELATIVE:
case JIT_OP_LADD:
case JIT_OP_LOR:
case JIT_OP_LAND:
case JIT_OP_LXOR:
{
	switch(state)
	{
	case 0x19:
	{
        inst = masm_mov_membase_imm(inst, X86_EBP, param[0], param[1], sourceType);
        inst = masm_alu_membase_imm(inst, machine_code1, machine_code2, X86_EBP, param[0], param[2]);
    }
	break;
	case 0x10:
	{
        if(param[0] != param[2])
        {
            find_one_gp_reg(inst);
            if(!machine_code2)
            {
                x86_mov_reg_imm(inst, gpreg1, param[1]);
            }
            else
            {
                x86_mov_reg_imm(inst, gpreg1, ((jit_nint *)(param[1]))[0]);
            }
            x86_alu_reg_membase(inst, machine_code1, gpreg1, X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            if(machine_code2)
            {
                x86_mov_reg_imm(inst, gpreg1, ((jit_nint *)(param[1]))[1]);
                x86_alu_reg_membase(inst, machine_code2, gpreg1, X86_EBP, param[2] + 4);
                x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
            }
            
        }
        else
        {
            inst = masm_alu_membase_imm(inst, machine_code1, machine_code2, X86_EBP, param[0], param[1]);
        }
    }
	break;
	case 0x16:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            if(!machine_code2)
            {
                x86_mov_reg_imm(inst, gpreg1, param[2]);
            }
            else
            {
                x86_mov_reg_imm(inst, gpreg1, ((jit_nint *)(param[2]))[0]);
            }
            x86_alu_reg_membase(inst, machine_code1, gpreg1, X86_EBP, param[1]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            if(machine_code2)
            {
                x86_mov_reg_imm(inst, gpreg1, ((jit_nint *)(param[2]))[1]);
                x86_alu_reg_membase(inst, machine_code2, gpreg1, X86_EBP, param[1] + 4);
                x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
            }
            
        }
        else
        {
            inst = masm_alu_membase_imm(inst, machine_code1, machine_code2, X86_EBP, param[0], param[2]);
        }
    }
	break;
	case 0x7:
	{
        if(!jite_gp_reg_is_free(func, param[2]))
        {
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
                inst = masm_alu_membase_imm(inst, machine_code1, machine_code2, X86_EBP, param[0], param[1]);
        }
        else
        {
            inst = masm_alu_reg_imm(inst, machine_code1, machine_code2, param[2], param[1]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
        }
    }
	break;
	case 0xd:
	{
        find_one_gp_reg(inst);
        if(param[0] != param[1] && param[0] != param[2])
        {
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_reg_membase(inst, machine_code1, gpreg1, X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            if(machine_code2)
            {
                x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
                x86_alu_reg_membase(inst, machine_code2, gpreg1, X86_EBP, param[2] + 4);
                x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
            }
        }
        else if(param[0] == param[1])
        {
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
            x86_alu_membase_reg(inst, machine_code1, X86_EBP, param[0], gpreg1);
            if(machine_code2)
            {
                x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
                x86_alu_membase_reg(inst, machine_code2, X86_EBP, param[0], gpreg1);
            }
        }
        else
        {
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_membase_reg(inst, machine_code1, X86_EBP, param[0], gpreg1);
            if(machine_code2)
            {
                x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
                x86_alu_membase_reg(inst, machine_code2, X86_EBP, param[0], gpreg1);
            }
        }
        
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            if(jite_gp_reg_is_free(func, param[2]))
            {
                inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[2], X86_EBP, param[1]);
                inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
            }
            else if(have_free_gpregs())
            {
                find_one_gp_reg(inst);
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                x86_alu_reg_membase(inst, machine_code1, gpreg1, X86_EBP, param[1]);
                x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
                if(machine_code2)
                {
                    x86_mov_reg_reg(inst, gpreg1, jite_register_pair(param[2]), 4);
                    x86_alu_reg_membase(inst, machine_code2, gpreg1, X86_EBP, param[1] + 4);
                    x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
                }

                
            }
            else
            {
                inst = masm_mov_membase_reg(inst, X86_ESP, -8, param[2], sourceType);
                inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[2], X86_EBP, param[1]);
                inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
                inst = masm_mov_reg_membase(inst, param[2], X86_ESP, -8, sourceType);
            }
        }
        else
        {
            inst = masm_alu_membase_reg(inst, machine_code1, machine_code2, X86_EBP, param[0], param[2]);
        }
    }
	break;
	case 0x13:
	{
        if(!jite_gp_reg_is_free(func, param[1]))
        {
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
            inst = masm_alu_membase_imm(inst, machine_code1, machine_code2, X86_EBP, param[0], param[2]); 
        }
        else
        {
            inst = masm_alu_reg_imm(inst, machine_code1, machine_code2, param[1], param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
    }
	break;
	case 0xa:
	{
        if(param[0] != param[2])
        {
            if(jite_gp_reg_is_free(func, param[1]))
            {
                inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[1], X86_EBP, param[2]);
                inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
            }
            else if(have_free_gpregs())
            {
                find_one_gp_reg(inst);
                x86_mov_reg_reg(inst, gpreg1, param[1], 4);
                x86_alu_reg_membase(inst, machine_code1, gpreg1, X86_EBP, param[2]);
                x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
                if(machine_code2)
                {
                    x86_mov_reg_reg(inst, gpreg1, jite_register_pair(param[1]), 4);
                    x86_alu_reg_membase(inst, machine_code2, gpreg1, X86_EBP, param[2] + 4);
                    x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
                }

                
            }
            else
            {
                inst = masm_mov_membase_reg(inst, X86_ESP, -8, param[1], sourceType);
                inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[1], X86_EBP, param[2]);
                inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
                inst = masm_mov_reg_membase(inst, param[1], X86_ESP, -8, sourceType);
            }
        }
        else
        {
            inst = masm_alu_membase_reg(inst, machine_code1, machine_code2, X86_EBP, param[0], param[1]);
        }
    }
	break;
	case 0x1:
	{
        if(!jite_gp_reg_is_free(func, param[1]) && !jite_gp_reg_is_free(func, param[2]))
        {
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
                inst = masm_alu_membase_reg(inst, machine_code1, machine_code2, X86_EBP, param[0], param[2]);
        }
        else if(jite_gp_reg_is_free(func, param[1]))
        {
            inst = masm_alu_reg_reg(inst, machine_code1, machine_code2, param[1], param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
        else
        {
            inst = masm_alu_reg_reg(inst, machine_code1, machine_code2, param[2], param[1]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
        }
    }
	break;
	case 0x18:
	{
        inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
        inst = masm_alu_reg_imm(inst, machine_code1, machine_code2, param[0], param[2]);
    }
	break;
	case 0xf:
	{
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[2], sourceType);
        inst = masm_alu_reg_imm(inst, machine_code1, machine_code2, param[0], param[1]);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2]) inst = masm_mov_reg_reg(inst, param[0], param[2], sourceType);
        inst = masm_alu_reg_imm(inst, machine_code1, machine_code2, param[0], param[1]);
    }
	break;
	case 0x15:
	{
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        inst = masm_alu_reg_imm(inst, machine_code1, machine_code2, param[0], param[2]);
    }
	break;
	case 0xc:
	{
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[0], X86_EBP, param[2]);
    }
	break;
	case 0x3:
	{
        if(param[0] != param[2]) inst = masm_mov_reg_reg(inst, param[0], param[2], sourceType);
        inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[0], X86_EBP, param[1]);
    }
	break;
	case 0x12:
	{
        if(param[0] != param[1]) inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
        inst = masm_alu_reg_imm(inst, machine_code1, machine_code2, param[0], param[2]);
    }
	break;
	case 0x9:
	{
        if(param[0] != param[1])
        {
                inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
                inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[0], X86_EBP, param[2]);
        }
        else inst = masm_alu_reg_membase(inst, machine_code1, machine_code2, param[0], X86_EBP, param[2]);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1] && param[0] != param[2])
        {
            inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
            inst = masm_alu_reg_reg(inst, machine_code1, machine_code2, param[0], param[2]);
        }
        else if(param[0] == param[2]) inst = masm_alu_reg_reg(inst, machine_code1, machine_code2, param[0], param[1]);
        else inst = masm_alu_reg_reg(inst, machine_code1, machine_code2, param[0], param[2]);
    }
	break;
	}
}
break;

case JIT_OP_ISUB:
{
	switch(state)
	{
	case 0x19:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1] - param[2], 4);
    }
	break;
	case 0x10:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_imm(inst, gpreg1, param[1]);
        x86_alu_reg_membase(inst, X86_SUB, gpreg1, X86_EBP, param[2]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
        x86_alu_membase_reg(inst, X86_SUB, X86_EBP, param[0], param[2]);
    }
	break;
	case 0x16:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_reg_imm(inst, X86_SUB, gpreg1, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else x86_alu_membase_imm(inst, X86_SUB, X86_EBP, param[0], param[2]);
    }
	break;
	case 0xd:
	{
        if(param[0] != param[1] && param[1] != param[2])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_reg_membase(inst, X86_SUB, gpreg1, X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else if(param[0] == param[1] && param[1] != param[2])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
            x86_alu_membase_reg(inst, X86_SUB, X86_EBP, param[0], gpreg1);
            
        }
        else
        {
            x86_mov_membase_imm(inst, X86_EBP, param[0], 0, 4);
        }
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg_cond1(inst, param[2]);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_reg_reg(inst, X86_SUB, gpreg1, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            x86_alu_membase_reg(inst, X86_SUB, X86_EBP, param[0], param[2]);
        }
    }
	break;
	case 0x13:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        if(param[2]) x86_alu_membase_imm(inst, X86_SUB, X86_EBP, param[0], param[2]); 
    }
	break;
	case 0xa:
	{
        if(jite_gp_reg_is_free(func, param[1]))
        {
            x86_alu_reg_membase(inst, X86_SUB, param[1], X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
        else if(have_free_gpregs())
        {
            find_one_gp_reg(inst);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_alu_reg_membase(inst, X86_SUB, gpreg1, X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            x86_mov_membase_reg(inst, X86_ESP, -4, param[1], 4);
            x86_alu_reg_membase(inst, X86_SUB, param[1], X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
            x86_mov_reg_membase(inst, param[1], X86_ESP, -4, 4);
        }
    }
	break;
	case 0x1:
	{
        if(!jite_gp_reg_is_free(func, param[1]))
        {
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
            x86_alu_membase_reg(inst, X86_SUB, X86_EBP, param[0], param[2]);
        }
        else
        {
            x86_alu_reg_reg(inst, X86_SUB, param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
    }
	break;
	case 0x18:
	{
        x86_mov_reg_imm(inst, param[0], param[1] - param[2]);
    }
	break;
	case 0xf:
	{
        x86_mov_reg_imm(inst, param[0], param[1]);
        x86_alu_reg_membase(inst, X86_SUB, param[0], X86_EBP, param[2]);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2])
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_alu_reg_reg(inst, X86_SUB, param[0], param[2]);
        }
        else
        {
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_alu_reg_reg(inst, X86_SUB, gpreg1, param[2]);
            x86_mov_reg_reg(inst, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x15:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        if(param[2]) x86_alu_reg_imm(inst, X86_SUB, param[0], param[2]);
    }
	break;
	case 0xc:
	{
        if(param[1] != param[2])
        {
            x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
            x86_alu_reg_membase(inst, X86_SUB, param[0], X86_EBP, param[2]);
        }
        else x86_clear_reg(inst, param[0]);
    }
	break;
	case 0x3:
	{
        if(param[0] != param[2])
        {
            x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
            x86_alu_reg_reg(inst, X86_SUB, param[0], param[2]);
        }
        else
        {
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_alu_reg_reg(inst, X86_SUB, gpreg1, param[2]);
            x86_mov_reg_reg(inst, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x12:
	{
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
        if(param[2]) x86_alu_reg_imm(inst, X86_SUB, param[0], param[2]);
    }
	break;
	case 0x9:
	{
        if(param[0] != param[1])
        {
                x86_mov_reg_reg(inst, param[0], param[1], 4);
                x86_alu_reg_membase(inst, X86_SUB, param[0], X86_EBP, param[2]);
        }
        else x86_alu_reg_membase(inst, X86_SUB, param[0], X86_EBP, param[2]);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[2] && param[1] != param[2])
        {
            if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
            x86_alu_reg_reg(inst, X86_SUB, param[0], param[2]);
        }
        else if(param[1] != param[2])
        {
            find_one_gp_reg_cond1(inst, param[2]);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_alu_reg_reg(inst, X86_SUB, gpreg1, param[2]);
            x86_mov_reg_reg(inst, param[0], gpreg1, 4);
            
        }
        else
        {
            x86_clear_reg(inst, param[0]);
        }
    }
	break;
	}
}
break;

case JIT_OP_IMUL:
{
	switch(state)
	{
	case 0x19:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1] * param[2], 4);
    }
	break;
	case 0x10:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
        inst = masm_imul_reg_reg_imm(inst, gpreg1, gpreg1, param[1]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        find_one_gp_reg(inst);
        inst = masm_imul_reg_reg_imm(inst, gpreg1, param[2], param[1]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x16:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        inst = masm_imul_reg_reg_imm(inst, gpreg1, gpreg1, param[2]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0xd:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
        x86_imul_reg_membase(inst, gpreg1, X86_EBP, param[1]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0x4:
	{
        if(jite_gp_reg_is_free(func, param[2]))
        {
            x86_imul_reg_membase(inst, param[2], X86_EBP, param[1]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[2], 4);
        }
        else if(have_free_gpregs())
        {
            find_one_gp_reg(inst);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_imul_reg_membase(inst, gpreg1, X86_EBP, param[1]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            x86_mov_membase_reg(inst, X86_ESP, -4, param[2], 4);
            x86_imul_reg_membase(inst, param[2], X86_EBP, param[1]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[2], 4);
            x86_mov_reg_membase(inst, param[2], X86_ESP, -4, 4);
        }
    }
	break;
	case 0x13:
	{
        find_one_gp_reg(inst);
        inst = masm_imul_reg_reg_imm(inst, gpreg1, param[1], param[2]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0xa:
	{
        if(jite_gp_reg_is_free(func, param[1]))
        {
            x86_imul_reg_membase(inst, param[1], X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
        else if(have_free_gpregs())
        {
            find_one_gp_reg(inst);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_imul_reg_membase(inst, gpreg1, X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            x86_mov_membase_reg(inst, X86_ESP, -4, param[1], 4);
            x86_imul_reg_membase(inst, param[1], X86_EBP, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
            x86_mov_reg_membase(inst, param[1], X86_ESP, -4, 4);
        }
    }
	break;
	case 0x1:
	{
        if(jite_gp_reg_is_free(func, param[1]))
        {
            x86_imul_reg_reg(inst, param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
        else if(jite_gp_reg_is_free(func, param[2]))
        {
            x86_imul_reg_reg(inst, param[2], param[1]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[2], 4);
        }
        else if(have_free_gpregs())
        {
            find_one_gp_reg(inst);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_imul_reg_reg(inst, gpreg1, param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            x86_mov_membase_reg(inst, X86_ESP, -4, param[1], 4);
            x86_imul_reg_reg(inst, param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
            x86_mov_reg_membase(inst, param[1], X86_ESP, -4, 4);
        }
    }
	break;
	case 0x18:
	{
        x86_mov_reg_imm(inst, param[0], param[1] * param[2]);
    }
	break;
	case 0xf:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[2], 4);
        inst = masm_imul_reg_reg_imm(inst, param[0], param[0], param[1]);
    }
	break;
	case 0x6:
	{
        inst = masm_imul_reg_reg_imm(inst, param[0], param[2], param[1]);
    }
	break;
	case 0x15:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        inst = masm_imul_reg_reg_imm(inst, param[0], param[0], param[2]);
    }
	break;
	case 0xc:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        x86_imul_reg_membase(inst, param[0], X86_EBP, param[2]);
    }
	break;
	case 0x3:
	{
        if(param[0] != param[2]) x86_mov_reg_reg(inst, param[0], param[2], 4);
        x86_imul_reg_membase(inst, param[0], X86_EBP, param[1]);    
    }
	break;
	case 0x12:
	{
        inst = masm_imul_reg_reg_imm(inst, param[0], param[1], param[2]);
    }
	break;
	case 0x9:
	{
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
        x86_imul_reg_membase(inst, param[0], X86_EBP, param[2]);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1] && param[0] != param[2])
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_imul_reg_reg(inst, param[0], param[1]);
        }
        else if(param[0] == param[2])
        {
            x86_imul_reg_reg(inst, param[0], param[1]);
        }
        else
        {
            x86_imul_reg_reg(inst, param[0], param[2]);
        }
    }
	break;
	}
}
break;

case JIT_OP_IDIV:
{
	switch(state)
	{
	case 0x19:
	{
        if(param[2] != 0 && (param[2] != -1 || param[1] != jit_min_int))
        {
            x86_mov_membase_imm(inst, X86_EBP, param[0], param[1] / param[2], 4);
        }
        else if(param[2] == 0)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        }
        else
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        }
    }
	break;
	case 0x10:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        if(param[1] == jit_min_int)
        {
            x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif

        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x7:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        if(param[1] == jit_min_int)
        {
            x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif
        
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x16:
	{    
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        inst = masm_idiv_reg_reg_imm(inst, gpreg1, gpreg1, param[2]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0xd:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x4:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif
        
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x13:
	{
        if(!jite_gp_reg_is_free(func, param[1]))
        {
            find_one_gp_reg(inst);
            inst = masm_idiv_reg_reg_imm(inst, gpreg1, param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            inst = masm_idiv_reg_reg_imm(inst, param[1], param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
    }
	break;
	case 0xa:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x1:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            if(gpreg1 != param[1])
            {
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            }
            else if(param[2] != X86_EAX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            }
            else
            {
                x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
            }
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x18:
	{
        if(param[2] != 0 && (param[2] != -1 || param[1] != jit_min_int))
        {
            x86_mov_reg_imm(inst, param[0], param[1] / param[2]);
        }
        else if(param[2] == 0)
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        }
        else
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        }
    }
	break;
	case 0xf:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);

        if(param[1] == jit_min_int)
        {
            x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif
        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x6:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        if(param[1] == jit_min_int)
        {
            x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, param[0], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }
        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x15:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        inst = masm_idiv_reg_reg_imm(inst, param[0], param[0], param[2]);
    }
	break;
	case 0xc:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1] != param[2])
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_membase(inst, X86_EBP, param[2], 1);
            if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
        }
        else x86_mov_reg_imm(inst, param[0], 1);
    }
	break;
	case 0x3:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[0], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }

        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x12:
	{
        inst = masm_idiv_reg_reg_imm(inst, param[0], param[1], param[2]);
    }
	break;
	case 0x9:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x0:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1] != param[2])
        {
            if(param[2] != X86_EAX && param[2] != X86_EDX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_cdq(inst);
                x86_div_reg(inst, param[2], 1);
            }
            else if(param[0] != X86_EAX && param[0] != X86_EDX)
            {
                if(param[0] != param[1])
                {
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, param[0], 4);
                }
                x86_cdq(inst);
                x86_div_reg(inst, param[0], 1);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != param[1])
                {
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
                }
                x86_cdq(inst);
                x86_div_reg(inst, gpreg1, 1);

                
            }
            if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
        }
        else x86_mov_reg_imm(inst, param[0], 1);
    }
	break;
	}
}
break;

case JIT_OP_IDIV_UN:
{
	switch(state)
	{
	case 0x19:
	{
        if(param[2] != 0) x86_mov_membase_imm(inst, X86_EBP, param[0], param[1] / param[2], 4);
        else inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }
	break;
	case 0x10:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x7:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x16:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        inst = masm_idiv_un_reg_reg_imm(inst, gpreg1, gpreg1, param[2]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0xd:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x4:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif
        
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, param[2], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, gpreg1, 0);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x13:
	{
        if(!jite_gp_reg_is_free(func, param[1]))
        {
            find_one_gp_reg(inst);
            inst = masm_idiv_un_reg_reg_imm(inst, gpreg1, param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            inst = masm_idiv_un_reg_reg_imm(inst, param[1], param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
    }
	break;
	case 0xa:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x1:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            if(gpreg1 != param[1])
            {
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            }
            else if(param[2] != X86_EAX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            }
            else
            {
                x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
            }
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EAX, 4);
    }
	break;
	case 0x18:
	{
        if(param[2] != 0) x86_mov_reg_imm(inst, param[0], param[1] / param[2]);
        else inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }
	break;
	case 0xf:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x6:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[0], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }
        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x15:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        inst = masm_idiv_un_reg_reg_imm(inst, param[0], param[0], param[2]);
    }
	break;
	case 0xc:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != param[2])
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_membase(inst, X86_EBP, param[2], 0);
            if(param[0]!=X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
        }
        else x86_mov_reg_imm(inst, param[0], 1);
    }
	break;
	case 0x3:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[0], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }

        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x12:
	{
        inst = masm_idiv_un_reg_reg_imm(inst, param[0], param[1], param[2]);
    }
	break;
	case 0x9:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
    }
	break;
	case 0x0:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != param[2])
        {
            if(param[2] != X86_EAX && param[2] != X86_EDX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, param[2], 0);
            }
            else if(param[0] != X86_EAX && param[0] != X86_EDX)
            {
                if(param[0] != param[1])
                {
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, param[0], 4);
                }
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, param[0], 0);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != param[1])
                {
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
                }
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, gpreg1, 0);

                
            }

            if(param[0] != X86_EAX) x86_mov_reg_reg(inst, param[0], X86_EAX, 4);
        }
        else x86_mov_reg_imm(inst, param[0], 1);
    }
	break;
	}
}
break;

case JIT_OP_IREM:
{
	switch(state)
	{
	case 0x19:
	{
        if(param[2] != 0 && (param[2] != -1 || param[1] != jit_min_int))
        {
            x86_mov_membase_imm(inst, X86_EBP, param[0], param[1] % param[2], 4);    
        }
        else if(param[2] == 0) inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        else
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        }
    }
	break;
	case 0x10:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        if(param[1] == jit_min_int)
        {
            x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif

        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x7:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        if(param[1] == jit_min_int)
        {
            x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x16:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        inst = masm_irem_reg_reg_imm(inst, gpreg1, gpreg1, param[2]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0xd:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x4:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x13:
	{
        if(!jite_gp_reg_is_free(func, param[1]))
        {
            find_one_gp_reg(inst);
            inst = masm_irem_reg_reg_imm(inst, gpreg1, param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            inst = masm_irem_reg_reg_imm(inst, param[1], param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
    }
	break;
	case 0xa:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1]!=X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x1:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            if(gpreg1 != param[1])
            {
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            }
            else if(param[2] != X86_EAX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            }
            else
            {
                x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
            }
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x18:
	{
        if(param[2] != 0 && (param[2] != -1 || param[1] != jit_min_int))
        {
            x86_mov_reg_imm(inst, param[0], param[1] % param[2]);    
        }
        else if(param[2] == 0) inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        else
        {
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        }
    }
	break;
	case 0xf:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);

        if(param[1] == jit_min_int)
        {
            x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif
        
        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x6:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        if(param[1] == jit_min_int)
        {
            x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
            patch = inst;
            x86_branch8(inst, X86_CC_NE, 0, 0);
            inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
            x86_patch(patch, inst);
        }
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, param[0], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }
        if(param[0]!=X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x15:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        inst = masm_irem_reg_reg_imm(inst, param[0], param[0], param[2]);
    }
	break;
	case 0xc:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1] != param[2])
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_membase(inst, X86_EBP, param[2], 1);
            if(param[0]!=X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
        }
        else x86_mov_reg_imm(inst, param[0], 0);
    }
	break;
	case 0x3:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[2], 1);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, param[0], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_cdq(inst);
            x86_div_reg(inst, gpreg1, 1);
            
        }

        if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x12:
	{
        inst = masm_irem_reg_reg_imm(inst, param[0], param[1], param[2]);
    }
	break;
	case 0x9:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_cdq(inst);
        x86_div_membase(inst, X86_EBP, param[2], 1);
        if(param[0]!=X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x0:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch, *patch2;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        x86_alu_reg_imm(inst, X86_CMP, param[2], -1);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        x86_alu_reg_imm(inst, X86_CMP, param[1], jit_min_int);
        patch2 = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_patch(patch2, inst);
        #endif

        if(param[1] != param[2])
        {
            if(param[2] != X86_EAX && param[2] != X86_EDX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_cdq(inst);
                x86_div_reg(inst, param[2], 1);
            }
            else if(param[0] != X86_EAX && param[0] != X86_EDX)
            {
                if(param[0] != param[1])
                {
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, param[0], 4);
                }
                x86_cdq(inst);
                x86_div_reg(inst, param[0], 1);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != param[1])
                {
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
                }
                x86_cdq(inst);
                x86_div_reg(inst, gpreg1, 1);

                
            }

            if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);            
        }
        else x86_mov_reg_imm(inst, param[0], 1);
    }
	break;
	}
}
break;

case JIT_OP_IREM_UN:
{
	switch(state)
	{
	case 0x19:
	{
        if(param[2] != 0) x86_mov_membase_imm(inst, X86_EBP, param[0], param[1] % param[2], 4);
        else inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }
	break;
	case 0x10:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x7:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif
        
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }
        
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x16:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        inst = masm_irem_un_reg_reg_imm(inst, gpreg1, gpreg1, param[2]);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        
    }
	break;
	case 0xd:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x4:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }
        
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x13:
	{
        if(!jite_gp_reg_is_free(func, param[1]))
        {
            find_one_gp_reg(inst);
            inst = masm_irem_un_reg_reg_imm(inst, gpreg1, param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            inst = masm_irem_un_reg_reg_imm(inst, param[1], param[1], param[2]);
            x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        }
    }
	break;
	case 0xa:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x1:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif
        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            if(gpreg1 != param[1])
            {
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
            }
            else if(param[2] != X86_EAX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            }
            else
            {
                x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
            }
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }

        x86_mov_membase_reg(inst, X86_EBP, param[0], X86_EDX, 4);
    }
	break;
	case 0x18:
	{
        if(param[2] != 0) x86_mov_reg_imm(inst, param[0], param[1] % param[2]);
        else inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }
	break;
	case 0xf:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        x86_mov_reg_imm(inst, X86_EAX, param[1]);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x6:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[0], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, X86_EAX, param[1]);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }

        if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x15:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        inst = masm_irem_un_reg_reg_imm(inst, param[0], param[0], param[2]);
    }
	break;
	case 0xc:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != param[2])
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_membase(inst, X86_EBP, param[2], 0);
            if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
        }
        else x86_mov_reg_imm(inst, param[0], 0);
    }
	break;
	case 0x3:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[2] != X86_EAX && param[2] != X86_EDX)
        {
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[2], 0);
        }
        else if(param[0] != X86_EAX && param[0] != X86_EDX)
        {
            x86_mov_reg_reg(inst, param[0], param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, param[0], 0);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_membase(inst, X86_EAX, X86_EBP, param[1], 4);
            x86_clear_reg(inst, X86_EDX);
            x86_div_reg(inst, gpreg1, 0);
            
        }

        if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x12:
	{
        inst = masm_irem_un_reg_reg_imm(inst, param[0], param[1], param[2]);
    }
	break;
	case 0x9:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_membase_imm(inst, X86_CMP, X86_EBP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
        x86_clear_reg(inst, X86_EDX);
        x86_div_membase(inst, X86_EBP, param[2], 0);
        if(param[0]!=X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
    }
	break;
	case 0x0:
	{
        #ifndef JIT_USE_SIGNALS
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, param[2], 0);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
        x86_patch(patch, inst);
        #endif

        if(param[1] != param[2])
        {
            if(param[2] != X86_EAX && param[2] != X86_EDX)
            {
                if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, param[2], 0);
            }
            else if(param[0] != X86_EAX && param[0] != X86_EDX)
            {
                if(param[0] != param[1])
                {
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, param[0], param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, param[0], 4);
                }
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, param[0], 0);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != param[1])
                {
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                }
                else if(param[2] != X86_EAX)
                {
                    if(param[1] != X86_EAX) x86_mov_reg_reg(inst, X86_EAX, param[1], 4);
                    x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                }
                else
                {
                    x86_xchg_reg_reg(inst, X86_EAX, gpreg1, 4);
                }
                x86_clear_reg(inst, X86_EDX);
                x86_div_reg(inst, gpreg1, 0);

                
            }

            if(param[0] != X86_EDX) x86_mov_reg_reg(inst, param[0], X86_EDX, 4);
        }
        else x86_mov_reg_imm(inst, param[0], 0);
    }
	break;
	}
}
break;

case JIT_OP_INEG:
{
	switch(state)
	{
	case 0x7:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], -param[1], 4);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_neg_reg(inst, gpreg1);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        else
        {
            x86_neg_membase(inst, X86_EBP, param[0]);
        }
    }
	break;
	case 0x1:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        x86_neg_membase(inst, X86_EBP, param[0]);
    }
	break;
	case 0x6:
	{
        x86_mov_reg_imm(inst, param[0], -param[1]);
    }
	break;
	case 0x3:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        x86_neg_reg(inst, param[0]);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            x86_mov_reg_reg(inst, param[0], param[1], 4);
            x86_neg_reg(inst, param[0]);
        }
        else
        {
            x86_neg_reg(inst, param[0]);
        }
    }
	break;
	}
}
break;

case JIT_OP_FADD:
case JIT_OP_DADD:
case JIT_OP_FMUL:
case JIT_OP_DMUL:
case JIT_OP_FMAX:
case JIT_OP_DMAX:
case JIT_OP_FMIN:
case JIT_OP_DMIN:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_ESP, -32);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x10:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[2]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x7:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[1], sourceType);
        if(!jite_xmm_reg_is_free(func, param[2]))
        {
            find_one_xmm_reg(inst);
            if(param[2] != xmmreg1) inst = masm_mov_reg_reg(inst, xmmreg1, param[2], sourceType);
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_ESP, -32);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else
        {
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[2], X86_ESP, -32);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
        }
    }
	break;
	case 0x16:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_ESP, -32);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0xd:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[2]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x4:
	{
        if(!jite_xmm_reg_is_free(func, param[2]))
        {
            find_one_xmm_reg(inst);
            if(param[2] != xmmreg1) inst = masm_mov_reg_reg(inst, xmmreg1, param[2], sourceType);
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[1]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else
        {
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[2], X86_EBP, param[1]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
        }
    }
	break;
	case 0x13:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        if(!jite_xmm_reg_is_free(func, param[1]))
        {
            find_one_xmm_reg(inst);
            if(param[1] != xmmreg1) inst = masm_mov_reg_reg(inst, xmmreg1, param[1], sourceType);
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_ESP, -32);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else
        {
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[1], X86_ESP, -32);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
    }
	break;
	case 0xa:
	{
        if(!jite_xmm_reg_is_free(func, param[1]))
        {
            find_one_xmm_reg(inst);
            if(param[1] != xmmreg1) inst = masm_mov_reg_reg(inst, xmmreg1, param[1], sourceType);
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else
        {
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[1], X86_EBP, param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
    }
	break;
	case 0x1:
	{
        if(!jite_xmm_reg_is_free(func, param[1]) && !jite_xmm_reg_is_free(func, param[2]))
        {
            find_one_xmm_reg_cond1(inst, param[2]);
            inst = masm_mov_reg_reg(inst, xmmreg1, param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), xmmreg1, param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else if(jite_xmm_reg_is_free(func, param[1]))
        {
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[1], param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
        else
        {
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[2], param[1]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[2], sourceType);
        }
    }
	break;
	case 0x18:
	{
        inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_ESP, -16);
    }
	break;
	case 0xf:
	{
        inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_EBP, param[2]);
    }
	break;
	case 0x6:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[1], sourceType);
        if(param[0] != param[2]) inst = masm_mov_reg_reg(inst, param[0], param[2], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_ESP, -16);
    }
	break;
	case 0x15:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_ESP, -16);
    }
	break;
	case 0xc:
	{
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_EBP, param[2]);
    }
	break;
	case 0x3:
	{
        if(param[0] != param[2]) inst = masm_mov_reg_reg(inst, param[0], param[2], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_EBP, param[1]);
    }
	break;
	case 0x12:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        if(param[0] != param[1]) inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_ESP, -16);
    }
	break;
	case 0x9:
	{
        if(param[0] != param[1]) inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_EBP, param[2]);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[2] && param[0] != param[1])
        {
            inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[2]);
        }
        else if (param[0] != param[2])
        {
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[2]);
        }
        else
        {
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[1]);
        }
    }
	break;
	}
}
break;

case JIT_OP_FSUB:
case JIT_OP_DSUB:
case JIT_OP_FDIV:
case JIT_OP_DDIV:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_ESP, -32);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x10:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[2]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x7:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
        emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), xmmreg1, param[2]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x16:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_ESP, -32);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0xd:
	{
        find_one_xmm_reg(inst);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[2]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
        
    }
	break;
	case 0x4:
	{
        find_one_xmm_reg_cond1(inst, param[2]);
        inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), xmmreg1, param[2]);
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);        
    }
	break;
	case 0x13:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, param[2], sourceType);
        if(!jite_xmm_reg_is_free(func, param[1]))
        {
            find_one_xmm_reg(inst);
            inst = masm_mov_reg_reg(inst, xmmreg1, param[1], sourceType);
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_ESP, -32);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else
        {
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[1], X86_ESP, -32);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
    }
	break;
	case 0xa:
	{
        if(!jite_xmm_reg_is_free(func, param[1]))
        {
            find_one_xmm_reg(inst);
            inst = masm_mov_reg_reg(inst, xmmreg1, param[1], sourceType);
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), xmmreg1, X86_EBP, param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else
        {
            emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[1], X86_EBP, param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
    }
	break;
	case 0x1:
	{
        if(!jite_xmm_reg_is_free(func, param[1]))
        {
            find_one_xmm_reg_cond1(inst, param[2]);
            inst = masm_mov_reg_reg(inst, xmmreg1, param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), xmmreg1, param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], xmmreg1, sourceType);
            
        }
        else
        {
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[1], param[2]);
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        }
    }
	break;
	case 0x18:
	{
        inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_ESP, -16);
    }
	break;
	case 0xf:
	{
        inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_EBP, param[2]);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2])
        {
            inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[2]);
        }
        else
        {
            find_one_xmm_reg_cond1(inst, param[2]);
            inst = masm_mov_reg_imm(inst, xmmreg1, param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), xmmreg1, param[2]);
            inst = masm_mov_reg_reg(inst, param[0], xmmreg1, sourceType);
        }
    }
	break;
	case 0x15:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_ESP, -16);
    }
	break;
	case 0xc:
	{
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_EBP, param[2]);
    }
	break;
	case 0x3:
	{
        if(param[0] != param[2])
        {
            inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[2]);
        }
        else
        {
            find_one_xmm_reg_cond1(inst, param[2]);
            inst = masm_mov_reg_membase(inst, xmmreg1, X86_EBP, param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), xmmreg1, param[2]);
            inst = masm_mov_reg_reg(inst, param[0], xmmreg1, sourceType);
        }
    }
	break;
	case 0x12:
	{
        inst = masm_mov_membase_imm(inst, X86_ESP, -16, param[2], sourceType);
        if(param[0] != param[1]) inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_ESP, -16);
    }
	break;
	case 0x9:
	{
        if(param[0] != param[1]) inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
        emit_sse_instruction_reg_membase((inst), (machine_code1), (machine_code2), param[0], X86_EBP, param[2]);
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1] && param[0] != param[2])
        {
            inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[2]);
        }
        else if (param[0] != param[2])
        {
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[2]);
        }
        else if (param[0] != param[1])
        {
            if(jite_xmm_reg_is_free(func, param[1]))
            {
                emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[1], param[2]);
                inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
            }
            else if(have_free_xmmregs())
            {
                find_one_xmm_reg_cond1(inst, param[2]);
                inst = masm_mov_reg_reg(inst, xmmreg1, param[1], sourceType);
                emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), xmmreg1, param[2]);
                inst = masm_mov_reg_reg(inst, param[0], xmmreg1, sourceType);
                
            }
            else
            {
                inst = masm_mov_membase_reg(inst, X86_ESP, -16, param[1], sourceType);
                emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[1], param[2]);
                inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
                inst = masm_mov_reg_membase(inst, param[1], X86_ESP, -16, sourceType);
            }
        }
        else
        {
            emit_sse_instruction_reg_reg((inst), (machine_code1), (machine_code2), param[0], param[0]);
        }
    }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_IADD:
case JIT_OP_IOR:
case JIT_OP_IAND:
case JIT_OP_IXOR:
case JIT_OP_ADD_RELATIVE:
case JIT_OP_LADD:
case JIT_OP_LOR:
case JIT_OP_LAND:
case JIT_OP_LXOR:
case JIT_OP_ISUB:
case JIT_OP_IMUL:
case JIT_OP_IDIV:
case JIT_OP_IDIV_UN:
case JIT_OP_IREM:
case JIT_OP_IREM_UN:
case JIT_OP_INEG:
case JIT_OP_FADD:
case JIT_OP_DADD:
case JIT_OP_FMUL:
case JIT_OP_DMUL:
case JIT_OP_FMAX:
case JIT_OP_DMAX:
case JIT_OP_FMIN:
case JIT_OP_DMIN:
case JIT_OP_FSUB:
case JIT_OP_DSUB:
case JIT_OP_FDIV:
case JIT_OP_DDIV:
	return 1;

#endif
