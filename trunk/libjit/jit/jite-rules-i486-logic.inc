/* Automatically generated from ./jite-rules-i486-logic.lir - DO NOT EDIT */
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

case JIT_OP_INOT:
case JIT_OP_LNOT:
{
	switch(state)
	{
	case 0x7:
	{
        inst = masm_mov_membase_imm(inst, X86_EBP, param[0], param[1], sourceType);
        x86_not_membase(inst, X86_EBP, param[0]);
        if(machine_code2) x86_not_membase(inst, X86_EBP, param[0] + 4);
    }
	break;
	case 0x4:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_not_reg(inst, gpreg1);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            if(machine_code2)
            {
                x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
                x86_not_reg(inst, gpreg1);
                x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
            }
        }
        else
        {
            x86_not_membase(inst, X86_EBP, param[0]);
            if(machine_code2) x86_not_membase(inst, X86_EBP, param[0] + 4);
        }
    }
	break;
	case 0x1:
	{
        if(!jite_gp_reg_is_free(func, param[1])) inst = masm_mov_membase_reg(inst, X86_ESP, -8, param[1], sourceType);
        x86_not_reg(inst, param[1]);
        if(machine_code2) x86_not_reg(inst, jite_register_pair(param[0]));
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
        if(!jite_gp_reg_is_free(func, param[1])) inst = masm_mov_reg_membase(inst, param[1], X86_ESP, -8, sourceType);
    }
	break;
	case 0x6:
	{
        inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
        x86_not_reg(inst, param[0]);
        if(machine_code2) x86_not_reg(inst, jite_register_pair(param[0]));
    }
	break;
	case 0x3:
	{
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
        x86_not_reg(inst, param[0]);
        if(machine_code2) x86_not_reg(inst, jite_register_pair(param[0]));
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1]) inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
        x86_not_reg(inst, param[0]);
        if(machine_code2) x86_not_reg(inst, jite_register_pair(param[0]));
    }
	break;
	}
}
break;

case JIT_OP_ISHL:
case JIT_OP_ISHR:
case JIT_OP_ISHR_UN:
{
	switch(state)
	{
	case 0x19:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
        x86_mov_reg_imm(inst, X86_ECX, param[2]);
        x86_shift_membase(inst, machine_code1, X86_EBP, param[0]);
    }
	break;
	case 0x10:
	{
        x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
        x86_shift_membase(inst, machine_code1, X86_EBP, param[0]);
    }
	break;
	case 0x7:
	{
        x86_mov_membase_imm(inst, X86_EBP, param[0], param[1], 4);
        if(param[2] != X86_ECX) x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
        x86_shift_membase(inst, machine_code1, X86_EBP, param[0]);
    }
	break;
	case 0x16:
	{
        if(param[0] != param[1])
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            x86_shift_membase_imm(inst, machine_code1, X86_EBP, param[0], param[2]);
            
        }
        else x86_shift_membase_imm(inst, machine_code1, X86_EBP, param[0], (param[2] & 0x1F));
    }
	break;
	case 0xd:
	{
        if(param[0] != param[1])
        {
            x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], X86_ECX, 4);
        }
        x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
        x86_shift_membase(inst, machine_code1, X86_EBP, param[0]);
    }
	break;
	case 0x4:
	{
        if(param[2] != X86_ECX) x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
        if(param[0] != param[1])
        {
            find_one_gp_reg_cond1(inst, X86_ECX);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
            
        }
        x86_shift_membase(inst, machine_code1, X86_EBP, param[0]);
    }
	break;
	case 0x13:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        x86_shift_membase_imm(inst, machine_code1, X86_EBP, param[0], (param[2] & 0x1F));
    }
	break;
	case 0xa:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
        x86_shift_membase(inst, machine_code1, X86_EBP, param[0]);
    }
	break;
	case 0x1:
	{
        x86_mov_membase_reg(inst, X86_EBP, param[0], param[1], 4);
        if(X86_ECX != param[2]) x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
        x86_shift_membase(inst, machine_code1, X86_EBP, param[0]);
    }
	break;
	case 0x18:
	{
        if(param[0] != X86_ECX)
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_mov_reg_imm(inst, X86_ECX, param[2]);
            x86_shift_reg(inst, machine_code1, param[0]);
        }
        else
        {
            find_one_gp_reg_cond1(inst, X86_ECX);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_mov_reg_imm(inst, X86_ECX, param[2]);
            x86_shift_reg(inst, machine_code1, gpreg1);
            x86_mov_reg_reg(inst, X86_ECX, gpreg1, 4);
            
        }
    }
	break;
	case 0xf:
	{
        if(param[0] != X86_ECX)
        {
            x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_shift_reg(inst, machine_code1, param[0]);
        }
        else
        {
            find_one_gp_reg_cond1(inst, X86_ECX);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
            x86_shift_reg(inst, machine_code1, gpreg1);
            x86_mov_reg_reg(inst, X86_ECX, gpreg1, 4);
            
        }
    }
	break;
	case 0x6:
	{
        if(param[0] != X86_ECX && param[2] != X86_ECX)
        {
            x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_shift_reg(inst, machine_code1, param[0]);
        }
        else if(param[0] == X86_ECX && param[2] == X86_ECX)
        {
            find_one_gp_reg(inst);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_shift_reg(inst, machine_code1, gpreg1);
            x86_mov_reg_reg(inst, X86_ECX, gpreg1, 4);
            
        }
        else if(param[2] == X86_ECX)
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_shift_reg(inst, machine_code1, param[0]);    
        }
        else if(param[0] == X86_ECX)
        {
            x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_shift_reg(inst, machine_code1, gpreg1);
            x86_mov_reg_reg(inst, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x15:
	{
        x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
        x86_shift_reg_imm(inst, machine_code1, param[0], (param[2] & 0x1F));
    }
	break;
	case 0xc:
	{
        if(param[0] != X86_ECX)
        {
            x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
            x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
            x86_shift_reg(inst, machine_code1, param[0]);
        }
        else
        {
            find_one_gp_reg(inst);
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
            x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
            x86_shift_reg(inst, machine_code1, gpreg1);
            x86_mov_reg_reg(inst, X86_ECX, gpreg1, 4);
            
        }
    }
	break;
	case 0x3:
	{
        if(param[2] != X86_ECX)
        {
            if(param[0] != X86_ECX)
            {
                x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
                x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
                x86_shift_reg(inst, machine_code1, param[0]);
            }
            else
            {
                find_one_gp_reg_cond1(inst, X86_ECX);
                x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
                x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
                x86_shift_reg(inst, machine_code1, gpreg1);
                x86_mov_reg_reg(inst, X86_ECX, gpreg1, 4);
                
            }
        }
        else
        {
            if(param[0] != X86_ECX)
            {
                x86_mov_reg_membase(inst, param[0], X86_EBP, param[1], 4);
                x86_shift_reg(inst, machine_code1, param[0]);
            }
            else
            {
                find_one_gp_reg_cond1(inst, X86_ECX);
                x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
                x86_shift_reg(inst, machine_code1, gpreg1);
                x86_mov_reg_reg(inst, X86_ECX, gpreg1, 4);
                
            }
        }
    }
	break;
	case 0x12:
	{
        if(param[0] != param[1]) x86_mov_reg_reg(inst, param[0], param[1], 4);
        x86_shift_reg_imm(inst, machine_code1, param[0], (param[2] & 0x1F));
    }
	break;
	case 0x9:
	{
        if(param[0] != X86_ECX)
        {
            x86_mov_reg_reg(inst, param[0], param[1], 4);
            x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
            x86_shift_reg(inst, machine_code1, param[0]);
        }
        else
        {
            find_one_gp_reg_cond1(inst, X86_ECX);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_mov_reg_membase(inst, X86_ECX, X86_EBP, param[2], 4);
            x86_shift_reg(inst, machine_code1, gpreg1);
            x86_mov_reg_reg(inst, param[0], gpreg1, 4);
            
        }
    }
	break;
	case 0x0:
	{
        if(param[1] != X86_ECX)
        {
            x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
            if(jite_gp_reg_is_free(func, param[1]))
            {
                x86_shift_reg(inst, machine_code1, param[1]);
                x86_mov_reg_reg(inst, param[0], param[1], 4);
            }
            else if(param[0] != X86_ECX)
            {
                x86_mov_reg_reg(inst, param[0], param[1], 4);
                x86_shift_reg(inst, machine_code1, param[0]);
            }
            else
            {
                find_one_gp_reg_cond1(inst, X86_ECX);
                x86_mov_reg_reg(inst, gpreg1, param[1], 4);
                x86_shift_reg(inst, machine_code1, gpreg1);
                x86_mov_reg_reg(inst, param[0], gpreg1, 4);
                
            }
        }
        else if(param[0] != X86_ECX && param[0] != param[2])
        {    
            x86_mov_reg_reg(inst, param[0], param[1], 4);
            x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
            x86_shift_reg(inst, machine_code1, param[0]);
        }
        else 
        {    
            find_one_gp_reg_cond2(inst, X86_ECX, param[2]);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_mov_reg_reg(inst, X86_ECX, param[2], 4);
            x86_shift_reg(inst, machine_code1, gpreg1);
            x86_mov_reg_reg(inst, param[0], gpreg1, 4);
            
        }
    }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_INOT:
case JIT_OP_LNOT:
case JIT_OP_ISHL:
case JIT_OP_ISHR:
case JIT_OP_ISHR_UN:
	return 1;

#endif
