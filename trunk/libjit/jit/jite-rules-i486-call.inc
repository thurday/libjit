/* Automatically generated from ./jite-rules-i486-call.lir - DO NOT EDIT */
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

case JIT_OP_RETURN:
{
	switch(state)
	{
	default:
	{
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	}
}
break;

case JIT_OP_RETURN_INT:
case JIT_OP_RETURN_LONG:
{
	switch(state)
	{
	case 0x2:
	{
        inst = masm_mov_reg_imm(inst, X86_EAX, param[0], sourceType);
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x1:
	{
        inst = masm_mov_reg_membase(inst, X86_EAX, X86_EBP, param[0], sourceType);
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x0:
	{
        inst = masm_mov_reg_reg(inst, X86_EAX, param[0], sourceType);
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	}
}
break;

case JIT_OP_RETURN_FLOAT32:
{
	switch(state)
	{
	case 0x2:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        switch(abi)
        {
            case jit_abi_internal:
            {
                x86_mov_reg_imm(inst, X86_EAX, ((jit_uint *)(param[0]))[0]);
                sse2_movd_xmreg_reg(inst, XMM0, X86_EAX);
            }
            break;
            default:
            {
                x86_mov_membase_imm(inst, X86_ESP, -8, ((jit_uint *)(param[0]))[0], 4);
                x86_fld_membase(inst, X86_ESP, -8, 0);
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x1:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        switch(abi)
        {
            case jit_abi_internal:
            {
                sse_movss_xmreg_membase(inst, XMM0, X86_EBP, param[0]);
            }
            break;
            default:
            {
                x86_fld_membase(inst, X86_EBP, param[0], 0);
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x0:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        switch(abi)
        {
            case jit_abi_internal:
            {
                if(param[0] != XMM0) sse_movss_xmreg_xmreg(inst, XMM0, param[0]);
            }
            break;
            default:
            {
                sse_movss_membase_xmreg(inst, X86_ESP, -8, XMM0);
                x86_fld_membase(inst, X86_ESP, -8, 0);
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	}
}
break;

case JIT_OP_RETURN_FLOAT64:
{
	switch(state)
	{
	case 0x2:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        jit_nint *ptr = (jit_nint *)param[0];
        jit_nint low_dword = ptr[0];
        jit_nint high_dword = ptr[1];
        x86_mov_membase_imm(inst, X86_ESP, -8, low_dword, 4);
        x86_mov_membase_imm(inst, X86_ESP, -4, high_dword, 4);
        switch(abi)
        {
            case jit_abi_internal:
            {
                sse2_movsd_xmreg_membase(inst, XMM0, X86_ESP, -8);
            }
            break;
            default:
            {
                x86_fld_membase(inst, X86_ESP, -8, 1);
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x1:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        switch(abi)
        {
            case jit_abi_internal:
            {
                sse2_movsd_xmreg_membase(inst, XMM0, X86_EBP, param[0]);
            }
            break;
            default:
            {
                x86_fld_membase(inst, X86_EBP, param[0], 1);
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x0:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        switch(abi)
        {
            case jit_abi_internal:
            {
                if(param[0] != XMM0) sse2_movsd_xmreg_xmreg(inst, XMM0, param[0]);
            }
            break;
            default:
            {
                sse2_movsd_membase_xmreg(inst, X86_ESP, -8, XMM0);
                x86_fld_membase(inst, X86_ESP, -8, 1);
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	}
}
break;

case JIT_OP_RETURN_NFLOAT:
{
	switch(state)
	{
	case 0x2:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        jit_nint *ptr = (jit_nint *)param[0];
        x86_mov_membase_imm(inst, X86_ESP, -16, ptr[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, -12, ptr[1], 4);
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_mov_membase_imm(inst, X86_ESP, -8, ptr[2], 4);
        }
        switch(abi)
        {
            case jit_abi_internal:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    x86_fld80_membase(inst, X86_ESP, -16);
                }
                else
                {    
                    sse2_movsd_xmreg_membase(inst, XMM0, X86_ESP, -16);
                }
            }
            break;
            default:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    x86_fld80_membase(inst, X86_ESP, -16);
                }
                else
                {
                    x86_fld_membase(inst, X86_ESP, -16, 1);
                }
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x1:
	{
        jit_abi_t abi = jit_function_get_abi(func);
        switch(abi)
        {
            case jit_abi_internal:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    x86_fld80_membase(inst, X86_EBP, param[0]);
                }
                else
                {
                    sse2_movsd_xmreg_membase(inst, XMM0, X86_EBP, param[0]);
                }
            }
            break;
            default:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    x86_fld80_membase(inst, X86_EBP, param[0]);
                }
                else
                {
                    x86_fld_membase(inst, X86_EBP, param[0], 1);
                }
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	case 0x0:
	{
        // If this case is reached then sizeof(jit_nfloat) == sizeof(jit_float64)
        // and we use xmm registers for both jit_float64 and jit_nfloat values.
        jit_abi_t abi = jit_function_get_abi(func);
        switch(abi)
        {
            case jit_abi_internal:
            {    
                if(param[0] != XMM0)
                {
                    sse2_movsd_xmreg_xmreg(inst, XMM0, param[0]);
                }
            }
            break;
            default:
            {
                sse2_movsd_membase_xmreg(inst, X86_ESP, -8, XMM0);
                x86_fld_membase(inst, X86_ESP, -8, 1);
            }
            break;
        }
        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	}
}
break;

case JIT_OP_RETURN_SMALL_STRUCT:
{
	switch(state)
	{
	case 0x6:
	{
        switch(param[1])
        {
        case 1:
            x86_widen_membase(inst, X86_EAX, param[0], 0, 0, 0);
            break;

        case 2:
            x86_widen_membase(inst, X86_EAX, param[0], 0, 0, 1);
            break;

        case 3:
            if(X86_EAX == param[0])
            {
                x86_widen_membase(inst, X86_EDX, param[0], 0, 0, 1);
                x86_widen_membase(inst, X86_EAX, param[0], 2, 0, 0);
            }
            else
            {
                x86_widen_membase(inst, X86_EAX, param[0], 2, 0, 0);
                x86_widen_membase(inst, X86_EDX, param[0], 0, 0, 1);
            }
            x86_shift_reg_imm(inst, X86_SHL, X86_EAX, 16);
            x86_alu_reg_reg(inst, X86_OR, X86_EAX, X86_EDX);
            break;

        case 4:
            x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
            break;

        case 5:
            if(X86_EAX == param[0])
            {
                x86_widen_membase(inst, X86_EDX, param[0], 4, 0, 0);
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
            }
            else
            {
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
                x86_widen_membase(inst, X86_EDX, param[0], 4, 0, 0);
            }
            break;

        case 6:
            if(X86_EAX == param[0])
            {
                x86_widen_membase(inst, X86_EDX, param[0], 4, 0, 1);
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
            }
            else
            {
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
                x86_widen_membase(inst, X86_EDX, param[0], 4, 0, 1);
            }
            break;

        case 7:
            if(X86_EAX == param[0])
            {
                x86_widen_membase(inst, X86_ECX, param[0], 4, 0, 1);
                x86_widen_membase(inst, X86_EDX, param[0], 6, 0, 0);
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
            }
            else if(X86_ECX == param[0])
            {
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
                x86_widen_membase(inst, X86_EDX, param[0], 6, 0, 0);
                x86_widen_membase(inst, X86_ECX, param[0], 4, 0, 1);
            }
            else
            {
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
                x86_widen_membase(inst, X86_ECX, param[0], 4, 0, 1);
                x86_widen_membase(inst, X86_EDX, param[0], 6, 0, 0);
            }
            x86_shift_reg_imm(inst, X86_SHL, X86_EDX, 16);
            x86_alu_reg_reg(inst, X86_OR, X86_EDX, X86_ECX);
            break;

        case 8:
            if(X86_EAX == param[0])
            {
                x86_mov_reg_membase(inst, X86_EDX, param[0], 4, 4);
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
            }
            else
            {
                x86_mov_reg_membase(inst, X86_EAX, param[0], 0, 4);
                x86_mov_reg_membase(inst, X86_EDX, param[0], 4, 4);
            }
            break;
        }

        inst = jite_jump_to_epilog(gen, inst, block);
    }
	break;
	}
}
break;

case JIT_OP_RETURN_REG:
{
	switch(state)
	{
	case 0x6:
	{
        int sourceX86Reg = (int)jit_value_get_nint_constant(insn->value2);
        int sourceReg = jite_x86reg_to_reg(sourceX86Reg);
        if(sourceX86Reg >= X86_REG_EAX && sourceX86Reg <= X86_REG_ESP)
        {
            inst = masm_mov_reg_reg(inst, param[0], sourceReg, sourceType);
        }
        else if(sourceX86Reg >= X86_REG_ST0 && sourceX86Reg <= X86_REG_ST7)
        {
            inst = masm_mov_value_st0(inst, insn->value1);
        }
        else if(sourceX86Reg >= X86_REG_XMM0 && sourceX86Reg <= X86_REG_XMM7)
        {
            inst = masm_mov_reg_reg(inst, param[0], sourceReg, sourceType);
        }
    }
	break;
	case 0x7:
	{
        int sourceX86Reg = (int)jit_value_get_nint_constant(insn->value2);
        int sourceReg = jite_x86reg_to_reg(sourceX86Reg);
        if(sourceX86Reg >= X86_REG_EAX && sourceX86Reg <= X86_REG_ESP)
        {
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], sourceReg, sourceType);
        }
        else if(sourceX86Reg >= X86_REG_ST0 && sourceX86Reg <= X86_REG_ST7)
        {
            inst = masm_mov_value_st0(inst, insn->value1);
        }
        else if(sourceX86Reg >= X86_REG_XMM0 && sourceX86Reg <= X86_REG_XMM7)
        {
            inst = masm_mov_membase_reg(inst, X86_EBP, param[0], sourceReg, sourceType);
        }
    }
	break;
	}
}
break;

case JIT_OP_TAIL_CALL:
{
	switch(state)
	{
	default:
	{
        unsigned int size = jite_stack_depth_used(func);
        // Add two extra dwords for the return address after call and EBP in stack.
        size += 8;
        x86_lea_membase(inst, X86_ESP, X86_EBP, size);
    }
	break;
	}
}
break;

case JIT_OP_PUSH_INT:
{
	switch(state)
	{
	case 0x2:
	{
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        x86_push_imm(inst, param[0]);
        func->jite->relative_sp_offset = 0;
        gen->stack_changed = 1;
    }
	break;
	case 0x1:
	{
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        x86_push_membase(inst, X86_EBP, param[0]);
        func->jite->relative_sp_offset = 0;
        gen->stack_changed = 1;
    }
	break;
	case 0x0:
	{
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        x86_push_reg(inst, param[0]);
        func->jite->relative_sp_offset = 0;
        gen->stack_changed = 1;
    }
	break;
	}
}
break;

case JIT_OP_PUSH_LONG:
{
	switch(state)
	{
	case 0x0:
	{
        func->jite->relative_sp_offset -= 8;
        x86_mov_membase_reg(inst, X86_ESP, func->jite->relative_sp_offset + 4, jite_register_pair(param[0]), 4);
        x86_mov_membase_reg(inst, X86_ESP, func->jite->relative_sp_offset, param[0], 4);
        gen->stack_changed = 1;
    }
	break;
	case 0x1:
	{
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        x86_push_membase(inst, X86_EBP, param[0] + 4);
        x86_push_membase(inst, X86_EBP, param[0]);
        func->jite->relative_sp_offset = 0;
        gen->stack_changed = 1;
    }
	break;
	case 0x2:
	{
        func->jite->relative_sp_offset -= 8;
        x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset + 4, ((jit_uint *)(insn->value1->address))[1], 4);
        x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset, ((jit_uint *)(insn->value1->address))[0], 4);
        gen->stack_changed = 1;
    }
	break;
	}
}
break;

case JIT_OP_PUSH_FLOAT32:
{
	switch(state)
	{
	case 0x0:
	{
        func->jite->relative_sp_offset -= 4;
        sse_movss_membase_xmreg(inst, X86_ESP, func->jite->relative_sp_offset, param[0]);
        gen->stack_changed = 1;
    }
	break;
	case 0x1:
	{
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        x86_push_membase(inst, X86_EBP, param[0]);
        func->jite->relative_sp_offset = 0;
        gen->stack_changed = 1;
    }
	break;
	case 0x2:
	{
        func->jite->relative_sp_offset -= 4;
        x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset, ((jit_uint *)(insn->value1->address))[0], 4);
        gen->stack_changed = 1;
    }
	break;
	}
}
break;

case JIT_OP_PUSH_FLOAT64:
{
	switch(state)
	{
	case 0x0:
	{
        func->jite->relative_sp_offset -= 8;
        sse2_movsd_membase_xmreg(inst, X86_ESP, func->jite->relative_sp_offset, param[0]);
        gen->stack_changed = 1;
    }
	break;
	case 0x1:
	{
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        x86_push_membase(inst, X86_EBP, param[0] + 4);
        x86_push_membase(inst, X86_EBP, param[0]);
        func->jite->relative_sp_offset = 0;
        gen->stack_changed = 1;
    }
	break;
	case 0x2:
	{
        func->jite->relative_sp_offset -= 8;
        x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset + 4, ((jit_uint *)(insn->value1->address))[1], 4);
        x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset, ((jit_uint *)(insn->value1->address))[0], 4);
        gen->stack_changed = 1;
    }
	break;
	}
}
break;

case JIT_OP_PUSH_NFLOAT:
{
	switch(state)
	{
	case 0x0:
	{
        func->jite->relative_sp_offset -= 8;
        sse2_movsd_membase_xmreg(inst, X86_ESP, func->jite->relative_sp_offset, param[0]);
        gen->stack_changed = 1;
    }
	break;
	case 0x1:
	{
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }

        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_push_membase(inst, X86_EBP, param[0] + 8);
        }
        x86_push_membase(inst, X86_EBP, param[0] + 4);
        x86_push_membase(inst, X86_EBP, param[0]);
        func->jite->relative_sp_offset = 0;
        gen->stack_changed = 1;
    }
	break;
	case 0x2:
	{
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            func->jite->relative_sp_offset -= 12;
            x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset + 8, ((jit_uint *)(insn->value1->address))[2], 4);
            x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset + 4, ((jit_uint *)(insn->value1->address))[1], 4);
            x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset,     ((jit_uint *)(insn->value1->address))[0], 4);
        }
        else
        {
            func->jite->relative_sp_offset -= 8;
            x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset + 4, ((jit_uint *)(insn->value1->address))[1], 4);
            x86_mov_membase_imm(inst, X86_ESP, func->jite->relative_sp_offset,     ((jit_uint *)(insn->value1->address))[0], 4);
        }
        gen->stack_changed = 1;
    }
	break;
	}
}
break;

case JIT_OP_PUSH_STRUCT:
{
	switch(state)
	{
	default:
	{
        unsigned int size = jite_type_get_size(sourceType);
        func->jite->relative_sp_offset -= size;
        find_one_gp_reg(inst);
        inst = jite_memory_copy_with_reg(inst, X86_ESP, func->jite->relative_sp_offset, X86_EBP, insn->value1->vreg->frame->frame_offset, size, gpreg1);
        gen->stack_changed = 1;
    }
	break;
	}
}
break;

case JIT_OP_OUTGOING_REG:
{
	switch(state)
	{
	case 0x6:
	{
        inst = masm_mov_reg_reg(inst, param[1], param[0], sourceType);
    }
	break;
	case 0x7:
	{
        inst = masm_mov_reg_membase(inst, param[1], X86_EBP, param[0], sourceType);
    }
	break;
	case 0x8:
	{
        inst = masm_mov_reg_imm(inst, param[1], param[0], sourceType);
    }
	break;
	}
}
break;

case JIT_OP_POP_STACK:
{
	switch(state)
	{
	default:
	{
        x86_alu_reg_imm(inst, X86_ADD, X86_ESP, insn->value1->address);
//        func->jite->sp_offset = 0;
    }
	break;
	}
}
break;

case JIT_OP_FLUSH_SMALL_STRUCT:
{
	switch(state)
	{
	default:
	{
    }
	break;
	}
}
break;

case JIT_OP_CALL:
{
	switch(state)
	{
	default:
	{
        jit_function_t called_func = (jit_function_t)(insn->dest);
        unsigned int num = (*insn->call_params)->num;
        jit_value_t *args = (*insn->call_params)->args;

        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        func->jite->relative_sp_offset = 0;

        inst = jite_emit_function_call(gen, inst, func, num,
                            jit_function_to_closure(called_func),
                            0,
                            args,
                            insn->value2,
                            NORMAL_CALL);
    }
	break;
	}
}
break;

case JIT_OP_CALL_TAIL:
{
	switch(state)
	{
	default:
	{
        jit_function_t called_func = (jit_function_t)(insn->dest);
        jit_abi_t abi = (*insn->call_params)->abi;
        unsigned int num = (*insn->call_params)->num;
        jit_value_t *args = (*insn->call_params)->args;
        int callType = NORMAL_CALL;
        
        if(abi == jit_function_get_abi(func))
        {
            callType = TAIL_CALL;
        }
        
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        func->jite->relative_sp_offset = 0;

        inst = jite_emit_function_call(gen, inst, func, num,
                            jit_function_to_closure(called_func),
                            0,
                            args,
                            insn->value2,
                            callType);
    }
	break;
	}
}
break;

case JIT_OP_CALL_EXTERNAL:
{
	switch(state)
	{
	default:
	{
        void *called_func = (void*)(insn->dest);
        unsigned int num = (*insn->call_params)->num;
        jit_value_t *args = (*insn->call_params)->args;
    
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        func->jite->relative_sp_offset = 0;

        inst = jite_emit_function_call(gen, inst, func, num,
                            called_func,
                            0,
                            args,
                            insn->value2,
                            NORMAL_CALL);
    }
	break;
	}
}
break;

case JIT_OP_CALL_EXTERNAL_TAIL:
{
	switch(state)
	{
	default:
	{
        void *called_func = (void*)(insn->dest);
        jit_abi_t abi = (*insn->call_params)->abi;
        unsigned int num = (*insn->call_params)->num;
        jit_value_t *args = (*insn->call_params)->args;
        int callType = NORMAL_CALL;
        if(abi == jit_function_get_abi(func))
        {
            callType = TAIL_CALL;
        }
        
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        func->jite->relative_sp_offset = 0;
        
        inst = jite_emit_function_call(gen, inst, func, num,
                            called_func,
                            0,
                            args,
                            insn->value2,
                            callType);
    }
	break;
	}
}
break;

case JIT_OP_CALL_INDIRECT:
case JIT_OP_CALL_VTABLE_PTR:
{
	switch(state)
	{
	default:
	{    
        jit_value_t indirect_ptr = insn->value1;
        unsigned int num = (*insn->call_params)->num;
        jit_value_t *args = (*insn->call_params)->args;

        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        func->jite->relative_sp_offset = 0;

        inst = jite_emit_function_call(gen, inst, func, num,
                            0,
                            indirect_ptr,
                            args,
                            insn->value2,
                            INDIRECT_CALL);
    }
	break;
	}
}
break;

case JIT_OP_CALL_INDIRECT_TAIL:
case JIT_OP_CALL_VTABLE_PTR_TAIL:
{
	switch(state)
	{
	default:
	{
        jit_value_t indirect_ptr = insn->value1;
        jit_abi_t abi = (*insn->call_params)->abi;
        unsigned int num = (*insn->call_params)->num;
        jit_value_t *args = (*insn->call_params)->args;
        int callType = INDIRECT_CALL;
        if(abi == jit_function_get_abi(func))
        {
            callType = INDIRECT_TAIL_CALL;
        }
        
        if(func->jite->relative_sp_offset)
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, -func->jite->relative_sp_offset);
        }
        func->jite->relative_sp_offset  = 0;

        inst = jite_emit_function_call(gen, inst, func, num,
                            0,
                            indirect_ptr,
                            args,
                            insn->value2,
                            callType);
    }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_RETURN:
case JIT_OP_RETURN_INT:
case JIT_OP_RETURN_LONG:
case JIT_OP_RETURN_FLOAT32:
case JIT_OP_RETURN_FLOAT64:
case JIT_OP_RETURN_NFLOAT:
case JIT_OP_RETURN_SMALL_STRUCT:
case JIT_OP_RETURN_REG:
case JIT_OP_TAIL_CALL:
case JIT_OP_PUSH_INT:
case JIT_OP_PUSH_LONG:
case JIT_OP_PUSH_FLOAT32:
case JIT_OP_PUSH_FLOAT64:
case JIT_OP_PUSH_NFLOAT:
case JIT_OP_PUSH_STRUCT:
case JIT_OP_OUTGOING_REG:
case JIT_OP_POP_STACK:
case JIT_OP_FLUSH_SMALL_STRUCT:
case JIT_OP_CALL:
case JIT_OP_CALL_TAIL:
case JIT_OP_CALL_EXTERNAL:
case JIT_OP_CALL_EXTERNAL_TAIL:
case JIT_OP_CALL_INDIRECT:
case JIT_OP_CALL_VTABLE_PTR:
case JIT_OP_CALL_INDIRECT_TAIL:
case JIT_OP_CALL_VTABLE_PTR_TAIL:
	return 1;

#endif
