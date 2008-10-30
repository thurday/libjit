/* Automatically generated from ./jite-rules-i486-except.lir - DO NOT EDIT */
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

case JIT_OP_THROW:
{
	switch(state)
	{
	case 0x0:
	{
        if(func->builder->setjmp_value != 0)
        {
            /* We have a "setjmp" block in the current function,
               so we must record the location of the throw first */
            if(func->builder->position_independent)
            {
                x86_call_imm(inst, 0);
                x86_pop_membase(inst, X86_EBP,
                        func->builder->setjmp_value->vreg->frame->frame_offset
                        + jit_jmp_catch_pc_offset);
            }
            else
            {

                int pc = (int) inst;
                x86_mov_membase_imm(inst, X86_EBP,
                            func->builder->setjmp_value->vreg->frame->frame_offset
                            + jit_jmp_catch_pc_offset, pc, 4);
            }
        }
        x86_push_reg(inst, param[0]);
        x86_call_code(inst, (void *)jit_exception_throw);
    }
	break;
	case 0x1:
	{
        if(func->builder->setjmp_value != 0)
        {
            /* We have a "setjmp" block in the current function,
               so we must record the location of the throw first */
            if(func->builder->position_independent)
            {
                x86_call_imm(inst, 0);
                x86_pop_membase(inst, X86_EBP,
                            func->builder->setjmp_value->vreg->frame->frame_offset
                            + jit_jmp_catch_pc_offset);
            }
            else
            {
                int pc = (int) inst;
                x86_mov_membase_imm(inst, X86_EBP,
                            func->builder->setjmp_value->vreg->frame->frame_offset
                            + jit_jmp_catch_pc_offset, pc, 4);
            }
        }
        x86_push_membase(inst, X86_EBP, param[0]);
        x86_call_code(inst, (void *)jit_exception_throw);
    }
	break;
	}
}
break;

case JIT_OP_RETHROW:
{
	switch(state)
	{
	default:
	{ /* Not used in native code back ends */ }
	break;
	}
}
break;

case JIT_OP_LOAD_PC:
{
	switch(state)
	{
	case 0x0:
	{
        int pc = (int) inst;
        x86_mov_reg_imm(inst, param[0], pc);
    }
	break;
	case 0x1:
	{
        int pc = (int) inst;
        x86_mov_membase_imm(inst, X86_EBP, param[0], pc, 4);
    }
	break;
	}
}
break;

case JIT_OP_LOAD_EXCEPTION_PC:
{
	switch(state)
	{
	default:
	{ /* Not used in native code back ends */ }
	break;
	}
}
break;

case JIT_OP_ENTER_FINALLY:
{
	switch(state)
	{
	default:
	{ /* Nothing to do here: return address on the stack */ }
	break;
	}
}
break;

case JIT_OP_LEAVE_FINALLY:
{
	switch(state)
	{
	default:
	{
        /* The "finally" return address is on the stack */
        x86_ret(inst);
    }
	break;
	}
}
break;

case JIT_OP_CALL_FINALLY:
{
	switch(state)
	{
	default:
	{
        jit_block_t block;

        block = jit_block_from_label(func, (jit_label_t)(insn->dest));
        if(!block)
        {
            return;
        }

        if(block->address)
        {
            x86_call_code(inst, block->address);
        }
        else
        {
            x86_call_imm(inst, block->fixup_list);
            block->fixup_list = (void *)(inst - 4);
        }
    }
	break;
	}
}
break;

case JIT_OP_ENTER_FILTER:
{
	switch(state)
	{
	default:
	{
        /* TODO */
    }
	break;
	}
}
break;

case JIT_OP_LEAVE_FILTER:
{
	switch(state)
	{
	default:
	{
        /* TODO */
    }
	break;
	}
}
break;

case JIT_OP_CALL_FILTER:
{
	switch(state)
	{
	default:
	{
        /* TODO */
    }
	break;
	}
}
break;

case JIT_OP_CALL_FILTER_RETURN:
{
	switch(state)
	{
	default:
	{
        /* TODO */
    }
	break;
	}
}
break;

case JIT_OP_ADDRESS_OF_LABEL:
{
	switch(state)
	{
	case 0x0:
	{
        block = jit_block_from_label(func, (jit_label_t)(insn->value1));
        if(func->builder->position_independent)
        {
            /* TODO */
        }
        else
        {
            if(block->address)
            {
                x86_mov_reg_imm(inst, param[0], block->address);
            }
            else
            {
                /* Output a placeholder and record on the block's fixup list */
                x86_mov_reg_imm(inst, param[0], (int)(block->fixup_absolute_list));
                block->fixup_absolute_list = (void *)(inst - 4);
            }
        }
    }
	break;
	case 0x1:
	{
        block = jit_block_from_label(func, (jit_label_t)(insn->value1));
        if(func->builder->position_independent)
        {
            /* TODO */
        }
        else
        {
            if(block->address)
            {
                x86_mov_membase_imm(inst, X86_EBP, param[0], (int)(block->address), 4);
            }
            else
            {
                /* Output a placeholder and record on the block's fixup list */
                x86_mov_membase_imm(inst, X86_EBP, param[0], (int)(block->fixup_absolute_list), 4);
                block->fixup_absolute_list = (void *)(inst - 4);
            }
        }
    }
	break;
	}
}
break;

#elif defined(JIT_INCLUDE_SUPPORTED)

case JIT_OP_THROW:
case JIT_OP_RETHROW:
case JIT_OP_LOAD_PC:
case JIT_OP_LOAD_EXCEPTION_PC:
case JIT_OP_ENTER_FINALLY:
case JIT_OP_LEAVE_FINALLY:
case JIT_OP_CALL_FINALLY:
case JIT_OP_ENTER_FILTER:
case JIT_OP_LEAVE_FILTER:
case JIT_OP_CALL_FILTER:
case JIT_OP_CALL_FILTER_RETURN:
case JIT_OP_ADDRESS_OF_LABEL:
	return 1;

#endif
