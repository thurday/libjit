/* Automatically generated from ./jite-rules-i486-obj.lir - DO NOT EDIT */
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

case JIT_OP_LOAD_RELATIVE_SBYTE:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2], 1, 0);
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2], 1, 0);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_UBYTE:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2], 0, 0);
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2], 0, 0);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_SHORT:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2], 1, 1);
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2], 1, 1);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_USHORT:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2], 0, 1);
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2], 0, 1);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_INT:
{
	switch(state)
	{
	case 0x18:
	{
        x86_mov_reg_mem(inst, param[0], param[1] + param[2], 4);
    }
	break;
	case 0x12:
	{
        x86_mov_reg_membase(inst, param[0], param[1], param[2], 4);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_LONG:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4); 
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        
    }
	break;
	case 0x18:
	{
        x86_mov_reg_mem(inst, param[0], param[1] + param[2], 4);
        x86_mov_reg_mem(inst, jite_register_pair(param[0]), param[1] + param[2] + 4, 4);
    }
	break;
	case 0x13:
	{ 
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2] + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        
    }
	break;
	case 0x12:
	{
        inst = masm_mov_reg_membase(inst, param[0], param[1], param[2], jit_value_get_type(dest));
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_FLOAT32:
{
	switch(state)
	{
	case 0x18:
	{
        sse_movss_xmreg_mem(inst, param[0], param[1] + param[2]);
    }
	break;
	case 0x12:
	{
        sse_movss_xmreg_membase(inst, param[0], param[1], param[2]);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_FLOAT64:
{
	switch(state)
	{
	case 0x18:
	{
        sse2_movsd_xmreg_mem(inst, param[0], param[1] + param[2]);
    }
	break;
	case 0x12:
	{
        sse2_movsd_xmreg_membase(inst, param[0], param[1], param[2]);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_NFLOAT:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4); 
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] + 8, 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0] + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x13:
	{ 
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2] + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg1, param[1], param[2] + 8, 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0] + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x18:
	{
        sse2_movsd_xmreg_mem(inst, param[0], param[1] + param[2]);
    }
	break;
	case 0x12:
	{
        sse2_movsd_xmreg_membase(inst, param[0], param[1], param[2]);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_RELATIVE_STRUCT:
{
	switch(state)
	{
	case 0x19:
	{
        int size = jit_type_get_size(jit_value_get_type(dest));
        find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
        if(save_gpreg1) x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 16);
        inst = jite_memory_copy_from_mem(inst, X86_EBP, param[0], (void *)(param[1] + param[2]), size, gpreg1);
        if(save_gpreg1) x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 16);
        
    }
	break;
	case 0x13:
	{
        inst = masm_mov_membase_membase(inst, X86_EBP, param[0], param[1], param[2], jit_type_get_size(jit_value_get_type(dest)));
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_BYTE:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[2], param[1], 1);
    }
	break;
	case 0x14:
	{
        if(param[1] != X86_ESI && param[1] != X86_EDI)
        {
            x86_mov_mem_reg(inst, param[0] + param[2], param[1], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_mov_mem_reg(inst, param[0] + param[2], gpreg1, 1);
            
        }
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[2], param[1], 1);
    }
	break;
	case 0x12:
	{
        if(param[1] != X86_ESI && param[1] != X86_EDI)
        {
            x86_mov_membase_reg(inst, param[0], param[2], param[1], 1);
        }
        else
        {
            find_one_gp_reg_cond3(inst, param[0], X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_mov_membase_reg(inst, param[0], param[2], gpreg1, 1);
            
        }
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_SHORT:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[2], param[1], 2);
    }
	break;
	case 0x14:
	{
        x86_mov_mem_reg(inst, param[0] + param[2], param[1], 2);
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[2], param[1], 2);
    }
	break;
	case 0x12:
	{
        x86_mov_membase_reg(inst, param[0], param[2], param[1], 2);
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_INT:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[2], param[1], 4);
    }
	break;
	case 0x14:
	{
        x86_mov_mem_reg(inst, param[0] + param[2], param[1], 4);
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[2], param[1], 4);
    }
	break;
	case 0x12:
	{
        x86_mov_membase_reg(inst, param[0], param[2], param[1], 4);
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_LONG:
{
	switch(state)
	{
	case 0x1a:
	{
        jit_uint *long_value = (jit_uint*)(param[1]);
        jit_uint low_dword = long_value[0];
        jit_uint high_dword = long_value[1];
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_mem_imm(inst, param[0] + param[2], low_dword, 4);
        x86_mov_mem_imm(inst, param[0] + param[2] + 4, high_dword, 4);
    }
	break;
	case 0x17:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_mem_reg(inst, param[0] + param[2], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
        x86_mov_mem_reg(inst, param[0] + param[2] + 4, gpreg1, 4);
        
    }
	break;
	case 0x14:
	{
        x86_mov_mem_reg(inst, param[0] + param[2], param[1], 4);
        x86_mov_mem_reg(inst, param[0] + param[2] + 4, jite_register_pair(param[1]), 4);
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[2], *((jit_uint *)(param[1])), 4);
        x86_mov_membase_imm(inst, param[0], param[2] + 4, *((jit_uint *)(param[1] + 4)), 4);
    }
	break;
	case 0x15:
	{
        find_one_gp_reg_cond1(inst, param[0]);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_membase_reg(inst, param[0], param[2], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
        x86_mov_membase_reg(inst, param[0], param[2] + 4, gpreg1, 4);
        
    }
	break;
	case 0x12:
	{
        inst = masm_mov_membase_reg(inst, param[0], param[2], param[1], sourceType);
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_FLOAT32:
{
	switch(state)
	{
	case 0x1a:
	{
        jit_uint *float_value = (jit_uint*)(param[1]);
        jit_uint low_dword = float_value[0];
        x86_mov_mem_imm(inst, param[0] + param[2], low_dword, 4);
    }
	break;
	case 0x14:
	{
        sse_movss_mem_xmreg(inst, param[0] + param[2], param[1]);
    }
	break;
	case 0x18:
	{
        jit_uint float_value = *((jit_uint *)(param[1]));
        x86_mov_membase_imm(inst, param[0], param[2], float_value, 4);
    }
	break;
	case 0x12:
	{
        sse_movss_membase_xmreg(inst, param[0], param[2], param[1]);
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_FLOAT64:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[2], ((jit_uint*)(param[1]))[0], 4);
        x86_mov_mem_imm(inst, param[0] + param[2] + 4, ((jit_uint*)(param[1]))[1], 4);
    }
	break;
	case 0x14:
	{
        sse2_movsd_mem_xmreg(inst, param[0] + param[2], param[1]);
    }
	break;
	case 0x18:
	{
        jit_uint *value = (jit_uint*)(param[1]);
        x86_mov_membase_imm(inst, param[0], param[2], value[0], 4);
        x86_mov_membase_imm(inst, param[0], param[2] + 4, value[1], 4);
    }
	break;
	case 0x12:
	{
        sse2_movsd_membase_xmreg(inst, param[0], param[2], param[1]);
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_NFLOAT:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[2], ((jit_uint *)(param[1]))[0], 4);
        x86_mov_mem_imm(inst, param[0] + param[2] + 4, ((jit_uint *)(param[1]))[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_mem_imm(inst, param[0] + param[2] + 8, ((jit_uint *)(param[1]))[2], 4);
        }
    }
	break;
	case 0x17:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_mem_reg(inst, param[0] + param[2], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
        x86_mov_mem_reg(inst, param[0] + param[2] + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 8, 4);
            x86_mov_mem_reg(inst, param[0] + param[2] + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x14:
	{
        sse2_movsd_mem_xmreg(inst, param[0] + param[2], param[1]);
    }
	break;
	case 0x18:
	{
        jit_uint *ptr = (jit_uint*)(param[1]);
        x86_mov_membase_imm(inst, param[0], param[2], ptr[0], 4);
        x86_mov_membase_imm(inst, param[0], param[2] + 4, ptr[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_membase_imm(inst, param[0], param[2] + 8, ptr[2], 4);
        }
    }
	break;
	case 0x15:
	{
        find_one_gp_reg_cond1(inst, param[0]);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1], 4);
        x86_mov_membase_reg(inst, param[0], param[2], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 4, 4);
        x86_mov_membase_reg(inst, param[0], param[2] + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[1] + 8, 4);
            x86_mov_membase_reg(inst, param[0], param[2] + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x12:
	{
        sse2_movsd_membase_xmreg(inst, param[0], param[2], param[1]);
    }
	break;
	}
}
break;

case JIT_OP_STORE_RELATIVE_STRUCT:
{
	switch(state)
	{
	case 0x17:
	{
        int size = jit_type_get_size(jit_value_get_type(value1));
        find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
        if(save_gpreg1) x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 16);
        inst = jite_memory_copy_to_mem(inst, (void *)(param[0] + param[2]), X86_EBP, param[1], size, gpreg1);
        if(save_gpreg1) x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 16);
        
    }
	break;
	case 0x15:
	{
        inst = masm_mov_membase_membase(inst, param[0], param[2], X86_EBP, param[1], jit_type_get_size(sourceType));
    }
	break;
	}
}
break;

case JIT_OP_STORE_ELEMENT_BYTE:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[1], param[2], 1);
    }
	break;
	case 0x8:
	{
        if(param[2] != X86_ESI && param[2] != X86_EDI)
        {
            x86_mov_mem_reg(inst, param[0] + param[1], param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond2(inst, X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_mem_reg(inst, param[0] + param[1], gpreg1, 1);
            
        }
    }
	break;
	case 0x14:
	{
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_imm(inst, gpreg1, 0, param[1], 0, param[2], 1);
        
    }
	break;
	case 0x2:
	{
        if(param[2] != X86_ESI && param[2] != X86_EDI)
        {
            find_one_gp_reg_cond2(inst, param[1], param[2]);
            x86_mov_reg_imm(inst, gpreg1, param[0]);
            x86_mov_memindex_reg(inst, gpreg1, 0, param[1], 0, param[2], 1);
            
        }
        else
        {
            // was find_two_gp_regs_cond2_cond2_for_gpreg1(inst, param[1], param[2], X86_ESI, X86_EDI);
            find_two_gp_regs_cond1_cond2_for_gpreg1(inst, param[1], X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_reg_imm(inst, gpreg2, param[0]);
            x86_mov_memindex_reg(inst, gpreg2, 0, param[1], 0, gpreg1, 1);
            
        }
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[1], param[2], 1);
    }
	break;
	case 0x6:
	{
        if(param[2] != X86_ESI && param[2] != X86_EDI)
        {
            x86_mov_membase_reg(inst, param[0], param[1], param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond3(inst, param[0], X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_membase_reg(inst, param[0], param[1], gpreg1, 1);
            
        }
    }
	break;
	case 0x12:
	{
        x86_mov_memindex_imm(inst, param[0], 0, param[1], 0, param[2], 1);
    }
	break;
	case 0x0:
	{
        if(param[2] != X86_ESI && param[2] != X86_EDI)
        {
            x86_mov_memindex_reg(inst, param[0], 0, param[1], 0, param[2], 1);
        }
        else
        {
            find_one_gp_reg_cond4(inst, param[0], param[1], X86_ESI, X86_EDI);
            x86_mov_reg_reg(inst, gpreg1, param[2], 4);
            x86_mov_memindex_reg(inst, param[0], 0, param[1], 0, gpreg1, 1);
            
        }
    }
	break;
	}
}
break;

case JIT_OP_STORE_ELEMENT_SHORT:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[1] * 2, param[2], 2);
    }
	break;
	case 0x8:
	{
        x86_mov_mem_reg(inst, param[0] + param[1] * 2, param[2], 2);
    }
	break;
	case 0x14:
	{
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_imm(inst, gpreg1, 0, param[1], 1, param[2], 2);
        
    }
	break;
	case 0x2:
	{
        find_one_gp_reg_cond2(inst, param[1], param[2]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_reg(inst, gpreg1, 0, param[1], 1, param[2], 2);
        
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[1] * 2, param[2], 2);
    }
	break;
	case 0x6:
	{
        x86_mov_membase_reg(inst, param[0], param[1] * 2, param[2], 2);
    }
	break;
	case 0x12:
	{
        x86_mov_memindex_imm(inst, param[0], 0, param[1], 1, param[2], 2);
    }
	break;
	case 0x0:
	{
        x86_mov_memindex_reg(inst, param[0], 0, param[1], 1, param[2], 2);
    }
	break;
	}
}
break;

case JIT_OP_STORE_ELEMENT_INT:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[1] * 4, param[2], 4);
    }
	break;
	case 0x8:
	{
        x86_mov_mem_reg(inst, param[0] + param[1] * 4, param[2], 4);
    }
	break;
	case 0x14:
	{
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_imm(inst, gpreg1, 0, param[1], 2, param[2], 4);
        
    }
	break;
	case 0x2:
	{
        find_one_gp_reg_cond2(inst, param[1], param[2]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_reg(inst, gpreg1, 0, param[1], 2, param[2], 4);
        
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[1] * 4, param[2], 4);
    }
	break;
	case 0x6:
	{
        x86_mov_membase_reg(inst, param[0], param[1] * 4, param[2], 4);
    }
	break;
	case 0x12:
	{
        x86_mov_memindex_imm(inst, param[0], 0, param[1], 2, param[2], 4);
    }
	break;
	case 0x0:
	{
        x86_mov_memindex_reg(inst, param[0], 0, param[1], 2, param[2], 4);
    }
	break;
	}
}
break;

case JIT_OP_STORE_ELEMENT_LONG:
{
	switch(state)
	{
	case 0x1a:
	{
        jit_uint low_dword = ((jit_uint *)(param[2]))[0];
        jit_uint high_dword = ((jit_uint *)(param[2]))[1];
        x86_mov_mem_imm(inst, param[0] + param[1] * 8, low_dword, 4);
        x86_mov_mem_imm(inst, param[0] + param[1] * 8 + 4, high_dword, 4);
    }
	break;
	case 0x11:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
        x86_mov_mem_reg(inst, param[0] + param[1] * 8, gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2] + 4, 4);
        x86_mov_mem_reg(inst, param[0] + param[1] * 8 + 4, gpreg1, 4);
        
    }
	break;
	case 0x8:
	{
        x86_mov_mem_reg(inst, param[0] + param[1] * 8, param[2], 4);
        x86_mov_mem_reg(inst, param[0] + param[1] * 8 + 4, jite_register_pair(param[2]), 4);
    }
	break;
	case 0x14:
	{
        jit_uint low_dword = ((jit_uint *)(param[2]))[0];
        jit_uint high_dword = ((jit_uint *)(param[2]))[1];
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_imm(inst, gpreg1, 0, param[1], 3, low_dword, 4);
        x86_mov_memindex_imm(inst, gpreg1, 4, param[1], 3, high_dword, 4);
        
    }
	break;
	case 0xb:
	{
        find_two_gp_regs_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2], 4);    
        x86_mov_memindex_reg(inst, gpreg1, 0, param[1], 3, gpreg2, 4);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2] + 4, 4);
        x86_mov_memindex_reg(inst, gpreg1, 4, param[1], 3, gpreg2, 4);
        
    }
	break;
	case 0x2:
	{
        find_one_gp_reg_cond3(inst, param[1], param[2], jite_register_pair(param[2]));
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_reg(inst, gpreg1, 0, param[1], 3, param[2], 4);
        x86_mov_memindex_reg(inst, gpreg1, 4, param[1], 3, jite_register_pair(param[2]), 4);
        
    }
	break;
	case 0x18:
	{
        jit_uint low_dword = ((jit_uint *)(param[2]))[0];
        jit_uint high_dword = ((jit_uint *)(param[2]))[1];
        x86_mov_membase_imm(inst, param[0], param[1] * 8, low_dword, 4);
        x86_mov_membase_imm(inst, param[0], param[1] * 8 + 4, high_dword, 4);
    }
	break;
	case 0xf:
	{
        find_one_gp_reg_cond1(inst, param[0]);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
        x86_mov_membase_reg(inst, param[0], param[1] * 8, gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2] + 4, 4);
        x86_mov_membase_reg(inst, param[0], param[1] * 8 + 4, gpreg1, 4);
        
    }
	break;
	case 0x6:
	{
        x86_mov_membase_reg(inst, param[0], param[1] * 8, param[2], 4);
        x86_mov_membase_reg(inst, param[0], param[1] * 8 + 4, jite_register_pair(param[2]), 4);
    }
	break;
	case 0x12:
	{
        jit_uint low_dword = ((jit_uint *)(param[2]))[0];
        jit_uint high_dword = ((jit_uint *)(param[2]))[1];
        x86_mov_memindex_imm(inst, param[0], 0, param[1], 3, low_dword, 4);
        x86_mov_memindex_imm(inst, param[0], 4, param[1], 3, high_dword, 4);
    }
	break;
	case 0x9:
	{
        find_two_gp_regs(inst);
        x86_lea_memindex(inst, gpreg1, param[0], 0, param[1], 3);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2], 4);
        x86_mov_membase_reg(inst, gpreg1, 0, gpreg2, 4);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2] + 4, 4);
        x86_mov_membase_reg(inst, gpreg1, 4, gpreg2, 4);
        
    }
	break;
	case 0x0:
	{
        find_one_gp_reg_cond2(inst, param[2], jite_register_pair(param[2]));
        x86_lea_memindex(inst, gpreg1, param[0], 0, param[1], 3);
        x86_mov_membase_reg(inst, gpreg1, 0, param[2], 4);
        x86_mov_membase_reg(inst, gpreg1, 4, jite_register_pair(param[2]), 4);
        
    }
	break;
	}
}
break;

case JIT_OP_STORE_ELEMENT_FLOAT32:
{
	switch(state)
	{
	case 0x1a:
	{
        jit_uint value = ((jit_uint *)(param[2]))[0];
        x86_mov_mem_imm(inst, param[0] + param[1] * 4, value, 4);
    }
	break;
	case 0x8:
	{
        sse_movss_mem_xmreg(inst, param[0] + param[1] * 4, param[2]);
    }
	break;
	case 0x14:
	{
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        jit_uint value = ((jit_uint *)(param[2]))[0];
        x86_mov_memindex_imm(inst, gpreg1, 0, param[1], 2, value, 4);
        
    }
	break;
	case 0x2:
	{
        if(!jite_gp_reg_is_free(func, param[1]))
        {
            find_one_gp_reg(inst);
            x86_mov_reg_reg(inst, gpreg1, param[1], 4);
            x86_alu_reg_imm(inst, X86_SHL, gpreg1, 2);
            sse_movss_membase_xmreg(inst, gpreg1, param[0], param[2]);        
            
        }
        else
        {
            x86_alu_reg_imm(inst, X86_SHL, param[1], 2);
            sse_movss_membase_xmreg(inst, param[1], param[0], param[2]);
        }
    }
	break;
	case 0x18:
	{
        jit_uint value = ((jit_uint*)(param[2]))[0];
        x86_mov_membase_imm(inst, param[0], param[1] * 4, value, 4);
    }
	break;
	case 0x6:
	{
        sse_movss_membase_xmreg(inst, param[0], param[1] * 4, param[2]);
    }
	break;
	case 0x12:
	{
        jit_uint value = ((jit_uint *)(param[2]))[0];
        x86_mov_memindex_imm(inst, param[0], 0, param[1], 2, value, 4);
    }
	break;
	case 0x0:
	{
        find_one_gp_reg(inst);
        x86_lea_memindex(inst, gpreg1, param[0], 0, param[1], 2);
        sse_movss_membase_xmreg(inst, gpreg1, 0, param[2]);
        
    }
	break;
	}
}
break;

case JIT_OP_STORE_ELEMENT_FLOAT64:
{
	switch(state)
	{
	case 0x1a:
	{
        jit_uint low_dword = ((jit_uint *)(param[2]))[0];
        jit_uint high_dword = ((jit_uint *)(param[2]))[1];
        x86_mov_mem_imm(inst, param[0] + param[1] * 8, low_dword, 4);
        x86_mov_mem_imm(inst, param[0] + param[1] * 8 + 4, high_dword, 4);
    }
	break;
	case 0x8:
	{
        sse2_movsd_mem_xmreg(inst, param[0] + param[1] * 8, param[2]);
    }
	break;
	case 0x14:
	{
        jit_uint low_dword = ((jit_uint *)(param[2]))[0];
        jit_uint high_dword = ((jit_uint *)(param[2]))[1];
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_imm(inst, gpreg1, 0, param[1], 3, low_dword, 4);
        x86_mov_memindex_imm(inst, gpreg1, 4, param[1], 3, high_dword, 4);
        
    }
	break;
	case 0x2:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_reg(inst, gpreg1, param[1], 4);
        x86_alu_reg_imm(inst, X86_SHL, gpreg1, 3);
        sse2_movsd_membase_xmreg(inst, gpreg1, param[0], param[2]);        
        
    }
	break;
	case 0x18:
	{
        jit_uint low_dword = ((jit_uint*)(param[2]))[0];
        jit_uint high_dword = ((jit_uint*)(param[2]))[1];
        x86_mov_membase_imm(inst, param[0], param[1] * 8, low_dword, 4);
        x86_mov_membase_imm(inst, param[0], param[1] * 8 + 4, high_dword, 4);
    }
	break;
	case 0x6:
	{
        sse2_movsd_membase_xmreg(inst, param[0], param[1] * 8, param[2]);
    }
	break;
	case 0x12:
	{
        jit_uint low_dword = ((jit_uint *)(param[2]))[0];
        jit_uint high_dword = ((jit_uint *)(param[2]))[1];
        x86_mov_memindex_imm(inst, param[0], 0, param[1], 3, low_dword, 4);
        x86_mov_memindex_imm(inst, param[0], 4, param[1], 3, high_dword, 4);
    }
	break;
	case 0x0:
	{
        find_one_gp_reg(inst);
        x86_lea_memindex(inst, gpreg1, param[0], 0, param[1], 2);
        sse2_movsd_membase_xmreg(inst, gpreg1, 0, param[2]);
        
    }
	break;
	}
}
break;

case JIT_OP_STORE_ELEMENT_NFLOAT:
{
	switch(state)
	{
	case 0x1a:
	{
        x86_mov_mem_imm(inst, param[0] + param[1] * 8, ((jit_uint *)(param[2]))[0], 4);
        x86_mov_mem_imm(inst, param[0] + param[1] * 8 + 4, ((jit_uint *)(param[2]))[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_mem_imm(inst, param[0] + param[1] * 8 + 8, ((jit_uint *)(param[2]))[2], 4);
        }
    }
	break;
	case 0x11:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
        x86_mov_mem_reg(inst, param[0] + param[1] * 8, gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2] + 4, 4);
        x86_mov_mem_reg(inst, param[0] + param[1] * 8 + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2] + 8, 4);
            x86_mov_mem_reg(inst, param[0] + param[1] * 8 + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x8:
	{
        sse2_movsd_mem_xmreg(inst, param[0] + param[1] * 8, param[2]);
    }
	break;
	case 0x14:
	{
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_memindex_imm(inst, gpreg1, 0, param[1], 3, ((jit_uint *)(param[2]))[0], 4);
        x86_mov_memindex_imm(inst, gpreg1, 4, param[1], 3, ((jit_uint *)(param[2]))[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_memindex_imm(inst, gpreg1, 8, param[1], 3, ((jit_uint *)(param[2]))[2], 4);
        }
        
    }
	break;
	case 0xb:
	{
        find_two_gp_regs_cond1(inst, param[1]);
        x86_mov_reg_imm(inst, gpreg1, param[0]);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2], 4);    
        x86_mov_memindex_reg(inst, gpreg1, 0, param[1], 3, gpreg2, 4);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2] + 4, 4);
        x86_mov_memindex_reg(inst, gpreg1, 4, param[1], 3, gpreg2, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2] + 8, 4);
            x86_mov_memindex_reg(inst, gpreg1, 8, param[1], 3, gpreg2, 4);
        }
        
    }
	break;
	case 0x2:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_reg(inst, gpreg1, param[1], 4);
        x86_alu_reg_imm(inst, X86_SHL, gpreg1, 3);
        sse2_movsd_membase_xmreg(inst, gpreg1, param[0], param[2]);        
        
    }
	break;
	case 0x18:
	{
        x86_mov_membase_imm(inst, param[0], param[1] * 8, ((jit_uint *)(param[2]))[0], 4);
        x86_mov_membase_imm(inst, param[0], param[1] * 8 + 4, ((jit_uint *)(param[2]))[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_membase_imm(inst, param[0], param[1] * 8 + 8, ((jit_uint *)(param[2]))[2], 4);
        }
    }
	break;
	case 0xf:
	{
        find_one_gp_reg_cond1(inst, param[0]);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2], 4);
        x86_mov_membase_reg(inst, param[0], param[1] * 8, gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2] + 4, 4);
        x86_mov_membase_reg(inst, param[0], param[1] * 8 + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg1, X86_EBP, param[2] + 8, 4);
            x86_mov_membase_reg(inst, param[0], param[1] * 8 + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x6:
	{
        sse2_movsd_membase_xmreg(inst, param[0], param[1] * 8, param[2]);
    }
	break;
	case 0x12:
	{
        x86_mov_memindex_imm(inst, param[0], 0, param[1], 3, ((jit_uint *)(param[2]))[0], 4);
        x86_mov_memindex_imm(inst, param[0], 4, param[1], 3, ((jit_uint *)(param[2]))[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_memindex_imm(inst, param[0], 8, param[1], 3, ((jit_uint *)(param[2]))[2], 4);
        }
    }
	break;
	case 0x9:
	{
        find_two_gp_regs(inst);
        x86_lea_memindex(inst, gpreg1, param[0], 0, param[1], 3);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2], 4);
        x86_mov_membase_reg(inst, gpreg1, 0, gpreg2, 4);
        x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2] + 4, 4);
        x86_mov_membase_reg(inst, gpreg1, 4, gpreg2, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg2, X86_EBP, param[2] + 8, 4);
            x86_mov_membase_reg(inst, gpreg1, 8, gpreg2, 4);
        }
        
    }
	break;
	case 0x0:
	{
        find_one_gp_reg(inst);
        x86_lea_memindex(inst, gpreg1, param[0], 0, param[1], 2);
        sse2_movsd_membase_xmreg(inst, gpreg1, 0, param[2]);
        
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_SBYTE:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2], 1, 0);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2])
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_widen_memindex(inst, param[0], param[0], 0, param[2], 0, 1, 0);
        }
        else
        {
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_widen_memindex(inst, param[0], gpreg1, 0, param[2], 0, 1, 0);
            
        }
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2], 1, 0);
    }
	break;
	case 0x0:
	{
        x86_widen_memindex(inst, param[0], param[1], 0, param[2], 0, 1, 0);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_UBYTE:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2], 0, 0);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2])
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_widen_memindex(inst, param[0], param[0], 0, param[2], 0, 0, 0);
        }
        else
        {
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_widen_memindex(inst, param[0], gpreg1, 0, param[2], 0, 0, 0);
            
        }
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2] * 2, 0, 0);
    }
	break;
	case 0x0:
	{
        x86_widen_memindex(inst, param[0], param[1], 0, param[2], 0, 0, 0);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_SHORT:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2] * 2, 1, 1);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2])
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_widen_memindex(inst, param[0], param[0], 0, param[2], 1, 1, 1);
        }
        else
        {
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_widen_memindex(inst, param[0], gpreg1, 0, param[2], 1, 1, 1);
            
        }
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2] * 2, 1, 1);
    }
	break;
	case 0x0:
	{
        x86_widen_memindex(inst, param[0], param[1], 0, param[2], 1, 1, 1);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_USHORT:
{
	switch(state)
	{
	case 0x18:
	{
        x86_widen_mem(inst, param[0], param[1] + param[2] * 2, 0, 1);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2])
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_widen_memindex(inst, param[0], param[0], 0, param[2], 1, 0, 1);
        }
        else
        {
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_widen_memindex(inst, param[0], gpreg1, 0, param[2], 1, 0, 1);
            
        }
    }
	break;
	case 0x12:
	{
        x86_widen_membase(inst, param[0], param[1], param[2] * 2, 0, 1);
    }
	break;
	case 0x0:
	{
        x86_widen_memindex(inst, param[0], param[1], 0, param[2], 1, 0, 1);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_INT:
{
	switch(state)
	{
	case 0x18:
	{
        x86_mov_reg_mem(inst, param[0], param[1] + param[2] * 4, 4);
    }
	break;
	case 0x6:
	{
        if(param[0] != param[2])
        {
            x86_mov_reg_imm(inst, param[0], param[1]);
            x86_mov_reg_memindex(inst, param[0], param[0], 0, param[2], 2, 4);
        }
        else
        {
            find_one_gp_reg_cond1(inst, param[0]);
            x86_mov_reg_imm(inst, gpreg1, param[1]);
            x86_mov_reg_memindex(inst, param[0], gpreg1, 0, param[2], 2, 4);
            
        }
    }
	break;
	case 0x12:
	{
        x86_mov_reg_membase(inst, param[0], param[1], param[2] * 4, 4);
    }
	break;
	case 0x0:
	{
        x86_mov_reg_memindex(inst, param[0], param[1], 0, param[2], 2, 4);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_LONG:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] * 8, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4); 
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] * 8 + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        
    }
	break;
	case 0x7:
	{
        find_two_gp_regs(inst);
        x86_mov_reg_reg(inst, gpreg1, param[2], 4);
        x86_shift_reg_imm(inst, X86_SHL, gpreg1, 3);
        x86_mov_reg_membase(inst, gpreg2, gpreg1, param[1], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg2, 4);
        x86_mov_reg_membase(inst, gpreg2, gpreg1, param[1] + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg2, 4);
        
    }
	break;
	case 0x13:
	{ 
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2] * 8, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2] * 8 + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        
    }
	break;
	case 0x1:
	{
        find_one_gp_reg_cond2(inst, param[1], param[2]);
        x86_mov_reg_memindex(inst, gpreg1, param[1], 0, param[2], 3, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_mov_reg_memindex(inst, gpreg1, param[1], 4, param[2], 3, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        
    }
	break;
	case 0x18:
	{
        x86_mov_reg_mem(inst, param[0], param[1] + param[2] * 8, 4);
        x86_mov_reg_mem(inst, jite_register_pair(param[0]), param[1] + param[2] * 8 + 4, 4);
    }
	break;
	case 0x6:
	{
        x86_mov_reg_reg(inst, param[0], param[2], 4);
        x86_shift_reg_imm(inst, X86_SHL, param[0], 3);
        x86_mov_reg_membase(inst, jite_register_pair(param[0]), param[0], param[1] + 4, 4);
        x86_mov_reg_membase(inst, param[0], param[0], param[1], 4);
    }
	break;
	case 0x12:
	{ 
        inst = masm_mov_reg_membase(inst, param[0], param[1], param[2] * 8, jit_value_get_type(dest));
    }
	break;
	case 0x0:
	{
        if(param[0] != param[1])
        {
            if(param[0] != param[2])
            {
                x86_mov_reg_memindex(inst, param[0], param[1], 0, param[2], 3, 4);
                x86_mov_reg_memindex(inst, jite_register_pair(param[0]), param[1], 4, param[2], 3, 4);
            }
            else if(jite_register_pair(param[0]) != param[1])
            {
                x86_mov_reg_memindex(inst, jite_register_pair(param[0]), param[1], 4, param[2], 3, 4);
                x86_mov_reg_memindex(inst, param[0], param[1], 0, param[2], 3, 4);
            }
            else // jite_register_pair(param[0]) == param[1] && param[0] == param[2]
            {
                find_one_gp_reg_cond2(inst, param[1], param[2]);
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                x86_mov_reg_memindex(inst, param[0], param[1], 0, gpreg1, 3, 4);
                x86_mov_reg_memindex(inst, jite_register_pair(param[0]), param[1], 4, gpreg1, 3, 4);
                
            }
        }
        else
        {
            if(jite_register_pair(param[0]) != param[2])
            {
                x86_mov_reg_memindex(inst, jite_register_pair(param[0]), param[1], 4, param[2], 3, 4);
                x86_mov_reg_memindex(inst, param[0], param[1], 0, param[2], 3, 4);
            }
            else if(param[0] != param[1])
            {
                x86_mov_reg_memindex(inst, param[0], param[1], 0, param[2], 3, 4);
                x86_mov_reg_memindex(inst, jite_register_pair(param[0]), param[1], 4, param[2], 3, 4);
            }
            else // jite_register_pair(param[0]) == param[2] && param[0] == param[1]
            {
                find_one_gp_reg_cond2(inst, param[1], param[2]);
                x86_mov_reg_reg(inst, gpreg1, param[2], 4);
                x86_mov_reg_memindex(inst, jite_register_pair(param[0]), param[1], 4, gpreg1, 3, 4);
                x86_mov_reg_memindex(inst, param[0], param[1], 0, gpreg1, 3, 4);
                
            }
        }
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_FLOAT32:
{
	switch(state)
	{
	case 0x18:
	{
        sse_movss_xmreg_mem(inst, param[0], param[1] + param[2] * 4);
    }
	break;
	case 0x6:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_reg(inst, gpreg1, param[2], 4);
        x86_shift_reg_imm(inst, X86_SHL, gpreg1, 2);
        sse_movss_xmreg_membase(inst, param[0], gpreg1, param[1]);
        
    }
	break;
	case 0x12:
	{
        sse_movss_xmreg_membase(inst, param[0], param[1], param[2] * 4);
    }
	break;
	case 0x0:
	{
        find_one_gp_reg(inst);
        x86_lea_memindex(inst, gpreg1, param[1], 0, param[2], 2);
        sse_movss_xmreg_membase(inst, param[0], gpreg1, 0);
        
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_FLOAT64:
{
	switch(state)
	{
	case 0x18:
	{
        sse2_movsd_xmreg_mem(inst, param[0], param[1] + param[2] * 8);
    }
	break;
	case 0x6:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_reg(inst, gpreg1, param[2], 4);
        x86_shift_reg_imm(inst, X86_SHL, gpreg1, 3);
        sse2_movsd_xmreg_membase(inst, param[0], gpreg1, param[1]);
        
    }
	break;
	case 0x12:
	{
        sse2_movsd_xmreg_membase(inst, param[0], param[1], param[2] * 8);
    }
	break;
	case 0x0:
	{
        find_one_gp_reg(inst);
        x86_lea_memindex(inst, gpreg1, param[1], 0, param[2], 3);
        sse2_movsd_xmreg_membase(inst, param[0], gpreg1, 0);
        
    }
	break;
	}
}
break;

case JIT_OP_LOAD_ELEMENT_NFLOAT:
{
	switch(state)
	{
	case 0x19:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] * 8, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4); 
        x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] * 8 + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_mem(inst, gpreg1, param[1] + param[2] * 8 + 8, 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0] + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x7:
	{
        find_two_gp_regs(inst);
        x86_mov_reg_reg(inst, gpreg1, param[2], 4);
        x86_shift_reg_imm(inst, X86_SHL, gpreg1, 3);
        x86_mov_reg_membase(inst, gpreg2, gpreg1, param[1], 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg2, 4);
        x86_mov_reg_membase(inst, gpreg2, gpreg1, param[1] + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg2, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg2, gpreg1, param[1] + 8, 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0] + 8, gpreg2, 4);
        }
        
    }
	break;
	case 0x13:
	{ 
        find_one_gp_reg_cond1(inst, param[1]);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2] * 8, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_mov_reg_membase(inst, gpreg1, param[1], param[2] * 8 + 4, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_membase(inst, gpreg1, param[1], param[2] * 8 + 8, 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0] + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x1:
	{
        find_one_gp_reg_cond2(inst, param[1], param[2]);
        x86_mov_reg_memindex(inst, gpreg1, param[1], 0, param[2], 3, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0], gpreg1, 4);
        x86_mov_reg_memindex(inst, gpreg1, param[1], 4, param[2], 3, 4);
        x86_mov_membase_reg(inst, X86_EBP, param[0] + 4, gpreg1, 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_reg_memindex(inst, gpreg1, param[1], 8, param[2], 3, 4);
            x86_mov_membase_reg(inst, X86_EBP, param[0] + 8, gpreg1, 4);
        }
        
    }
	break;
	case 0x18:
	{
        sse2_movsd_xmreg_mem(inst, param[0], param[1] + param[2] * 8);
    }
	break;
	case 0x6:
	{
        find_one_gp_reg(inst);
        x86_mov_reg_reg(inst, gpreg1, param[2], 4);
        x86_shift_reg_imm(inst, X86_SHL, gpreg1, 3);
        sse2_movsd_xmreg_membase(inst, param[0], gpreg1, param[1]);
        
    }
	break;
	case 0x12:
	{
        sse2_movsd_xmreg_membase(inst, param[0], param[1], param[2] * 8);
    }
	break;
	case 0x0:
	{
        find_one_gp_reg(inst);
        x86_lea_memindex(inst, gpreg1, param[1], 0, param[2], 3);
        sse2_movsd_xmreg_membase(inst, param[0], gpreg1, 0);
        
    }
	break;
	}
}
break;

case JIT_OP_COPY_LOAD_SBYTE:
case JIT_OP_COPY_LOAD_UBYTE:
case JIT_OP_COPY_STORE_BYTE:
case JIT_OP_COPY_LOAD_SHORT:
case JIT_OP_COPY_LOAD_USHORT:
case JIT_OP_COPY_STORE_SHORT:
case JIT_OP_COPY_INT:
case JIT_OP_COPY_LONG:
case JIT_OP_COPY_FLOAT32:
case JIT_OP_COPY_FLOAT64:
case JIT_OP_COPY_NFLOAT:
case JIT_OP_COPY_STRUCT:
{
	switch(state)
	{
	case 0x0:
	{
        // Both registers and locals allocated are 32-bits long.
        // Just storing the first 8 or 16 bits we could lose the format,
        // for example, in "int a = 0xffeedd; short b = (short)a;"
        // gives 0xffffeedd, and not 0xeedd.
        inst = masm_mov_reg_reg(inst, param[0], param[1], sourceType);
    }
	break;
	case 0x3:
	{
        inst = masm_mov_reg_membase(inst, param[0], X86_EBP, param[1], sourceType);
    }
	break;
	case 0x6:
	{
        inst = masm_mov_reg_imm(inst, param[0], param[1], sourceType);
    }
	break;
	case 0x1:
	{
        inst = masm_mov_membase_reg(inst, X86_EBP, param[0], param[1], sourceType);
    }
	break;
	case 0x4:
	{
        inst = masm_mov_membase_membase(inst, X86_EBP, param[0], X86_EBP, param[1], jite_type_get_size(jit_value_get_type(dest)));
    }
	break;
	case 0x7:
	{
        inst = masm_mov_membase_imm(inst, X86_EBP, param[0], param[1], sourceType);
    }
	break;
	}
}
break;

case JIT_OP_ADDRESS_OF:
{
	switch(state)
	{
	case 0x0:
	{
        /* This case can be if the value, address of which we try to load, was saved temporary to a register */
        x86_lea_membase(inst, param[0], X86_EBP, value1->vreg->frame->frame_offset);	
    }
	break;
	case 0x3:
	{
        x86_lea_membase(inst, param[0], X86_EBP, param[1]);
    }
	break;
	}
}
break;

case JIT_OP_MEMCPY:
{
	switch(state)
	{
	case 0x12:
	{
             inst = masm_mov_membase_membase(inst, param[0], 0, param[1], 0, param[2]);
       }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_LOAD_RELATIVE_SBYTE:
case JIT_OP_LOAD_RELATIVE_UBYTE:
case JIT_OP_LOAD_RELATIVE_SHORT:
case JIT_OP_LOAD_RELATIVE_USHORT:
case JIT_OP_LOAD_RELATIVE_INT:
case JIT_OP_LOAD_RELATIVE_LONG:
case JIT_OP_LOAD_RELATIVE_FLOAT32:
case JIT_OP_LOAD_RELATIVE_FLOAT64:
case JIT_OP_LOAD_RELATIVE_NFLOAT:
case JIT_OP_LOAD_RELATIVE_STRUCT:
case JIT_OP_STORE_RELATIVE_BYTE:
case JIT_OP_STORE_RELATIVE_SHORT:
case JIT_OP_STORE_RELATIVE_INT:
case JIT_OP_STORE_RELATIVE_LONG:
case JIT_OP_STORE_RELATIVE_FLOAT32:
case JIT_OP_STORE_RELATIVE_FLOAT64:
case JIT_OP_STORE_RELATIVE_NFLOAT:
case JIT_OP_STORE_RELATIVE_STRUCT:
case JIT_OP_STORE_ELEMENT_BYTE:
case JIT_OP_STORE_ELEMENT_SHORT:
case JIT_OP_STORE_ELEMENT_INT:
case JIT_OP_STORE_ELEMENT_LONG:
case JIT_OP_STORE_ELEMENT_FLOAT32:
case JIT_OP_STORE_ELEMENT_FLOAT64:
case JIT_OP_STORE_ELEMENT_NFLOAT:
case JIT_OP_LOAD_ELEMENT_SBYTE:
case JIT_OP_LOAD_ELEMENT_UBYTE:
case JIT_OP_LOAD_ELEMENT_SHORT:
case JIT_OP_LOAD_ELEMENT_USHORT:
case JIT_OP_LOAD_ELEMENT_INT:
case JIT_OP_LOAD_ELEMENT_LONG:
case JIT_OP_LOAD_ELEMENT_FLOAT32:
case JIT_OP_LOAD_ELEMENT_FLOAT64:
case JIT_OP_LOAD_ELEMENT_NFLOAT:
case JIT_OP_COPY_LOAD_SBYTE:
case JIT_OP_COPY_LOAD_UBYTE:
case JIT_OP_COPY_STORE_BYTE:
case JIT_OP_COPY_LOAD_SHORT:
case JIT_OP_COPY_LOAD_USHORT:
case JIT_OP_COPY_STORE_SHORT:
case JIT_OP_COPY_INT:
case JIT_OP_COPY_LONG:
case JIT_OP_COPY_FLOAT32:
case JIT_OP_COPY_FLOAT64:
case JIT_OP_COPY_NFLOAT:
case JIT_OP_COPY_STRUCT:
case JIT_OP_ADDRESS_OF:
case JIT_OP_MEMCPY:
	return 1;

#endif
