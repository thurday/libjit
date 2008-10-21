#include "jit-internal.h"
#include "config.h"
#include "jit-setjmp.h"
#include "jit/jit-dump.h"

#if defined(__i386) || defined(__i386__) || defined(_M_IX86)


unsigned int ejit_value_in_reg(jit_value_t value)
{
    if(value && value->vreg && value->vreg->in_reg && value->vreg->reg)
    {
        return 1;
    }
    return 0;
}

unsigned int ejit_value_in_frame(jit_value_t value)
{
    if(value && value->vreg && value->vreg->in_frame && value->vreg->frame)
    {
        return 1;
    }
    return 0;
}


int ejit_count_items(jit_function_t func, ejit_linked_list_t list)
{
    int num = 0;
    while(list)
    {
        if(list->item) num++;
        list = list->next;
    }
    return num;
}


int ejit_type_num_params(jit_type_t signature)
{
    jit_type_t type = jit_type_get_return(signature);

    if(!jit_type_return_via_pointer(type))
    {
        return jit_type_num_params(signature);
    }

    return jit_type_num_params(signature) + 1;
}

jit_type_t ejit_type_get_param(jit_type_t signature, int index)
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

jit_value_t ejit_value_get_param(jit_function_t func, int index)
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

jit_value_t ejit_find_work_value(jit_insn_t insn, jit_value_t value)
{
    return value;
}

ejit_vreg_t ejit_create_vreg(jit_function_t func, jit_value_t value)
{
    ejit_instance_t ejit = func->ejit;
    ejit_vreg_t vreg = 0;
    ejit_linked_list_t list;
    if(!_jit_function_ensure_builder(func) || !value)
    {
        return 0;
    }
    if(!ejit_value_is_in_vreg(func, value))
    {
        if(ejit->vregs_list)
        {
            list = ejit->vregs_list;
            while(list->next)
            {
                list = list->next;
            }
            // Means, that at this point list->next == 0.
            // Allocate some place for a new item.

            list->next = jit_memory_pool_alloc(&(func->builder->ejit_list_pool),
                                        struct _ejit_linked_list);
            list->next->prev = list;
            list = list->next;
        }
        else
        {
            ejit->vregs_list = jit_memory_pool_alloc(&(func->builder->ejit_list_pool),
                                        struct _ejit_linked_list);
            list = ejit->vregs_list;
        }
        vreg = jit_memory_pool_alloc(&(func->builder->ejit_vreg_pool),
                                    struct _ejit_vreg);
        vreg->value = value;
        vreg->min_range = 0;
        vreg->max_range = 0;
        vreg->in_reg = 0;
        vreg->in_frame = 0;
        vreg->index = ejit->vregs_num;
        vreg->weight = 0;
        vreg->reg = 0;
        vreg->frame = 0;
        ejit->vregs_num++;
        list->item = (void*)vreg;
        value->vreg = vreg;
    }
    return value->vreg;
}

char ejit_value_is_in_vreg(jit_function_t func, jit_value_t value)
{
    if(value->vreg) return 1;
    return 0;
}

ejit_instance_t ejit_create_instance(jit_function_t func)
{
    ejit_instance_t ejit = jit_cnew(struct _ejit_instance);
    ejit->func = func;
    ejit->cpoints_list = 0;
    ejit->vregs_num = 0;
    ejit->regs_state = 0;
    ejit->frame_state = jit_memory_pool_alloc(&(func->builder->ejit_list_pool),
                                struct _ejit_list);
    ejit->incoming_params_state = 0;
    ejit->scratch_regs = 0;
    ejit->scratch_frame = 0;
    func->ejit = ejit;
    int index = 0;
    for(index = 0; index < 32; index++)
    {
        ejit->reg_holes[index] = 0;
    }
    ejit_init(func); // Platform dependent init.
    return ejit;
}

void ejit_destroy_instance(jit_function_t func)
{
    if(func && func->ejit && func->ejit->incoming_params_state) jit_free(func->ejit->incoming_params_state);
    if(func && func->ejit) jit_free(func->ejit);
    func->ejit = 0;
}

void ejit_create_critical_point(jit_function_t func, jit_insn_t insn)
{
    ejit_critical_point_t cpoint;
    if(insn && insn->cpoint==0)
    {
        cpoint = jit_memory_pool_alloc(&(func->builder->ejit_critical_point_pool),
                                            struct _ejit_critical_point);
        cpoint->is_branch_target = 0;
        cpoint->vregs_die = jit_memory_pool_alloc(&(func->builder->ejit_list_pool),
                                            struct _ejit_list);
        cpoint->vregs_born = jit_memory_pool_alloc(&(func->builder->ejit_list_pool),
                                            struct _ejit_list);
        cpoint->regs_state_1 = 0xffffffff;
        cpoint->regs_state_2 = 0xffffffff;
        insn->cpoint = cpoint;
    }
    // if insn == 0 then there is no opcodes in this block
}

void ejit_add_vreg_to_complex_list(jit_function_t func, ejit_vreg_t vreg, ejit_list_t list)
{
    if(vreg && list)
    {
        ejit_linked_list_t linked_list = 0;
        jit_type_t type = jit_value_get_type(vreg->value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            CASE_USE_WORD
            {
                if(!list->item1)
                {
                    list->item1 = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                        struct _ejit_linked_list);
                }
                linked_list = (ejit_linked_list_t)list->item1;
            }
            break;
            CASE_USE_LONG
            {
                if(!list->item2)
                {
                    list->item2 = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                        struct _ejit_linked_list);
                }
                linked_list = (ejit_linked_list_t)list->item2;
            }
            break;
            case JIT_TYPE_FLOAT32: 
            case JIT_TYPE_FLOAT64:
            {
                if(!list->item3)
                {
                    list->item3 = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                        struct _ejit_linked_list);
                }
                linked_list = (ejit_linked_list_t)list->item3;
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    if(!list->item4)
                    {
                        list->item4 = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                            struct _ejit_linked_list);
                    }
                    linked_list = (ejit_linked_list_t)list->item4;
                }
                else
                {
                    if(!list->item3)
                    {
                        list->item3 = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                            struct _ejit_linked_list);
                    }
                    linked_list = (ejit_linked_list_t)list->item3;
                }
            }
            break;
            default:
            {
                if(!list->item4)
                {
                    list->item4 = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                        struct _ejit_linked_list);
                }
                linked_list = (ejit_linked_list_t)list->item4;
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
                linked_list->next = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                        struct _ejit_linked_list);
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

void ejit_remove_vreg_from_complex_list(jit_function_t func, ejit_vreg_t vreg, ejit_list_t list)
{
    if(vreg && list)
    {
        ejit_linked_list_t linked_list = 0;
        jit_type_t type = jit_value_get_type(vreg->value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            CASE_USE_WORD
            {
                linked_list = (ejit_linked_list_t)list->item1;
            }
            break;
            CASE_USE_LONG
            {
                linked_list = (ejit_linked_list_t)list->item2;
            }
            break;
            case JIT_TYPE_FLOAT32: 
            case JIT_TYPE_FLOAT64:
            {
                linked_list = (ejit_linked_list_t)list->item3;
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) != sizeof(jit_float64))
                {
                    linked_list = (ejit_linked_list_t)list->item4;
                }
                else
                {
                    linked_list = (ejit_linked_list_t)list->item3;
                }
            }
            break;
            default:
            {
                linked_list = (ejit_linked_list_t)list->item4;
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
                if(linked_list->prev) // not the first node
                {
                    linked_list->prev->next = linked_list->next;
                    if(linked_list->next) linked_list->next->prev = linked_list->prev;
                }
                else // the first node
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

unsigned char ejit_add_item_to_linked_list(jit_function_t func, void *item, ejit_linked_list_t linked_list)
{
    unsigned char updated = 0;
    if(linked_list && item) // cannot add a null value.
    {
        while(linked_list->next && linked_list->item != item)
        {
            linked_list = linked_list->next;
        }
        if(linked_list->item != item)
        {
            if(linked_list->item)
            {
                linked_list->next = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool),
                                        struct _ejit_linked_list);
                linked_list->next->item = item;
                linked_list->next->prev = linked_list;
            }
            else
            {
                linked_list->item = item;
            }
            updated = 1;
        }
    }
    return updated;
}

unsigned char ejit_add_two_linked_lists(jit_function_t func, ejit_linked_list_t dest_list, ejit_linked_list_t source_list)
{
    unsigned char updated = 0;
    while(source_list)
    {
        void *item = source_list->item;
        if(item)
        {
        
            unsigned char new_update = ejit_add_item_to_linked_list(func, item, dest_list);
            updated = updated || new_update;
        }
        source_list = source_list->next;
    }
    return updated;
}

unsigned char ejit_sub_two_linked_lists(jit_function_t func, ejit_linked_list_t dest_list, ejit_linked_list_t source_list)
{
    unsigned char updated = 0;
    while(source_list)
    {
        void *item = source_list->item;
        if(item)
        {
            unsigned char new_update = ejit_remove_item_from_linked_list(func, item, dest_list);
            updated = updated || new_update;
        }
        source_list = source_list->next;
    }
    return updated;
}

unsigned char ejit_add_add_sub_linked_lists(jit_function_t func, ejit_linked_list_t dest, ejit_linked_list_t list1, ejit_linked_list_t list2, ejit_linked_list_t list3)
{
    unsigned char updated = 0;
    // do dest = dest + a + b - c;
    ejit_linked_list_t list = jit_memory_pool_alloc(&(func->builder->ejit_linked_list_pool), 
                                struct _ejit_linked_list);
    updated = ejit_add_two_linked_lists(func, list, dest);
    ejit_add_two_linked_lists(func, dest, list1);
    ejit_add_two_linked_lists(func, dest, list2);
    ejit_sub_two_linked_lists(func, dest, list3);
    updated = ejit_add_two_linked_lists(func, list, dest);
    jit_memory_pool_dealloc(&(func->builder->ejit_linked_list_pool),
                            list);
    return updated;
}

unsigned char ejit_remove_item_from_linked_list(jit_function_t func, void *item, ejit_linked_list_t list)
{
    unsigned char updated = 0;
    if(list && item) // Cannot remove a null value.
    {
        ejit_linked_list_t linked_list = list;
        if(linked_list)
        {
            while(linked_list->next && linked_list->item != item)
            {
                linked_list = linked_list->next;
            }
            if(linked_list->item == item)
            {
                if(linked_list->prev) // not the first node
                {
                    linked_list->prev->next = linked_list->next;
                    if(linked_list->next) linked_list->next->prev = linked_list->prev;
                }
                else // the first node
                {
                    if(linked_list->next) 
                    {
                        list->item = linked_list->next;
                        linked_list->next->prev = 0;
                    }
                    else list->item = 0;
                }
                updated = 1;
            }
        }
    }
    return updated;
}

unsigned char ejit_update_liveness_for_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to)
{
    if(!insn_from || !insn_to) return 0;
    ejit_instance_t ejit = func->ejit;
    unsigned int range1 = insn_from->insn_num;
    unsigned int range2 = insn_to->insn_num;
    ejit_linked_list_t list = ejit->vregs_list;
    ejit_vreg_t vreg;
    unsigned char updated = 0;
    while(list)
    {
        vreg = (ejit_vreg_t)(list->item);
        list = list->next;
        if(!jit_value_is_temporary(vreg->value) && (vreg->min_range->insn_num != vreg->max_range->insn_num))
        {
            if(vreg->min_range->insn_num <= range2 && vreg->max_range->insn_num >= range2)
            {
                if(vreg->min_range->insn_num > range1)
                    {
                        ejit_add_vreg_to_complex_list(func, vreg, insn_from->cpoint->vregs_born);
                    ejit_remove_vreg_from_complex_list(func, vreg, vreg->min_range->cpoint->vregs_born);
                    vreg->min_range = insn_from;
                    updated = 1;
                }
                else if(vreg->max_range->insn_num < range1)
                {
                    ejit_add_vreg_to_complex_list(func, vreg, insn_from->cpoint->vregs_die);
                    ejit_remove_vreg_from_complex_list(func, vreg, vreg->max_range->cpoint->vregs_die);
                    vreg->max_range = insn_from;
                    updated = 1;
                }
            }

        }
    }
    return updated;
}

void ejit_add_fixup_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to)
{
    ejit_list_t list = func->ejit->branch_list;
    ejit_list_t new_list = jit_memory_pool_alloc(&(func->builder->ejit_list_pool),
                            struct _ejit_list);
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
        func->ejit->branch_list = new_list;
    }
    new_list->item1 = (void*)insn_from;
    new_list->item2 = (void*)insn_to;
    new_list->prev = list;
}

void ejit_free_registers(jit_function_t func, ejit_list_t list)
{
    ejit_linked_list_t linked_list;
    if(func && list && func->ejit)
        {
        linked_list = (ejit_linked_list_t)list->item1;
        while(linked_list)
        {
            ejit_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (ejit_linked_list_t)list->item2;
        while(linked_list)
        {
            ejit_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (ejit_linked_list_t)list->item3;
        while(linked_list)
        {
            ejit_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (ejit_linked_list_t)list->item4;
        while(linked_list)
        {
            ejit_free_reg(func, linked_list->item);
            linked_list = linked_list->next;
        }
        }
}


void ejit_free_frames(jit_function_t func, ejit_list_t list)
{
    ejit_linked_list_t linked_list;
    if(func && list && func->ejit)
        {
        linked_list = (ejit_linked_list_t)list->item1;
        while(linked_list)
        {
            ejit_free_vreg_frame(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (ejit_linked_list_t)list->item2;
        while(linked_list)
        {
            ejit_free_vreg_frame(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (ejit_linked_list_t)list->item3;
        while(linked_list)
        {
            ejit_free_vreg_frame(func, linked_list->item);
            linked_list = linked_list->next;
        }
        linked_list = (ejit_linked_list_t)list->item4;
        while(linked_list)
        {
            ejit_free_vreg_frame(func, linked_list->item);
            linked_list = linked_list->next;
        }
        }
}


void ejit_free_vreg_frame(jit_function_t func, ejit_vreg_t vreg)
{
    if(vreg && vreg->frame && vreg->frame->hash_code != -1)
    {
        ejit_frame_t frame = vreg->frame;
        ejit_list_t temp;
        int count;
        for(count = 0; count < frame->length; count++)
        {
            unsigned int offset = (frame->hash_code + count) % 96;
            unsigned int segment = (frame->hash_code + count) / 96;
            unsigned int index;
            temp = func->ejit->frame_state;
            for(index = 0; index < segment; index++) // find the right list
            {
                if(temp->next) temp = temp->next;
                else
                {
                    return;
                }
            }
            // now, at temp is the right list
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

void ejit_free_frame(jit_function_t func, ejit_frame_t frame)
{
    if(frame && frame->hash_code != - 1)
    {
        ejit_list_t temp;
        int count;
        for(count = 0; count < frame->length; count++)
        {
            unsigned int offset = (frame->hash_code + count) % 96;
            unsigned int segment = (frame->hash_code + count) / 96;
            unsigned int index;
            temp = func->ejit->frame_state;
            for(index = 0; index < segment; index++) // find the right list
            {
                if(temp->next) temp = temp->next;
                else
                {
                    return;
                }
            }
            // now, at temp is the right list
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

int ejit_count_vregs(jit_function_t func, ejit_list_t list)
{
    int num = 0;
    unsigned int segment, offset;    
    if(func && list && func->ejit)
        {
        ejit_list_t temp = list;
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

short ejit_vreg_weight(jit_function_t func, ejit_vreg_t vreg)
{
    if(vreg)
    {
        vreg->weight = vreg->max_range->insn_num;
        return vreg->weight;
    }
    return 0;
}

ejit_vreg_t ejit_regs_higher_criteria(jit_function_t func, ejit_vreg_t vreg1, ejit_vreg_t vreg2)
{
    if(vreg1 && vreg2)
    {
        if(vreg1->weight < vreg2->weight) return vreg1;
        return vreg2;
    }
    else if(vreg1) return vreg1;
    return vreg2;
}

void ejit_add_branch_target(jit_function_t func, jit_insn_t insn, jit_label_t label)
{
    jit_block_t temp_block = jit_block_from_label(func, label);
    jit_insn_iter_t temp_iter;
    jit_insn_t dest_insn;
    // We find the first block, in which there is at least one opcode.
    // We need this because of a possible case of two blocks which
    // follow one another, when the first block does not contain opcodes.
    do
    {
        jit_insn_iter_init(&temp_iter, temp_block);
        do
        {
            dest_insn = jit_insn_iter_next(&temp_iter);
        }
        while(dest_insn && (dest_insn->opcode == JIT_OP_MARK_OFFSET ||
                dest_insn->opcode == JIT_OP_INCOMING_REG ||
                dest_insn->opcode == JIT_OP_INCOMING_FRAME_POSN));
                temp_block = jit_block_next(func, temp_block);
    } while(dest_insn == 0);

    ejit_add_fixup_branch(func, insn, dest_insn);
    ejit_create_critical_point(func, dest_insn);
}

void ejit_compute_liveness(jit_function_t func)
{
    _jit_function_compute_liveness(func);
    jit_block_t block;
    jit_insn_iter_t iter;
    jit_insn_t insn;

    ejit_linked_list_t linked_list;
    ejit_list_t list;
    jit_insn_t min_insn = 0, max_insn = 0;
    unsigned char updated;
    // Step 1. Compute vregs liveness without branches.
        block = 0;

    if(!func->has_try)
    {
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
                ejit_create_critical_point(func, insn); // Points to which there exist branches.
            }
            jit_insn_iter_init(&iter, block);
            while((insn = jit_insn_iter_next(&iter)) != 0)
            {
                jit_value_t dest = insn->dest;
                jit_value_t value1 = insn->value1;
                jit_value_t value2 = insn->value2;
                if(insn && (insn->flags & JIT_INSN_DEST_IS_LABEL) !=0)
                {
                    ejit_create_critical_point(func, insn); // Point from which there is a branch.
                    ejit_add_branch_target(func, insn, (jit_label_t)dest);
                }
                // constants are not considered to be associated with virtual registers,
                // though in the future we may store the low and high ranges that are 
                // applicable for the virtual registers as it walks thru code.
                // This might be used for optimization. In that sense a constant
                // is a very special case of it, but we avoid this abstraction as
                // it simplifies things.
                if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
                    && !(dest->is_constant)
                    && !(dest->is_nint_constant)
                    && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
                    && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
                {
                    if(!ejit_value_is_in_vreg(func, dest))
                        {
                        ejit_create_vreg(func, dest);
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
                    if(!ejit_value_is_in_vreg(func, value1))
                    {
                        ejit_create_vreg(func, value1);
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
                    if(!ejit_value_is_in_vreg(func, value2))
                    {
                        ejit_create_vreg(func, value2);
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
                    && insn->opcode != JIT_OP_MARK_OFFSET)
                {
                    if(min_insn == 0) min_insn = insn;
                    max_insn = insn;
                }
                if(insn && insn->opcode == JIT_OP_JUMP_TABLE)
                {
                    // Process every possible target label.
                    ejit_create_critical_point(func, insn);
                    jit_label_t *labels = (jit_label_t*)(insn->value1->address);
                    jit_nint num_labels = (jit_nint)(insn->value2->address);
                    int index;
                    for(index = 0; index < num_labels; index++)
                    {
                        ejit_add_branch_target(func, insn, labels[index]);
                    }
                }
            }
        }

        // Step 2. Mark critical points in opcodes of where new vregs are born and are destroyed.
        linked_list = func->ejit->vregs_list;
        while(linked_list)
        {
            ejit_vreg_t vreg = (ejit_vreg_t)(linked_list->item);
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
            if(vreg->min_range == 0) // Remove this later.
            {
                vreg->min_range = min_insn;
                vreg->max_range = min_insn;
            }

            if(vreg->min_range != vreg->max_range)
            {
                ejit_create_critical_point(func, vreg->min_range);
                ejit_add_vreg_to_complex_list(func, vreg, vreg->min_range->cpoint->vregs_born);
                ejit_create_critical_point(func, vreg->max_range);
                ejit_add_vreg_to_complex_list(func, vreg, vreg->max_range->cpoint->vregs_die);
            }
        }

        // Step 3. Compute vregs liveness with branches.

        do
        {
            updated = 0;
            ejit_list_t last_item = func->ejit->branch_list;
            list = func->ejit->branch_list;
            while(list)
            {
                unsigned char new_update = ejit_update_liveness_for_branch(func, (jit_insn_t)(list->item1), (jit_insn_t)(list->item2));
                updated = updated || new_update;
                list = list->next;
                if(list) last_item = list;
            }
            if(!updated) break;
            updated = 0;
            list = last_item;
            while(list)
            {
                unsigned char new_update = ejit_update_liveness_for_branch(func, (jit_insn_t)(list->item1), (jit_insn_t)(list->item2));
                updated = updated || new_update;
                list = list->prev;
            }
        } while(updated);

        // At this point all critical points and vregs liveness should be ready.
    }
    else // The function has a try/catch/finally block, we need to set all variables the maximum liveness range.
    {
        while((block = jit_block_next(func, block)) != 0)
        {
            jit_insn_iter_init(&iter, block);
            while((insn = jit_insn_iter_next(&iter)) != 0)
            {
                jit_value_t dest = insn->dest;
                jit_value_t value1 = insn->value1;
                jit_value_t value2 = insn->value2;
                ejit_vreg_t vreg = 0;
                if(insn && dest && !(insn->flags & JIT_INSN_DEST_IS_LABEL)
                    && !(dest->is_constant)
                    && !(dest->is_nint_constant)
                    && !(insn->flags & JIT_INSN_DEST_IS_FUNCTION)
                    && !(insn->flags & JIT_INSN_DEST_IS_NATIVE))
                {
                    if(!ejit_value_is_in_vreg(func, dest))
                        {
                        vreg = ejit_create_vreg(func, dest);                    
                    }
                    if(!dest->vreg->min_range) dest->vreg->min_range = insn;
                    if(!dest->vreg->max_range
                        || (dest->vreg->max_range && dest->vreg->max_range->insn_num < insn->insn_num))
                    {
                        dest->vreg->max_range = insn;
                    }
                }
                if(insn && value1 && !(insn->flags & JIT_INSN_VALUE1_IS_NAME)
                    && !(insn->flags & JIT_INSN_VALUE1_IS_LABEL) && !value1->is_constant
                    && !value1->is_nint_constant)
                {
                    if(!ejit_value_is_in_vreg(func, value1))
                    {
                        vreg = ejit_create_vreg(func, value1);
                    }
                    if(!value1->vreg->min_range) value1->vreg->min_range = insn;
                    if(!value1->vreg->max_range
                        || (value1->vreg->max_range && value1->vreg->max_range->insn_num < insn->insn_num))
                    {
                        value1->vreg->max_range = insn;
                    }
                }
                if(insn && value2 && !(insn->flags & JIT_INSN_VALUE2_IS_SIGNATURE)
                    && !value2->is_constant && !value2->is_nint_constant)
                {
                    if(!ejit_value_is_in_vreg(func, value2))
                    {
                        vreg = ejit_create_vreg(func, value2);
                    }
                    if(!value2->vreg->min_range) value2->vreg->min_range = insn;
                    if(!value2->vreg->max_range
                        || (value2->vreg->max_range && value2->vreg->max_range->insn_num < insn->insn_num))
                    {
                        value2->vreg->max_range = insn;
                    }
                }

                if(insn && insn->opcode != JIT_OP_INCOMING_REG && insn->opcode != JIT_OP_INCOMING_FRAME_POSN
                    && insn->opcode != JIT_OP_MARK_OFFSET)
                {            
                    if(min_insn == 0) min_insn = insn;
                    max_insn = insn;
                }
            }
        }
        ejit_create_critical_point(func, min_insn);
        ejit_create_critical_point(func, max_insn);
        // Set the maximum liveness range to all variables.
        linked_list = func->ejit->vregs_list;
        while(linked_list)
        {
            ejit_vreg_t vreg = (ejit_vreg_t)(linked_list->item);
            linked_list = linked_list->next;
            if(vreg->min_range != vreg->max_range || jit_value_is_addressable(vreg->value))
            {
                if(vreg->min_range->insn_num >= min_insn->insn_num)
                {
                    if(jit_value_is_temporary(vreg->value) && !jit_value_is_addressable(vreg->value))
                    {
                        ejit_create_critical_point(func, vreg->min_range);
                    }
                    else
                    {
                        vreg->min_range = min_insn;
                    }
                    ejit_add_vreg_to_complex_list(func, vreg, vreg->min_range->cpoint->vregs_born);
                }
                if(jit_value_is_temporary(vreg->value) && !jit_value_is_addressable(vreg->value))//
                {
                    ejit_create_critical_point(func, vreg->max_range);
                }
                else
                {
                    vreg->max_range = max_insn;
                }
                ejit_add_vreg_to_complex_list(func, vreg, vreg->max_range->cpoint->vregs_die);
            }
        }
        // At this point all critical points and vregs liveness should be ready.
    }
    ejit_compute_register_holes(func);
}

#ifdef EJIT_DEBUG_ENABLED
void ejit_debug_print_vregs_ranges(jit_function_t func)
{
    ejit_linked_list_t list = func->ejit->vregs_list;
    ejit_vreg_t vreg;
    while(list)
    {
        vreg = (ejit_vreg_t)(list->item);
        if(vreg)
        {
            printf("Virtual Register %d, ", vreg->index);
            jit_dump_value(stdout, func, vreg->value, 0);
            printf(", lives from %d to %d; ", vreg->min_range->insn_num, vreg->max_range->insn_num);
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
#endif

int ejit_compile(jit_function_t func, void **entry_point)
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
    func->ejit = ejit_create_instance(func);
    ejit_compute_liveness(func);
    // ejit_preallocate_global_registers(func);
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
    ejit_preallocate_global_registers(func);
    for(;;)
    {
        ejit_reinit(func);
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
            ejit_compile_block(&gen, func, block);
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
#ifdef EJIT_DEBUG_ENABLED
    ejit_debug_print_vregs_ranges(func);
#endif
    /* Free the builder structure, which no longer require */
    _jit_function_free_builder(func);

    /* Record the entry point */
    if(entry_point)
    {
        *entry_point = start;
    }
    return 1;
}


#endif
