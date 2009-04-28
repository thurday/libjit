#include "jit-internal.h"
#include <config.h>
#include "jit-rules.h"
#include "jit-setjmp.h"
#include "jit/jit-dump.h"

#if defined(JITE_ENABLED) && !defined(JIT_BACKEND_INTERP)

unsigned int jite_value_in_reg(jit_value_t value)
{
    if(value && value->vreg && value->vreg->in_reg && value->vreg->reg)
    {
        return 1;
    }
    return 0;
}


unsigned int jite_value_in_frame(jit_value_t value)
{
    if(value && value->vreg && value->vreg->in_frame && value->vreg->frame)
    {
        return 1;
    }
    return 0;
}


int jite_count_items(jit_function_t func, jite_linked_list_t list)
{
    int num = 0;
    while(list)
    {
        if(list->item) num++;
        list = list->next;
    }
    return num;
}


int jite_type_num_params(jit_type_t signature)
{
    jit_type_t type = jit_type_get_return(signature);

    if(!jit_type_return_via_pointer(type))
    {
        return jit_type_num_params(signature);
    }

    return jit_type_num_params(signature) + 1;
}

jit_type_t jite_type_get_param(jit_type_t signature, int index)
{
    jit_type_t type = jit_type_get_return(signature);
    if(!jit_type_is_struct(type) && !jit_type_is_union(type))
    {
        return jit_type_get_param(signature, index);
    }
    else
    {
        if(index) return jit_type_get_param(signature, index - 1);
        else return jit_type_create_pointer(jit_type_get_return(signature), 1);
    }
}

jit_value_t jite_value_get_param(jit_function_t func, int index)
{
    jit_type_t type = jit_type_get_return(jit_function_get_signature(func));
    if(!jit_type_is_struct(type) && !jit_type_is_union(type))
    {
        return jit_value_get_param(func, index);
    }
    else
    {
        if(index) return jit_value_get_param(func, index - 1);
        else return jit_value_get_struct_pointer(func);
    }
}


jite_vreg_t jite_create_vreg(jit_value_t value)
{
    jit_function_t func = jit_value_get_function(value);
    jite_instance_t jite = func->jite;
    jite_vreg_t vreg = 0;
    jite_linked_list_t list;
    if(!_jit_function_ensure_builder(func) || !value)
    {
        return 0;
    }
    if(!jite_value_is_in_vreg(value))
    {
        if(jite->vregs_list)
        {
            list = jite->vregs_list;
            while(list->next)
            {
                list = list->next;
            }
            /* This means that at this point list->next == 0.
               Allocate some place for a new item. */

            list->next = jit_memory_pool_alloc(&(func->builder->jite_list_pool),
                                        struct _jite_linked_list);
            list->next->prev = list;
            list = list->next;
        }
        else
        {
            jite->vregs_list = jit_memory_pool_alloc(&(func->builder->jite_list_pool),
                                        struct _jite_linked_list);
            list = jite->vregs_list;
        }
        vreg = jit_memory_pool_alloc(&(func->builder->jite_vreg_pool),
                                    struct _jite_vreg);
        vreg->value = value;
        vreg->min_range = 0;
        vreg->max_range = 0;
        vreg->in_reg = 0;
        vreg->in_frame = 0;
        vreg->index = jite->vregs_num;
        vreg->weight = 0;
        vreg->reg = 0;
        vreg->frame = 0;
	/* Create an empty list of liveness ranges.
	   It will be filled latter at liveness analysis. */
	vreg->liveness = 0;
        jite->vregs_num++;
        list->item = (void*)vreg;
        value->vreg = vreg;
	jite_value_set_weight(value, func->builder->num_insns);
    }
    return value->vreg;
}

char jite_value_is_in_vreg(jit_value_t value)
{
    if(value->vreg) return 1;
    return 0;
}

jite_instance_t jite_create_instance(jit_function_t func)
{
    jite_instance_t jite = jit_cnew(struct _jite_instance);
    jite->func = func;
    jite->cpoints_list = 0;
    jite->vregs_num = 0;
    jite->regs_state = 0;
    jite->frame_state = jit_memory_pool_alloc(&(func->builder->jite_list_pool),
                                struct _jite_list);
    jite->incoming_params_state = 0;
    jite->scratch_regs = 0;
    jite->scratch_frame = 0;
    func->jite = jite;
    int index = 0;
    for(index = 0; index < 32; index++)
    {
        jite->reg_holes[index] = 0;
    }

    
    jite_init(func); /* Platform dependent init. */
//    jite_function_set_register_allocator_euristic(func, BYHAND_EURISTIC);
//    jite_function_set_register_allocator_euristic(func, NUMBER_OF_USE_EURISTIC);
//    jite_function_set_register_allocator_euristic(func, NUMBER_OF_USE_EURISTIC);
    jite_function_set_register_allocator_euristic(func, LINEAR_SCAN_EURISTIC);
    return jite;
}


unsigned int jite_get_max_weight()
{
    return 0xffffffff;
}


void jite_destroy_instance(jit_function_t func)
{
    if(func->jite->incoming_params_state) jit_free(func->jite->incoming_params_state);
    jit_free(func->jite->vregs_table);
    jit_free(func->jite);

    func->jite = 0;
}

void jite_create_critical_point(jit_function_t func, jit_insn_t insn)
{
    jite_critical_point_t cpoint;
    if(insn && insn->cpoint==0)
    {
        cpoint = jit_memory_pool_alloc(&(func->builder->jite_critical_point_pool),
                                            struct _jite_critical_point);
        cpoint->is_branch_target = 0;
        cpoint->vregs_die = jit_memory_pool_alloc(&(func->builder->jite_list_pool),
                                            struct _jite_list);
        cpoint->vregs_born = jit_memory_pool_alloc(&(func->builder->jite_list_pool),
                                            struct _jite_list);
        cpoint->regs_state_1 = 0xffffffff;
        cpoint->regs_state_2 = 0xffffffff;
        insn->cpoint = cpoint;
    }
    /* if insn == 0 then there is no opcodes in this block */
}

void jite_add_vreg_to_complex_list(jit_function_t func, jite_vreg_t vreg, jite_list_t list)
{
    if(vreg && list)
    {
        jite_linked_list_t linked_list = 0;
        jit_type_t type = jit_value_get_type(vreg->value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            CASE_USE_WORD
            {
                if(!list->item1)
                {
                    list->item1 = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                }
                linked_list = (jite_linked_list_t)list->item1;
            }
            break;
            CASE_USE_LONG
            {
                if(!list->item2)
                {
                    list->item2 = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                }
                linked_list = (jite_linked_list_t)list->item2;
            }
            break;
            case JIT_TYPE_FLOAT32: 
            case JIT_TYPE_FLOAT64:
            {
                if(!list->item3)
                {
                    list->item3 = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                }
                linked_list = (jite_linked_list_t)list->item3;
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    if(!list->item4)
                    {
                        list->item4 = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                            struct _jite_linked_list);
                    }
                    linked_list = (jite_linked_list_t)list->item4;
                }
                else
                {
                    if(!list->item3)
                    {
                        list->item3 = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                            struct _jite_linked_list);
                    }
                    linked_list = (jite_linked_list_t)list->item3;
                }
            }
            break;
            default:
            {
                if(!list->item4)
                {
                    list->item4 = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                }
                linked_list = (jite_linked_list_t)list->item4;
            }
            break;
        }
        while(linked_list->next && linked_list->item != vreg)
        {
            linked_list = linked_list->next;
        }
        if(!linked_list->next && linked_list->item != vreg)
        {
            if(linked_list->item)
            {
                linked_list->next = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                linked_list->next->item = vreg;
                linked_list->next->prev = linked_list;
            }
            else
            {
                linked_list->item = vreg;
            }
        }
    }
}

void jite_remove_vreg_from_complex_list(jit_function_t func, jite_vreg_t vreg, jite_list_t list)
{
    if(vreg && list)
    {
        jite_linked_list_t linked_list = 0;
        jit_type_t type = jit_value_get_type(vreg->value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            CASE_USE_WORD
            {
                linked_list = (jite_linked_list_t)list->item1;
            }
            break;
            CASE_USE_LONG
            {
                linked_list = (jite_linked_list_t)list->item2;
            }
            break;
            case JIT_TYPE_FLOAT32: 
            case JIT_TYPE_FLOAT64:
            {
                linked_list = (jite_linked_list_t)list->item3;
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    linked_list = (jite_linked_list_t)list->item4;
                }
                else
                {
                    linked_list = (jite_linked_list_t)list->item3;
                }
            }
            break;
            default:
            {
                linked_list = (jite_linked_list_t)list->item4;
            }
            break;
        }
        if(linked_list)
        {
            while(linked_list->next && linked_list->item != vreg)
            {
                linked_list = linked_list->next;
            }
            if(linked_list->item == vreg)
            {
                if(linked_list->prev) /* not the first node */
                {
                    linked_list->prev->next = linked_list->next;
                    if(linked_list->next) linked_list->next->prev = linked_list->prev;
                }
                else /* the first node */
                {
                    if(linked_list == list->item1)
                    {
                        if(linked_list->next) 
                        {
                            list->item1 = linked_list->next;
                            linked_list->next->prev = 0;
                        }
                        else list->item1 = 0;
                    }
                    else if(linked_list == list->item2)
                    {
                        if(linked_list->next)
                        {
                            list->item2 = linked_list->next;
                            linked_list->next->prev = 0;
                        }
                        else list->item2 = 0;
                    }
                    else if(linked_list == list->item3)
                    {
                        if(linked_list->next)
                        {
                            list->item3 = linked_list->next;
                            linked_list->next->prev = 0;
                        }
                        else list->item3 = 0;
                    }
                    else if(linked_list == list->item4)
                    {
                        if(linked_list->next)
                        {
                            list->item4 = linked_list->next;
                            linked_list->next->prev = 0;
                        }
                        else list->item4 = 0;
                    }
                }
            }
        }
    }
}

unsigned char jite_replace_item_in_linked_list(jit_function_t func, void *dest_item, void *src_item, jite_linked_list_t linked_list)
{
    // TODO. Cleanup
    unsigned char updated = 0;
    if(linked_list && dest_item && src_item) /* cannot add a null value. */
    {
        while(linked_list)
        {
	    if(linked_list->item == dest_item)
	    {
	        linked_list->item = src_item;
		updated = 1;
	    }
            linked_list = linked_list->next;
        }
    }
    return updated;
}

void jite_clear_linked_list(jit_function_t func, jite_linked_list_t linked_list)
{
    if(linked_list)
    {
        linked_list->next = 0;
	linked_list->item = 0;
    }
}

jite_linked_list_t jite_add_item_no_duplicate_to_linked_list(jit_function_t func, void *item, jite_linked_list_t linked_list)
{
    // TODO. Cleanup
    if(linked_list && item) /* cannot add a null value. */
    {
        while(linked_list->next && linked_list->item != item)
        {
            linked_list = linked_list->next;
        }
        if(linked_list->item != item)
        {
            if(linked_list->item)
            {
                linked_list->next = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                linked_list->next->item = item;
                linked_list->next->prev = linked_list;
                return linked_list->next;
            }
            else
            {
                linked_list->item = item;
		return linked_list;
            }
        }
    }
    return (jite_linked_list_t)(0);
}


jite_linked_list_t jite_insert_item_after_linked_list(jit_function_t func, void *item, jite_linked_list_t linked_list)
{
    // TODO. Cleanup
    if(linked_list && item) /* cannot add a null value. */
    {
        jite_linked_list_t next_list = linked_list->next;
        // if(linked_list->item != item)
        {
            if(linked_list->item)
            {
                linked_list->next = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                linked_list->next->item = item;
                linked_list->next->prev = linked_list;
		linked_list->next->next = next_list;
		if(next_list)
		{
		    next_list->prev     = linked_list->next;
		}
	        return linked_list->next;
            }
            else
            {
                linked_list->item = item;
	        return linked_list;
            }
        }
    }
    return (jite_linked_list_t)(0);
}



jite_linked_list_t jite_add_item_to_linked_list(jit_function_t func, void *item, jite_linked_list_t linked_list)
{
    // TODO. Cleanup
    if(linked_list && item) /* cannot add a null value. */
    {
        while(linked_list->next && linked_list->item)
        {
            linked_list = linked_list->next;
        }
        {
            if(linked_list->item)
            {
                linked_list->next = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                        struct _jite_linked_list);
                linked_list->next->item = item;
                linked_list->next->prev = linked_list;
		return linked_list->next;
            }
            else
            {
                linked_list->item = item;
		return linked_list;
            }
        }
    }
    return (jite_linked_list_t)(0);
}


jite_linked_list_t jite_remove_item_from_linked_list(jit_function_t func, void *item, jite_linked_list_t list)
{
    jite_linked_list_t linked_list = list;

    if(list && item) /* Cannot remove a null value. */
    {
        if(linked_list)
        {
            while(linked_list->next && linked_list->item != item)
            {
                linked_list = linked_list->next;
            }
            if(linked_list->item == item)
            {
                if(linked_list->prev) /* not the first node */
                {
                    linked_list->prev->next = linked_list->next;
                    if(linked_list->next) linked_list->next->prev = linked_list->prev;
		    return linked_list->prev;
                }
                else /* the first node */
                {
                    if(list->next)
                    {
                        list->item = list->next->item;
			list->next = list->next->next;;
			if(list->next) list->next->prev = list;
                    }
                    else
		    {
		        list->item = 0;
		    }
		    return list;
                }
            }
        }
    }
    return (jite_linked_list_t)(0);
}


unsigned char jite_update_liveness_for_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to)
{
    if(!insn_from || !insn_to) return 0;
    jite_instance_t jite = func->jite;
    unsigned int range1 = insn_from->insn_num;
    unsigned int range2 = insn_to->insn_num;
    jite_linked_list_t list = jite->vregs_list;
    jite_vreg_t vreg;
    unsigned char updated = 0;
    while(list)
    {
        vreg = (jite_vreg_t)(list->item);
        list = list->next;
        if(!jit_value_is_temporary(vreg->value) && (vreg->min_range->insn_num != vreg->max_range->insn_num))
        {
            if(vreg->min_range->insn_num <= range2 && vreg->max_range->insn_num >= range2)
            {
                if(vreg->min_range->insn_num > range1)
                {
                    jite_add_vreg_to_complex_list(func, vreg, insn_from->cpoint->vregs_born);
                    jite_remove_vreg_from_complex_list(func, vreg, vreg->min_range->cpoint->vregs_born);
                    vreg->min_range = insn_from;
                    updated = 1;
                }
                else if(vreg->max_range->insn_num < range1)
                {
                    jite_add_vreg_to_complex_list(func, vreg, insn_from->cpoint->vregs_die);
                    jite_remove_vreg_from_complex_list(func, vreg, vreg->max_range->cpoint->vregs_die);
                    vreg->max_range = insn_from;
                    updated = 1;
                }
            }
        }
    }
    return updated;
}


void jite_add_fixup_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to)
{
    jite_list_t list = func->jite->branch_list;
    jite_list_t new_list = jit_memory_pool_alloc(&(func->builder->jite_list_pool),
                            struct _jite_list);
    if(list)
    {
        while(list->next)
            {
                list = list->next;
        }
        list->next = new_list;
    }
    else
    {
        func->jite->branch_list = new_list;
    }
    new_list->item1 = (void*)insn_from;
    new_list->item2 = (void*)insn_to;
    new_list->prev = list;
}

void jite_free_registers(jit_function_t func, jite_list_t list)
{
    jite_linked_list_t linked_list;
    if(func && list && func->jite)
    {
        linked_list = (jite_linked_list_t)list->item1;
        while(linked_list)
        {
            jite_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (jite_linked_list_t)list->item2;
        while(linked_list)
        {
            jite_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (jite_linked_list_t)list->item3;
        while(linked_list)
        {
            jite_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (jite_linked_list_t)list->item4;
        while(linked_list)
        {
            jite_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
    }
}


void jite_free_frames(jit_function_t func, jit_insn_t insn)
{
    jite_linked_list_t linked_list;
    jite_list_t list = insn->cpoint->vregs_die;

    if(func && list && func->jite)
    {
        linked_list = (jite_linked_list_t)list->item1;
        while(linked_list)
        {
	    jite_vreg_t vreg = (jite_vreg_t)(linked_list->item);

            if(vreg->max_range->insn_num == insn->insn_num) jite_free_vreg_frame(func, vreg);
            linked_list = linked_list->next;
        }
        linked_list = (jite_linked_list_t)list->item2;
        while(linked_list)
        {
	    jite_vreg_t vreg = (jite_vreg_t)(linked_list->item);

            if(vreg->max_range->insn_num == insn->insn_num) jite_free_vreg_frame(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (jite_linked_list_t)list->item3;
        while(linked_list)
        {
	    jite_vreg_t vreg = (jite_vreg_t)(linked_list->item);

            if(vreg->max_range->insn_num == insn->insn_num) jite_free_vreg_frame(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (jite_linked_list_t)list->item4;
        while(linked_list)
        {
	    jite_vreg_t vreg = (jite_vreg_t)(linked_list->item);

            if(vreg->max_range->insn_num == insn->insn_num) jite_free_vreg_frame(func, linked_list->item);
            linked_list = linked_list->next;
        }
    }
}

void jite_free_vreg_frame(jit_function_t func, jite_vreg_t vreg)
{
    if(vreg && vreg->frame && vreg->frame->hash_code != -1)
    {
        jite_frame_t frame = vreg->frame;
        jite_list_t temp;
        int count;
        for(count = 0; count < frame->length; count++)
        {
            unsigned int offset = (frame->hash_code + count) % 96;
            unsigned int segment = (frame->hash_code + count) / 96;
            unsigned int index;
            temp = func->jite->frame_state;
            for(index = 0; index < segment; index++) /* find the right list */
            {
                if(temp->next) temp = temp->next;
                else
                {
                    return;
                }
            }
            /* now, at temp is the right list */
            segment = offset / 32;
            offset = offset % 32;
            switch(segment)
            {
                case 0:
                {
                    temp->item1 = (void*)((unsigned int)(temp->item1) & ~(1 << offset));
                }        
                break;
                case 1:
                {
                    temp->item2 = (void*)((unsigned int)(temp->item2) & ~(1 << offset));
                }
                break;
                case 2:
                {
                    temp->item3 = (void*)((unsigned int)(temp->item3) & ~(1 << offset));
                }
                break;
            }
        }
    }
}

void jite_free_frame(jit_function_t func, jite_frame_t frame)
{
    if(frame && frame->hash_code != - 1)
    {
        jite_list_t temp;
        int count;
        for(count = 0; count < frame->length; count++)
        {
            unsigned int offset = (frame->hash_code + count) % 96;
            unsigned int segment = (frame->hash_code + count) / 96;
            unsigned int index;
            temp = func->jite->frame_state;
            for(index = 0; index < segment; index++) /* find the right list */
            {
                if(temp->next) temp = temp->next;
                else
                {
                    return;
                }
            }
            /* now, at temp is the right list */
            segment = offset / 32;
            offset = offset % 32;
            switch(segment)
            {
                case 0:
                {
                    temp->item1 = (void*)((unsigned int)(temp->item1) & ~(1 << offset));
                }        
                break;
                case 1:
                {
                    temp->item2 = (void*)((unsigned int)(temp->item2) & ~(1 << offset));
                }
                break;
                case 2:
                {
                    temp->item3 = (void*)((unsigned int)(temp->item3) & ~(1 << offset));
                }
                break;
            }
        }
    }
}

int jite_count_vregs(jit_function_t func, jite_list_t list)
{
    int num = 0;
    unsigned int segment, offset;    
    if(func && list && func->jite)
    {
        jite_list_t temp = list;
        while(temp)
        {
            for(segment = 0; segment < 3; segment++)
            {
                for(offset = 0; offset < 32; offset++)
                {
                    switch(segment)
                    {
                        case 0:
                        {
                            num += ((unsigned int)(temp->item1) & (1 << offset))!=0;
                        }
                        break;
                        case 1:
                        {
                            num += ((unsigned int)(temp->item2) & (1 << offset))!=0;
                        }
                        break;
                        case 2:
                        {
                            num += ((unsigned int)(temp->item3) & (1 << offset))!=0;
                        }
                        break;
                    }
                }
            }
            temp = temp->next;
        }
    }
    return num;
}

void jite_value_set_weight(jit_value_t value, unsigned int weight)
{
    if(value->vreg == 0)
    {
         value->vreg = jite_create_vreg(value);
    }
    value->vreg->weight = weight;
}

unsigned int jite_vreg_weight(jite_vreg_t vreg)
{
    return vreg->weight;
}

unsigned int jite_vreg_get_weight(jite_vreg_t vreg)
{
    return jite_vreg_weight(vreg);
}


unsigned int jite_value_get_weight(jit_value_t value)
{
    return value->vreg->weight;
}


void jite_value_set_weight_using_insn(jit_value_t value, jit_insn_t insn)
{
    return;


    unsigned int num_use = 0;
    jite_linked_list_t list = value->vreg->liveness;
    while(list && list->item)
    {
        jit_insn_t min_insn = ((jit_insn_t)(list->item));
	list = list->next;
	jit_insn_t max_insn = ((jit_insn_t)(list->item));
	list = list->next;
	if(min_insn->insn_num <= insn->insn_num && max_insn->insn_num >= insn->insn_num)
	{
	    num_use += max_insn->insn_num - insn->insn_num;
	}
	else if(insn->insn_num >= min_insn->insn_num)
	{
	    num_use += max_insn->insn_num - min_insn->insn_num;
	}
    }
    
    jite_value_set_weight(value, (value->vreg->max_range->insn_num - insn->insn_num) - num_use);
}


jite_vreg_t jite_regs_higher_criteria(jite_vreg_t vreg1, jite_vreg_t vreg2)
{
    if(vreg1 && vreg2)
    {
        if(vreg1->weight < vreg2->weight) return vreg1;
        return vreg2;
    }
    else if(vreg1) return vreg1;
    return vreg2;
}

jit_value_t jite_vreg_get_value(jite_vreg_t vreg)
{
    return vreg->value;
}

void jite_add_branch_target(jit_function_t func, jit_insn_t insn, jit_label_t label)
{
    jit_block_t temp_block = jit_block_from_label(func, label);
    jit_insn_iter_t temp_iter;
    jit_insn_t dest_insn;
    /* We find the first block, in which there is at least one opcode.
       We need this because of a possible case of two blocks which
       follow one another, when the first block does not contain opcodes. */
    do
    {
        jit_insn_iter_init(&temp_iter, temp_block);
        do
        {
            dest_insn = jit_insn_iter_next(&temp_iter);
        }
        while(dest_insn && (dest_insn->opcode == JIT_OP_MARK_OFFSET ||
	        dest_insn->opcode == JIT_OP_NOP ||
                dest_insn->opcode == JIT_OP_INCOMING_REG ||
                dest_insn->opcode == JIT_OP_INCOMING_FRAME_POSN));
                temp_block = jit_block_next(func, temp_block);
    } while(dest_insn == 0);

    jite_add_fixup_branch(func, insn, dest_insn);
    jite_create_critical_point(func, dest_insn);
}


void jite_compute_values_weight(jit_function_t func)
{
    int type = jite_function_get_register_allocator_euristic(func);
    unsigned int vregs_num = func->jite->vregs_num;
    unsigned int index = 0;

    switch(type)
    {

	case NUMBER_OF_USE_EURISTIC:
	{
    	    for(index = 0; index < vregs_num; index++)
    	    {
        	jite_vreg_t vreg = func->jite->vregs_table[index];

  		if(vreg->max_range != vreg->min_range)
		{
		    jite_value_set_weight(vreg->value, func->builder->num_insns - vreg->value->usage_count);
		    jite_value_set_weight(vreg->value, 1);
  		}
		else jite_value_set_weight(vreg->value, -1);
    	    }
	}
	break;

	case LINEAR_SCAN_EURISTIC:
	{
    	    for(index = 0; index < vregs_num; index++)
    	    {
        	jite_vreg_t vreg = func->jite->vregs_table[index];
  		if(vreg->max_range) jite_value_set_weight(vreg->value, vreg->max_range->insn_num);
		else jite_value_set_weight(vreg->value, -1);
    	    }
	}
	break;
    }
}

void jite_compute_values_weight_using_insn(jit_function_t func, jit_insn_t insn)
{
    return;
    int type = jite_function_get_register_allocator_euristic(func);
    unsigned int vregs_num = func->jite->vregs_num;
    unsigned int index = 0;

    switch(type)
    {

	case NUMBER_OF_USE_EURISTIC:
	{
    	    for(index = 0; index < vregs_num; index++)
    	    {
        	jite_vreg_t vreg = func->jite->vregs_table[index];

  		if(vreg->max_range != vreg->min_range)
		{
		    jite_value_set_weight(vreg->value, func->builder->num_insns - vreg->value->usage_count);

		    jite_value_set_weight(vreg->value, 1);
  		}
		else jite_value_set_weight(vreg->value, -1);
    	    }
	}
	break;

	case LINEAR_SCAN_EURISTIC:
	{
    	    for(index = 0; index < vregs_num; index++)
    	    {
        	jite_vreg_t vreg = func->jite->vregs_table[index];
  		if(vreg->max_range) jite_value_set_weight_using_insn(vreg->value, insn);
		else jite_value_set_weight(vreg->value, -1);
    	    }
	}
	break;
    }
}




void jite_compute_liveness(jit_function_t func)
{
    _jit_function_compute_liveness(func);

    int level = jit_function_get_optimization_level(func);

    switch(level)
    {
        case 0:
	case 1:
	{
            jite_compute_local_liveness(func);
	    jite_create_vregs_table(func);
	}
	break;
	case 2:
	{
            jite_compute_fast_liveness(func);
	    jite_create_vregs_table(func);
	}
	break;
	default:
	{
            jite_compute_full_liveness(func);
        }
	break;
    }
    
    jite_compute_register_holes(func);
}

void jite_create_vregs_table(jit_function_t func)
{
    jite_linked_list_t linked_list = func->jite->vregs_list;
    jite_vreg_t *vregs_table = jit_malloc(func->jite->vregs_num * sizeof(jite_vreg_t));

    while(linked_list != 0)
    {
        jite_vreg_t vreg = (jite_vreg_t)(linked_list->item);
	vregs_table[vreg->index] = vreg;
        linked_list = linked_list->next;
    }

    func->jite->vregs_table = vregs_table;
}


void jite_compute_local_liveness(jit_function_t func)
{
    jit_block_t block;
    jit_insn_iter_t iter;
    jit_insn_t insn;

    jite_linked_list_t linked_list;

    jit_insn_t min_insn = 0, max_insn = 0;
    

    /* Step 1. Compute vregs liveness without branches. */
    block = 0;

    while((block = jit_block_next(func, block)) != 0)
    {
        if(!(block->entered_via_top) && !(block->entered_via_branch))
        {
            continue;
        }
        if(block->entered_via_branch)
        {    
            jit_insn_iter_init(&iter, block);
            insn = jit_insn_iter_next(&iter);
            jite_create_critical_point(func, insn); /* Points to which there exist branches. */
        }
        jit_insn_iter_init(&iter, block);
        while((insn = jit_insn_iter_next(&iter)) != 0)
        {
            jit_value_t dest = insn->dest;
            jit_value_t value1 = insn->value1;
            jit_value_t value2 = insn->value2;
	    if(insn && (insn->opcode == JIT_OP_NOP || insn->opcode == JIT_OP_MARK_OFFSET))
	    {
	        continue;
            }


            if(insn && (insn->flags & JIT_INSN_DEST_IS_LABEL) !=0)
            {
                jite_create_critical_point(func, insn); /* Point from which there is a branch. */
                jite_add_branch_target(func, insn, (jit_label_t)dest);
            }

            if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
                && !(dest->is_constant)
                && !(dest->is_nint_constant)
                && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
                && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
            {
                if(!jite_value_is_in_vreg(dest))
                {
                    jite_create_vreg( dest);
                }
                if(!dest->vreg->min_range)
                {
                    dest->vreg->min_range = insn;
                }
                if(!dest->vreg->max_range ||
                    (dest->vreg->max_range && (dest->vreg->max_range->insn_num < insn->insn_num)))
                {
                    dest->vreg->max_range = insn;
                }                    
            }
            if(insn && value1 && !(insn->flags & JIT_INSN_VALUE1_IS_NAME)
                && !(insn->flags & JIT_INSN_VALUE1_IS_LABEL) && !value1->is_constant
                && !value1->is_nint_constant)
            {
                if(!jite_value_is_in_vreg(value1))
                {
                    jite_create_vreg( value1);
                }
                if(!value1->vreg->min_range)
                {
                    value1->vreg->min_range = insn;
                }
                if(!value1->vreg->max_range ||
                    (value1->vreg->max_range && (value1->vreg->max_range->insn_num < insn->insn_num)))
                {
                    value1->vreg->max_range = insn;
                }    
            }
            if(insn && value2 && !(insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE)
                && !value2->is_constant && !value2->is_nint_constant)
            {
                if(!jite_value_is_in_vreg(value2))
                {
                    jite_create_vreg( value2);
                }
                if(!value2->vreg->min_range)
                {
                    value2->vreg->min_range = insn;
                }
                if(!value2->vreg->max_range ||
                    (value2->vreg->max_range && (value2->vreg->max_range->insn_num < insn->insn_num)))
                {
                    value2->vreg->max_range = insn;
                }
            }
            if(insn && insn->opcode != JIT_OP_INCOMING_REG && insn->opcode != JIT_OP_INCOMING_FRAME_POSN
                    && insn->opcode != JIT_OP_MARK_OFFSET && insn->opcode != JIT_OP_NOP)
            {
                if(min_insn == 0) min_insn = insn;
                max_insn = insn;
            }
            if(insn && insn->opcode == JIT_OP_JUMP_TABLE)
            {
                /* Process every possible target label. */
                jite_create_critical_point(func, insn);
                jit_label_t *labels = (jit_label_t*)(insn->value1->address);
                jit_nint num_labels = (jit_nint)(insn->value2->address);
                int index;
                for(index = 0; index < num_labels; index++)
                {
                    jite_add_branch_target(func, insn, labels[index]);
                }
            }	    
        }
    }

    /* Step 2. Mark critical points in opcodes of where new vregs are born and are destroyed. */
    linked_list = func->jite->vregs_list;
    while(linked_list)
    {
        jite_vreg_t vreg = (jite_vreg_t)(linked_list->item);
        linked_list = linked_list->next;
        if(jit_value_is_addressable(vreg->value))
        {
            if(!vreg->min_range
                || (vreg->min_range && vreg->min_range->insn_num > min_insn->insn_num))
            {
                vreg->min_range = min_insn;
            }
            vreg->max_range = max_insn;
        }
	else if(!jit_value_is_temporary(vreg->value) || jit_value_is_local(vreg->value))
	{
            if(!vreg->min_range
                || (vreg->min_range && vreg->min_range->insn_num > min_insn->insn_num))
            {
                vreg->min_range = min_insn;
            }
            vreg->max_range = max_insn;
	}

        if(vreg->min_range == 0) /* Remove this later. */
        {
            vreg->min_range = min_insn;
            vreg->max_range = min_insn;
        }

        if(vreg->min_range != vreg->max_range)
        {
            jite_create_critical_point(func, vreg->min_range);
            jite_add_vreg_to_complex_list(func, vreg, vreg->min_range->cpoint->vregs_born);
            jite_create_critical_point(func, vreg->max_range);
            jite_add_vreg_to_complex_list(func, vreg, vreg->max_range->cpoint->vregs_die);
        }
    }


    jite_linked_list_t vregItem = func->jite->vregs_list;
    while(vregItem)
    {
        jite_vreg_t vreg = (jite_vreg_t)(vregItem->item);
	if(vreg && vreg->value)
	{
            if(vreg->min_range && vreg->max_range)
	    {
	    	vreg->liveness = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                         struct _jite_linked_list);

	        jite_add_item_to_linked_list(func, vreg->min_range, vreg->liveness);
	        jite_add_item_to_linked_list(func, vreg->max_range, vreg->liveness);
	    }
	}
	vregItem = vregItem->next;
    }
}


void jite_compute_fast_liveness(jit_function_t func)
{
    jit_block_t block;
    jit_insn_iter_t iter;
    jit_insn_t insn;

    jite_linked_list_t linked_list;
    jite_list_t list;
    jit_insn_t min_insn = 0, max_insn = 0;
    unsigned char updated;
    /* Step 1. Compute vregs liveness without branches. */
    block = 0;

    while((block = jit_block_next(func, block)) != 0)
    {
        if(!(block->entered_via_top) && !(block->entered_via_branch))
        {
            continue;
        }
        if(block->entered_via_branch)
        {    
            jit_insn_iter_init(&iter, block);
            insn = jit_insn_iter_next(&iter);
            jite_create_critical_point(func, insn); /* Points to which there exist branches. */
        }
        jit_insn_iter_init(&iter, block);
        while((insn = jit_insn_iter_next(&iter)) != 0)
        {
            jit_value_t dest = insn->dest;
            jit_value_t value1 = insn->value1;
            jit_value_t value2 = insn->value2;
	    if(insn && (insn->opcode == JIT_OP_NOP || insn->opcode == JIT_OP_MARK_OFFSET))
	    {
	        continue;
            }


            if(insn && (insn->flags & JIT_INSN_DEST_IS_LABEL) !=0)
            {
                jite_create_critical_point(func, insn); /* Point from which there is a branch. */
                jite_add_branch_target(func, insn, (jit_label_t)dest);
            }

            if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
                && !(dest->is_constant)
                && !(dest->is_nint_constant)
                && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
                && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
            {
                if(!jite_value_is_in_vreg(dest))
                {
                    jite_create_vreg( dest);
                }
                if(!dest->vreg->min_range)
                {
                    dest->vreg->min_range = insn;
                }
                if(!dest->vreg->max_range ||
                    (dest->vreg->max_range && (dest->vreg->max_range->insn_num < insn->insn_num)))
                {
                    dest->vreg->max_range = insn;
                }                    
            }
            if(insn && value1 && !(insn->flags & JIT_INSN_VALUE1_IS_NAME)
                && !(insn->flags & JIT_INSN_VALUE1_IS_LABEL) && !value1->is_constant
                && !value1->is_nint_constant)
            {
                if(!jite_value_is_in_vreg(value1))
                {
                    jite_create_vreg( value1);
                }
                if(!value1->vreg->min_range)
                {
                    value1->vreg->min_range = insn;
                }
                if(!value1->vreg->max_range ||
                    (value1->vreg->max_range && (value1->vreg->max_range->insn_num < insn->insn_num)))
                {
                    value1->vreg->max_range = insn;
                }    
            }
            if(insn && value2 && !(insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE)
                && !value2->is_constant && !value2->is_nint_constant)
            {
                if(!jite_value_is_in_vreg(value2))
                {
                    jite_create_vreg( value2);
                }
                if(!value2->vreg->min_range)
                {
                    value2->vreg->min_range = insn;
                }
                if(!value2->vreg->max_range ||
                    (value2->vreg->max_range && (value2->vreg->max_range->insn_num < insn->insn_num)))
                {
                    value2->vreg->max_range = insn;
                }
            }
            if(insn && insn->opcode != JIT_OP_INCOMING_REG && insn->opcode != JIT_OP_INCOMING_FRAME_POSN
                    && insn->opcode != JIT_OP_MARK_OFFSET && insn->opcode != JIT_OP_NOP)
            {
                if(min_insn == 0) min_insn = insn;
                max_insn = insn;
            }
            if(insn && insn->opcode == JIT_OP_JUMP_TABLE)
            {
                /* Process every possible target label. */
                jite_create_critical_point(func, insn);
                jit_label_t *labels = (jit_label_t*)(insn->value1->address);
                jit_nint num_labels = (jit_nint)(insn->value2->address);
                int index;
                for(index = 0; index < num_labels; index++)
                {
                    jite_add_branch_target(func, insn, labels[index]);
                }
            }	    
        }
    }

    /* Step 2. Mark critical points in opcodes of where new vregs are born and are destroyed. */
    linked_list = func->jite->vregs_list;
    while(linked_list)
    {
        jite_vreg_t vreg = (jite_vreg_t)(linked_list->item);
        linked_list = linked_list->next;
        if(jit_value_is_addressable(vreg->value))
        {
            if(!vreg->min_range
                || (vreg->min_range && vreg->min_range->insn_num > min_insn->insn_num))
            {
                vreg->min_range = min_insn;
            }
            vreg->max_range = max_insn;
        }
        if(vreg->min_range == 0) /* Remove this later. */
        {
            vreg->min_range = min_insn;
            vreg->max_range = min_insn;
        }

        if(vreg->min_range != vreg->max_range)
        {
            jite_create_critical_point(func, vreg->min_range);
            jite_add_vreg_to_complex_list(func, vreg, vreg->min_range->cpoint->vregs_born);
            jite_create_critical_point(func, vreg->max_range);
            jite_add_vreg_to_complex_list(func, vreg, vreg->max_range->cpoint->vregs_die);
        }
    }

    /* Step 3. Compute vregs liveness with branches. */
    do
    {
        updated = 0;
        jite_list_t last_item = func->jite->branch_list;
        list = func->jite->branch_list;
        while(list)
        {
            unsigned char new_update = jite_update_liveness_for_branch(func, (jit_insn_t)(list->item1), (jit_insn_t)(list->item2));
            updated = updated || new_update;
            list = list->next;
            if(list) last_item = list;
        }
        if(!updated) break;
        updated = 0;
        list = last_item;
        while(list)
        {
            unsigned char new_update = jite_update_liveness_for_branch(func, (jit_insn_t)(list->item1), (jit_insn_t)(list->item2));
            updated = updated || new_update;
            list = list->prev;
        }
    } while(updated);


    jite_linked_list_t vregItem = func->jite->vregs_list;
    while(vregItem)
    {
        jite_vreg_t vreg = (jite_vreg_t)(vregItem->item);
	if(vreg && vreg->value)
	{
            if(vreg->min_range && vreg->max_range)
	    {
	    	vreg->liveness = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                         struct _jite_linked_list);

	        jite_add_item_to_linked_list(func, vreg->min_range, vreg->liveness);
	        jite_add_item_to_linked_list(func, vreg->max_range, vreg->liveness);
	    }
	}
	vregItem = vregItem->next;
    }
}



void jite_liveness_node_remove_bit(jit_uint *node, jit_uint index)
{
    unsigned int dwordOffset = index / (sizeof(jit_uint) * 8);
    unsigned int bitOffset   = index % (sizeof(jit_uint) * 8);
    node[dwordOffset] = node[dwordOffset] & ~(1 << bitOffset);
}

void jite_liveness_node_add_bit(jit_uint *node, jit_uint index)
{
    unsigned int dwordOffset = index / (sizeof(jit_uint) * 8);
    unsigned int bitOffset   = index % (sizeof(jit_uint) * 8);
    node[dwordOffset] = node[dwordOffset] | (1 << bitOffset);
}

unsigned int jite_liveness_node_get_bit(jit_uint *node, jit_uint index)
{
    unsigned int dwordOffset = index / (sizeof(jit_uint) * 8);
    unsigned int bitOffset   = index % (sizeof(jit_uint) * 8);
    if(node[dwordOffset] & (1 << bitOffset)) return 1;
    return 0;
}


void jite_liveness_remove_nodes(jit_uint *dest_node, jit_uint *src_node1, jit_uint *src_node2, jit_int size)
{
    while( size > 0 )
    {
        size--;
        dest_node[size] = src_node1[size] & ~src_node2[size];
    }
}

void jite_liveness_unit_nodes(jit_uint *dest_node, jit_uint *src_node1, jit_uint *src_node2, jit_int size)
{
    while( size > 0 )
    {
        size--;
        dest_node[size] = src_node1[size] | src_node2[size];
    }
}

void jite_liveness_intersect_nodes(jit_uint *dest_node, jit_uint *src_node1, jit_uint *src_node2, jit_int size)
{
    while( size > 0 )
    {
        size--;
        dest_node[size] = src_node1[size] & src_node2[size];
    }
}


jit_insn_t jite_get_insn_from_label(jit_function_t func, jit_label_t dest)
{
    jit_block_t temp_block = jit_block_from_label(func, dest);
    jit_insn_iter_t temp_iter;
    jit_insn_t dest_insn;
    /* We find the first block, in which there is at least one opcode.
       We need this because of a possible case of two blocks which
       follow one another, when the first block does not contain opcodes. */
    do
    {
	jit_insn_iter_init(&temp_iter, temp_block);
	do
	{
	    dest_insn = jit_insn_iter_next(&temp_iter);
	}
	while (dest_insn && (dest_insn->opcode == JIT_OP_MARK_OFFSET || dest_insn->opcode == JIT_OP_NOP));
	temp_block = jit_block_next(func, temp_block);
    } while(dest_insn == 0);
    return dest_insn;
}

jit_insn_t jite_get_insn_from_block(jit_function_t func, jit_block_t block)
{
    jit_insn_iter_t temp_iter;
    jit_insn_t dest_insn;
    /* We find the first block, in which there is at least one opcode.
       We need this because of a possible case of two blocks which
       follow one another, when the first block does not contain opcodes. */
    do
    {
	jit_insn_iter_init(&temp_iter, block);
	do
	{
	    dest_insn = jit_insn_iter_next(&temp_iter);
	}
	while (dest_insn && (dest_insn->opcode == JIT_OP_MARK_OFFSET || dest_insn->opcode == JIT_OP_NOP));
	block = jit_block_next(func, block);
    } while(dest_insn == 0);
    return dest_insn;
}


unsigned char jite_vreg_lives_at_insn(jite_vreg_t vreg, jit_insn_t insn)
{
    jite_linked_list_t list = vreg->liveness;
    while(list && list->item)
    {
        jit_insn_t min_insn = ((jit_insn_t)(list->item));
	list = list->next;
	jit_insn_t max_insn = ((jit_insn_t)(list->item));
	list = list->next;
	if(min_insn->insn_num <= insn->insn_num && max_insn->insn_num >= insn->insn_num)
	{
	    return 1;
	}
    }
    return 0;
}


void jite_compute_full_liveness(jit_function_t func)
{
    jite_linked_list_t blocks = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                        struct _jite_linked_list);

    jite_linked_list_t reverse_nodes = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                        struct _jite_linked_list);

    jite_linked_list_t finally_clauses = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                                       struct _jite_linked_list);
    jite_linked_list_t last_clause = finally_clauses;


    jit_block_t block;
    jit_insn_iter_t iter;
    jit_insn_t insn = 0;

    jit_insn_t min_insn = 0, max_insn = 0;
    block = 0;
    jit_insn_t first_insn = 0;
    
    jit_insn_t insn_enter_finally = 0;
    
    /* Create virtual registers */
    while((block = jit_block_next(func, block)) != 0)
    {
        if(!block->entered_via_top && !block->entered_via_branch)
        {
            continue;
        }
	

        jit_insn_iter_init(&iter, block);
        while((insn = jit_insn_iter_next(&iter)) != 0)
        {
            jit_value_t dest = insn->dest;
            jit_value_t value1 = insn->value1;
            jit_value_t value2 = insn->value2;
	    if(insn && (insn->opcode == JIT_OP_NOP || insn->opcode == JIT_OP_MARK_OFFSET))
	    {
	        continue;
            }
	    else if(first_insn == 0)
	    {
	        first_insn = insn;
	    }


            if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
                && !(dest->is_constant)
                && !(dest->is_nint_constant)
                && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
                && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
            {
                if(!jite_value_is_in_vreg(dest))
                {
                    jite_create_vreg( dest);
                }
            }
            if(insn && value1 && !(insn->flags & JIT_INSN_VALUE1_IS_NAME)
                && !(insn->flags & JIT_INSN_VALUE1_IS_LABEL) && !value1->is_constant
                && !value1->is_nint_constant)
            {
                if(!jite_value_is_in_vreg(value1))
                {
                    jite_create_vreg( value1);
                }
            }
            if(insn && value2 && !(insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE)
                && !value2->is_constant && !value2->is_nint_constant)
            {
                if(!jite_value_is_in_vreg(value2))
                {
                    jite_create_vreg( value2);
                }
            }
            if(insn && insn->opcode != JIT_OP_INCOMING_REG && insn->opcode != JIT_OP_INCOMING_FRAME_POSN
                    && insn->opcode != JIT_OP_MARK_OFFSET && insn->opcode != JIT_OP_NOP)
            {
                if(min_insn == 0) min_insn = insn;
                max_insn = insn;
            }


            /* Store information about finally clauses. CALL_FINALLY jumps to code starting from ENTER_FINALLY.
	       ENTER_FINALLY can be entered via multiple places. LEAVE_FINALLY should return to next opcode after CALL_FINALLY. */
	    if(insn && insn->opcode == JIT_OP_CALL_FINALLY)
	    {
                jit_insn_t insn_next = jite_get_insn_from_label(func, (jit_label_t)(insn->dest));
		jit_block_t block_next = jit_block_next(func, block);

		if(insn_next->opcode == JIT_OP_ENTER_FINALLY && insn_next->dest == 0)
		{
		    jite_linked_list_t return_targets = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                                       struct _jite_linked_list);
		    insn_next->dest = (jit_value_t)(return_targets);
		    insn_next->flags |= JIT_INSN_DEST_IS_NATIVE;
		    /* Set this flag that other parts of the code handle this extra info in dest field correctly. */
                }
		else if(insn_next->opcode != JIT_OP_ENTER_FINALLY)
		{
		    /* TODO. Throw here a runtime exception. */
		    fprintf(stderr, "JIT_OP_CALL_FINALLY does not jump to JIT_OP_ENTER_FINALLY\n");
        	}
	
		jite_add_item_to_linked_list(func, block_next, (jite_linked_list_t)insn_next->dest);
	    }

	    if(insn && insn->opcode == JIT_OP_ENTER_FINALLY)
	    {
	        last_clause = jite_insert_item_after_linked_list(func, insn, last_clause);
	    }

	    if(insn && insn->opcode == JIT_OP_LEAVE_FINALLY)
	    {
               if(last_clause->item)
	       {
	           insn_enter_finally = (jit_insn_t) last_clause->item;
	           last_clause = jite_remove_item_from_linked_list(func, insn_enter_finally, last_clause);
	       }
	       

               /* This is a leave_finally, without an enter_finally. This case means that this opcode is never executed
	          or there is a bug in the program. */

	       insn->dest = insn_enter_finally->dest;

	       insn->flags |= JIT_INSN_DEST_IS_NATIVE;
	    }
        }
    }
    
    jite_create_vregs_table(func);


    unsigned int modelSize = func->builder->num_insns;
    unsigned int vregsNum = func->jite->vregs_num;

    unsigned int nodeLength = (vregsNum + sizeof(jit_uint) * 4) / (sizeof(jit_uint) * 4);

    /* Compute IN, OUT, DEF, USED bit vectors */
    jit_uint *model = malloc(modelSize * 4 * nodeLength * sizeof(jit_uint));
    jit_uint *deadcode  = malloc(modelSize  * 4 * nodeLength * sizeof(jit_uint));
    jit_uint *matches = malloc(modelSize * 4 * nodeLength * sizeof(jit_uint));

    memset(model, 0, modelSize * 4 * nodeLength * sizeof(jit_uint));
    memset(deadcode, 0, modelSize * 4 * nodeLength * sizeof(jit_uint));


    unsigned int inOffset = 0, outOffset = nodeLength, defOffset = nodeLength * 2, usedOffset = nodeLength * 3;
    unsigned int sideEffectOffset = 0, defEffectOffset = nodeLength, candidatesInOffset = nodeLength * 2, candidatesOutOffset = nodeLength * 3;


    block = 0;
    jit_insn_t insn_prev = 0;

    while((block = jit_block_next(func, block)) != 0)
    {
    	if(!block->entered_via_top)
	{
	    insn_prev = 0;
	}

        if(!block->entered_via_top && !block->entered_via_branch)
        {
            continue;
        }

        jit_insn_iter_init(&iter, block);
        while((insn = jit_insn_iter_next(&iter)) != 0)
        {
            jit_value_t dest   = insn->dest;
            jit_value_t value1 = insn->value1;
            jit_value_t value2 = insn->value2;

	    if(jite_insn_has_side_effect(insn))
	    {
	        jite_liveness_node_add_bit(&deadcode[insn->insn_num * nodeLength * 4 + defEffectOffset], vregsNum);
	    }

	    jite_liveness_node_add_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], vregsNum);


	    if(insn && (insn->opcode == JIT_OP_NOP || insn->opcode == JIT_OP_MARK_OFFSET))
	    {
	        continue;
            }

            if(insn_prev)
	    {
	        insn_prev->next = insn;
		insn->prev = insn_prev;
	    }

	    insn_prev = insn;




            if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
                && !(dest->is_constant)
                && !(dest->is_nint_constant)
                && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
                && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
            {
		if(jite_insn_dest_defined(insn))
		{
		    jite_liveness_node_add_bit(&model[insn->insn_num * nodeLength * 4 + defOffset], dest->vreg->index);
		}
		else
		{
		    jite_liveness_node_add_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], dest->vreg->index);

		    if(jite_insn_has_side_effect(insn))
		    {
	                jite_liveness_node_add_bit(&deadcode[insn->insn_num * nodeLength * 4 + defEffectOffset], dest->vreg->index);
		    }
		}
            }
            if(insn && value1 && !(insn->flags & JIT_INSN_VALUE1_IS_NAME)
                    && !(insn->flags & JIT_INSN_VALUE1_IS_LABEL) && !value1->is_constant
                    && !value1->is_nint_constant)
            {
		if(insn->opcode == JIT_OP_INCOMING_REG || insn->opcode == JIT_OP_INCOMING_FRAME_POSN
		                                       || insn->opcode == JIT_OP_RETURN_REG)
		{
		    jite_liveness_node_add_bit(&model[insn->insn_num * nodeLength * 4 + defOffset], value1->vreg->index);
		}
		else
		{
		    jite_liveness_node_add_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], value1->vreg->index);

		    if(jite_insn_has_side_effect(insn))
		    {
	                jite_liveness_node_add_bit(&deadcode[insn->insn_num * nodeLength * 4 + defEffectOffset], value1->vreg->index);
		    }
		}
            }
            if(insn && value2 && !(insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE)
                    && !value2->is_constant && !value2->is_nint_constant)
            {
		jite_liveness_node_add_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], value2->vreg->index);

		if(jite_insn_has_side_effect(insn))
		{
	            jite_liveness_node_add_bit(&deadcode[insn->insn_num * nodeLength * 4 + defEffectOffset], value2->vreg->index);
		}
            }
        }

	if(jit_block_ends_in_dead(block))
	{
            insn_prev = 0;
	}
    }


    jite_linked_list_t current_node = blocks;

    jite_linked_list_t reverse_node = reverse_nodes;

    block = jit_block_next(func, 0); /* Find the first block in the current method */
    jite_linked_list_t new_node = jite_insert_item_after_linked_list(func, block, current_node);
    new_node = jite_insert_item_after_linked_list(func, block, reverse_node);
    block->list = new_node;


    /* We consider that a conditional branch or an indexed branch is taken */
    while(current_node && current_node->item)
    {
        jite_linked_list_t next_node   = 0; // current_node;
	jite_linked_list_t branch_node = 0;	

	
        block = current_node->item;

        if((!block->entered_via_top && !block->entered_via_branch))
	{
	    current_node = current_node->next;
	    continue;
	}

	if(block->analysed)
	{
	    current_node = current_node->next;
	    continue;
	}


	block->analysed = 1;

	if(!jit_block_ends_in_dead(block))
	{
	    jit_block_t block_next = jit_block_next(func, block);
	    if(!block_next->analysed)
	    {
	        next_node = jite_insert_item_after_linked_list(func, block_next, current_node);
	    }
	}


        jit_insn_iter_init(&iter, block);
        while((insn = jit_insn_iter_next(&iter)) != 0)
        {
	    if(insn && (insn->opcode == JIT_OP_NOP || insn->opcode == JIT_OP_MARK_OFFSET))
	    {
	        continue;
            }

            if(insn && (insn->flags & JIT_INSN_DEST_IS_LABEL) !=0)
            {
	        jit_block_t block_next = jit_block_from_label(func, (jit_label_t)(insn->dest));
                if(!block_next->analysed)
		{
 	     	    jite_insert_item_after_linked_list(func, block_next, current_node);
		    branch_node = jite_insert_item_after_linked_list(func, block_next, block->list);
                    block_next->list = branch_node;
		}
            }

            if(insn && insn->opcode == JIT_OP_JUMP_TABLE)
            {
                /* Process every possible target label. */
                jit_label_t *labels = (jit_label_t*)(insn->value1->address);
                jit_nint num_labels = (jit_nint)(insn->value2->address);
                int index;
                for(index = num_labels - 1; index >= 0; index--)
                {
                    jit_block_t block_next = jit_block_from_label(func, (jit_label_t)(labels[index]));
                    if(!block_next->analysed)
		    {
	                jite_insert_item_after_linked_list(func, block_next, current_node);
	                branch_node = jite_insert_item_after_linked_list(func, block_next, block->list);
                        block_next->list = branch_node;
		    }
                }
            }
	    
	    if(insn && insn->opcode == JIT_OP_LEAVE_FINALLY)
	    {
	        jite_linked_list_t next_blocks = (jite_linked_list_t)(insn->dest);
		while(next_blocks && next_blocks->item)
		{
	            jit_block_t block_next = (jit_block_t)(next_blocks->item);
                    if(!block_next->analysed)
		    {
	                jite_insert_item_after_linked_list(func, block_next, current_node);
	                branch_node = jite_insert_item_after_linked_list(func, block_next, block->list);
                        block_next->list = branch_node;
		    }		    
		    next_blocks = next_blocks->next;
		}
	    }
	}


	if(!jit_block_ends_in_dead(block))
	{
	    jit_block_t block_next = jit_block_next(func, block);
	    if(!block_next->analysed)
	    {
	        next_node = jite_insert_item_after_linked_list(func, block_next, block->list);
	        block_next->list = next_node;
	    }
	}

	current_node = current_node->next;
    }


    jit_uint *model_next = jit_malloc(modelSize * nodeLength * 4 * sizeof(jit_uint));
    memset(model_next, 0, modelSize * 4 * nodeLength * sizeof(jit_uint));

    jite_vreg_t *vregs_table = func->jite->vregs_table;

    if(func->jite->vregs_list)
    {
        jite_linked_list_t list = func->jite->vregs_list;
	while(list->next)
	{
	    jite_vreg_t vreg = (jite_vreg_t)list->item;
	    vreg->liveness = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                         struct _jite_linked_list);
	    list = list->next;
	}
	jite_vreg_t vreg = (jite_vreg_t)list->item;
	vreg->liveness = jit_memory_pool_alloc(&(func->builder->jite_linked_list_pool),
                                                         struct _jite_linked_list);
    }

/*
        block = 0;
        while((block = jit_block_next(func, block)) != 0)
        {
            if(!block->entered_via_top && !block->entered_via_branch)
            {
                continue;
            }

            jit_insn_iter_init(&iter, block);

            while((insn = jit_insn_iter_next(&iter)) != 0)
            {
		printf("\n* Handling insn = %d ", insn->insn_num);
	        jit_dump_insn(stdout, func, insn);
     	        printf("\n");
	        fflush(stdout);

		unsigned int count;
	    	for(count = 0; count < vregsNum; count++)
	        {
  	  	    jit_dump_value(stdout, func, vregs_table[count]->value, 0);
		    printf(" ");
	            unsigned int live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + defOffset], count);
		    printf(" def = %d, ", live_def_bit);
	            live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], count);
		    printf(" use = %d, ", live_def_bit);
	            live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + outOffset], count);
		    printf(" out = %d, ", live_def_bit);
	            live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + inOffset], count);
		    printf(" in = %d, ", live_def_bit);
		}
	    }
	}
	printf("\n");
*/

    unsigned int blocks_num = 1;
    jite_linked_list_t last_node = reverse_nodes;
    while(last_node->next)
    {
        blocks_num++;
        last_node = last_node->next;
    }

    int counter = 0;
    unsigned char bEqual;



  if(jit_function_get_optimization_level(func) >= 4)
  {
//    printf("compute dead-code\n");
    
    memcpy(model_next, deadcode, modelSize * nodeLength * 4 * sizeof(jit_uint));

    do
    {
        bEqual = 1;
        block = 0;


        jite_linked_list_t list = last_node; // reverse_nodes;

        while(list)
        {
	    block = list->item;
	    list  = list->prev;


            if(!block->entered_via_top && !block->entered_via_branch)
            {
                continue;
            }

	    insn = _jit_block_get_last(block);

            while(insn)
            {
  	      	jit_insn_t insn_next = insn->next;

                if(insn_next)
		{
  	            jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset], 
	                                     &deadcode[insn_next->insn_num * nodeLength * 4 + candidatesInOffset],
	                                     &deadcode[insn_next->insn_num * nodeLength * 4 + candidatesInOffset],
			  		     nodeLength);
		}

                unsigned int computeBranch = 0;
                if(insn && (insn->flags & JIT_INSN_DEST_IS_LABEL) !=0)
                {

	            jit_insn_t insn_next = jite_get_insn_from_label(func, (jit_label_t)(insn->dest));

  	            jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset], 
	                                     &deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset],
	                                     &deadcode[insn_next->insn_num * nodeLength * 4 + candidatesInOffset],
			  		     nodeLength);

	            computeBranch = jite_liveness_node_get_bit(&deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset], vregsNum);
        	}

                if(insn && insn->opcode == JIT_OP_JUMP_TABLE)
                {
                    /* Process every possible target label. */
                    jit_label_t *labels = (jit_label_t*)(insn->value1->address);
                    jit_nint num_labels = (jit_nint)(insn->value2->address);
                    int index;
                    for(index = 0; index < num_labels; index++)
                    {
                        jit_insn_t insn_next = jite_get_insn_from_label(func, (jit_label_t)(labels[index]));

  	                jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset], 
	                                         &deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset],
	                                         &deadcode[insn_next->insn_num * nodeLength * 4 + candidatesInOffset],
			  		         nodeLength);
	
	                computeBranch = jite_liveness_node_get_bit(&deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset], vregsNum);
                    }
                }

                if(insn && insn->opcode == JIT_OP_LEAVE_FINALLY)
                {
                    /* Process every possible target label. */
	            jite_linked_list_t next_blocks = (jite_linked_list_t)(insn->dest);
	  	    while(next_blocks && next_blocks->item)
		    {
	                jit_block_t block_next = (jit_block_t)(next_blocks->item);
			jit_insn_t insn_next = jite_get_insn_from_block(func, block_next);

  	                jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset], 
	                                         &deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset],
	                                         &deadcode[insn_next->insn_num * nodeLength * 4 + candidatesInOffset],
			  		         nodeLength);


	                computeBranch = jite_liveness_node_get_bit(&deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset], vregsNum);

		        next_blocks = next_blocks->next;
		    }
                }

	        jite_liveness_intersect_nodes(matches,
	                                      &deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset],
	                                      &model[insn->insn_num * nodeLength * 4 + defOffset],
				  	      nodeLength);
		unsigned int match = 0;
		unsigned int count = 0;


		if(computeBranch)
		{
	                jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset],
	                                      &deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset],
	                                      &model[insn->insn_num * nodeLength * 4 + usedOffset],
			  	              nodeLength);
		}
		else
		{
	             jite_liveness_intersect_nodes(matches,
	                                             &deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset],
	                                             &model[insn->insn_num * nodeLength * 4 + defOffset],
				   	             nodeLength);


	             jite_liveness_unit_nodes(matches,
	                                        matches,
	                                        &deadcode[insn->insn_num * nodeLength * 4 + defEffectOffset],
				                nodeLength);


	             match = 0;
		     count = 0;

		     for(count = 0; count < nodeLength - 1; count++)
		     {
		         match = match | matches[count];
		     }
		
	   	     match |= (matches[nodeLength - 1] & ~(1 << (vregsNum % 32)));

                     if(match)
	 	     {
	                    jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset],
	                                          &deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset],
	                                          &model[insn->insn_num * nodeLength * 4 + usedOffset],
			  	                  nodeLength);

                     }
		}

	        jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset],
	                                  &deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset],
	                                  &deadcode[insn->insn_num * nodeLength * 4 + defEffectOffset],
				          nodeLength);


	        jite_liveness_remove_nodes(&deadcode[insn->insn_num * nodeLength * 4 + candidatesInOffset],
	                                  &deadcode[insn->insn_num * nodeLength * 4 + candidatesOutOffset],
	                                  &model[insn->insn_num * nodeLength * 4 + defOffset],
				          nodeLength);

	        jite_liveness_unit_nodes(&deadcode[insn->insn_num * nodeLength * 4 + candidatesInOffset],
	                                      &deadcode[insn->insn_num * nodeLength * 4 + candidatesInOffset],
	                                      &deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset],
		 	                      nodeLength);


                insn = insn->prev;
            }
        }

	counter++;

        unsigned int count = 0;
        for(count = 0; (count < modelSize * nodeLength * 4); count++)
        {
            bEqual = bEqual && (deadcode[count] == model_next[count]);
        }
        if(!bEqual) memcpy(model_next, deadcode, modelSize * nodeLength * 4 * sizeof(jit_uint));
    } while(!bEqual);




    block = 0;

    /* Remove dead-code. And update register liveness information. */

    jite_linked_list_t list = last_node;

    while(list)
    {
	block = list->item;
	list  = list->prev;


        if(!block->entered_via_top && !block->entered_via_branch)
        {
            continue;
        }

	insn = _jit_block_get_last(block);

        while(insn)
        {
	    unsigned int sideEffect = jite_liveness_node_get_bit(&deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset], vregsNum);

	    unsigned int match = 0;
	    unsigned int count = 0;

	    for(count = 0; count < nodeLength - 1; count++)
	    {
	        match = match | deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset + count];
	    }
	    match = match | deadcode[insn->insn_num * nodeLength * 4 + sideEffectOffset + nodeLength - 1];


	    if(!(match || (sideEffect && (insn && ((insn->flags & JIT_INSN_DEST_IS_LABEL) != 0 || insn->opcode == JIT_OP_JUMP_TABLE || JIT_OP_LEAVE_FINALLY)))))
	    {
                    int flag = jite_insn_has_side_effect(insn);
		    if(!flag)
		    {
		        jit_value_t dest = insn->dest;
			jit_value_t value1 = insn->value1;
			jit_value_t value2 = insn->value2;
		        if(insn->opcode != JIT_OP_NOP && insn->opcode != JIT_OP_MARK_OFFSET)
			{
		            if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
        	    		    && !(dest->is_constant)
	        	            && !(dest->is_nint_constant)
    		    	            && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
			            && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
		            {
                                if(jite_insn_dest_defined(insn))
				{
				    jite_liveness_node_remove_bit(&model[insn->insn_num * nodeLength * 4 + defOffset], dest->vreg->index);
				}
				else
				{
				    jite_liveness_node_remove_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], dest->vreg->index);
				}
		    	    }
	    	    	    if(insn && value1 && !(insn->flags & JIT_INSN_VALUE1_IS_NAME)
	    	                && !(insn->flags & JIT_INSN_VALUE1_IS_LABEL) && !value1->is_constant
    			        && !value1->is_nint_constant)
	            	    {
				if(insn->opcode == JIT_OP_INCOMING_REG || insn->opcode == JIT_OP_INCOMING_FRAME_POSN
		                                       || insn->opcode == JIT_OP_RETURN_REG)
				{
				    jite_liveness_node_remove_bit(&model[insn->insn_num * nodeLength * 4 + defOffset], value1->vreg->index);
			        }
				else
			        {
			    	    jite_liveness_node_remove_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], value1->vreg->index);
				}
		            }
	            	    if(insn && value2 && !(insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE)
        	        	&& !value2->is_constant && !value2->is_nint_constant)
    			    {
				jite_liveness_node_remove_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], value2->vreg->index);

	            	    }
		            insn->opcode = JIT_OP_NOP;
			}	
		    }
	    }
            insn = insn->prev;
	}
    }

   }

    memset(model_next, 0, modelSize * 4 * nodeLength * sizeof(jit_uint));

    do
    {
        bEqual = 1;
        block = 0;


        jite_linked_list_t list = last_node;

        while(list)
        {
	    block = list->item;
	    list  = list->prev;


            if(!block->entered_via_top && !block->entered_via_branch)
            {
                continue;
            }

	    insn = _jit_block_get_last(block);

            while(insn)
            {


  	      	jit_insn_t insn_next = insn->next;

                if(insn_next)
		{
  	            jite_liveness_unit_nodes(&model[insn->insn_num * nodeLength * 4 + outOffset], 
	                                     &model[insn->insn_num * nodeLength * 4 + outOffset],
	                                     &model[insn_next->insn_num * nodeLength * 4 + inOffset],
			  		     nodeLength);
		}

                if(insn && (insn->flags & JIT_INSN_DEST_IS_LABEL) !=0)
                {
	            jit_insn_t insn_next = jite_get_insn_from_label(func, (jit_label_t)(insn->dest));
	            jite_liveness_unit_nodes(&model[insn->insn_num * nodeLength * 4 + outOffset], 
	                                     &model[insn->insn_num * nodeLength * 4 + outOffset],
	                                     &model[insn_next->insn_num * nodeLength * 4 + inOffset],
					     nodeLength);
        	}

                if(insn && insn->opcode == JIT_OP_JUMP_TABLE)
                {
                    /* Process every possible target label. */
                    jit_label_t *labels = (jit_label_t*)(insn->value1->address);
                    jit_nint num_labels = (jit_nint)(insn->value2->address);
                    int index;
                    for(index = 0; index < num_labels; index++)
                    {
                        jit_insn_t insn_next = jite_get_insn_from_label(func, (jit_label_t)(labels[index]));
	                jite_liveness_unit_nodes(&model[insn->insn_num * nodeLength * 4 + outOffset], 
	                                             &model[insn->insn_num * nodeLength * 4 + outOffset],
	                                             &model[insn_next->insn_num * nodeLength * 4 + inOffset],
					  	     nodeLength);
                    }
                }

                if(insn && insn->opcode == JIT_OP_LEAVE_FINALLY)
                {
                    /* Process every possible target label. */
	            jite_linked_list_t next_blocks = (jite_linked_list_t)(insn->dest);
	  	    while(next_blocks && next_blocks->item)
		    {
	                jit_block_t block_next = (jit_block_t)(next_blocks->item);
			jit_insn_t insn_next = jite_get_insn_from_block(func, block_next);
	                jite_liveness_unit_nodes(&model[insn->insn_num * nodeLength * 4 + outOffset],
	                                             &model[insn->insn_num * nodeLength * 4 + outOffset],
	                                             &model[insn_next->insn_num * nodeLength * 4 + inOffset],
					  	     nodeLength);
		        next_blocks = next_blocks->next;
		    }
                }

	        jite_liveness_remove_nodes(&model[insn->insn_num * nodeLength * 4 + inOffset],
	                                   &model[insn->insn_num * nodeLength * 4 + outOffset],
	                                   &model[insn->insn_num * nodeLength * 4 + defOffset],
					   nodeLength);

   	        jite_liveness_unit_nodes(&model[insn->insn_num * nodeLength * 4 + inOffset], 
	                                 &model[insn->insn_num * nodeLength * 4 + inOffset],
	                                 &model[insn->insn_num * nodeLength * 4 + usedOffset],
					 nodeLength);					 

                insn = insn->prev;
            }
        }

	counter++;

        unsigned int count = 0;
        for(count = 0; (count < modelSize * nodeLength * 4); count++)
        {
            bEqual = bEqual && (model[count] == model_next[count]);
        }
        memcpy(model_next, model, modelSize * nodeLength * 4 * sizeof(jit_uint));
    } while(!bEqual);


//    printf("Computed block reverse order. Doing equation solution...\n");



//    jit_dump_function(stdout, func, 0);
//    printf("\nLiveness analysis done in %d iterations, for %d instructions, %d blocks, and %d variables\n\n", counter, func->builder->num_insns, blocks_num, func->jite->vregs_num);

// if(counter == 1)
/*
{
        block = 0;
        while((block = jit_block_next(func, block)) != 0)
        {
            if(!block->entered_via_top && !block->entered_via_branch)
            {
                continue;
            }

            jit_insn_iter_init(&iter, block);

            while((insn = jit_insn_iter_next(&iter)) != 0)
            {
		printf("\nHandling insn = %d ", insn->insn_num);
	        jit_dump_insn(stdout, func, insn);
     	        printf("\n");
	        fflush(stdout);

		unsigned int count;
	    	for(count = 0; count < vregsNum; count++)
	        {
  	  	    jit_dump_value(stdout, func, vregs_table[count]->value, 0);
		    printf(" ");
	            unsigned int live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + defOffset], count);
		    printf(" def = %d, ", live_def_bit);
	            live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + usedOffset], count);
		    printf(" use = %d, ", live_def_bit);
	            live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + outOffset], count);
		    printf(" out = %d, ", live_def_bit);
	            live_def_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + inOffset], count);
		    printf(" in = %d, ", live_def_bit);
		}
	    }
	}
	printf("\n\n\n");
}

*/

    block = 0;
    jite_vreg_t live_out[vregsNum], live_in[vregsNum];
    unsigned char live_state[vregsNum];
    memset(live_out, 0, sizeof(jite_vreg_t) * vregsNum);
    memset(live_in,  0, sizeof(jite_vreg_t) * vregsNum);
    memset(live_state, 0, sizeof(unsigned char) * vregsNum);




    jit_insn_t prev_insn = 0;
    while((block = jit_block_next(func, block)) != 0)
    {
        if(!block->entered_via_top && !block->entered_via_branch)
	{
	    continue;
	}
	jit_insn_iter_init(&iter, block);
	while((insn = jit_insn_iter_next(&iter)) != 0)
	{
	    if(insn && (insn->opcode == JIT_OP_NOP || insn->opcode == JIT_OP_MARK_OFFSET))
	    {
	       continue;
            }

#ifdef JITE_DEBUG_ENABLED
	    printf("\nHandling insn = %d ", insn->insn_num);
	    jit_dump_insn(stdout, func, insn);
	    printf("\n");
	    fflush(stdout);
#endif
	    unsigned int count;
	    for(count = 0; count < vregsNum; count++)
	    {
	        unsigned int live_in_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + inOffset], count);

		if(!live_in_bit)
		{
		    if(live_state[count] && live_in[count]) /* Last time a virtual register is live in, means it is dying */
		    {
		        live_state[count] = 0;
		        if(!vregs_table[count]->max_range || vregs_table[count]->max_range->insn_num < insn->insn_num)
                        {
                            vregs_table[count]->max_range = prev_insn;
			}
		 	jite_add_item_to_linked_list(func, prev_insn, vregs_table[count]->liveness);
#ifdef JITE_DEBUG_ENABLED
			jit_dump_value(stdout, func, vregs_table[count]->value, 0);
			printf(" 4. die at %d\n", prev_insn->insn_num);
#endif
		    }
		    live_in[count] = 0;
		}

	        // outOffset
	        unsigned int live_out_bit = jite_liveness_node_get_bit(&model[insn->insn_num * nodeLength * 4 + outOffset], count);

		if(live_out_bit)
		{
		    if(!live_state[count]) /* First time a virtual register is live out, means it is born */
		    {
		        if(!vregs_table[count]->min_range)
                        {
                            vregs_table[count]->min_range = insn;
			    vregs_table[count]->max_range = insn;
                        }
			else if(vregs_table[count]->max_range->insn_num < insn->insn_num)
			{
			    vregs_table[count]->max_range = insn;
			}
#ifdef JITE_DEBUG_ENABLED
			jit_dump_value(stdout, func, vregs_table[count]->value, 0);
			printf(" 1. born at %d\n", insn->insn_num);
#endif
			jite_add_item_to_linked_list(func, insn, vregs_table[count]->liveness);
			live_state[count] = 1;
		    }
		    live_out[count] = vregs_table[count];
		}
		else
		{
		    live_out[count] = 0;
		}

		// inOffset
		if(live_in_bit)
		{
		    if(!live_state[count]) /* The register is live in in this insn, although in previous insn it was dead, then it was dead only temporary in that previous block */
		    {
		        if(!vregs_table[count]->min_range)
                        {
                            vregs_table[count]->min_range = insn;
			    vregs_table[count]->max_range = insn;
                        }
			else if(vregs_table[count]->max_range->insn_num < insn->insn_num)
			{
			    vregs_table[count]->max_range = insn;
			}
#ifdef JITE_DEBUG_ENABLED
			jit_dump_value(stdout, func, vregs_table[count]->value, 0);
			printf(" 2. born at %d\n", insn->insn_num);
#endif
			jite_add_item_to_linked_list(func, insn, vregs_table[count]->liveness);
			live_state[count] = 1;

		    }
		    live_in[count] = vregs_table[count];
		}
	    }
	    prev_insn = insn;
	}
    }


    unsigned int count;
    for(count = 0; count < vregsNum; count++)
    {
        if(live_state[count] == 1)
	{
	    vregs_table[count]->max_range = prev_insn;
#ifdef JITE_DEBUG_ENABLED
	    jit_dump_value(stdout, func, vregs_table[count]->value, 0);
	    printf("die at %d\n", prev_insn->insn_num);
#endif
	    jite_add_item_to_linked_list(func, prev_insn, vregs_table[count]->liveness);
	}
    }


//    jite_debug_print_vregs_ranges(func);


    block = 0;

    /* Create virtual registers */


    int level = jit_function_get_optimization_level(func);

    while((block = jit_block_next(func, block)) != 0)
    {
        if(!block->entered_via_top && !block->entered_via_branch)
        {
            continue;
        }

        jit_insn_iter_init(&iter, block);
        while((insn = jit_insn_iter_next(&iter)) != 0)
        {
            jit_value_t dest = insn->dest;
            jit_value_t value1 = insn->value1;
            jit_value_t value2 = insn->value2;
	    if(insn && (insn->opcode == JIT_OP_NOP || insn->opcode == JIT_OP_MARK_OFFSET))
	    {
	        continue;
            }

	    if(insn && insn->opcode == JIT_OP_ENTER_FINALLY)
	    {
                if(insn->dest) jit_memory_pool_dealloc(&(func->builder->jite_linked_list_pool), insn->dest);
		insn->dest = 0;
	    }
	    
	    
	    if(block && !block->analysed)
	    {
	    	insn->opcode = JIT_OP_NOP;
	    }

	    prev_insn = insn;


	    if(level <= 3)
	    {
        	if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
            	    && !(dest->is_constant)
            	    && !(dest->is_nint_constant)
            	    && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
            	    && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
        	{
            	    if(jite_value_is_in_vreg(dest))
            	    {
                	if(!jite_vreg_lives_at_insn(dest->vreg, insn))
			{
		    	    insn->opcode = JIT_OP_NOP;
			}
            	    }
		    else
		    {
			insn->opcode = JIT_OP_NOP;
		    }
        	}
        	if(insn && value1 && !(insn->flags & JIT_INSN_VALUE1_IS_NAME)
            	    && !(insn->flags & JIT_INSN_VALUE1_IS_LABEL) && !value1->is_constant
            	    && !value1->is_nint_constant)
        	{
            	    if(jite_value_is_in_vreg(value1))
            	    {
                	if(!jite_vreg_lives_at_insn(value1->vreg, insn) && insn->opcode != JIT_OP_RETURN_REG)
			{
		    	    insn->opcode = JIT_OP_NOP;
			}
            	    }
		    else
		    {
			insn->opcode = JIT_OP_NOP;
		    }
        	}

        	if(insn && value2 && !(insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE)
                    && !value2->is_constant && !value2->is_nint_constant)
	        {
    	            if(jite_value_is_in_vreg(value2))
    	            {
        	        if(!jite_vreg_lives_at_insn(value2->vreg, insn))
		        {
			    insn->opcode = JIT_OP_NOP;
			}
        	    }
		    else
		    {
		        insn->opcode = JIT_OP_NOP;
		    }
    	        }    
	    }
        }
    }
    
    


    for(count = 0; count < vregsNum; count++)
    {
        jite_vreg_t vreg = vregs_table[count];

	/* Values can be in and out already at begin of function. INCOMING_* opcodes are usually the very first instructions.
	   But we allocate registers specially in INCOMING_* opcodes.
	   So for these values we need to change the insn where they are born (to the first which is not incoming_* opcode).
	   The first instruction is "first_insn".
	   The first none INCOMING_* instruction is min_insn. */


        if(vreg->min_range && vreg->max_range && (vreg->min_range->insn_num == vreg->max_range->insn_num))
	{
	    unsigned int live_out_bit = jite_liveness_node_get_bit(&model[prev_insn->insn_num * nodeLength * 4 + outOffset], count);
            if(live_out_bit == 0)
	    {
	        jite_remove_item_from_linked_list(func, vreg->min_range, vregs_table[count]->liveness);
	        jite_remove_item_from_linked_list(func, vreg->min_range, vregs_table[count]->liveness);
	    }
	}
	if(!vreg->min_range || !vreg->max_range)
	{
	    jite_remove_item_from_linked_list(func, vreg, func->jite->vregs_list);
	}
    }

    for(count = 0; count < vregsNum; count++)
    {
        jite_vreg_t vreg = vregs_table[count];
	if(vreg && vreg->value && jit_value_is_addressable(vreg->value))
	{
            jite_linked_list_t list = vreg->liveness;
  	    while(list && list->item)// && list->next)
	    {
	        jit_insn_t min_insn = ((jit_insn_t)(list->item));
	        list = list->next;
	        jit_insn_t max_insn = ((jit_insn_t)(list->item));
	        list = list->next;
	        jite_remove_item_from_linked_list(func, min_insn, vreg->liveness);
	        jite_remove_item_from_linked_list(func, max_insn, vreg->liveness);
	    }
	    vreg->min_range = min_insn;
            vreg->max_range = max_insn;

	    jite_add_item_to_linked_list(func, min_insn, vreg->liveness);
	    jite_add_item_to_linked_list(func, max_insn, vreg->liveness);
	}
    }


//    jite_debug_print_vregs_ranges(func);


    for(count = 0; count < vregsNum; count++)
    {
        jite_linked_list_t list = vregs_table[count]->liveness;
        while(list && list->item) //&& list->next)
	{
	    jit_insn_t min_insn = ((jit_insn_t)(list->item));
	    list = list->next;
	    jit_insn_t max_insn = ((jit_insn_t)(list->item));
	    list = list->next;
	    if(min_insn->insn_num != max_insn->insn_num)
	    {
	        jite_create_critical_point(func, min_insn);
	        jite_create_critical_point(func, max_insn);

	        jite_add_vreg_to_complex_list(func, vregs_table[count], min_insn->cpoint->vregs_born);
  	        jite_add_vreg_to_complex_list(func, vregs_table[count], max_insn->cpoint->vregs_die);
	    }
	}
    }


    free(model);
    free(model_next);
    free(deadcode);
    free(matches);

    jit_memory_pool_dealloc(&(func->builder->jite_linked_list_pool), blocks);
    jit_memory_pool_dealloc(&(func->builder->jite_linked_list_pool), reverse_nodes);
    jit_memory_pool_dealloc(&(func->builder->jite_linked_list_pool), finally_clauses);

//    printf("allocation done\n");
}

// #ifdef JITE_DEBUG_ENABLED
void jite_debug_print_vregs_ranges(jit_function_t func)
{
    jite_linked_list_t list = func->jite->vregs_list;
    jite_vreg_t vreg;
    while(list)
    {
        vreg = (jite_vreg_t)(list->item);
        if(vreg)
        {
            printf("Virtual Register %d, ", vreg->index);
            jit_dump_value(stdout, func, vreg->value, 0);
	    printf(", ");

            jite_linked_list_t list = vreg->liveness;
	    if(list && list->item)
	    {
	        printf("lives from ");
                while(list && list->item)
                {
                    jit_insn_t min_insn = ((jit_insn_t)(list->item));
                    list = list->next;
	            jit_insn_t max_insn = ((jit_insn_t)(list->item));
	            list = list->next;
		    printf("%d to %d; ", min_insn->insn_num, max_insn->insn_num);
                }
//	        printf("min_range = %d, max_range = %d", vreg->min_range->insn_num, vreg->max_range->insn_num);
		printf("\n");
	    }
	    else if(vreg->min_range && vreg->max_range) printf(", lives from %d to %d; ", vreg->min_range->insn_num, vreg->max_range->insn_num);

            if(vreg->in_reg) printf("reg used %d; ", vreg->reg->index);
            if(vreg->in_frame)
            {
                printf("frame used 0x%x; ", vreg->frame->frame_offset);
                printf("hash_code 0x%x; ", vreg->frame->hash_code);
                printf("length %d; ", vreg->frame->length);
                fflush(stdout);
            }
            if(jit_value_is_addressable(vreg->value)) printf("is_addressable; ");
            if(jit_value_is_temporary(vreg->value)) printf("is_temporary; ");

            printf("\n");
            fflush(stdout);
        }
        list = list->next;
    }
    printf("\n");
}
// #endif



void jite_function_set_register_allocator_euristic(jit_function_t func, int type)
{
     func->jite->register_allocator_euristic = type;
}

unsigned char jite_function_get_register_allocator_euristic(jit_function_t func)
{
    return func->jite->register_allocator_euristic;
}


int jite_compile(jit_function_t func, void **entry_point)
{
    struct jit_gencode gen;
    jit_cache_t cache;
    void *start;
#if !defined(jit_redirector_size) || !defined(jit_indirector_size) || defined(JIT_BACKEND_INTERP)
    void *recompilable_start = 0;
#endif
    void *end;
    jit_block_t block;
    int result;
    int page_factor = 0;
    end = 0;
    start = 0;
    /* Bail out if we have nothing to do */
    if(!func)
    {
        return 0;
    }
    if(func->is_compiled && !(func->builder))
    {
        /* The function is already compiled, and we don't need to recompile */
        *entry_point = func->entry_point;
        return 1;
    }
    if(!(func->builder))
    {
        /* We don't have anything to compile at all */
        return 0;
    }
    
    if(!(func->builder->may_throw))
    {
        func->no_throw = 1;
    }
    if(!(func->builder->ordinary_return))
    {
        func->no_return = 1;
    }

    /* We need the cache lock while we are compiling the function */
    jit_mutex_lock(&(func->context->cache_lock));

    /* Get the method cache */
    cache = _jit_context_get_cache(func->context);
    if(!cache)
    {
        jit_mutex_unlock(&(func->context->cache_lock));
        return 0;
    }

    /* Initialize the code generation state */
    jit_memzero(&gen, sizeof(gen));
    func->jite = jite_create_instance(func);
    jite_compute_liveness(func);
    jite_compute_values_weight(func);

//    jit_dump_function(stdout, func, 0);
//    fflush(stdout);
//    printf("\n");

    /* We may need to perform output twice, if the first attempt fails
       due to a lack of space in the current method cache page */

#ifdef _JIT_COMPILE_DEBUG
    printf("\n*** Start compilation ***\n\n");
    func->builder->block_count = 0;
    func->builder->insn_count = 0;
#endif

    /* Start function output to the cache */
    result = _jit_cache_start_method(cache, &(gen.posn),
                     page_factor++,
                     JIT_FUNCTION_ALIGNMENT,
                     func);
    if(result == JIT_CACHE_RESTART)
    {
        result = _jit_cache_start_method(cache, &(gen.posn),
                         page_factor++,
                         JIT_FUNCTION_ALIGNMENT,
                         func);
    }
    if(result != JIT_CACHE_OK)
    {
        jit_mutex_unlock(&(func->context->cache_lock));
        return 0;
    }
    jite_preallocate_global_registers(func);
    for(;;)
    {
        jite_reinit(func);
        start = gen.posn.ptr;
#ifdef jit_extra_gen_init
        /* Initialize information that may need to be reset each loop */
        jit_extra_gen_init(&gen);
#endif

#ifdef JIT_PROLOG_SIZE
        /* Output space for the function prolog */
        if(!jit_cache_check_for_n(&(gen.posn), JIT_PROLOG_SIZE))
        {
            jit_cache_mark_full(&(gen.posn));
            goto restart;
        }
        gen.posn.ptr += JIT_PROLOG_SIZE;
#endif
        /* Generate code for the blocks in the function */
        block = 0;
        while((block = jit_block_next(func, block)) != 0)
        {
            /* If this block is never entered, then discard it */
            if(!(block->entered_via_top) && !(block->entered_via_branch))
            {
                continue;
            }

            /* Notify the back end that the block is starting */
            gen_start_block(&gen, block);
            /* Generate the block's code */
            jite_compile_block(&gen, func, block);
            if(_jit_cache_is_full(cache, &(gen.posn)))
            {
                goto restart;
            }

            /* Notify the back end that the block is finished */
            gen_end_block(&gen, block);
        }
        
        /* Output the function epilog.  All return paths will jump to here */
        gen_epilog(&gen, func);
        if(_jit_cache_is_full(cache, &(gen.posn)))
        {
            goto restart;
        }
        end = gen.posn.ptr;

#ifdef JIT_PROLOG_SIZE
        /* Back-patch the function prolog and get the real entry point */
        start = gen_prolog(&gen, func, start);
#endif

#if !defined(jit_redirector_size) || !defined(jit_indirector_size) || defined(JIT_BACKEND_INTERP)
        /* If the function is recompilable, then we need an extra entry
           point to properly redirect previous references to the function */
        if(func->is_recompilable)
        {
            recompilable_start = _jit_gen_redirector(&gen, func);
        }
#endif

    restart:
//      printf("Restart compilation\n");
//	fflush(stdout);

        /* End the function's output process */
        result = _jit_cache_end_method(&(gen.posn));
        if(result != JIT_CACHE_RESTART)
        {
            break;
        }
#ifdef _JIT_COMPILE_DEBUG
        printf("\n*** Restart compilation ***\n\n");
        func->builder->block_count = 0;
        func->builder->insn_count = 0;
#endif
        block = 0;
        while((block = jit_block_next(func, block)) != 0)
        {
            block->address = 0;
            block->fixup_list = 0;
            block->fixup_absolute_list = 0;
        }
        
        result = _jit_cache_start_method(cache, &(gen.posn),
                        page_factor,
                        JIT_FUNCTION_ALIGNMENT, func);
        if(result != JIT_CACHE_OK)
        {
#ifdef jit_extra_gen_cleanup
            jit_extra_gen_cleanup(gen);
#endif
            jit_mutex_unlock(&(func->context->cache_lock));
            return 0;
        }
        page_factor *= 2;
    }

#ifdef jit_extra_gen_cleanup
    /* Clean up the extra code generation state */
    jit_extra_gen_cleanup(gen);
#endif

    /* Bail out if we ran out of memory while translating the function */
    if(result != JIT_CACHE_OK)
    {
        jit_mutex_unlock(&(func->context->cache_lock));
        return 0;
    }

#ifndef JIT_BACKEND_INTERP
    /* Perform a CPU cache flush, to make the code executable */
    jit_flush_exec(start, (unsigned int)(end - start));
#endif
    
    jit_mutex_unlock(&(func->context->cache_lock));

#ifdef JITE_DUMP_LIVENESS_RANGES
    jite_debug_print_vregs_ranges(func);
#endif

    /* Free the builder structure, which no longer require */
    _jit_function_free_builder(func);

    /* Record the entry point */
    if(entry_point)
    {
        *entry_point = start;
    }
    
    jite_destroy_instance(func);

    return 1;
}


#endif
