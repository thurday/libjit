#if defined(__i386) || defined(__i386__) || defined(_M_IX86)
#include "jit-internal.h"
#include "jit-linear-scan-i486.h"
#include "jit-setjmp.h"
#include "jit-gen-i486-masm.h"
#include <math.h>

void lir_init(jit_function_t func)
{
	incoming_params_state_t state = func->lir->incoming_params_state;
	if(func->lir->incoming_params_state == 0)
	{
		state = jit_cnew(struct _incoming_params_state);
		func->lir->incoming_params_state = state;
	}

	state->frame_index = 2;
	state->gp_index = 0;
	state->xmm_index = 0;

	func->lir->regs_state = 0;
	lir_list_t list = func->lir->frame_state;
	while(list)
	{
		list->item1 = 0;
		list->item2 = 0;
		list->item3 = 0;
		list = list->next;
	}

	lir_linked_list_t linked_list = func->lir->vregs_list;
	while(linked_list)
	{
		lir_vreg_t vreg = (lir_vreg_t)linked_list->item;
		lir_free_frame(func, vreg->frame);
		linked_list = linked_list->next;
	}

	func->lir->scratch_regs = 0;
	func->lir->scratch_frame = 0;

	int index;
	for(index = 0; index < LIR_N_GP_REGISTERS; index++)
	{
		lir_gp_regs_map[index].vreg = 0;
		lir_gp_regs_map[index].local_vreg = 0;
		lir_free_frame(func, lir_gp_regs_map[index].temp_frame);
		lir_gp_regs_map[index].temp_frame = 0;
	}

	for(index = 0; index < LIR_N_XMM_REGISTERS; index++)
	{
		lir_xmm_regs_map[index].vreg = 0;
		lir_xmm_regs_map[index].local_vreg = 0;
		lir_free_frame(func, lir_xmm_regs_map[index].temp_frame);
		lir_xmm_regs_map[index].temp_frame = 0;
	}

	func->lir->relative_sp_offset = 0;
}

void lir_reinit(jit_function_t func)
{
	incoming_params_state_t state = func->lir->incoming_params_state;
	if(func->lir->incoming_params_state == 0)
	{
		state = jit_cnew(struct _incoming_params_state);
		func->lir->incoming_params_state = state;
	}

	state->frame_index = 2;
	state->gp_index = 0;
	state->xmm_index = 0;

	func->lir->regs_state = 0;
	lir_list_t list = func->lir->frame_state;
	while(list)
	{
		list->item1 = 0;
		list->item2 = 0;
		list->item3 = 0;
		list = list->next;
	}
	lir_linked_list_t linked_list = func->lir->vregs_list;
	while(linked_list)
	{
		lir_vreg_t vreg = (lir_vreg_t)linked_list->item;
		lir_free_frame(func, vreg->frame);
		linked_list = linked_list->next;
	}

	func->lir->scratch_regs = 0;
	func->lir->scratch_frame = 0;

	int index;
	for(index = 0; index < LIR_N_GP_REGISTERS; index++)
	{
		lir_gp_regs_map[index].vreg = 0;
		lir_vreg_t local_vreg = lir_gp_regs_map[index].local_vreg;
		if(local_vreg)
		{
			local_vreg->in_frame = 1;
			local_vreg->in_reg = 0;
		}
		lir_gp_regs_map[index].local_vreg = 0;
		lir_free_frame(func, lir_gp_regs_map[index].temp_frame);
		lir_gp_regs_map[index].temp_frame = 0;
	}

	for(index = 0; index < LIR_N_XMM_REGISTERS; index++)
	{
		lir_xmm_regs_map[index].vreg = 0;
		lir_xmm_regs_map[index].local_vreg = 0;
		lir_vreg_t local_vreg = lir_xmm_regs_map[index].local_vreg;
		if(local_vreg)
		{
			local_vreg->in_frame = 1;
			local_vreg->in_reg = 0;
		}
		lir_xmm_regs_map[index].local_vreg = 0;
		lir_free_frame(func, lir_xmm_regs_map[index].temp_frame);
		lir_xmm_regs_map[index].temp_frame = 0;
	}
	
	func->lir->relative_sp_offset = 0;
}

// Check for the X86_EAX, X86_ECX, X86_EDX, etc directly
unsigned char lir_gp_reg_is_free(jit_function_t func, int index) 
{
	return (func->lir->regs_state & (0x1 << gp_reg_map[index])) == 0;
}

unsigned char lir_xmm_reg_is_free(jit_function_t func, int index)
{
	return (func->lir->regs_state & (64 << index)) == 0;
}

unsigned char lir_gp_reg_index_is_free(void *handler, int index) 
{
	return (((jit_function_t)handler)->lir->regs_state & (0x1 << index)) == 0;
}

unsigned char lir_xmm_reg_index_is_free(void *handler, int index)
{
	return (((jit_function_t)handler)->lir->regs_state & (64 << index)) == 0;
}

unsigned char lir_register_is_free(jit_function_t func, int reg, jit_value_t value)
{
	jit_type_t type = jit_value_get_type(value);
	type = jit_type_remove_tags(type);
	int typeKind = jit_type_get_kind(type);
	switch(typeKind)
	{
		CASE_USE_WORD
		{
			return lir_gp_reg_is_free(func, reg);
		}
		break;
		CASE_USE_FLOAT
		{
			return lir_xmm_reg_is_free(func, reg);
		}
		break;
		CASE_USE_LONG
		{
			return (lir_gp_reg_is_free(func, reg) && lir_gp_reg_index_is_free(func, gp_reg_map[reg] + 1));
		}
		break;
	}
	return 0;
}

unsigned char lir_regIndex_is_free(jit_function_t func, int regIndex, jit_value_t value)
{
	jit_type_t type = jit_value_get_type(value);
	type = jit_type_remove_tags(type);
	int typeKind = jit_type_get_kind(type);
	switch(typeKind)
	{
		CASE_USE_WORD
		{
			return lir_gp_reg_index_is_free(func, regIndex);
		}
		break;
		CASE_USE_FLOAT
		{
			return lir_xmm_reg_index_is_free(func, regIndex);
		}
		break;
		CASE_USE_LONG
		{
			return (lir_gp_reg_index_is_free(func, regIndex) && lir_gp_reg_index_is_free(func, regIndex + 1));
		}
		break;
	}
	return 0;
}

int lir_regIndex2reg(jit_function_t func, int regIndex, jit_type_t type)
{
	type = jit_type_remove_tags(type);
	int typeKind = jit_type_get_kind(type);
	switch(typeKind)
	{
		CASE_USE_WORD
		{
			return lir_gp_regs_map[regIndex].reg;
		}
		break;
		CASE_USE_FLOAT
		{
			return X86_REG_XMM0 + lir_xmm_regs_map[regIndex].reg;
		}
		break;
	}
	return -1;
}

unsigned int lir_type_get_size(jit_type_t type)
{
	return ROUND_STACK(jit_type_get_size(type));
}

unsigned char lir_regIndex_CanBeUsedOnInput(jit_function_t func, unsigned int regIndex, jit_value_t value)
{
	jit_type_t signature = jit_function_get_signature(func);
	unsigned int num = lir_type_num_params(signature);
	int count;
	int index = 0;
	jit_abi_t abi = jit_function_get_abi(func);
	if(!lir_regIndex_is_free(func, regIndex, value)) return 0;
	else if((regIndex > 2 || abi != jit_abi_internal) && !lir_vreg_is_in_register_hole(func, value->vreg, regIndex)) return 1;
	else if(regIndex > 2 || abi != jit_abi_internal) return 0;
	for(count = 0; (index <= regIndex) && (count < num); count++)
	{
		jit_type_t type = lir_type_get_param(signature, count);
		type = jit_type_remove_tags(type);
		int typeKind = jit_type_get_kind(type);
		switch(typeKind)
		{
			CASE_USE_WORD
			{
				if(index == regIndex)
				{
					jit_value_t param = lir_value_get_param(func, count);
					// We check if this register holds an input value passed to this function.
					// If so we cannot use it right now.
					if(param->vreg && param->vreg->min_range != param->vreg->max_range)
					{
						return 0;
					}
				}
				index++;
			}
			break;
			CASE_USE_FLOAT
			{
				if(index == regIndex)
				{
					jit_value_t param = lir_value_get_param(func, count);
					if(param->vreg && param->vreg->min_range != param->vreg->max_range)
					{
						return 0;
					}
				}
				index++;
			}
			break;
		}
	}
	if(!lir_vreg_is_in_register_hole(func, value->vreg, regIndex)) return 1;
	return 0;
}

void gen_insn(jit_gencode_t gen, jit_function_t func,
				   jit_block_t block, jit_insn_t insn)
{
	init_local_register_allocation();
	switch(insn->opcode)
	{
		case JIT_OP_NOP:
		{
		}
		break;

		case JIT_OP_INCOMING_FRAME_POSN:
		{
			jit_value_t value = insn->value1;
			jit_type_t type = jit_value_get_type(value);
			int frame_offset = (int)jit_value_get_nint_constant(insn->value2);
			if(value->vreg->in_frame)
			{
				lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool), struct _lir_frame);			
				frame->hash_code = -1;
				frame->frame_offset = (int)frame_offset;
				frame->length = lir_type_get_size(type) / 4;
				value->vreg->frame = frame;
			}
			else if(value->vreg->in_reg)
			{
				func->lir->regs_state = func->lir->regs_state | value->vreg->reg->hash_code;
				func->lir->scratch_regs = func->lir->scratch_regs | value->vreg->reg->hash_code;
				value->vreg->reg->vreg = value->vreg;
				if(!jit_cache_check_for_n(&(gen->posn), 32))
				{
					jit_cache_mark_full(&(gen->posn));
					return;
				}
				unsigned char *inst = (unsigned char *)(gen->posn.ptr);
				inst = masm_mov_reg_membase(inst, value->vreg->reg->reg, X86_EBP, frame_offset, type);
				gen->posn.ptr = (unsigned char *)inst;
			}
		}
		break;

		case JIT_OP_INCOMING_REG:
		{
			jit_value_t value = insn->value1;
			jit_type_t type = jit_value_get_type(value);
			int typeKind = jit_type_get_kind(type);
			int reg = (int)jit_value_get_nint_constant(insn->value2);
			int size = lir_type_get_size(type);
			
			switch(typeKind)
			{
				CASE_USE_FLOAT
				{
					reg -= X86_REG_XMM0;
				}
				break;
			}
			
			if(value->vreg->in_frame)
			{
				lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool), struct _lir_frame);
				lir_allocate_large_frame(func, frame, size);
				value->vreg->frame = frame;
				unsigned char *inst = (unsigned char *)(gen->posn.ptr);
				inst = masm_mov_membase_reg(inst, X86_EBP, value->vreg->frame->frame_offset, reg, type);
				gen->posn.ptr = (unsigned char *)inst;
			}
			else if(value->vreg->in_reg)
			{
				func->lir->regs_state = func->lir->regs_state | value->vreg->reg->hash_code;
				func->lir->scratch_regs = func->lir->scratch_regs | value->vreg->reg->hash_code;
				value->vreg->reg->vreg = value->vreg;
				if(!jit_cache_check_for_n(&(gen->posn), 32))
				{
					jit_cache_mark_full(&(gen->posn));
					return;
				}
				unsigned char *inst = (unsigned char *)(gen->posn.ptr);
				inst = masm_mov_reg_reg(inst, value->vreg->reg->reg, reg, type);
				gen->posn.ptr = (unsigned char *)inst;
			}
		}
		break;

		default:
		{
			unsigned char *inst = (unsigned char *)(gen->posn.ptr);
			if(!jit_cache_check_for_n(&(gen->posn), 256)) // was 32
			{
				jit_cache_mark_full(&(gen->posn));
				return;
			}
			if(insn->cpoint)
			{
				lir_free_frames(func, insn->cpoint->vregs_die);
				if((insn->flags & JIT_INSN_DEST_IS_LABEL) == 0)
				{
					lir_free_registers(func, insn->cpoint->vregs_die);
				}

				inst = lir_allocate_registers_and_frames(inst, func, insn->cpoint->vregs_born->item1);
				inst = lir_allocate_registers_and_frames(inst, func, insn->cpoint->vregs_born->item2);
				inst = lir_allocate_registers_and_frames(inst, func, insn->cpoint->vregs_born->item3);
				inst = lir_allocate_registers_and_frames(inst, func, insn->cpoint->vregs_born->item4);

				gen->posn.ptr = (unsigned char *)inst;
				/* Notify the back end that the block is starting */
			}

			jit_value_t dest = jit_insn_get_dest(insn);
			jit_value_t value1 = jit_insn_get_value1(insn);
			jit_value_t value2 = jit_insn_get_value2(insn);
			// Allocate local registers that are free at this point or restore local registers that will be used
			// for generation of native code in current opcode.
			switch(insn->opcode)
			{
				case JIT_OP_LOAD_RELATIVE_SBYTE:
				case JIT_OP_LOAD_RELATIVE_UBYTE:
				case JIT_OP_LOAD_RELATIVE_SHORT:
				case JIT_OP_LOAD_RELATIVE_USHORT:
				case JIT_OP_LOAD_RELATIVE_INT:
				case JIT_OP_LOAD_RELATIVE_LONG:
				case JIT_OP_LOAD_RELATIVE_FLOAT32:
				case JIT_OP_LOAD_RELATIVE_FLOAT64:
				case JIT_OP_LOAD_RELATIVE_NFLOAT:
				case JIT_OP_LOAD_ELEMENT_SBYTE:
				case JIT_OP_LOAD_ELEMENT_UBYTE:
				case JIT_OP_LOAD_ELEMENT_SHORT:
				case JIT_OP_LOAD_ELEMENT_USHORT:
				case JIT_OP_LOAD_ELEMENT_INT:
				case JIT_OP_LOAD_ELEMENT_LONG:
				case JIT_OP_LOAD_ELEMENT_FLOAT32:
				case JIT_OP_LOAD_ELEMENT_FLOAT64:
				case JIT_OP_LOAD_ELEMENT_NFLOAT:
				{
					inst = lir_allocate_local_register(inst, func, value2->vreg, value1->vreg, value2->vreg, LOCAL_ALLOCATE_FOR_INPUT,  0, 0, 0);
					inst = lir_allocate_local_register(inst, func, value1->vreg, value1->vreg, value2->vreg, LOCAL_ALLOCATE_FOR_INPUT,  0, 0, 0);
					inst = lir_allocate_local_register(inst, func, dest->vreg, value1->vreg, value2->vreg,   LOCAL_ALLOCATE_FOR_OUTPUT, 0, 0, 0);
				}
				break;
				case JIT_OP_LOAD_RELATIVE_STRUCT:
				{
					jit_nuint size = jit_type_get_size(jit_type_normalize(jit_value_get_type(dest)));
					// restore EAX, EDX
					if(value1->vreg && value1->vreg->in_frame) inst = lir_restore_local_registers(inst, func, 0x3);
					else if(!value1->vreg || (value1->vreg && value1->vreg->in_reg && value1->vreg->reg->hash_code != 0x1)) inst = lir_restore_local_registers(inst, func, 0x1);
					else inst = lir_restore_local_registers(inst, func, 0x2);
					inst = lir_restore_local_registers(inst, func, 0x7);

					if(size > (4 * sizeof(void *)))
					{
						// restore ECX
						inst = lir_restore_local_registers(inst, func, 0x4);
					}

					inst = lir_allocate_local_register(inst, func, value1->vreg, 0, 0, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
				}
				break;
				case JIT_OP_STORE_RELATIVE_BYTE:
				case JIT_OP_STORE_RELATIVE_SHORT:
				case JIT_OP_STORE_RELATIVE_INT:
				case JIT_OP_STORE_ELEMENT_BYTE:
				case JIT_OP_STORE_ELEMENT_SHORT:
				case JIT_OP_STORE_ELEMENT_INT:
				case JIT_OP_STORE_ELEMENT_LONG:
				case JIT_OP_STORE_ELEMENT_FLOAT32:
				case JIT_OP_STORE_ELEMENT_FLOAT64:
				case JIT_OP_STORE_ELEMENT_NFLOAT:
				case JIT_OP_MEMCPY:
				{
					inst = lir_allocate_local_register(inst, func, dest->vreg, value1->vreg, value2->vreg, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
					inst = lir_allocate_local_register(inst, func, value1->vreg, value2->vreg, dest->vreg, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
					inst = lir_allocate_local_register(inst, func, value2->vreg, value1->vreg, dest->vreg, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
				}
				break;
				case JIT_OP_STORE_RELATIVE_LONG:
				case JIT_OP_STORE_RELATIVE_FLOAT32:
				case JIT_OP_STORE_RELATIVE_FLOAT64:
				case JIT_OP_STORE_RELATIVE_NFLOAT:
				{
					inst = lir_allocate_local_register(inst, func, dest->vreg, value1->vreg, value2->vreg, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
					inst = lir_allocate_local_register(inst, func, value1->vreg, value2->vreg, dest->vreg, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
				}
				break;
				case JIT_OP_STORE_RELATIVE_STRUCT:
				{
					jit_nuint size = jit_type_get_size(jit_type_normalize(jit_value_get_type(value1)));
					// restore EAX, EDX
					if(dest->vreg && dest->vreg->in_frame) inst = lir_restore_local_registers(inst, func, 0x3);
					else if(!dest->vreg || (dest->vreg && dest->vreg->in_reg && dest->vreg->reg->hash_code != 0x1)) inst = lir_restore_local_registers(inst, func, 0x1);
					else inst = lir_restore_local_registers(inst, func, 0x2);

					if(size > (4 * sizeof(void *)))
					{
						// restore ECX
						inst = lir_restore_local_registers(inst, func, 0x4);
					}
					inst = lir_allocate_local_register(inst, func, dest->vreg, value1->vreg, value2->vreg, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
				}
				break;
				case JIT_OP_COPY_STRUCT:
				{
					jit_nuint size = jit_type_get_size(jit_type_normalize(jit_value_get_type(dest)));
					// restore eax
					inst = lir_restore_local_registers(inst, func, 0x1);
					if(size > (4 * sizeof(void *)))
					{
						// restore edx and ecx
						inst = lir_restore_local_registers(inst, func, 0x6);
					}
				}
				break;
				case JIT_OP_RETURN_SMALL_STRUCT:
				{
					inst = lir_allocate_local_register(inst, func, value1->vreg, 0, 0, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
				}
				break;
				case JIT_OP_ADDRESS_OF:
				{
					inst = lir_allocate_local_register(inst, func, dest->vreg, 0, 0, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
				}
				break;
				case JIT_OP_CALL:
				case JIT_OP_CALL_TAIL:
				case JIT_OP_CALL_INDIRECT:
				case JIT_OP_CALL_INDIRECT_TAIL:
				case JIT_OP_CALL_VTABLE_PTR:
				case JIT_OP_CALL_VTABLE_PTR_TAIL:
				case JIT_OP_CALL_EXTERNAL:
				case JIT_OP_CALL_EXTERNAL_TAIL:
				{
					inst = lir_restore_local_registers(inst, func, 0x3fffc7);
				}
				break;
				case JIT_OP_IDIV:
				case JIT_OP_IDIV_UN:
				case JIT_OP_IREM:
				case JIT_OP_IREM_UN:
				{
					// restore EAX, EDX
					inst = lir_restore_local_registers(inst, func, 0x3);
				}
				break;
				case JIT_OP_LDIV:
				case JIT_OP_LDIV_UN:
				{
					// restore EAX, EDX, ECX, EBX, EDI, ESI
					inst = lir_restore_local_registers(inst, func, 0x3f);
				}
				break;
				case JIT_OP_LMUL:
				{
					// Restore EAX, EDX, ECX and EBX
					inst = lir_restore_local_registers(inst, func, 0x15);
				}
				break;
				case JIT_OP_LSHR:
				case JIT_OP_LSHR_UN:
				case JIT_OP_LSHL:
				{
					// Restore ECX
					inst = lir_restore_local_registers(inst, func, 0x4);
		    		}
				break;
				case JIT_OP_ISHR:
				case JIT_OP_ISHR_UN:
				case JIT_OP_ISHL:
				{
					// Restore ECX
					inst = lir_restore_local_registers(inst, func, 0x4);
				}
				break;
				case JIT_OP_THROW:
				case JIT_OP_RETHROW:
				case JIT_OP_ENTER_FINALLY:
				case JIT_OP_LEAVE_FINALLY:
				case JIT_OP_CALL_FINALLY:
				case JIT_OP_ENTER_FILTER:
				case JIT_OP_LEAVE_FILTER:
				case JIT_OP_CALL_FILTER:
				case JIT_OP_CALL_FILTER_RETURN:
				{
					inst = lir_restore_local_registers(inst, func, 0xffffffff);
				}
				break;
				case JIT_OP_JUMP_TABLE:
				{
					inst = lir_restore_local_registers(inst, func, 0xffffffff);
					// We allocate EAX if the value is in frame, which we will free right after
					// generating code for the 'switch'.
					if(dest->vreg && dest->vreg->in_frame)
					{
						inst = lir_allocate_local_register(inst, func, dest->vreg, 0, 0, LOCAL_ALLOCATE_FOR_INPUT, 0, 0, 0);
					}
				}
				break;
			}

			// if the current opcode is a branch then restore all registers
			if((insn->flags & JIT_INSN_DEST_IS_LABEL) != 0)
			{
				inst = lir_restore_local_registers(inst, func, 0xffffffff);
			}
			jit_type_t sourceType = 0; // type of source value

			// Build current input state
			jit_nint param[3];
			int index = 0;
			unsigned int offset = 1;
			unsigned int state = 0;
			if(dest)
			{
				param[index] = dest->address;
				if(dest->vreg && dest->vreg->in_reg)
				{
					param[index] = dest->vreg->reg->reg;
				}
				else if(dest->vreg && dest->vreg->in_frame)
				{
					param[index] = dest->vreg->frame->frame_offset;
					state += 0x1;
				}
				else if(dest->is_constant)
				{
					state += 0x2;
				}
				else
				{
					state = 0xff;
				}
				index++;
				offset *= 3;
			}
			
			if(value1)
			{
				param[index] = value1->address;
				if(value1->vreg)
				{	
					inst = lir_restore_local_vreg(inst, func, value1->vreg);
				        if(value1->vreg->in_reg)
					{
						param[index] = value1->vreg->reg->reg;
					}
					else if(value1->vreg->in_frame)
					{
						param[index] = value1->vreg->frame->frame_offset;
						state += (0x1 * offset);
					}
				}
				if(value1->is_constant)
				{
					state += (0x2 * offset);
				}
				index++;
				offset *= 3;
				sourceType = jit_value_get_type(value1);
			}

			if(value2)
			{
				param[index] = value2->address;
				if(value2->vreg)
				{
					inst = lir_restore_local_vreg(inst, func, value2->vreg);
					if(value2->vreg->in_reg)
					{
						param[index] = value2->vreg->reg->reg;
					}
					else if(value2->vreg->in_frame)
					{
						param[index] = value2->vreg->frame->frame_offset;
						state += (0x1 * offset);
					}
				}
				if(value2->is_constant)
				{
					state += (0x2 * offset);
				}
				index++;
			}

			unsigned char machine_code1 = lir_opcodes_map[insn->opcode].machine_code1;
			unsigned char machine_code2 = lir_opcodes_map[insn->opcode].machine_code2;
			unsigned char machine_code3 = lir_opcodes_map[insn->opcode].machine_code3;
			unsigned char machine_code4 = lir_opcodes_map[insn->opcode].machine_code4;
			
			switch(insn->opcode)
			{
				#include "jit-i486-extra-arith.h"
				#define JIT_INCLUDE_RULES
				#include "jit-rules-i486-arith.inc"
				#include "jit-rules-i486-conv.inc"
				#include "jit-rules-i486-math.inc"
				#include "jit-rules-i486-obj.inc"
				#include "jit-rules-i486-call.inc"
				#include "jit-rules-i486-branch.inc"
				#include "jit-rules-i486-except.inc"
				#include "jit-rules-i486-logic.inc"
				#undef JIT_INCLUDE_RULES
				default:
				{
					fprintf(stderr, "TODO(%x) at %s, %d\n",
					(int)(insn->opcode), __FILE__, (int)__LINE__);
					exit(1);
				}
				break;
			}
			
			if(insn->opcode == JIT_OP_JUMP_TABLE)
			{
				// Free EAX used for JUMP_TABLE is the value was in local frame.
				inst = lir_restore_local_registers(inst, func, 0x1);
			}
			// LibJIT does not use true SSA form because jit-insn.c may transform a jit_insn_store
			// to a change of the dest value in the previous opcode.
			// If a value is used for output then restore the frame which the value
			// uses locally.
			if(dest && dest->vreg && !jit_insn_dest_is_value(insn))
			{
				inst = lir_restore_local_frame(inst, func, dest->vreg);
			}

			if(insn->cpoint)
			{
				if((insn->flags & JIT_INSN_DEST_IS_LABEL) != 0)
				{
					lir_free_registers(func, insn->cpoint->vregs_die);
				}
			}
			gen->posn.ptr = (unsigned char *)inst;
		}
		break;
	}
}

void lir_compile_block(jit_gencode_t gen, jit_function_t func, jit_block_t block)
{
	jit_insn_iter_t iter;
	jit_insn_t insn;
	jit_insn_iter_init(&iter, block);
	while((insn = jit_insn_iter_next(&iter)) != 0)
	{
		if(!jit_cache_check_for_n(&gen->posn, 64))
		{
			jit_cache_mark_full(&gen->posn);
			return;
		}
		switch(insn->opcode)
		{
			case JIT_OP_MARK_OFFSET:
			{
				/* Mark the current code position as corresponding
				   to a particular bytecode offset */
				_jit_cache_mark_bytecode
					(&(gen->posn), (unsigned long)(long)
							jit_value_get_nint_constant(insn->value1));
			}
			break;

			default:
			{
				/* Generate code for the instruction with the back end */
				gen_insn(gen, func, block, insn);
			}
			break;
		}
	}
}


void *gen_prolog(jit_gencode_t gen, jit_function_t func, void *buf)
{
	unsigned char prolog[JIT_PROLOG_SIZE];
	unsigned char *inst = prolog;
	int reg;

	/* Push ebp onto the stack */
	x86_push_reg(inst, X86_EBP);
	int index;

	/* Initialize EBP for current frame */
	x86_mov_reg_reg(inst, X86_EBP, X86_ESP, sizeof(void*));

	int stack_offset = func->lir->scratch_frame;

	for(index = 3; index < LIR_N_GP_REGISTERS; index++)
	{
		if(func->lir->scratch_regs & lir_gp_regs_map[index].hash_code)
		{
			stack_offset+=4;
			x86_mov_membase_reg(inst, X86_EBP, -stack_offset, lir_gp_regs_map[index].reg, 4);
		}
	}
	if(stack_offset > 0) x86_alu_reg_imm(inst, X86_SUB, X86_ESP, stack_offset);


	/* Copy the prolog into place and return the adjusted entry position */
	reg = (int)(inst - prolog);
	jit_memcpy(((unsigned char *)buf) + JIT_PROLOG_SIZE - reg, prolog, reg);
	return (void *)(((unsigned char *)buf) + JIT_PROLOG_SIZE - reg);
}

unsigned char *restore_callee_saved_registers(unsigned char *inst, jit_function_t func)
{
	int stack_offset = func->lir->scratch_frame;
	int index;

	for(index = 3; index < LIR_N_GP_REGISTERS; index++)
	{
		if(func->lir->scratch_regs & lir_gp_regs_map[index].hash_code)
		{
			stack_offset+=4;
			x86_mov_reg_membase(inst, lir_gp_regs_map[index].reg, X86_EBP, -stack_offset, 4);
		}
	}
	x86_mov_reg_reg(inst, X86_ESP, X86_EBP, sizeof(void *));
	x86_pop_reg(inst, X86_EBP);
	return inst;
}

void gen_epilog(jit_gencode_t gen, jit_function_t func)
{
	unsigned char *inst;
	void **fixup;
	void **next;

	/* Bail out if there is insufficient space for the epilog */
	if(!jit_cache_check_for_n(&(gen->posn), 96)) // was 48
	{
		jit_cache_mark_full(&(gen->posn));
		return;
	}

	/* Perform fixups on any blocks that jump to the epilog */
	inst = gen->posn.ptr;
	fixup = (void **)(gen->epilog_fixup);
	while(fixup != 0)
	{
		next = (void **)(fixup[0]);
		fixup[0] = (void *)(((jit_nint)inst) - ((jit_nint)fixup) - 4);
		fixup = next;
	}
	gen->epilog_fixup = 0;
	int index;
	int stack_offset = func->lir->scratch_frame;
	for(index = 3; index < LIR_N_GP_REGISTERS; index++)
	{
		if(func->lir->scratch_regs & lir_gp_regs_map[index].hash_code)
		{
			stack_offset+=4;
			x86_mov_reg_membase(inst, lir_gp_regs_map[index].reg, X86_EBP, -stack_offset, 4);
		}
	}

        jit_type_t signature = jit_function_get_signature(func);
	jit_abi_t abi = jit_type_get_abi(signature);

	if(func->lir->scratch_frame > 0 || stack_offset > 0 || gen->stack_changed)
	{
		x86_mov_reg_reg(inst, X86_ESP, X86_EBP, sizeof(void *));
	}
	x86_pop_reg(inst, X86_EBP);
	
	int pop_bytes = lir_stack_depth_used(func);
	if(pop_bytes != 0 && abi != jit_abi_cdecl)
	{
		x86_ret_imm(inst, pop_bytes);
	}
	else
	{
		x86_ret(inst);
	}
	gen->posn.ptr = inst;
}

void gen_start_block(jit_gencode_t gen, jit_block_t block)
{
	void **fixup;
	void **next;
	/* Set the address of this block */
	block->address = (void *)(gen->posn.ptr);

	/* If this block has pending fixups, then apply them now */
	fixup = (void **)(block->fixup_list);
	while(fixup != 0)
	{
		next = (void **)(fixup[0]);
		fixup[0] = (void *)
			(((jit_nint)(block->address)) - ((jit_nint)fixup) - 4);
		fixup = next;
	}
	block->fixup_list = 0;

	fixup = (void**)(block->fixup_absolute_list);
	while(fixup != 0)
	{
		next = (void **)(fixup[0]);
		fixup[0] = (void *)((jit_nint)(block->address));
		fixup = next;
	}
	block->fixup_absolute_list = 0;
}

void gen_end_block(jit_gencode_t gen, jit_block_t block)
{
	unsigned char *inst = (unsigned char *)(gen->posn.ptr);
	
	if(!jit_cache_check_for_n(&(gen->posn), 32))
	{
		jit_cache_mark_full(&(gen->posn));
		return;
	}

	inst = lir_restore_local_registers(inst, block->func, 0xffffffff);

	gen->posn.ptr = (unsigned char *)inst;
}

void lir_free_reg(jit_function_t func, lir_vreg_t vreg)
{
	if(vreg)
	{
		jit_type_t type = jit_value_get_type(vreg->value);
		type = jit_type_normalize(type);
		int typeKind = jit_type_get_kind(type);
		if(vreg->in_reg)
		{
				if(vreg->reg->vreg == vreg)
				{
					func->lir->regs_state = func->lir->regs_state & (~(vreg->reg->hash_code));
					lir_free_frame(func, vreg->reg->temp_frame);
					lir_free_frame(func, vreg->frame);
					vreg->reg->vreg = 0;

					if(typeKind == JIT_TYPE_LONG || typeKind == JIT_TYPE_ULONG)
					{
						lir_reg_t reg_pair = lir_object_register_pair(vreg->reg);
						func->lir->regs_state = func->lir->regs_state & (~(reg_pair->hash_code));
						reg_pair->vreg = 0;
					}
				}
				else if(vreg->reg->temp_frame == 0)
				{
					func->lir->regs_state = func->lir->regs_state & (~(vreg->reg->hash_code));
					if(typeKind == JIT_TYPE_LONG || typeKind == JIT_TYPE_ULONG)
					{
						func->lir->regs_state = func->lir->regs_state & (~(lir_object_register_pair(vreg->reg)->hash_code));
					}
				}
		}
	}
}

unsigned char *lir_restore_local_vreg(unsigned char *inst, jit_function_t func, lir_vreg_t vreg)
{
	// Called if a value is used for input.
	if(vreg && vreg->in_reg && vreg->reg)
	{
		// This value is used with the register globally.
		if(vreg->reg->vreg == vreg || (vreg->reg->vreg == 0 && vreg->reg->local_vreg != vreg))
		{
			// if this is the value which is used globally with the register
			if(vreg->reg->local_vreg) // if there is a value used locally with the register then restore it
			{
				vreg->reg->local_vreg->in_reg = 0;
				vreg->reg->local_vreg->in_frame = 1;
				vreg->reg->local_vreg = 0;
			}
			if(vreg->reg->temp_frame) // if the input value was saved to a temporary local frame restore it
			{
				jit_type_t type = jit_value_get_type(vreg->value);
				type = jit_type_remove_tags(type);
				inst = masm_mov_reg_membase(inst, vreg->reg->reg, X86_EBP, vreg->reg->temp_frame->frame_offset, type);
				lir_free_frame(func, vreg->reg->temp_frame);
				vreg->reg->temp_frame = 0;
			}
		}
	}
	return inst;
}

unsigned char *lir_restore_local_frame(unsigned char *inst, jit_function_t func, lir_vreg_t vreg)
{
	// Called if a value is used for output.
	if(vreg && vreg->in_reg && vreg->reg)
	{
		// if the value is used with the register locally then restore the value in frame
		if(vreg == vreg->reg->local_vreg)
		{
			inst = masm_mov_membase_reg(inst, X86_EBP, vreg->frame->frame_offset, vreg->reg->reg, jit_value_get_type(vreg->value));

		}
		else if(vreg == vreg->reg->vreg)
		{
			lir_vreg_t local_vreg = vreg->reg->local_vreg;
			if(local_vreg)
			{
				local_vreg->in_reg = 0;
				local_vreg->in_frame = 1;
				local_vreg->reg = 0;
				vreg->reg->local_vreg = 0;
			
				lir_free_frame(func, vreg->reg->temp_frame);
				vreg->reg->temp_frame = 0;
			}
		}
	}
	return inst;
}


unsigned char *lir_restore_local_registers(unsigned char *inst, jit_function_t func, unsigned int regMask)
{
	unsigned int index;
	for(index = 0; index < LIR_N_GP_REGISTERS; index++)
	{
		lir_vreg_t local_vreg = lir_gp_regs_map[index].local_vreg;
		if(local_vreg && (lir_gp_regs_map[index].hash_code & regMask))
		{
			local_vreg->in_reg = 0;
			local_vreg->in_frame = 1;
			lir_gp_regs_map[index].local_vreg = 0;
			if(lir_gp_regs_map[index].temp_frame == 0)
			{
				func->lir->regs_state = func->lir->regs_state & (~(lir_gp_regs_map[index].hash_code));
			}
			else
			{
				x86_mov_reg_membase(inst, local_vreg->reg->reg, X86_EBP, lir_gp_regs_map[index].temp_frame->frame_offset, 4);
				lir_free_frame(func, lir_gp_regs_map[index].temp_frame);
			}
			local_vreg->reg = 0;
			lir_gp_regs_map[index].temp_frame = 0;
		}
	}

	for(index = 0; index < LIR_N_XMM_REGISTERS; index++)
	{
		lir_vreg_t local_vreg = lir_xmm_regs_map[index].local_vreg;
		if(local_vreg && (lir_xmm_regs_map[index].hash_code & regMask))
		{
			local_vreg->in_reg = 0;
			local_vreg->in_frame = 1;
			lir_xmm_regs_map[index].local_vreg = 0;
			if(lir_xmm_regs_map[index].temp_frame == 0)
			{
				func->lir->regs_state = func->lir->regs_state & (~(lir_xmm_regs_map[index].hash_code));
			}
			else
			{
				jit_type_t type = jit_value_get_type(local_vreg->value);
				type = jit_type_remove_tags(type);
				inst = masm_mov_reg_membase(inst, local_vreg->reg->reg, X86_EBP, lir_xmm_regs_map[index].temp_frame->frame_offset, type);
				lir_free_frame(func, lir_xmm_regs_map[index].temp_frame);
			}
			local_vreg->reg = 0;
			lir_xmm_regs_map[index].temp_frame = 0;
		}
	}
	return inst;
}



unsigned char *ejit_jump_to_epilog
	(jit_gencode_t gen, unsigned char *inst, jit_block_t block)
{
	/* If the epilog is the next thing that we will output,
	   then fall through to the epilog directly */
	block = block->next;
	while(block != 0 && block->first_insn > block->last_insn)
	{
		block = block->next;
	}
	if(!block)
	{
		return inst;
	}

	/* Output a placeholder for the jump and add it to the fixup list */
	*inst++ = (unsigned char)0xE9;
	x86_imm_emit32(inst, (int)(gen->epilog_fixup));
	gen->epilog_fixup = (void *)(inst - 4);
	return inst;
}

/*
 * Throw a builtin exception.
 */
unsigned char *ejit_throw_builtin
		(unsigned char *inst, jit_function_t func, int type)
{
	/* We need to update "catch_pc" if we have a "try" block */
	if(func->builder->setjmp_value != 0)
	{
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

	/* Push the exception type onto the stack */
	x86_push_imm(inst, type);

	/* Call the "jit_exception_builtin" function, which will never return */
	x86_call_code(inst, jit_exception_builtin);
	return inst;
}


void lir_preallocate_global_registers(jit_function_t func)
{
	jit_block_t block = 0;
	jit_insn_iter_t iter;
	jit_insn_t insn;
	while((block = jit_block_next(func, block)) != 0)
	{
		if(!(block->entered_via_top) && !(block->entered_via_branch))
		{
			continue;
		}
		jit_insn_iter_init(&iter, block);
		while((insn = jit_insn_iter_next(&iter)) != 0)
		{
			switch(insn->opcode)
			{
				case JIT_OP_NOP:
				{
				}
				break;

				case JIT_OP_INCOMING_REG:
				{
				jit_value_t value = insn->value1;

				jit_type_t type = jit_value_get_type(value);
				type = jit_type_remove_tags(type);
				int typeKind = jit_type_get_kind(type);
				switch(typeKind)
				{
					CASE_USE_WORD
					{
						int regIndex = gp_reg_map[(int)jit_value_get_nint_constant(insn->value2)];
						if(value->vreg->min_range != value->vreg->max_range)
						{
							int indexFound = -1;
							
							// If the value is used less than 2 times (created once and used once)
							// then there is no need to allocate a register for it.
							if(!func->has_try && !jit_value_is_addressable(value))
							{
								if(!lir_vreg_is_in_register_hole(func, value->vreg, regIndex) && lir_regIndex_is_free(func, regIndex, value))
								{
								    indexFound = regIndex;
								}
								else
								{
									int index = 0;
									for(index = 0; index < LIR_N_GP_REGISTERS; index++)
									{
										if(!lir_vreg_is_in_register_hole(func, value->vreg, index) && lir_regIndex_is_free(func, index, value))
										{
											indexFound = index;
											break;
										}
									}
								}

								if(indexFound != -1)
								{
									func->lir->regs_state = func->lir->regs_state | lir_gp_regs_map[indexFound].hash_code;
									func->lir->scratch_regs = func->lir->scratch_regs | lir_gp_regs_map[indexFound].hash_code;
				
									value->vreg->in_reg = 1;
									value->vreg->in_frame = 0;
		    							value->vreg->reg = (lir_reg_t)(&(lir_gp_regs_map[indexFound]));
									value->vreg->reg->vreg = value->vreg;
								}
	    						}

							if(indexFound == - 1)
							{
								value->vreg->in_frame = 1;
								value->vreg->in_reg = 0;
							}
						}
					}
					break;
					case JIT_TYPE_FLOAT32:
					case JIT_TYPE_FLOAT64:
					{
						int regIndex = (int)jit_value_get_nint_constant(insn->value2) - X86_REG_XMM0;
						if(value->vreg->min_range != value->vreg->max_range)
						{
							int indexFound = -1;
							if(!func->has_try && !jit_value_is_addressable(value))
							{
								if(!lir_vreg_is_in_register_hole(func, value->vreg, regIndex) && lir_regIndex_is_free(func, regIndex, value))
								{
									indexFound = regIndex;
								}
								else
								{
									int index;
									for(index = 0; index < LIR_N_XMM_REGISTERS; index++)
									{
										if(!lir_vreg_is_in_register_hole(func, value->vreg, index) && lir_regIndex_is_free(func, index, value))
										{
											indexFound = index;
											break;
										}
									}
								}

								if(indexFound != -1)
								{
									func->lir->regs_state = func->lir->regs_state | lir_xmm_regs_map[indexFound].hash_code;
									func->lir->scratch_regs = func->lir->scratch_regs | lir_xmm_regs_map[indexFound].hash_code;
									value->vreg->in_reg = 1;
									value->vreg->in_frame = 0;
				    					value->vreg->reg = (lir_reg_t)(&(lir_xmm_regs_map[indexFound]));
									value->vreg->reg->vreg = value->vreg;
								}
							}

							if(indexFound == - 1)
							{
								value->vreg->in_frame = 1;
								value->vreg->in_reg = 0;
							}
						}
					}
					break;
					case JIT_TYPE_NFLOAT:
					{
						if(sizeof(jit_nfloat) != sizeof(jit_float64))
						{
							if(value->vreg->min_range != value->vreg->max_range)
							{
								value->vreg->in_frame = 1;
								value->vreg->in_reg = 0;
							}
						}
						else
						{
							int regIndex = (int)jit_value_get_nint_constant(insn->value2) - X86_REG_XMM0;
							if(value->vreg->min_range != value->vreg->max_range)
							{
								int indexFound = -1;
								if(!func->has_try && !jit_value_is_addressable(value))
								{
									if(!lir_vreg_is_in_register_hole(func, value->vreg, regIndex) && lir_regIndex_is_free(func, regIndex, value))
									{
										indexFound = regIndex;
									}
									else
									{
										int index;
										for(index = 0; index < LIR_N_XMM_REGISTERS; index++)
										{
											if(!lir_vreg_is_in_register_hole(func, value->vreg, regIndex) && lir_regIndex_is_free(func, regIndex, value))
											{
												indexFound = index;
												break;
											}
										}
									}

									if(indexFound != -1)
									{
										func->lir->regs_state = func->lir->regs_state | lir_xmm_regs_map[indexFound].hash_code;
										func->lir->scratch_regs = func->lir->scratch_regs | lir_xmm_regs_map[indexFound].hash_code;
										value->vreg->in_reg = 1;
										value->vreg->in_frame = 0;
					    					value->vreg->reg = (lir_reg_t)(&(lir_xmm_regs_map[indexFound]));
										value->vreg->reg->vreg = value->vreg;
									}
								}

								if(indexFound == - 1)
								{
									value->vreg->in_frame = 1;
									value->vreg->in_reg = 0;
								}
							}
						}
					}
					break;

					CASE_USE_LONG
					{
						if(value->vreg->min_range != value->vreg->max_range)
						{
	    						value->vreg->in_frame = 1;
							value->vreg->in_reg = 0;
						}
					}
					break;

					default: // Anything else is a structure or a union.
					{
						if(value->vreg->min_range != value->vreg->max_range)
						{
							value->vreg->in_frame = 1;
							value->vreg->in_reg = 0;
						}
					}
					break;
				}
				}
				break;

				case JIT_OP_INCOMING_FRAME_POSN:
				{
				jit_value_t value = insn->value1;
				jit_type_t type = jit_value_get_type(value);
				type = jit_type_remove_tags(type);
				int typeKind = jit_type_get_kind(type);

				switch(typeKind)
				{
					CASE_USE_WORD
					{
						if(value->vreg->min_range != value->vreg->max_range)
						{
							int indexFound = -1;
							
							// If the value is used less than 2 times (created once and used once)
							// then there is no need to allocate a register for it.
							if(!func->has_try && !jit_value_is_addressable(value) && value->usage_count > 3 )
							{
								int index;
								for(index = 0; index < LIR_N_GP_REGISTERS; index++)
								{
									if(!lir_vreg_is_in_register_hole(func, value->vreg, index) && lir_regIndex_is_free(func, index, value))
									{
										indexFound = index;
										break;
									}
								}

								if(indexFound != -1)
								{
									func->lir->regs_state = func->lir->regs_state | lir_gp_regs_map[indexFound].hash_code;
									func->lir->scratch_regs = func->lir->scratch_regs | lir_gp_regs_map[indexFound].hash_code;
				
									value->vreg->in_reg = 1;
									value->vreg->in_frame = 0;
		    							value->vreg->reg = (lir_reg_t)(&(lir_gp_regs_map[indexFound]));
									value->vreg->reg->vreg = value->vreg;
								}
	    						}

							if(indexFound == - 1)
							{
								value->vreg->in_frame = 1;
								value->vreg->in_reg = 0;
							}
						}
					}
					break;
					case JIT_TYPE_FLOAT32:
					case JIT_TYPE_FLOAT64:
					{
						if(value->vreg->min_range != value->vreg->max_range)
						{
							int indexFound = -1;
							if(!func->has_try && !jit_value_is_addressable(value) && value->usage_count > 3 )
							{
								int index;
								for(index = 0; index < LIR_N_XMM_REGISTERS; index++)
								{
									if(!lir_vreg_is_in_register_hole(func, value->vreg, index) && lir_regIndex_is_free(func, index, value))
									{
										indexFound = index;
										break;
									}
								}

								if(indexFound != -1)
								{
									func->lir->regs_state = func->lir->regs_state | lir_xmm_regs_map[indexFound].hash_code;
									func->lir->scratch_regs = func->lir->scratch_regs | lir_xmm_regs_map[indexFound].hash_code;
									value->vreg->in_reg = 1;
									value->vreg->in_frame = 0;
				    					value->vreg->reg = (lir_reg_t)(&(lir_xmm_regs_map[indexFound]));
									value->vreg->reg->vreg = value->vreg;
								}
							}

							if(indexFound == - 1)
							{
								value->vreg->in_frame = 1;
								value->vreg->in_reg = 0;
							}
						}
					}
					break;
					case JIT_TYPE_NFLOAT:
					{
						if(sizeof(jit_nfloat) != sizeof(jit_float64))
						{
							if(value->vreg->min_range != value->vreg->max_range)
							{
								value->vreg->in_frame = 1;
								value->vreg->in_reg = 0;
							}
						}
						else
						{
							if(value->vreg->min_range != value->vreg->max_range && value->usage_count > 3 )
							{
								int indexFound = -1;
								if(!func->has_try && !jit_value_is_addressable(value))
								{
									int index;
									for(index = 0; index < LIR_N_XMM_REGISTERS; index++)
									{
										if(!lir_vreg_is_in_register_hole(func, value->vreg, index) && lir_regIndex_is_free(func, index, value))
										{
											indexFound = index;
											break;
										}
									}

									if(indexFound != -1)
									{
										func->lir->regs_state = func->lir->regs_state | lir_xmm_regs_map[indexFound].hash_code;
										func->lir->scratch_regs = func->lir->scratch_regs | lir_xmm_regs_map[indexFound].hash_code;
										value->vreg->in_reg = 1;
										value->vreg->in_frame = 0;
					    					value->vreg->reg = (lir_reg_t)(&(lir_xmm_regs_map[indexFound]));
										value->vreg->reg->vreg = value->vreg;
									}
								}

								if(indexFound == -1)
								{
									value->vreg->in_frame = 1;
									value->vreg->in_reg = 0;
								}
							}
						}
					}
					break;

					CASE_USE_LONG
					{
						if(value->vreg->min_range != value->vreg->max_range)
						{
	    						value->vreg->in_frame = 1;
							value->vreg->in_reg = 0;
						}
					}
					break;

					default: // Anything else is a structure or a union.
					{
						if(value->vreg->min_range != value->vreg->max_range)
						{
							value->vreg->in_frame = 1;
							value->vreg->in_reg = 0;
						}
					}
					break;
				}
				}
				break;

				default:
				{
				if(insn->cpoint)
				{
					if((insn->flags & JIT_INSN_DEST_IS_LABEL) == 0)
					{
						lir_free_registers(func, insn->cpoint->vregs_die);
						lir_preallocate_registers_and_frames(func, insn->cpoint->vregs_born);
					}
					else
					{
						lir_preallocate_registers_and_frames(func, insn->cpoint->vregs_born);
					}
				}
				if(insn->cpoint)
				{
					if((insn->flags & JIT_INSN_DEST_IS_LABEL) != 0)
					{
						lir_free_registers(func, insn->cpoint->vregs_die);
					}
				}
				}
				break;
			}
    		}
	}
}


void lir_preallocate_registers_and_frames(jit_function_t func, lir_list_t list)
{
	int max = (LIR_N_GP_REGISTERS > LIR_N_XMM_REGISTERS ? LIR_N_GP_REGISTERS: LIR_N_XMM_REGISTERS);
	char reg_weight[max];
	memset(reg_weight, 0, sizeof(char) * max);
	lir_vreg_t vregs[max];
	memset(vregs, 0, sizeof(lir_vreg_t) * max);

	// Group of virtual registers that use general purpose registers.
	lir_linked_list_t dword_list = list->item1;

	// Group of virtual registers that use 64-bit values.
	lir_linked_list_t qword_list = list->item2;

	// Group of virtual registers that use XMM registers.
	lir_linked_list_t float_list = list->item3;

	// Group of virtual registers that use frame and local memory.
	lir_linked_list_t large_frames_list = list->item4;

	unsigned int need_n_gp = lir_count_items(func, dword_list);
	unsigned int need_n_xmm = lir_count_items(func, float_list);
	unsigned int need_n_long = lir_count_items(func, qword_list);
	unsigned int need_n_large_frames = lir_count_items(func, large_frames_list);
	lir_vreg_t vreg;
	int weight;
	int index;

	// Allocates registers for the parallel group of virtual registers.

	for(index = 0; func && func->lir && list
			    && (index < LIR_N_GP_REGISTERS) && dword_list && (need_n_gp > 0); index++)
	{
		if(lir_gp_reg_index_is_free(func, index))
		{
			int use_reg = 0;
			lir_vreg_t vreg;
			if(func && list && func->lir)
        		{
				lir_linked_list_t temp = dword_list;
				while(temp)
				{
					vreg = (lir_vreg_t)(temp->item);
					weight = lir_vreg_weight(func, vreg);

					if(!lir_vreg_is_in_register_hole(func, vreg, index)
					    && (reg_weight[index] == 0 || weight < reg_weight[index])
					    && weight != 0 && vreg->in_reg==0 && !jit_value_is_addressable(vreg->value)
					    && (!func->has_try || jit_value_is_temporary(vreg->value)))
					{
						reg_weight[index] = weight;
						vregs[index] = vreg;
						use_reg = 1;
					}
					temp = temp->next;
				}
				if(use_reg == 1)
				{
				     need_n_gp--;
				     vregs[index]->in_reg = 1;
				     vregs[index]->in_frame = 0;
				     vregs[index]->reg = (lir_reg_t)(&(lir_gp_regs_map[index]));
				     vregs[index]->reg->vreg = vregs[index];

				     func->lir->scratch_regs = func->lir->scratch_regs | (&lir_gp_regs_map[index])->hash_code;
				     func->lir->regs_state = func->lir->regs_state | (&lir_gp_regs_map[index])->hash_code;
				}
       			 }
		}
	}


	for(index = 0; func && func->lir && list
			    && (index < LIR_N_GP_REGISTERS) && dword_list && (need_n_gp > 0); index++)
	{
		int found_index = -1;
		unsigned int count;
		for(count = 0; count < LIR_N_GP_REGISTERS; count++)
		{
			// All free register were used, or cannot be used.
			// The register is not free, and was not just allocated.
			if(reg_weight[count] == 0 && !lir_gp_reg_index_is_free(func, count))
			{
				if(found_index == -1
				    || ((found_index != -1) && lir_vreg_weight(func, lir_gp_regs_map[count].vreg)
					    > lir_vreg_weight(func, lir_gp_regs_map[found_index].vreg)))
				{
					found_index = count;
				}
			}
		}
		if(found_index == -1) break;

		int use_reg = 0;
		lir_vreg_t vreg;
		if(func && list && func->lir)
        	{
			lir_linked_list_t temp = dword_list;
			while(temp)
			{
				vreg = (lir_vreg_t)(temp->item);
				weight = lir_vreg_weight(func, vreg);

				if(lir_vreg_weight(func, lir_gp_regs_map[found_index].vreg) > weight
					&& !lir_vreg_is_in_register_hole(func, vreg, found_index)
					&& (reg_weight[found_index] == 0 || weight < reg_weight[found_index])
					&& weight != 0 && vreg->in_reg==0 && !jit_value_is_addressable(vreg->value)
					&& (!func->has_try || jit_value_is_temporary(vreg->value)))
				{
					reg_weight[found_index] = weight;
					vregs[found_index] = vreg;
					use_reg = 1;
				}
				temp = temp->next;
			}
			if(use_reg == 1)
			{
			     need_n_gp--;
			     vregs[found_index]->in_reg = 1;
			     vregs[found_index]->in_frame = 0;
			     vregs[found_index]->reg = (lir_reg_t)(&(lir_gp_regs_map[found_index]));

			     lir_gp_regs_map[found_index].vreg->in_reg = 0;
			     lir_gp_regs_map[found_index].vreg->in_frame = 1;

			     vregs[found_index]->reg->vreg = vregs[found_index];
			}
       		 }
	}

	// Allocate a memory frame.
	if((need_n_gp > 0) && func && list && func->lir && dword_list)
        {
		lir_linked_list_t temp = dword_list;
		while(temp)
		{
			vreg = (lir_vreg_t)(temp->item);
			if(vreg->in_reg == 0)
			{
				vreg->in_frame = 1;
			}
			temp = temp->next;
		}
       	}

	// Allocates registers for the parallel group of virtual registers.

	for(index = 0; func && func->lir && list
			    && (index < LIR_N_GP_REGISTERS - 1) && qword_list && (need_n_long > 0); index++)
	{
		int index_pair = lir_index_register_pair(index);
		if(lir_gp_reg_index_is_free(func, index) && lir_gp_reg_index_is_free(func, index_pair))
		{
			int use_reg = 0;
			lir_vreg_t vreg;
			if(func && list && func->lir)
        		{
				lir_linked_list_t temp = qword_list;
				while(temp)
				{
					vreg = (lir_vreg_t)(temp->item);
					weight = lir_vreg_weight(func, vreg);

					if(!lir_vreg_is_in_register_hole(func, vreg, index)
					    && (reg_weight[index] == 0 || weight < reg_weight[index])
					    && (reg_weight[index_pair] == 0 || weight < reg_weight[index_pair])
					    && weight != 0 && vreg->in_reg==0 && !jit_value_is_addressable(vreg->value)
					    && (!func->has_try || jit_value_is_temporary(vreg->value)))
					{
						reg_weight[index] = weight;
						reg_weight[index_pair] = weight;
						vregs[index] = vreg;
						use_reg = 1;
					}
					temp = temp->next;
				}
				if(use_reg == 1)
				{
				     need_n_long--;
				     vregs[index]->in_reg = 1;
				     vregs[index]->in_frame = 0;
				     vregs[index]->reg = (lir_reg_t)(&(lir_gp_regs_map[index]));
				     lir_gp_regs_map[index].vreg = vregs[index];
				     lir_gp_regs_map[index_pair].vreg = vregs[index];

				     func->lir->scratch_regs = func->lir->scratch_regs | (&lir_gp_regs_map[index])->hash_code;
				     func->lir->regs_state = func->lir->regs_state | (&lir_gp_regs_map[index])->hash_code;
				     func->lir->scratch_regs = func->lir->scratch_regs | (&lir_gp_regs_map[index_pair])->hash_code;
				     func->lir->regs_state = func->lir->regs_state | (&lir_gp_regs_map[index_pair])->hash_code;
				}
       			 }
		}
	}

	for(index = 0; func && func->lir && list
			    && (index < LIR_N_GP_REGISTERS - 1) && qword_list && (need_n_long > 0); index++)
	{
		int found_index = -1, found_index_pair = -1;
		unsigned int count;
		for(count = 0; count < LIR_N_GP_REGISTERS - 1; count++)
		{
			// All free register were used, or cannot be used.
			// The register is not free, and was not just allocated.
			int count_pair = lir_index_register_pair(count);
			if(reg_weight[count] == 0 && !lir_gp_reg_index_is_free(func, count)
				&& reg_weight[count_pair] == 0 && !lir_gp_reg_index_is_free(func, count_pair))

			{
				if(found_index == -1 || ((found_index != -1)
					&& lir_vreg_weight(func, lir_gp_regs_map[count].vreg) > lir_vreg_weight(func, lir_gp_regs_map[found_index].vreg)
					&& lir_vreg_weight(func, lir_gp_regs_map[count_pair].vreg) > lir_vreg_weight(func, lir_gp_regs_map[count_pair].vreg)))
				{
					found_index = count;
					found_index_pair = lir_index_register_pair(found_index);
				}
			}
		}
		if(found_index == -1) break;

		int use_reg = 0;
		lir_vreg_t vreg;
		if(func && list && func->lir)
        	{
			lir_linked_list_t temp = qword_list;
			while(temp)
			{
				vreg = (lir_vreg_t)(temp->item);
				weight = lir_vreg_weight(func, vreg);
				if(lir_vreg_weight(func, lir_gp_regs_map[found_index].vreg) > weight
					&& !lir_vreg_is_in_register_hole(func, vreg, found_index)
					&& (reg_weight[found_index] == 0 || weight < reg_weight[found_index])
					&& (reg_weight[found_index_pair] == 0 || weight < reg_weight[found_index_pair])
					&& weight != 0 && vreg->in_reg==0 && !jit_value_is_addressable(vreg->value)
					&& (!func->has_try || jit_value_is_temporary(vreg->value)))
				{
					reg_weight[found_index] = weight;
					reg_weight[found_index_pair] = weight;
					vregs[found_index] = vreg;
					use_reg = 1;
				}
				temp = temp->next;
			}
			if(use_reg == 1)
			{
			     need_n_long--;
			     vregs[found_index]->in_reg = 1;
			     vregs[found_index]->in_frame = 0;
			     vregs[found_index]->reg = (lir_reg_t)(&(lir_gp_regs_map[found_index]));

			     lir_gp_regs_map[found_index].vreg->in_reg = 0;
			     lir_gp_regs_map[found_index].vreg->in_frame = 1;
			     lir_gp_regs_map[found_index_pair].vreg->in_reg = 0;
			     lir_gp_regs_map[found_index_pair].vreg->in_frame = 1;

			     lir_gp_regs_map[found_index].vreg = vregs[found_index];
			     lir_gp_regs_map[found_index_pair].vreg = vregs[found_index];
			}
       		 }
	}

	// Allocate a memory frame.
	if((need_n_long > 0) && func && list && func->lir && qword_list)
        {
		lir_linked_list_t temp = qword_list;
		while(temp)
		{
			vreg = (lir_vreg_t)(temp->item);
			if(vreg->in_reg == 0)
			{
				vreg->in_frame = 1;
			}
			temp = temp->next;
		}
       	}

	memset(reg_weight, 0, sizeof(char) * max);
	for(index = 0; func && func->lir && list
			    && (index < LIR_N_XMM_REGISTERS) && float_list && (need_n_xmm > 0); index++)
	{
		if(lir_xmm_reg_index_is_free(func, index))
		{
			int use_reg = 0;
			lir_vreg_t vreg;
			if(func && list && func->lir)
        		{
				lir_linked_list_t temp = float_list;
				while(temp)
				{
					vreg = (lir_vreg_t)(temp->item);
					weight = lir_vreg_weight(func, vreg);
					if(!lir_vreg_is_in_register_hole(func, vreg, index)
						&& (reg_weight[index] == 0 || weight < reg_weight[index])
						&& weight != 0 && vreg->in_reg==0 && !jit_value_is_addressable(vreg->value)
						&& (!func->has_try || jit_value_is_temporary(vreg->value)))
					{
						reg_weight[index] = weight;
						vregs[index] = vreg;
						use_reg = 1;
					}
					temp = temp->next;
				}
				if(use_reg == 1)
				{
				     need_n_xmm--;
				     vregs[index]->in_reg = 1;
				     vregs[index]->in_frame = 0;
				     vregs[index]->reg = (lir_reg_t)(&(lir_xmm_regs_map[index]));
				     vregs[index]->reg->vreg = vregs[index];

				     func->lir->scratch_regs = func->lir->scratch_regs | (&lir_xmm_regs_map[index])->hash_code;
				     func->lir->regs_state = func->lir->regs_state | (&lir_xmm_regs_map[index])->hash_code;
				}
       			 }
		}
	}


	for(index = 0; func && func->lir && list
			    && (index < LIR_N_XMM_REGISTERS) && float_list && (need_n_xmm > 0); index++)
	{
		int found_index = -1;
		unsigned int count;
		for(count = 0; count < LIR_N_XMM_REGISTERS; count++)
		{
			// All free register were used, or cannot be used.
			// The register is not free, and was not just allocated.
			if(reg_weight[count] == 0 && !lir_xmm_reg_index_is_free(func, count))
			{
				if(found_index == -1
				    || ((found_index != -1) && lir_vreg_weight(func, lir_xmm_regs_map[count].vreg)
					    > lir_vreg_weight(func, lir_xmm_regs_map[found_index].vreg)))
				{
					found_index = count;
				}
			}
		}
		if(found_index == -1) break;

		int use_reg = 0;
		lir_vreg_t vreg;
		if(func && list && func->lir)
        	{
			lir_linked_list_t temp = float_list;
			while(temp)
			{
				vreg = (lir_vreg_t)(temp->item);
				weight = lir_vreg_weight(func, vreg);

				if(lir_vreg_weight(func, lir_xmm_regs_map[found_index].vreg) > weight
					&& !lir_vreg_is_in_register_hole(func, vreg, found_index)
					&& (reg_weight[found_index] == 0 || weight < reg_weight[found_index])
					&& weight != 0 && vreg->in_reg==0 && !jit_value_is_addressable(vreg->value)
					&& (!func->has_try || jit_value_is_temporary(vreg->value)))
				{
					reg_weight[found_index] = weight;
					vregs[found_index] = vreg;
					use_reg = 1;
				}
				temp = temp->next;
			}
			if(use_reg == 1)
			{
			     need_n_xmm--;
			     vregs[found_index]->in_reg = 1;
			     vregs[found_index]->in_frame = 0;
			     vregs[found_index]->reg = (lir_reg_t)(&(lir_xmm_regs_map[found_index]));

			     lir_xmm_regs_map[found_index].vreg->in_reg = 0;
			     lir_xmm_regs_map[found_index].vreg->in_frame = 1;
			     vregs[found_index]->reg->vreg = vregs[found_index];

			}
       		 }
	}

	// Allocate a memory frame.
	if((need_n_xmm > 0) && func && list && func->lir && float_list)
        {
		lir_linked_list_t temp = float_list;
		while(temp)
		{
			vreg = (lir_vreg_t)(temp->item);
			if(vreg->in_reg == 0)
			{
				vreg->in_frame = 1;
			}
			temp = temp->next;
		}
       	}

	// Allocate large memory frames.
	if(need_n_large_frames > 0 && func && list && func->lir && large_frames_list)
        {
		lir_linked_list_t temp = large_frames_list;
		while(temp)
		{
			vreg = (lir_vreg_t)(temp->item);
			if(vreg->in_reg==0)
			{
				vreg->in_frame = 1;
			}
			temp = temp->next;
		}
       	}
}


unsigned char *lir_allocate_registers_and_frames(unsigned char *inst, jit_function_t func, lir_linked_list_t list)
{
	while(list)
	{
		lir_vreg_t vreg = (lir_vreg_t)list->item;
		if(vreg)
		{
			jit_type_t type = jit_value_get_type(vreg->value);
			type = jit_type_remove_tags(type);
			int typeKind = jit_type_get_kind(type);
			switch(typeKind)
			{
				CASE_USE_WORD
				{
					if(vreg->in_frame)
					{
						lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool), 
											    struct _lir_frame);
						lir_allocate_frame(func, frame);
						vreg->frame = frame;
					}
					else if(vreg->in_reg)
					{
						lir_gp_regs_map[vreg->reg->index].vreg = vreg;
						func->lir->regs_state = func->lir->regs_state | vreg->reg->hash_code;
						func->lir->scratch_regs = func->lir->scratch_regs | vreg->reg->hash_code;
					}
				}
				break;
				CASE_USE_LONG
				{
					if(vreg->in_frame)
					{
						lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool), 
											    struct _lir_frame);
						lir_allocate_large_frame(func, frame, 8);
						vreg->frame = frame;
					}
					else if(vreg->in_reg)
					{
						int reg_pair_index = lir_index_register_pair(vreg->reg->index);
						lir_gp_regs_map[vreg->reg->index].vreg = vreg;
						lir_gp_regs_map[reg_pair_index].vreg = vreg;
						func->lir->regs_state = func->lir->regs_state | vreg->reg->hash_code;
						func->lir->scratch_regs = func->lir->scratch_regs | vreg->reg->hash_code;
						func->lir->regs_state = func->lir->regs_state | (&lir_gp_regs_map[reg_pair_index])->hash_code;
						func->lir->scratch_regs = func->lir->scratch_regs | (&lir_gp_regs_map[reg_pair_index])->hash_code;
					}
				}
				break;
				case JIT_TYPE_FLOAT32:
				case JIT_TYPE_FLOAT64:
				{
					if(vreg->in_frame)
					{
						lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool), 
											    struct _lir_frame);
						lir_allocate_large_frame(func, frame, 8);
						vreg->frame = frame;
					}
					else if(vreg->in_reg)
					{
						lir_xmm_regs_map[vreg->reg->index].vreg = vreg;
						func->lir->regs_state = func->lir->regs_state | vreg->reg->hash_code;
						func->lir->scratch_regs = func->lir->scratch_regs | vreg->reg->hash_code;				
					}
				}
				break;
				case JIT_TYPE_NFLOAT:
				{
					if(sizeof(jit_nfloat) != sizeof(jit_float64))
					{
						if(vreg->in_frame)
						{
							lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool), 
												    struct _lir_frame);
							lir_allocate_large_frame(func, frame, 12);
							vreg->frame = frame;
						}
					}
					else
					{
						if(vreg->in_frame)
						{
							lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool), 
												    struct _lir_frame);
							lir_allocate_large_frame(func, frame, 8);
							vreg->frame = frame;
						}
						else if(vreg->in_reg)
						{
							lir_vreg_t local_vreg = lir_xmm_regs_map[vreg->reg->index].local_vreg;
							if(local_vreg)
							{
								local_vreg->in_reg = 0;
								local_vreg->in_frame = 0;
								local_vreg->reg = 0;
								lir_xmm_regs_map[vreg->reg->index].local_vreg = 0;
							}
							lir_xmm_regs_map[vreg->reg->index].vreg = vreg;
							func->lir->regs_state = func->lir->regs_state | vreg->reg->hash_code;
							func->lir->scratch_regs = func->lir->scratch_regs | vreg->reg->hash_code;				
						}
					}
				}
				break;
				default:
				{
					lir_frame_t frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool),
											    struct _lir_frame);
					lir_allocate_large_frame(func, frame, jit_type_get_size(jit_value_get_type(vreg->value)));
					vreg->frame = frame;
				}
				break;
			}
		}
		list = list->next;
	}
	return inst;
}


void lir_allocate_large_frame_cond6(jit_function_t func, lir_frame_t frame, int size, lir_frame_t frame1, lir_frame_t frame2, lir_frame_t frame3, lir_frame_t frame4, lir_frame_t frame5, lir_frame_t frame6)
{
	int limit1_min = -1, limit2_min = -1, limit3_min = -1, limit4_min = -1, limit5_min = -1, limit6_min = -1;
	int limit1_max = -1, limit2_max = -1, limit3_max = -1, limit4_max = -1, limit5_max = -1, limit6_max = -1;

	if(frame1)
	{
		limit1_min = frame1->hash_code;
		limit1_max = limit1_min + frame1->length - 1;
	}

	if(frame2)
	{
		limit2_min = frame2->hash_code;
		limit2_max = limit2_min + frame2->length - 1;
	}

	if(frame3)
	{
		limit3_min = frame3->hash_code;
		limit3_max = limit3_min + frame3->length - 1;
	}

	if(frame4)
	{
		limit4_min = frame4->hash_code;
		limit4_max = limit4_min + frame4->length - 1;
	}
	
	if(frame5)
	{
		limit5_min = frame5->hash_code;
		limit5_max = limit5_min + frame5->length - 1;
	}

	if(frame6)
	{
		limit6_min = frame6->hash_code;
		limit6_max = limit6_min + frame6->length - 1;
	}

	lir_list_t temp = func->lir->frame_state;
	int segment, offset;
	unsigned int index = 0;
	int flag = 0;
	size = (size + 3) / 4;
	void *state_begin = temp;
	int segment_begin = 0;
	int offset_begin = 0;
	void *state_end = temp;
	int segment_end = 0;
	int offset_end = 0;

	while(temp && flag < size)
	{
		for(segment = 0; segment < 3 && flag < size; segment++)
		{
			for(offset = 0; offset < 32 && flag < size; offset++)
			{
				if(((index < limit1_min) || (index > limit1_max))
					&& ((index < limit2_min) || (index > limit2_max))
					&& ((index < limit3_min) || (index > limit3_max))
					&& ((index < limit4_min) || (index > limit4_max))
					&& ((index < limit5_min) || (index > limit5_max))
					&& ((index < limit6_min) || (index > limit6_max)))
				{
				switch(segment)
				{
					case 0:
					{
						if(((unsigned int)(temp->item1) & (1 << offset))==0)
						{
							// allocate memory aligned to 8 bytes
							if(flag == 0 && ((index & ~0x1) == index))
							{
								state_begin = temp;
								segment_begin = 0;
								offset_begin = offset;
								frame->hash_code = index;

								state_end = temp;
								segment_end = 0;
								offset_end = offset;
								flag = 1;
							}
							else if(flag)
							{
								state_end = temp;
								segment_end = 0;
								offset_end = offset;
								flag++;
							}
						}
						else
						{
							flag = 0;
						}
					}
					break;
					case 1:
					{
						if(((unsigned int)(temp->item2) & (1 << offset))==0)
						{
							if(flag == 0 && ((index & ~0x1) == index))
							{
								state_begin = temp;
								segment_begin = 1;
								offset_begin = offset;
								frame->hash_code = index;

								state_end = temp;
								segment_end = 1;
								offset_end = offset;
								flag = 1;
							}
							else if(flag)
						    	{
							    	state_end = temp;
							    	segment_end = 1;
								offset_end = offset;
								flag++;
							}
						}
						else
						{
							flag = 0;
						}
					}
					break;
					case 2:
					{
						if(((unsigned int)(temp->item3) & (1 << offset))==0)
						{
							if(flag == 0 && ((index & ~0x1) == index))
							{
								state_begin = temp;
								segment_begin = 2;
								offset_begin = offset;
								frame->hash_code = index;

								state_end = temp;
								segment_end = 2;
								offset_end = offset;
								flag = 1;
							}
							else if(flag)
							{
								state_end = temp;
								segment_end = 2;
								offset_end = offset;
								flag++;
							}
						}
						else
						{
							flag = 0;
						}
					}
					break;
				}
				}
				else
				{
					flag = 0;
				}
				index++;
			}
		}
		if(temp->next==0)
		{
			temp->next = jit_memory_pool_alloc(&(func->builder->lir_list_pool),
									struct _lir_list);
			temp->next->item1 = 0;
			temp->next->item2 = 0;
			temp->next->item3 = 0;
			temp->next->next = 0;
		}
		temp = temp->next;
	}
	index--;
	frame->frame_offset = -(8 + (index * 4));
	frame->length = size;
	if((8 + (index * 4)) > func->lir->scratch_frame) func->lir->scratch_frame = 8 + (index * 4);

	temp = state_begin;
	segment = segment_begin;
	offset = offset_begin;
	flag = 0;
	while(temp && flag < size)
	{
		for(; segment < 3 && flag < size; segment++)
		{
			for(; offset < 32 && flag < size; offset++)
			{
				switch(segment)
				{
					case 0:
					{
						if( ((unsigned int)(temp->item1) & (1 << offset))==0)
						{
							temp->item1 = (void*)((unsigned int)(temp->item1)
									 | (1 << offset));
							flag++;
						}
					}
					break;
					case 1:
					{
						if( ((unsigned int)(temp->item2) & (1 << offset))==0)
						{
							temp->item2 = (void*)((unsigned int)(temp->item2) 
									    | (1 << offset));
							flag++;
						}
					}
					break;
					case 2:
					{
						if( ((unsigned int)(temp->item3) & (1 << offset))==0)
						{
							temp->item3 = (void*)((unsigned int)(temp->item3)
									    | (1<<offset));
							flag++;
						}
					}
					break;
				}
			}
			offset = 0;
		}
		segment = 0;
		if(temp->next==0)
		{
			temp->next = jit_memory_pool_alloc(&(func->builder->lir_list_pool),
									struct _lir_list);
			temp->next->item1 = 0;
			temp->next->item2 = 0;
			temp->next->item3 = 0;
			temp->next->next = 0;
		}
		temp = temp->next;
	}
}

void lir_allocate_large_frame(jit_function_t func, lir_frame_t frame, int size)
{
	if(size <= 4)
	{
		lir_allocate_frame(func, frame);
		return;
	}

	lir_list_t temp = func->lir->frame_state;
	int segment, offset;
	unsigned int index = 0;
	int flag = 0;
	size = (size + 3) / 4;
	void *state_begin = temp;
	int segment_begin = 0;
	int offset_begin = 0;
	void *state_end = temp;
	int segment_end = 0;
	int offset_end = 0;

	while(temp && flag < size)
	{
		for(segment = 0; segment < 3 && flag < size; segment++)
		{
			for(offset = 0; offset < 32 && flag < size; offset++)
			{
				switch(segment)
				{
					case 0:
					{
						if(((unsigned int)(temp->item1) & (1 << offset))==0)
						{
							// allocate memory aligned to 8 bytes
							if(flag == 0 && ((index & ~0x1) == index))
							{
								state_begin = temp;
								segment_begin = 0;
								offset_begin = offset;
								frame->hash_code = index;

								state_end = temp;
								segment_end = 0;
								offset_end = offset;
								flag = 1;
							}
							else if(flag)
							{
								state_end = temp;
								segment_end = 0;
								offset_end = offset;
								flag++;
							}
						}
						else
						{
							flag = 0;
						}
					}
					break;
					case 1:
					{
						if(((unsigned int)(temp->item2) & (1 << offset))==0)
						{
							if(flag == 0 && ((index & ~0x1) == index))
							{
								state_begin = temp;
								segment_begin = 1;
								offset_begin = offset;
								frame->hash_code = index;

								state_end = temp;
								segment_end = 1;
								offset_end = offset;
								flag = 1;
							}
							else if(flag)
						    	{
							    	state_end = temp;
							    	segment_end = 1;
								offset_end = offset;
								flag++;
							}
						}
						else
						{
							flag = 0;
						}
					}
					break;
					case 2:
					{
						if(((unsigned int)(temp->item3) & (1 << offset))==0)
						{
							if(flag == 0 && ((index & ~0x1) == index))
							{
								state_begin = temp;
								segment_begin = 2;
								offset_begin = offset;
								frame->hash_code = index;

								state_end = temp;
								segment_end = 2;
								offset_end = offset;
								flag = 1;
							}
							else if(flag)
							{
								state_end = temp;
								segment_end = 2;
								offset_end = offset;
								flag++;
							}
						}
						else
						{
							flag = 0;
						}
					}
					break;
				}
				index++;
			}
		}
		if(temp->next==0)
		{
			temp->next = jit_memory_pool_alloc(&(func->builder->lir_list_pool),
									struct _lir_list);
			temp->next->item1 = 0;
			temp->next->item2 = 0;
			temp->next->item3 = 0;
			temp->next->next = 0;
		}
		temp = temp->next;
	}
	index--;
	frame->frame_offset = -(8 + (index * 4));
	frame->length = size;
	if((8 + (index * 4)) > func->lir->scratch_frame) func->lir->scratch_frame = 8 + (index * 4);

	temp = state_begin;
	segment = segment_begin;
	offset = offset_begin;
	flag = 0;
	while(temp && flag < size)
	{
		for(; segment < 3 && flag < size; segment++)
		{
			for(; offset < 32 && flag < size; offset++)
			{
				switch(segment)
				{
					case 0:
					{
						if( ((unsigned int)(temp->item1) & (1 << offset))==0)
						{
							temp->item1 = (void*)((unsigned int)(temp->item1)
									 | (1 << offset));
							flag++;
						}
					}
					break;
					case 1:
					{
						if( ((unsigned int)(temp->item2) & (1 << offset))==0)
						{
							temp->item2 = (void*)((unsigned int)(temp->item2) 
									    | (1 << offset));
							flag++;
						}
					}
					break;
					case 2:
					{
						if( ((unsigned int)(temp->item3) & (1 << offset))==0)
						{
							temp->item3 = (void*)((unsigned int)(temp->item3)
									    | (1<<offset));
							flag++;
						}
					}
					break;
				}
			}
			offset = 0;
		}
		segment = 0;
		if(temp->next==0)
		{
			temp->next = jit_memory_pool_alloc(&(func->builder->lir_list_pool),
									struct _lir_list);
			temp->next->item1 = 0;
			temp->next->item2 = 0;
			temp->next->item3 = 0;
			temp->next->next = 0;
		}
		temp = temp->next;
	}
}


void lir_allocate_frame(jit_function_t func, lir_frame_t frame)
{
	lir_list_t temp = func->lir->frame_state;
	int segment, offset;
	unsigned int index = 0;
	int flag = 0;
	while(temp && flag == 0)
	{
		for(segment = 0; segment < 3 && flag == 0; segment++)
		{
			for(offset = 0; offset < 32 && flag == 0; offset++)
			{
				switch(segment)
				{
					case 0:
					{
						if( ((unsigned int)(temp->item1) & (1 << offset))==0)
						{
							temp->item1 = (void*)((unsigned int)(temp->item1)
									 | (1 << offset));
							flag = 1;
						}
						else index++;
					}
					break;
					case 1:
					{
						if( ((unsigned int)(temp->item2) & (1 << offset))==0)
						{
							temp->item2 = (void*)((unsigned int)(temp->item2) 
									    | (1 << offset));
							flag = 1;
						}
						else index++;
					}
					break;
					case 2:
					{
						if( ((unsigned int)(temp->item3) & (1 << offset))==0)
						{
							temp->item3 = (void*)((unsigned int)(temp->item3)
									    | (1<<offset));
							flag = 1;
						}
						else index++;
					}
					break;
				}
			}
		}
		if(temp->next==0)
		{
			temp->next = jit_memory_pool_alloc(&(func->builder->lir_list_pool),
									struct _lir_list);
			temp->next->item1 = 0;
			temp->next->item2 = 0;
			temp->next->item3 = 0;
			temp->next->next = 0;
		}
		temp = temp->next;
	}
	frame->hash_code = index;
	frame->frame_offset = -(8 + (index * 4));
	frame->length = 1;
	if((8 + (index * 4)) > func->lir->scratch_frame) func->lir->scratch_frame = 8 + (index * 4);
}


unsigned char *lir_allocate_local_register(unsigned char *inst, jit_function_t func, lir_vreg_t vreg, lir_vreg_t vreg1, lir_vreg_t vreg2, unsigned char bUsage, unsigned int fRegCond, int typeKind, unsigned int *foundReg)
{
	if((vreg && vreg->in_frame) || bUsage == LOCAL_ALLOCATE_FOR_TEMP)
	{
		jit_type_t type;
		unsigned char bRegFound = 0;
		lir_reg_t reg1 = 0, reg2 = 0, reg3 = 0, reg4 = 0;
		if((vreg1 != 0) && vreg1->in_reg && vreg1->reg)
		{
			reg1 = vreg1->reg;
			type = jit_value_get_type(vreg1->value);
			typeKind = jit_type_get_kind(type);
			if(typeKind == JIT_TYPE_LONG || typeKind == JIT_TYPE_ULONG)
			{
				reg3 = lir_object_register_pair(reg1);
			}
		}
		if((vreg2 != 0) && vreg2->in_reg && vreg2->reg)
		{
			reg2 = vreg2->reg;
			type = jit_value_get_type(vreg2->value);
			typeKind = jit_type_get_kind(type);
			if(typeKind == JIT_TYPE_LONG || typeKind == JIT_TYPE_ULONG)
			{
				reg4 = lir_object_register_pair(reg2);
			}
		}

		lir_frame_t frame1 = 0, frame2 = 0, frame3 = 0, frame4 = 0, frame5 = 0, frame6 = 0;
		if(vreg1 && vreg1->frame) frame1 = vreg1->frame;
		if(vreg2 && vreg2->frame) frame2 = vreg2->frame;
		if(vreg  && vreg->frame)  frame3 = vreg->frame;
		if(vreg1 && vreg1->reg)   frame4 = vreg1->reg->temp_frame;
		if(vreg2 && vreg2->reg)   frame5 = vreg2->reg->temp_frame;
		if(vreg  && vreg->reg)    frame6 = vreg->reg->temp_frame;


		if(vreg)
		{
			type = jit_value_get_type(vreg->value);
			type = jit_type_normalize(type);
			typeKind = jit_type_get_kind(type);
		}
		switch(typeKind)
		{
			CASE_USE_WORD
			{
				// Can use a general-purpose register
				unsigned int count;
				// First, try to find a free local register
				for(count = 0; count < LIR_N_GP_REGISTERS; count++)
				{
					if(lir_gp_regs_map[count].vreg == 0 && lir_gp_regs_map[count].local_vreg == 0 &&
						&lir_gp_regs_map[count] != reg1 && &lir_gp_regs_map[count] != reg2 &&
						&lir_gp_regs_map[count] != reg3 && &lir_gp_regs_map[count] != reg4 &&
						!(lir_gp_regs_map[count].hash_code & fRegCond))
					{
						lir_gp_regs_map[count].local_vreg = vreg;
						if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) x86_mov_reg_membase(inst, lir_gp_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset, 4);
						if(vreg)
						{
							vreg->in_frame = 0;
							vreg->in_reg = 1;
							vreg->reg = &lir_gp_regs_map[count];
						}

						func->lir->scratch_regs = func->lir->scratch_regs | lir_gp_regs_map[count].hash_code;
						func->lir->regs_state = func->lir->regs_state | lir_gp_regs_map[count].hash_code;
						
						bRegFound = 1;
						if(foundReg) *foundReg = lir_gp_regs_map[count].reg;
						
						break;
 					}
				}
				if(!bRegFound) // if we did not find any free register unused
				{
					for(count = 0; count < LIR_N_GP_REGISTERS; count++)
					{
						if(lir_gp_regs_map[count].vreg == 0 &&
						     &lir_gp_regs_map[count] != reg1 && &lir_gp_regs_map[count] != reg2 &&
						     &lir_gp_regs_map[count] != reg3 && &lir_gp_regs_map[count] != reg4 &&
						     !(lir_gp_regs_map[count].hash_code & fRegCond))
						{
							// restore the old local register content to frame
							lir_vreg_t local_vreg = lir_gp_regs_map[count].local_vreg;
							if(local_vreg)
							{
								local_vreg->in_reg = 0;
								local_vreg->in_frame = 1;
							}
							
							// load new content into local register
							lir_gp_regs_map[count].local_vreg = vreg;
							if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) x86_mov_reg_membase(inst, lir_gp_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset, 4);
							if(vreg)
							{
								vreg->in_frame = 0;
								vreg->in_reg = 1;
								vreg->reg = &lir_gp_regs_map[count];
							}
							bRegFound = 1;
							if(foundReg) *foundReg = lir_gp_regs_map[count].reg;

							break;
		 				}
					}
				}

				if(!bRegFound) // if we did not find any free register at all try to find one already saved locally
				{
					for(count = 0; count < LIR_N_GP_REGISTERS; count++)
					{
						if(&lir_gp_regs_map[count] != reg1 && &lir_gp_regs_map[count] != reg2 &&
						   &lir_gp_regs_map[count] != reg3 && &lir_gp_regs_map[count] != reg4 &&
						   lir_gp_regs_map[count].temp_frame &&
						   !(lir_gp_regs_map[count].hash_code & fRegCond))
						{
							// load new content into local register
							if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) x86_mov_reg_membase(inst, lir_gp_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset, 4);
							lir_vreg_t local_vreg = lir_gp_regs_map[count].local_vreg;
							if(local_vreg)
							{
								local_vreg->in_frame = 1;
								local_vreg->in_reg = 0;
							}
							lir_gp_regs_map[count].local_vreg = vreg;
							
							if(vreg)
							{
								vreg->in_frame = 0;
								vreg->in_reg = 1;
								vreg->reg = &lir_gp_regs_map[count];
							}
							bRegFound = 1;							
							if(foundReg) *foundReg = lir_gp_regs_map[count].reg;

							break;
		 				}
					}
				}
				if(!bRegFound) // if we did not find any free register at all try to find one not saved yet locally
				{
					for(count = 0; count < LIR_N_GP_REGISTERS; count++)
					{
						if(&lir_gp_regs_map[count] != reg1 && &lir_gp_regs_map[count] != reg2 &&
						   &lir_gp_regs_map[count] != reg3 && &lir_gp_regs_map[count] != reg4 && 
						   !(lir_gp_regs_map[count].hash_code & fRegCond))
						{
							// load new content into local register
							lir_frame_t frame = lir_gp_regs_map[count].temp_frame;
							if(frame == 0)
							{
								frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool),
												    struct _lir_frame);
								lir_allocate_large_frame_cond6(func, frame, lir_type_get_size(jit_value_get_type(lir_gp_regs_map[count].vreg->value)), frame1, frame2, frame3, frame4, frame5, frame6);
								lir_gp_regs_map[count].temp_frame = frame;
							}
							
							jit_type_t type = jit_value_get_type(lir_gp_regs_map[count].vreg->value);
							type = jit_type_normalize(type);
							int typeKind = jit_type_get_kind(type);

							x86_mov_membase_reg(inst, X86_EBP, frame->frame_offset, lir_gp_regs_map[count].reg, 4);
			
							if(typeKind == JIT_TYPE_LONG || typeKind == JIT_TYPE_ULONG)
							{
								x86_mov_membase_reg(inst, X86_EBP, frame->frame_offset + 4, lir_register_pair(lir_gp_regs_map[count].reg), 4);
							}

							if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) x86_mov_reg_membase(inst, lir_gp_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset, 4);
			
							lir_vreg_t local_vreg = lir_gp_regs_map[count].local_vreg;
							if(local_vreg)
							{
								local_vreg->in_reg = 0;
								local_vreg->in_frame = 1;
							}
							lir_gp_regs_map[count].local_vreg = vreg;

							if(vreg)
							{
								vreg->in_frame = 0;
								vreg->in_reg = 1;
								vreg->reg = &lir_gp_regs_map[count];
							}
							bRegFound = 1;
							if(foundReg) *foundReg = lir_gp_regs_map[count].reg;
							
							break;
		 				}
					}
				}
			}
			break;
			CASE_USE_FLOAT
			{
				if(typeKind == JIT_TYPE_NFLOAT && sizeof(jit_nfloat) != sizeof(jit_float64)) break;
				// Can use an XMM register
				unsigned int count;
				// First, try to find a free register
				for(count = 0; count < LIR_N_XMM_REGISTERS; count++)
				{
					if(lir_xmm_regs_map[count].vreg == 0 && lir_xmm_regs_map[count].local_vreg == 0 &&
  					   &lir_xmm_regs_map[count] != reg1 && &lir_xmm_regs_map[count] != reg2 &&
					   !(lir_xmm_regs_map[count].hash_code & fRegCond))
					{
						lir_xmm_regs_map[count].local_vreg = vreg;
						if(typeKind == JIT_TYPE_FLOAT32)
						{
							if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse_movss_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
						}
						else
						{
							if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse2_movsd_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
						}
						
						if(vreg)
						{
							vreg->in_frame = 0;
							vreg->in_reg = 1;
							vreg->reg = &lir_xmm_regs_map[count];
						}
						func->lir->scratch_regs = func->lir->scratch_regs | lir_xmm_regs_map[count].hash_code;
						func->lir->regs_state = func->lir->regs_state | lir_xmm_regs_map[count].hash_code;
						bRegFound = 1;
						if(foundReg) *foundReg = lir_xmm_regs_map[count].reg;
						
						break;
 					}
				}
				if(!bRegFound) // if we did not find any free register
				{
					for(count = 0; count < LIR_N_XMM_REGISTERS; count++)
					{
						if(lir_xmm_regs_map[count].vreg == 0 &&
						     &lir_xmm_regs_map[count] != reg1 && &lir_xmm_regs_map[count] != reg2 &&
						     !(lir_xmm_regs_map[count].hash_code & fRegCond))
						{
							// restore the old local register content to frame
							lir_vreg_t local_vreg = lir_xmm_regs_map[count].local_vreg;
							if(local_vreg)
							{
								local_vreg->in_reg = 0;
								local_vreg->in_frame = 1;
							}
							// load new content into local register
							lir_xmm_regs_map[count].local_vreg = vreg;
							if(typeKind == JIT_TYPE_FLOAT32)
							{
								if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse_movss_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
							}
							else
							{
								if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse2_movsd_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
							}
							if(vreg)
							{
								vreg->in_frame = 0;
								vreg->in_reg = 1;
								vreg->reg = &lir_xmm_regs_map[count];
							}
							bRegFound = 1;
							if(foundReg) *foundReg = lir_xmm_regs_map[count].reg;
							
							break;
		 				}
					}
				}

				if(!bRegFound) // if we did not find any free register at all try to find one already saved locally
				{
					for(count = 0; count < LIR_N_XMM_REGISTERS; count++)
					{
						if(&lir_xmm_regs_map[count] != reg1 && &lir_xmm_regs_map[count] != reg2
						    && lir_xmm_regs_map[count].temp_frame
						    && !(lir_xmm_regs_map[count].hash_code & fRegCond))
						{
							// load new content into local register

							if(typeKind == JIT_TYPE_FLOAT32)
							{
								if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse_movss_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
							}
							else
							{
								if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse2_movsd_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
							}
							lir_vreg_t local_vreg = lir_xmm_regs_map[count].local_vreg;
							if(local_vreg)
							{
								local_vreg->in_frame = 1;
								local_vreg->in_reg = 0;
							}
							lir_xmm_regs_map[count].local_vreg = vreg;
							if(vreg)
							{
								vreg->in_frame = 0;
								vreg->in_reg = 1;
								vreg->reg = &lir_xmm_regs_map[count];
							}
							bRegFound = 1;
							if(foundReg) *foundReg = lir_xmm_regs_map[count].reg;
							
							break;
		 				}
					}
				}
				if(!bRegFound) // if we did not find any free register at all try to find one not saved yet locally
				{
					for(count = 0; count < LIR_N_XMM_REGISTERS; count++)
					{
						if(&lir_xmm_regs_map[count] != reg1 && &lir_xmm_regs_map[count] != reg2
						   && !(lir_xmm_regs_map[count].hash_code & fRegCond))
						{
							// load new content into local register
							lir_frame_t frame = lir_xmm_regs_map[count].temp_frame;
							if(frame == 0)
							{
								frame = jit_memory_pool_alloc(&(func->builder->lir_frame_pool),
												    struct _lir_frame);
								lir_allocate_large_frame_cond6(func, frame, 8, frame1, frame2, frame3, frame4, frame5, frame6);
								lir_xmm_regs_map[count].temp_frame = frame;
							}

							sse2_movsd_membase_xmreg(inst, X86_EBP, frame->frame_offset, lir_xmm_regs_map[count].reg);

							if(typeKind == JIT_TYPE_FLOAT32)
							{
								if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse_movss_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
							}
							else
							{
								if(bUsage == LOCAL_ALLOCATE_FOR_INPUT) sse2_movsd_xmreg_membase(inst, lir_xmm_regs_map[count].reg, X86_EBP, vreg->frame->frame_offset);
							}
							lir_vreg_t local_vreg = lir_xmm_regs_map[count].local_vreg;
							if(local_vreg)
							{
								local_vreg->in_reg = 0;
								local_vreg->in_frame = 1;
							}
							lir_xmm_regs_map[count].local_vreg = vreg;
							if(vreg)
							{
								vreg->in_frame = 0;
								vreg->in_reg = 1;
								vreg->reg = &lir_xmm_regs_map[count];
							}
							bRegFound = 1;
							if(foundReg) *foundReg = lir_xmm_regs_map[count].reg;
							
							break;
		 				}
					}
				}
			}
			break;
		}
	}
	return inst;
}

unsigned char *ejit_emit_trampoline_for_internal_abi(jit_gencode_t gen, unsigned char *buf, unsigned int num, jit_value_t *args, void *func_address, jit_value_t return_value, int call_type)
{
		int index;
		int long_index = 0;
		int xmm_index = 0;
		int gp_index = 0;
		unsigned int offset = 0;
		unsigned int n_frames = 0;
		unsigned int stack_size = 0;
		unsigned int stack_offset = 0;

		if(call_type == INDIRECT_CALL)
		{
			x86_push_reg(buf, X86_EAX);
			stack_offset += 4;
		}
		for(index = 0; index < num; index++)
		{
			jit_value_t value = args[index];
			jit_type_t type = jit_value_get_type(value);
			type = jit_type_remove_tags(type);
			int typeKind = jit_type_get_kind(type);
			switch(typeKind)
			{
				CASE_USE_WORD
				{
					stack_offset+=4;
					gp_index++;
				}
				break;
				case JIT_TYPE_FLOAT32:
				{
					stack_offset+=4;
					xmm_index++;
				}
				break;
				case JIT_TYPE_FLOAT64:
				{
					stack_offset+=8;
					xmm_index++;
				}
				break;
				case JIT_TYPE_NFLOAT:
				{
					if(sizeof(jit_nfloat) != sizeof(jit_float64))
					{
						stack_offset+=12;
					}
					else
					{
						stack_offset+=8;
						xmm_index++;
					}
				}
				break;
				CASE_USE_LONG
				{
					stack_offset+=8;
					long_index++;
				}
				break;
				default:
				{
					stack_offset+=jit_type_get_size(jit_value_get_type(value));
				}
				break;
			}
		}
		stack_size = stack_offset;

		for(index = (num - 1); index >= 0; index--)
		{
			if(!jit_cache_check_for_n(&(gen->posn), 32))
			{
				jit_cache_mark_full(&(gen->posn));
				return buf;
			}
			jit_value_t value = args[index];
			jit_type_t type = jit_value_get_type(value);
			type = jit_type_remove_tags(type);
			int typeKind = jit_type_get_kind(type);
			switch(typeKind)
			{
				CASE_USE_WORD
				{
				    stack_offset-=4;
				    switch(gp_index)
				    {
					    case 1:
					    {
						    x86_mov_reg_membase(buf, X86_EAX, X86_ESP, stack_offset + offset, 4);
					    }
					    break;
					    case 2:
					    {
						    x86_mov_reg_membase(buf, X86_EDX, X86_ESP, stack_offset + offset, 4);
					    }
					    break;
					    case 3:
					    {
						    x86_mov_reg_membase(buf, X86_ECX, X86_ESP, stack_offset + offset, 4);
					    }
					    break;
					    default:
					    {
						    n_frames++;
						    x86_mov_reg_membase(buf, X86_EAX, X86_ESP, stack_offset + offset, 4);
						    x86_alu_reg_imm(buf, X86_SUB, X86_ESP, 4);
						    x86_mov_membase_reg(buf, X86_ESP, 0, X86_EAX, 4);
						    offset+=4;
					    }
					    break;
				    }
				    gp_index--;
				}
				break;
				case JIT_TYPE_FLOAT32:
				{
				    stack_offset-=4;
				    switch(xmm_index)
				    {
					    case 1:
					    {
						    sse_movss_xmreg_membase(buf, XMM0, X86_ESP, stack_offset + offset);
					    }
					    break;
					    case 2:
					    {
						    sse_movss_xmreg_membase(buf, XMM1, X86_ESP, stack_offset + offset);
					    }
					    break;
					    case 3:
					    {
						    sse_movss_xmreg_membase(buf, XMM2, X86_ESP, stack_offset + offset);
					    }
					    break;
					    default:
					    {
						    n_frames++;
						    x86_push_membase(buf, X86_ESP, stack_offset + offset);
						    offset += 4;
					    }
					    break;
				    }
				    xmm_index--;
				}
				break;
				case JIT_TYPE_FLOAT64:
				{
				    stack_offset-=8;
				    switch(xmm_index)
				    {
					    case 1:
					    {
    						    sse2_movsd_xmreg_membase(buf, XMM0, X86_ESP, stack_offset + offset);
					    }
					    break;
					    case 2:
					    {
						    sse2_movsd_xmreg_membase(buf, XMM1, X86_ESP, stack_offset + offset);
					    }
					    break;
					    case 3:
		    			    {
						    sse2_movsd_xmreg_membase(buf, XMM2, X86_ESP, stack_offset + offset);
					    }
					    break;
					    default:
					    {
						    n_frames+=2;
						    sse2_movsd_xmreg_membase(buf, XMM0, X86_ESP, stack_offset + offset);
						    x86_alu_reg_imm(buf, X86_SUB, X86_ESP, 8);
						    sse2_movsd_membase_xmreg(buf, X86_ESP, 0, XMM0);
						    offset+=8;
					    }
					    break;
				    }
				    xmm_index--;
				}
				break;
				case JIT_TYPE_NFLOAT:
				{
					if(sizeof(jit_nfloat) != sizeof(jit_float64))
					{
						stack_offset-=12;
						n_frames+=3;
						x86_push_membase(buf, X86_ESP, stack_offset + offset + 8);
						x86_push_membase(buf, X86_ESP, stack_offset + offset + 8);
						x86_push_membase(buf, X86_ESP, stack_offset + offset + 8);
						offset += 12;
					}
					else
					{
						stack_offset-=8;
		    				switch(xmm_index)
						{
							case 1:
							{
								sse2_movsd_xmreg_membase(buf, XMM0, X86_ESP, stack_offset + offset);
							}
							break;
							case 2:
							{
								sse2_movsd_xmreg_membase(buf, XMM1, X86_ESP, stack_offset + offset);
							}
							break;
							case 3:
							{
								sse2_movsd_xmreg_membase(buf, XMM2, X86_ESP, stack_offset + offset);
							}
							break;
							default:
							{
								n_frames+=2;
								x86_push_membase(buf, X86_ESP, stack_offset + offset + 4);
								x86_push_membase(buf, X86_ESP, stack_offset + offset + 4);
								offset += 8;
							}
							break;
						}
						xmm_index--;
					}
				}
				break;
				CASE_USE_LONG
				{
					stack_offset-=8;
					n_frames+=2;
				        x86_push_membase(buf, X86_ESP, stack_offset + offset + 4);
				        x86_push_membase(buf, X86_ESP, stack_offset + offset + 4);
				        offset += 8;
				        long_index--;
				}
				break;
				default:
				{
				        stack_offset-=jit_type_get_size(jit_value_get_type(value));
				        int num_frames = ((jit_type_get_size(jit_value_get_type(value)) + 3)/4);
				        n_frames+=num_frames;
				        x86_alu_reg_imm(buf, X86_SUB, X86_ESP, num_frames * 4);
				        offset+=(num_frames * 4);
				        buf = (unsigned char*)lir_memory_copy_with_reg(buf, X86_ESP, 0,
									X86_ESP, (stack_offset + offset),
									(jit_type_get_size(jit_value_get_type(value))), X86_EAX);
				}
				break;
			}
		}
		
		if(call_type == NORMAL_CALL) x86_call_code(buf, func_address);
		else if(call_type == INDIRECT_CALL) x86_call_membase(buf, X86_ESP, ((n_frames << 2)));
		// We do not need to restore the stack. As an internal abi restores stack itself.
		// The items which we were pushed on stack to perform this call will be poped by the cdecl -O0 engine.

		jit_type_t type = jit_value_get_type(return_value);
		type = jit_type_remove_tags(type);
		int typeKind = jit_type_get_kind(type);
		switch(typeKind)
		{
			case JIT_TYPE_FLOAT32:
			{
				sse_movss_membase_xmreg(buf, X86_ESP, -8, XMM0);
				x86_fld_membase(buf, X86_ESP, -8, 0);
			}
			break;
			case JIT_TYPE_FLOAT64:
			{
				sse2_movsd_membase_xmreg(buf, X86_ESP, -8, XMM0);
				x86_fld_membase(buf, X86_ESP, -8, 1);
			}
			break;
			case JIT_TYPE_NFLOAT:
			{
				if(sizeof(jit_nfloat) == sizeof(jit_float64))
				{
					sse2_movsd_membase_xmreg(buf, X86_ESP, -8, XMM0);
					x86_fld_membase(buf, X86_ESP, -8, 1);
				}
			}
			break;
		}

		return buf;
}


unsigned char *ejit_emit_function_call(jit_gencode_t gen, unsigned char *buf, jit_function_t func,
				unsigned int num, void *func_address, jit_value_t indirect_ptr, jit_value_t *args, jit_value_t return_value, int call_type)
{
			if(!jit_cache_check_for_n(&(gen->posn), 32))
			{
				jit_cache_mark_full(&(gen->posn));
				return buf;
			}

#ifdef EJIT_DEBUG_ENABLED
			buf = ejit_generate_dump(buf, gen);
#endif

			if(call_type == NORMAL_CALL)
			{
				x86_call_code(buf, func_address);
			}
			else if(call_type == TAIL_CALL)
			{
				buf = restore_callee_saved_registers(buf, func);
				x86_jump_code(buf, func_address);
			}
			else if(call_type == INDIRECT_CALL)
			{
				buf = masm_call_indirect(buf, indirect_ptr);
			}
			else if(call_type == INDIRECT_TAIL_CALL)
			{
				buf = restore_callee_saved_registers(buf, func);
				// After call to restore_callee_saved_registers, ESP register is in state as after return address was just pushed at function call
				buf = masm_jump_indirect(buf, indirect_ptr);
			}

#ifdef EJIT_DEBUG_ENABLED
			buf = ejit_generate_dump(buf, gen);
#endif
			return buf;

}


unsigned int lir_stack_depth_used(jit_function_t func)
{
	int index;
	int gp_param = 0, long_param = 0, xmm_param = 0;
	int depth = 0;
	jit_abi_t abi = jit_function_get_abi(func);
	jit_type_t signature = jit_function_get_signature(func);
	unsigned int num = lir_type_num_params(signature);
	for(index = 0; index < num; index++)
	{
		jit_type_t type = lir_type_get_param(signature, index);
		type = jit_type_remove_tags(type);
		int typeKind = jit_type_get_kind(type);
		
		switch(typeKind)
		{
			CASE_USE_WORD
			{
				switch(abi)
				{
					case jit_abi_cdecl:
					case jit_abi_vararg:
					case jit_abi_stdcall:
					{
						depth += 4;
					}
					break;		
					case jit_abi_fastcall:
					{
						// TODO
					}
					break;			
					case jit_abi_internal:
					{
						if(gp_param > 2) depth += 4;
					}
					break;
				}
				gp_param++;
			}
			break;
			CASE_USE_LONG
			{
				switch(abi)
				{
					case jit_abi_cdecl:
					case jit_abi_vararg:
					case jit_abi_stdcall:
					{
						depth += 8;
					}
					break;
					case jit_abi_fastcall:
					{
						// TODO
					}
					break;
					case jit_abi_internal:
					{
						depth += 8;
					}
					break;
				}
				long_param++;
			}
			break;
			case JIT_TYPE_FLOAT32:
			{
				switch(abi)
				{
					case jit_abi_cdecl:
					case jit_abi_vararg:
					case jit_abi_stdcall:
					{
						depth += 4;
					}
					break;
					case jit_abi_fastcall:
					{
						// TODO
					}
					break;
					case jit_abi_internal:
					{
						if(xmm_param > 2) depth += 4;
						xmm_param++;
					}
					break;
				}
			}
			break;
			case JIT_TYPE_FLOAT64:
			{
				switch(abi)
				{
					case jit_abi_cdecl:
					case jit_abi_vararg:
					case jit_abi_stdcall:
					{
						depth += 8;
					}
					break;
					case jit_abi_fastcall:
					{
						// TODO
					}
					break;
					case jit_abi_internal:
					{
						if(xmm_param > 2) depth +=8;
						xmm_param++;
					}
					break;
				}
			}
			break;
			case JIT_TYPE_NFLOAT:
			{
				switch(abi)
				{
					case jit_abi_cdecl:
					case jit_abi_vararg:
					case jit_abi_stdcall:
					{
						if(sizeof(jit_nfloat) != sizeof(jit_float64))
						{
							depth += 12;
						}
						else
						{
							depth += 8;
						}
					}
					break;
					case jit_abi_fastcall:
					{
						// TODO
					}
					break;
					case jit_abi_internal:
					{
						if(sizeof(jit_nfloat) != sizeof(jit_float64)) depth += 12;
						else
						{
							if(xmm_param > 2) depth += 8;
							xmm_param++;
						}
					}
					break;
				}
			}
			break;
			default:
			{
				// structures are not always passed on top of stack,
				// but it is the case for cdecl and gcc/GNU/Linux
				depth += lir_type_get_size(type);
			}
			break;
		}
		
	}
	return depth;
}



void lir_compute_register_holes(jit_function_t func)
{
	jit_block_t block = 0;
	jit_insn_iter_t iter;
	jit_insn_t insn;
	lir_linked_list_t allFunctions = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	lir_linked_list_t nativeFunctions = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);			    
	lir_linked_list_t holes[32];

	holes[X86_REG_EAX] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_EDX] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_ECX] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_EBX] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_ESI] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_EDI] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_XMM0] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_XMM1] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	holes[X86_REG_XMM2] = jit_memory_pool_alloc(&(func->builder->lir_linked_list_pool),
										struct _lir_linked_list);

	while((block = jit_block_next(func, block)) != 0)
	{
		jit_insn_iter_init(&iter, block);
		while((insn = jit_insn_iter_next(&iter)) != 0)
		{
			if(insn)
			{
				switch(insn->opcode)
				{
					case JIT_OP_CALL:
					case JIT_OP_CALL_TAIL:
					case JIT_OP_CALL_INDIRECT:
					case JIT_OP_CALL_INDIRECT_TAIL:
					case JIT_OP_CALL_VTABLE_PTR:
					case JIT_OP_CALL_VTABLE_PTR_TAIL:
					case JIT_OP_CALL_EXTERNAL:
					case JIT_OP_CALL_EXTERNAL_TAIL:
					{
						jit_abi_t abi = (*insn->call_params)->abi;
						if(abi == jit_abi_internal)
						{
							lir_add_item_to_linked_list(func, insn, allFunctions);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_XMM0]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_XMM1]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_XMM2]);
						}
						else
						{
							lir_add_item_to_linked_list(func, insn, allFunctions);
							lir_add_item_to_linked_list(func, insn, nativeFunctions);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_XMM0]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_XMM1]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_XMM2]);
						}
					}
					break;
					case JIT_OP_IDIV:
					case JIT_OP_IDIV_UN:
					case JIT_OP_IREM:
					case JIT_OP_IREM_UN:
					{
						if(insn->value1->vreg || insn->value2->vreg)
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
						}
					}
					break;
					case JIT_OP_LDIV:
					case JIT_OP_LDIV_UN:
					{
						if(!jit_value_is_constant(insn->value1)
							|| !jit_value_is_constant(insn->value2))
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EBX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ESI]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDI]);
						}
					}
					break;
					case JIT_OP_LMUL:
					{
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EBX]);
					}
					break;
					case JIT_OP_LSHR:
					case JIT_OP_LSHR_UN:
					case JIT_OP_LSHL:
					{
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
		    			}
					break;
					case JIT_OP_ISHR:
					case JIT_OP_ISHR_UN:
					case JIT_OP_ISHL:
					{
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
					}
					break;
					case JIT_OP_LOAD_RELATIVE_STRUCT:
					{
						jit_value_t dest = jit_insn_get_dest(insn);
						jit_nuint size = jit_type_get_size(jit_type_normalize(jit_value_get_type(dest)));
						// We consider the worst case that an external function is called
						// to perform the operation.
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]); // At least one free register is always needed to perform a copy operation between memory.
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
						if(size > (4 * sizeof(void *))) // In this case an external 'memcpy' will be called and eax, edx, ecx are scratched.
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
						}
					}
					break;
					case JIT_OP_STORE_RELATIVE_STRUCT:
					{
						jit_value_t value1 = jit_insn_get_value1(insn);
						jit_nuint size = jit_type_get_size(jit_value_get_type(value1));
						// We consider the worst case that an external function is called
						// to perform the operation.
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]); // At least one free register is always needed to perform a copy operation between memory.
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
						if(size > (4 * sizeof(void *))) // In this case an external 'memcpy' will be called and eax, edx, ecx are scratched.
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
						}
					}
					break;
					case JIT_OP_COPY_STRUCT:
					{
						jit_value_t dest = jit_insn_get_dest(insn);
						jit_nuint size = jit_type_get_size(jit_value_get_type(dest));
						// We consider the worst case that an external function is called
						// to perform the operation.
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]); // At least one free register is always needed to perform a copy operation between memory.
						if(size > (4 * sizeof(void *))) // In this case an external 'memcpy' will be called and eax, edx, ecx are scratched.
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
						}
					}
					break;
					case JIT_OP_JUMP_TABLE:
					{
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
					}
					break;
					case JIT_OP_PUSH_STRUCT:
					{
						jit_value_t source = jit_insn_get_value1(insn);
						jit_nuint size = jit_type_get_size(jit_value_get_type(source));
						lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
						if(size > (4 * sizeof(void *)))
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
						}
					}
					break;
					case JIT_OP_MEMCPY:
					{
						jit_value_t value2 = jit_insn_get_value2(insn);
						if(!jit_value_is_constant(value2) ||
							(jit_value_is_constant(value2) && jit_value_get_nint_constant(value2) > (4 * sizeof(void *))))
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EAX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_EDX]);
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
						}
						else if(jit_value_is_constant(value2) && jit_value_get_nint_constant(value2) != 0)
						{
							lir_add_item_to_linked_list(func, insn, holes[X86_REG_ECX]);
						}
					}
					break;
					case JIT_OP_OUTGOING_REG:
					{
						int destReg = (int)jit_value_get_nint_constant(insn->value2);
						lir_add_item_to_linked_list(func, insn, holes[destReg]);
					}
					break;
					case JIT_OP_INCOMING_REG:
					{
						jit_value_t source1 = jit_insn_get_value1(insn);
						int destReg = (int)jit_value_get_nint_constant(insn->value2);
						if(source1->vreg->max_range != source1->vreg->min_range)
						{
							lir_add_item_to_linked_list(func, insn, holes[destReg]);
						}
					}
					break;
				}
			}
		}
	}

	func->lir->reg_holes[X86_EAX] = holes[X86_REG_EAX];
	func->lir->reg_holes[X86_EDX] = holes[X86_REG_EDX];
	func->lir->reg_holes[X86_ECX] = holes[X86_REG_ECX];
	func->lir->reg_holes[X86_EBX] = holes[X86_REG_EBX];
	func->lir->reg_holes[X86_ESI] = holes[X86_REG_ESI];
	func->lir->reg_holes[X86_EDI] = holes[X86_REG_EDI];

	func->lir->reg_holes[X86_REG_XMM0] = holes[X86_REG_XMM0];
	func->lir->reg_holes[X86_REG_XMM1] = holes[X86_REG_XMM1];
	func->lir->reg_holes[X86_REG_XMM2] = holes[X86_REG_XMM2];
	func->lir->reg_holes[X86_REG_XMM3] = allFunctions;
	func->lir->reg_holes[X86_REG_XMM4] = allFunctions;
	func->lir->reg_holes[X86_REG_XMM5] = allFunctions;
	func->lir->reg_holes[X86_REG_XMM6] = allFunctions;
	func->lir->reg_holes[X86_REG_XMM7] = allFunctions;
}



unsigned char lir_vreg_is_in_register_hole(jit_function_t func, lir_vreg_t vreg, unsigned int regIndex)
{
	jit_type_t type = jit_value_get_type(vreg->value);
	type = jit_type_remove_tags(type);
	int typeKind = jit_type_get_kind(type);
	unsigned int index[3] = {-1, -1, -1};
	unsigned int count = 0;
	switch(typeKind)
	{
		CASE_USE_WORD
		{
			index[0] = lir_gp_regs_map[regIndex].reg;
		}
		break;
		CASE_USE_LONG
		{
			index[0] = lir_gp_regs_map[regIndex].reg;
			index[1] = lir_register_pair(index[0]);
		}
		break;
		CASE_USE_FLOAT
		{
			index[0] = regIndex + X86_REG_XMM0;
		}
		break;
		default:
		{
		    return 0;
		}
		break;
	}
	unsigned int min = vreg->min_range->insn_num;
	unsigned int max = vreg->max_range->insn_num;
	while(index[count] != -1)
	{
	        lir_linked_list_t list = func->lir->reg_holes[index[count]];

	        while(list)
	        {
			jit_insn_t insn = (jit_insn_t)(list->item);
			if(insn)
			{
			    unsigned int current = insn->insn_num;
			    if(min < current && max > current) return 1;
			}
			list = list->next;
		}
		count++;
	}
	
	return 0;
}

unsigned int lir_register_pair(unsigned int reg)
{
	return lir_gp_regs_map[lir_index_register_pair(gp_reg_map[reg])].reg;
}

unsigned int lir_index_register_pair(unsigned int index)
{
	if(index < (LIR_N_GP_REGISTERS - 1)) return index + 1;
	return 0;
}

lir_reg_t lir_object_register_pair(lir_reg_t reg)
{
	return &(lir_gp_regs_map[lir_index_register_pair(gp_reg_map[reg->reg])]);
}

int ejit_x86reg_to_reg(int reg)
{
	if(reg >= X86_REG_EAX && reg <= X86_REG_ESP)
	{
		return reg;
	}
	else if(reg >= X86_REG_ST0 && reg <= X86_REG_ST7)
	{
		return reg - X86_REG_ST0;
	}
	else if(reg >= X86_REG_XMM0 && reg <= X86_REG_XMM7)
	{
		return reg - X86_REG_XMM0;
	}
	return -1;
}

#ifdef EJIT_DEBUG_ENABLED
void ejit_dump_registers(unsigned int buf, unsigned int ecx, unsigned int edx, unsigned int eax, unsigned int ebx, unsigned int edi, unsigned int esi, unsigned int ebp)
{
	printf("\naddress = 0x%x, ecx = 0x%x, edx = 0x%x, eax = 0x%x, ebx = 0x%x, edi = 0x%x, esi = 0x%x, ebp = 0x%x, \n", buf, ecx, edx, eax, ebx, edi, esi, ebp);
	unsigned int index;
	for(index = 0; index < 3; index++)
	{
		printf(" *(ebp + 0x%x) = 0x%x", (0x8 + index * 4), *((unsigned int *)(ebp + 0x8 + index * 4)));
	}
	printf("\n");
	fflush(stdout);
	return;
}

unsigned char* ejit_generate_dump(unsigned char *inst, jit_gencode_t gen)
{
	if(!jit_cache_check_for_n(&(gen->posn), 200))
	{
		jit_cache_mark_full(&(gen->posn));
		return inst;
	}
	x86_push_reg(inst, X86_EBP);
	x86_push_reg(inst, X86_ESI);
	x86_push_reg(inst, X86_EDI);
	x86_push_reg(inst, X86_EBX);
	x86_push_reg(inst, X86_EAX);
	x86_push_reg(inst, X86_EDX);
	x86_push_reg(inst, X86_ECX);

	x86_push_reg(inst, X86_EBP);
	x86_push_reg(inst, X86_ESI);
	x86_push_reg(inst, X86_EDI);
	x86_push_reg(inst, X86_EBX);
	x86_push_reg(inst, X86_EAX);
	x86_push_reg(inst, X86_EDX);
	x86_push_reg(inst, X86_ECX);
	x86_push_imm(inst, inst);

	x86_call_code(inst, ejit_dump_registers);
	
	x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 4 * 8);

	x86_pop_reg(inst, X86_ECX);
	x86_pop_reg(inst, X86_EDX);
	x86_pop_reg(inst, X86_EAX);
	x86_pop_reg(inst, X86_EBX);
	x86_pop_reg(inst, X86_EDI);
	x86_pop_reg(inst, X86_ESI);
	x86_pop_reg(inst, X86_EBP);
	return inst;
}

#endif
#endif
