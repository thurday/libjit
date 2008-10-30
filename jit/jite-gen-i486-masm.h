/*
 * jite-gen-i486-masm.h: Macro assembler for generating machine code.
 *
 */

#ifndef JIT_GEN_I486_MASM_H
#define JIT_GEN_I486_MASM_H

#include <jit-gen-i486-simd.h>
#include <jite-i486-extra-arith.h>
#include <jite-linear-scan.h>

// #define DEBUG_ENABLED 1



#define JITE_N_GP_REGISTERS     6
#define JITE_N_XMM_REGISTERS    8

#define LOCAL_ALLOCATE_FOR_INPUT  0x0
#define LOCAL_ALLOCATE_FOR_OUTPUT 0x1
#define LOCAL_ALLOCATE_FOR_TEMP   0x2

unsigned char jite_gp_reg_index_is_free(void *handler, int index);
unsigned char jite_xmm_reg_index_is_free(void *handler, int index);
unsigned char jite_gp_reg_is_free(jit_function_t func, int index);
unsigned char jite_xmm_reg_is_free(jit_function_t func, int index);


unsigned char *jite_allocate_local_register(unsigned char *inst, jit_function_t func, jite_vreg_t vreg, jite_vreg_t vreg1, jite_vreg_t vreg2, unsigned char bUsage, unsigned int fRegCond, int typeKind, unsigned int *regFound);

// Maps which general-purpose register represents which index from 0 to 5
// (not using EBP, and ESP, which are 5, and 4, respectively).
int gp_reg_map[8] = {0, 2, 1, 3, -1, -1, 5, 4};
// Maping of XMM registers to index is straightforward as 1 to 1.

struct local_regs_allocator
{
    unsigned int gpreg1;
    unsigned int save_gpreg1;
    unsigned int gpreg2;
    unsigned int save_gpreg2;
    unsigned int gpreg3;
    unsigned int save_gpreg3;
    unsigned int xmmreg1;
    unsigned int save_xmmreg1;
    unsigned int xmmreg2;
    unsigned int save_xmmreg2;
    unsigned int xmmreg3;
    unsigned int save_xmmreg3;
    unsigned int free_gpregs_index_map;
};

typedef struct local_regs_allocator *local_regs_allocator_t;

#define gpreg1 lrs->gpreg1
#define save_gpreg1 lrs->save_gpreg1
#define gpreg2 lrs->gpreg2
#define save_gpreg2 lrs->save_gpreg2
#define gpreg3 lrs->gpreg3
#define save_gpreg3 lrs->save_gpreg3
#define xmmreg1 lrs->xmmreg1
#define save_xmmreg1 lrs->save_xmmreg1
#define xmmreg2 lrs->xmmreg2
#define save_xmmreg2 lrs->save_xmmreg2
#define xmmreg3 lrs->xmmreg3
#define save_xmmreg3 lrs->save_xmmreg3
// #define sp_offset lrs->sp_offset

#define free_gpregs_index_map lrs->free_gpregs_index_map

#define init_local_register_allocation(inst) \
        struct local_regs_allocator temp;    \
        local_regs_allocator_t lrs = &temp;    \
        gpreg1 = 0; save_gpreg1 = 0;    \
        gpreg2 = 0; save_gpreg2 = 0; \
        gpreg3 = 0; save_gpreg3 = 0; \
        xmmreg1 = 0; save_xmmreg1 = 0;    \
        xmmreg2 = 0; save_xmmreg2 = 0;    \
        xmmreg3 = 0; save_xmmreg3 = 0;    \
        free_gpregs_index_map = 0;

static unsigned int count_free_gpregs_index(void *handler)
{
        unsigned int count = 0, index;
        for(index = 0; (index < JITE_N_GP_REGISTERS); index++)
        {
            unsigned int reg_is_free = jite_gp_reg_index_is_free(handler, index);
            if(reg_is_free) count = count | (1 << index);
        }
        return count;
}

static unsigned int count_free_xmmregs_index(void *handler)
{
        unsigned int count = 0, index;
        for(index = 0; (index < JITE_N_XMM_REGISTERS); index++)
        {
            unsigned int reg_is_free = jite_xmm_reg_index_is_free(handler, index);
            if(reg_is_free) count = count | (0x40 << index);
        }
        return count;
}

#define have_free_gpregs() count_free_gpregs_index(func)

#define have_free_xmmregs() count_free_xmmregs_index(func)

#define gpreg_index_is_free(index) (free_gpregs_index_map & (1 << index))

static unsigned char *__find_one_gp_reg(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst)
{
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, 0, JIT_TYPE_PTR, &gpreg1);
    return inst;
}

#define find_one_gp_reg(inst) inst = __find_one_gp_reg(func, lrs, inst);

#define find_one_gp_reg_cond1(inst, cond1) inst = __find_one_gp_reg_cond1(func, lrs, inst, cond1);

static unsigned char* __find_one_gp_reg_cond1(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1)
{
    unsigned int cond = jite_gp_regs_map[gp_reg_map[cond1]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg1);
    return inst;
}


#define find_one_gp_reg_cond2(inst, cond1, cond2) inst = __find_one_gp_reg_cond2(func, lrs, inst, cond1, cond2);

static unsigned char* __find_one_gp_reg_cond2(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1, unsigned char cond2)
{
    unsigned int cond = jite_gp_regs_map[gp_reg_map[cond1]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond2]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg1);
    return inst;
}

#define find_one_gp_reg_cond3(inst, cond1, cond2, cond3) inst = __find_one_gp_reg_cond3(func, lrs, inst, cond1, cond2, cond3);

static unsigned char* __find_one_gp_reg_cond3(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1, unsigned char cond2, unsigned char cond3)
{
    unsigned int cond = jite_gp_regs_map[gp_reg_map[cond1]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond2]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond3]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg1);
    return inst;
}
#define find_one_gp_reg_cond4(inst, cond1, cond2, cond3, cond4) inst = __find_one_gp_reg_cond4(func, lrs, inst, cond1, cond2, cond3, cond4);

static unsigned char* __find_one_gp_reg_cond4(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1, unsigned char cond2, unsigned char cond3, unsigned char cond4)
{
    unsigned int cond = jite_gp_regs_map[gp_reg_map[cond1]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond2]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond3]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond4]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg1);
    return inst;
}

#define find_one_gp_reg_cond6(inst, cond1, cond2, cond3, cond4, cond5, cond6) inst = __find_one_gp_reg_cond6(func, lrs, inst, cond1, cond2, cond3, cond4, cond5, cond6);

static unsigned char* __find_one_gp_reg_cond6(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1, unsigned char cond2, unsigned char cond3, unsigned char cond4, unsigned char cond5, unsigned char cond6)
{
    unsigned int cond = jite_gp_regs_map[gp_reg_map[cond1]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond2]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond3]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond4]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond5]].hash_code;
    cond |= jite_gp_regs_map[gp_reg_map[cond6]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg1);
    return inst;
}


static unsigned char *__find_two_gp_regs(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst)
{
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, 0, JIT_TYPE_PTR, &gpreg1);
    unsigned int cond = jite_gp_regs_map[gp_reg_map[gpreg1]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg2);
    return inst;
}

#define find_two_gp_regs(inst) inst = __find_two_gp_regs(func, lrs, inst);

#define find_two_gp_regs_cond1(inst, cond1) inst = __find_two_gp_regs_cond1(func, lrs, inst, cond1);

static unsigned char* __find_two_gp_regs_cond1(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1)
{
    unsigned int cond = jite_gp_regs_map[gp_reg_map[cond1]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg1);
    cond |= jite_gp_regs_map[gp_reg_map[gpreg1]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_PTR, &gpreg2);
    return inst;
}


#define find_two_gp_regs_cond1_cond2_for_gpreg1(inst, cond1, cond2, cond3) inst = __find_two_gp_regs_cond1_cond2_for_gpreg1(func, lrs, inst, cond1, cond2, cond3);

static unsigned char* __find_two_gp_regs_cond1_cond2_for_gpreg1(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1, unsigned char cond2, unsigned char cond3)
{
    unsigned int regCond1 = jite_gp_regs_map[gp_reg_map[cond1]].hash_code;
    unsigned int regCond2 = regCond1 | jite_gp_regs_map[gp_reg_map[cond2]].hash_code;
    regCond2 |= jite_gp_regs_map[gp_reg_map[cond3]].hash_code;

    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, regCond2, JIT_TYPE_PTR, &gpreg1);
    regCond1 |= jite_gp_regs_map[gp_reg_map[gpreg1]].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, regCond1, JIT_TYPE_PTR, &gpreg2);
    return inst;
}


#define release_two_gp_regs(inst) inst = jite_restore_local_registers(inst, func, jite_gp_regs_map[gp_reg_map[gpreg1]].hash_code | jite_gp_regs_map[gp_reg_map[gpreg2]].hash_code);

#define release_one_gp_reg(inst) inst = jite_restore_local_registers(inst, func, jite_gp_regs_map[gp_reg_map[gpreg1]].hash_code);

static unsigned char *__find_one_xmm_reg(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst)
{
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, 0, JIT_TYPE_FLOAT64, &xmmreg1);
    return inst;
}

#define find_one_xmm_reg(inst) inst = __find_one_xmm_reg(func, lrs, inst);

#define find_one_xmm_reg_cond1(inst, cond1) inst = __find_one_xmm_reg_cond1(func, lrs, inst, cond1);

static unsigned char* __find_one_xmm_reg_cond1(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, unsigned char cond1)
{
    unsigned int cond = jite_xmm_regs_map[cond1].hash_code;
    inst = jite_allocate_local_register(inst, func, 0, 0, 0, LOCAL_ALLOCATE_FOR_TEMP, cond, JIT_TYPE_FLOAT64, &xmmreg1);
    return inst;
}

#define release_one_xmm_reg(inst) inst = jite_restore_local_registers(inst, func, jite_xmm_regs_map[xmmreg1].hash_code);

#define set_gpreg_as_scratched(gpreg) \
    func->jite->scratch_regs |= (0x1 << gp_reg_map[gpreg]); \


/*
 * Set a register value based on a condition code.
 */
#define setcc_reg(inst,reg,cond,is_signed)    \
do {    \
    if(reg == X86_EAX || reg == X86_EBX || reg == X86_ECX || reg == X86_EDX)    \
    {    \
        x86_set_reg((inst), (cond), (reg), (is_signed));    \
        x86_widen_reg((inst), (reg), (reg), 0, 0);    \
    }    \
    else    \
    {    \
        unsigned char *patch1, *patch2;    \
        patch1 = inst;    \
        x86_branch8((inst), (cond), 0, (is_signed));    \
        x86_clear_reg((inst), (reg));    \
        patch2 = inst;    \
        x86_jump8((inst), 0);    \
        x86_patch((patch1), (inst));    \
        x86_mov_reg_imm((inst), (reg), 1);    \
        x86_patch((patch2), (inst));    \
    }    \
} while (0)

#define setcc_membase(inst,basereg,disp,cond,is_signed) \
do {    \
    unsigned char *patch1, *patch2; \
    patch1 = inst; \
    x86_branch8((inst), (cond), 0, (is_signed));    \
    x86_mov_membase_imm((inst), (basereg), (disp), 0, 4);    \
    patch2 = inst;    \
    x86_jump8((inst), 0);    \
    x86_patch((patch1), (inst));    \
    x86_mov_membase_imm((inst), (basereg), (disp), 1, 4);    \
    x86_patch((patch2), (inst));    \
} while (0)


/*
 * Get the long form of a branch opcode.
 */
static int long_form_branch(int opcode)
{
    if(opcode == 0xEB)
    {
        return 0xE9;
    }
    else
    {
        return opcode + 0x0F10;
    }
}

/*
 * Output a branch instruction.
 */
static unsigned char *output_branch
    (jit_function_t func, unsigned char *inst, int opcode, jit_insn_t insn)
{
    jit_block_t block;
    int offset;
    if((insn->flags & JIT_INSN_VALUE1_IS_LABEL) != 0)
    {
        /* "address_of_label" instruction */
        block = jit_block_from_label(func, (jit_label_t)(insn->value1));
    }
    else
    {
        block = jit_block_from_label(func, (jit_label_t)(insn->dest));
    }
    if(!block)
    {
        return inst;
    }
    if(block->address)
    {
        /* We already know the address of the block */
        offset = ((unsigned char *)(block->address)) - (inst + 2);
        if(x86_is_imm8(offset))
        {
            /* We can output a short-form backwards branch */
            *inst++ = (unsigned char)opcode;
            *inst++ = (unsigned char)offset;
        }
        else
        {
            /* We need to output a long-form backwards branch */
            offset -= 3;
            opcode = long_form_branch(opcode);
            if(opcode < 256)
            {
                *inst++ = (unsigned char)opcode;
            }
            else
            {
                *inst++ = (unsigned char)(opcode >> 8);
                *inst++ = (unsigned char)opcode;
                --offset;
            }
            x86_imm_emit32(inst, offset);
        }
    }
    else
    {
        /* Output a placeholder and record on the block's fixup list */
        opcode = long_form_branch(opcode);
        if(opcode < 256)
        {
            *inst++ = (unsigned char)opcode;
        }
        else
        {
            *inst++ = (unsigned char)(opcode >> 8);
            *inst++ = (unsigned char)opcode;
        }
        x86_imm_emit32(inst, (int)(block->fixup_list));
        block->fixup_list = (void *)(inst - 4);
    }
    return inst;
}

// Pseudo instruction for 'push' of XMM registers.

#define sse2_pushsd_xmreg(inst,dreg)        \
    do {    \
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 8);    \
            sse2_movsd_membase_xmreg(inst, X86_ESP, 0, (dreg));    \
    } while(0)

#define sse2_popsd_xmreg(inst,dreg)        \
    do {    \
            sse2_movsd_xmreg_xmreg(inst, (dreg), X86_ESP, 0);    \
            x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 8); \
    } while(0)

#define sse2_pushq_xmreg(inst,dreg)        \
    do {    \
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 8);    \
            sse2_movq_membase_xmreg(inst, X86_ESP, 0, (dreg));    \
    } while(0)

#define sse2_popq_xmreg(inst,dreg)        \
    do {    \
            sse2_movq_xmreg_membase(inst, (dreg), X86_ESP, 0);    \
            x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 8); \
    } while(0)

#define sse_pushss_xmreg(inst,dreg)        \
    do {    \
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 4);    \
            sse_movss_membase_xmreg(inst, X86_ESP, 0, (dreg));    \
    } while(0)

#define sse_popss_xmreg(inst,dreg)        \
    do {    \
            sse_movss_xmreg_membase(inst, (dreg), X86_ESP, 0);    \
            x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 4);    \
    } while(0)


// Assembler Macros
#define masm_align(inst,modulus)    \
    do {    \
        while(((unsigned int)inst) % modulus) x86_nop(inst);    \
    } while (0)

#define masm_enter(inst)        \
    do {    \
        x86_push_reg((inst), (X86_EBP));        \
        x86_mov_reg_reg((inst), (X86_EBP), (X86_ESP), (4));    \
    } while (0)

#define masm_leave(inst)        \
    do {    \
        x86_mov_reg_reg((inst), (X86_ESP), (X86_EBP), (4));    \
        x86_pop_reg((inst), (X86_EBP));        \
    } while (0)



/*
 * Copy a block of memory that has a specific size.  Other than
 * the parameter pointers, all registers must be unused at this point.
 */

unsigned char *jite_memory_copy_with_reg
    (unsigned char *inst, int dreg, jit_nint doffset,
     int sreg, jit_nint soffset, jit_nuint size, int temp_reg)
{
    if(size <= 4 * sizeof(void *))
    {
        /* Use direct copies to copy the memory */
        int offset = 0;
        while(size >= sizeof(void *))
        {
            x86_mov_reg_membase(inst, temp_reg, sreg,
                                soffset + offset, sizeof(void *));
            x86_mov_membase_reg(inst, dreg, doffset + offset,
                                temp_reg, sizeof(void *));
            size -= sizeof(void *);
            offset += sizeof(void *);
        }
    #ifdef JIT_NATIVE_INT64
        if(size >= 4)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 4);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 4);
            size -= 4;
            offset += 4;
        }
    #endif
        if(size >= 2)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 2);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 2);
            size -= 2;
            offset += 2;
        }
        if(size >= 1)
        {
            /* We assume that temp_reg is EAX, ECX, or EDX, which it
               should be after calling "get_temp_reg" */
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 1);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 1);
        }
    }
    else
    {        
        /* Call out to "jit_memcpy" to effect the copy */

        x86_push_imm(inst, size);
        if(sreg != X86_ESP)
        {
            if(soffset == 0)
            {
                x86_push_reg(inst, sreg);
            }
            else
            {
                soffset += sizeof(void *);
                x86_lea_membase(inst, temp_reg, sreg, soffset);
                x86_push_reg(inst, temp_reg);
            }
        }
        else
        {
            x86_lea_membase(inst, temp_reg, sreg, soffset + sizeof(void *));
            x86_push_reg(inst, temp_reg);
        }
        if(dreg != X86_ESP)
        {
            if(doffset == 0)
            {
                x86_push_reg(inst, dreg);
            }
            else
            {
                x86_lea_membase(inst, temp_reg, dreg, doffset);
                x86_push_reg(inst, temp_reg);
            }
        }
        else
        {

            /* Copying a structure value onto the stack */
            x86_lea_membase(inst, temp_reg, X86_ESP,
                            doffset + 2 * sizeof(void *));
            x86_push_reg(inst, temp_reg);
        }
        x86_call_code(inst, jit_memcpy);
        x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 3 * sizeof(void *));
    }
    return inst;
}

// Registers EAX, EDX and ECX are scratched if an external function is called
// to perform this operation.
unsigned char *jite_memory_copy
    (unsigned char *inst, int dreg, jit_nint doffset,
     int sreg, jit_nint soffset, jit_nuint size, int temp_reg)
{
    if(size <= 4 * sizeof(void *))
    {
        /* Use direct copies to copy the memory */
        int offset = 0;
        while(size >= sizeof(void *))
        {
            x86_mov_reg_membase(inst, temp_reg, sreg,
                                soffset + offset, sizeof(void *));
            x86_mov_membase_reg(inst, dreg, doffset + offset,
                                temp_reg, sizeof(void *));
            size -= sizeof(void *);
            offset += sizeof(void *);
        }
    #ifdef JIT_NATIVE_INT64
        if(size >= 4)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 4);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 4);
            size -= 4;
            offset += 4;
        }
    #endif
        if(size >= 2)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 2);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 2);
            size -= 2;
            offset += 2;
        }
        if(size >= 1)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 1);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 1);
        }
    }
    else
    {
        /* Call out to "jit_memcpy" to effect the copy */
        x86_push_imm(inst, size);
        if(soffset == 0)
        {
            x86_push_reg(inst, sreg);
        }
        else
        {
            x86_lea_membase(inst, temp_reg, sreg, soffset);
            x86_push_reg(inst, temp_reg);
        }
        if(dreg != X86_ESP)
        {
            if(doffset == 0)
            {
                x86_push_reg(inst, dreg);
            }
            else
            {
                x86_lea_membase(inst, temp_reg, dreg, doffset);
                x86_push_reg(inst, temp_reg);
            }
        }
        else
        {
            /* Copying a structure value onto the stack */
            x86_lea_membase(inst, temp_reg, X86_ESP,
                            doffset + 5 * sizeof(void *));
            x86_push_reg(inst, temp_reg);
        }
        x86_call_code(inst, jit_memcpy);
        x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 3 * sizeof(void *));
    }
    return inst;
}

// Registers EAX, EDX and ECX are scratched if an external function is called
// to perform this operation.
unsigned char *jite_memory_copy_to_mem
    (unsigned char *inst, void *doffset,
     int sreg, jit_nint soffset, jit_nuint size, int temp_reg)
{
    if(size <= 4 * sizeof(void *))
    {
        /* Use direct copies to copy the memory */
        int offset = 0;
        while(size >= sizeof(void *))
        {
            x86_mov_reg_membase(inst, temp_reg, sreg,
                                soffset + offset, sizeof(void *));
            x86_mov_mem_reg(inst, doffset + offset,
                                temp_reg, sizeof(void *));
            size -= sizeof(void *);
            offset += sizeof(void *);
        }
    #ifdef JIT_NATIVE_INT64
        if(size >= 4)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 4);
            x86_mov_mem_reg(inst, doffset + offset, temp_reg, 4);
            size -= 4;
            offset += 4;
        }
    #endif
        if(size >= 2)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 2);
            x86_mov_mem_reg(inst, doffset + offset, temp_reg, 2);
            size -= 2;
            offset += 2;
        }
        if(size >= 1)
        {
            x86_mov_reg_membase(inst, temp_reg, sreg, soffset + offset, 1);
            x86_mov_mem_reg(inst, doffset + offset, temp_reg, 1);
        }
    }
    else
    {
        /* Call out to "jit_memcpy" to effect the copy */
        x86_push_imm(inst, size);
        if(soffset == 0)
        {
            x86_push_reg(inst, sreg);
        }
        else
        {
            x86_lea_membase(inst, temp_reg, sreg, soffset);
            x86_push_reg(inst, temp_reg);
        }
        x86_push_imm(inst, doffset);
        x86_call_code(inst, jit_memcpy);
        x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 3 * sizeof(void *));
    }
    return inst;
}


// Registers EAX, EDX and ECX are scratched if an external function is called
// to perform this operation.
unsigned char *jite_memory_copy_from_mem
    (unsigned char *inst, int dreg, jit_nint doffset,
     void *soffset, jit_nuint size, int temp_reg)
{
    if(size <= 4 * sizeof(void *))
    {
        /* Use direct copies to copy the memory */
        int offset = 0;
        while(size >= sizeof(void *))
        {
            x86_mov_reg_mem(inst, temp_reg, soffset + offset, sizeof(void *));
            x86_mov_membase_reg(inst, dreg, doffset + offset,
                                temp_reg, sizeof(void *));
            size -= sizeof(void *);
            offset += sizeof(void *);
        }
    #ifdef JIT_NATIVE_INT64
        if(size >= 4)
        {
            x86_mov_reg_mem(inst, temp_reg, soffset + offset, 4);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 4);
            size -= 4;
            offset += 4;
        }
    #endif
        if(size >= 2)
        {
            x86_mov_reg_mem(inst, temp_reg, soffset + offset, 2);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 2);
            size -= 2;
            offset += 2;
        }
        if(size >= 1)
        {
            /* We assume that temp_reg is EAX, ECX, or EDX, which it
               should be after calling "get_temp_reg" */
            x86_mov_reg_mem(inst, temp_reg, soffset + offset, 1);
            x86_mov_membase_reg(inst, dreg, doffset + offset, temp_reg, 1);
        }
    }
    else
    {
        /* Call out to "jit_memcpy" to effect the copy */
        x86_push_imm(inst, size);
        x86_push_imm(inst, soffset);
        if(dreg != X86_ESP)
        {
            if(doffset == 0)
            {
                x86_push_reg(inst, dreg);
            }
            else
            {
                x86_lea_membase(inst, temp_reg, dreg, doffset);
                x86_push_reg(inst, temp_reg);
            }
        }
        else
        {
            /* Copying a structure value onto the stack */
            x86_lea_membase(inst, temp_reg, X86_ESP,
                            doffset + 5 * sizeof(void *));
            x86_push_reg(inst, temp_reg);
        }
        x86_call_code(inst, jit_memcpy);
        x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 3 * sizeof(void *));
    }
    return inst;
}

#define masm_mov_membase_membase(inst, dreg, doffset, sreg, soffset, size) _masm_mov_membase_membase(func, lrs, inst, dreg, doffset, sreg, soffset, size);

unsigned char * _masm_mov_membase_membase(jit_function_t func, local_regs_allocator_t lrs, unsigned char *inst, int dreg, jit_nint doffset, int sreg, jit_nint soffset, jit_nuint size)
{
    if(size > 0 && (dreg != sreg || doffset != soffset))
    {
        if((size % 2) == 0)
        {
            find_one_gp_reg_cond2(inst, sreg, dreg);
        }
        else
        {
            find_one_gp_reg_cond6(inst, sreg, dreg, X86_EBX, X86_EDI, X86_ESI, X86_EBP); // Hopefully one register can be found
        }

        if(save_gpreg1)
        {
            if(dreg == X86_ESP || sreg == X86_ESP)
            {
                x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 16);
            }
            if(dreg == X86_ESP) // try to do the operation anyway
            {
                doffset += 16;
            }
            if(sreg == X86_ESP)
            {
                soffset += 16;
            }
        }

        if(size <= (4 * sizeof(void *)))
        {
            if(doffset < soffset)
            {
                while(size >= sizeof(void*))
                {
                        x86_mov_reg_membase(inst, gpreg1, sreg, soffset, sizeof(void *));
                        x86_mov_membase_reg(inst, dreg, doffset, gpreg1, sizeof(void *));
                    soffset += sizeof(void *);
                    doffset += sizeof(void *);
                    size -= sizeof(void *);
                }

                while(size >= 4)
                {
                        x86_mov_reg_membase(inst, gpreg1, sreg, soffset, 4);
                        x86_mov_membase_reg(inst, dreg, doffset, gpreg1, 4);
                    soffset += 4;
                    doffset += 4;
                    size -= 4;
                }

                while(size >= 2)
                {
                        x86_mov_reg_membase(inst, gpreg1, sreg, soffset, 2);
                        x86_mov_membase_reg(inst, dreg, doffset, gpreg1, 2);
                    soffset += 2;
                    doffset += 2;
                    size -= 2;
                }

                while(size >= 1)
                {
                        x86_mov_reg_membase(inst, gpreg1, sreg, soffset, 1);
                        x86_mov_membase_reg(inst, dreg, doffset, gpreg1, 1);
                    soffset ++;
                    doffset ++;
                    size--;
                }
            }
            else
            {
                while(size >= sizeof(void *))
                {
                    size -= sizeof(void *);
                    x86_mov_reg_membase(inst, gpreg1, sreg, soffset + size, sizeof(void *));
                    x86_mov_membase_reg(inst, dreg, doffset + size, gpreg1, sizeof(void *));
                }

                while(size >= 4)
                {
                    size -= 4;
                    x86_mov_reg_membase(inst, gpreg1, sreg, soffset + size, 4);
                    x86_mov_membase_reg(inst, dreg, doffset + size, gpreg1, 4);
                }

                while(size >= 2)
                {
                    size -= 2;
                    x86_mov_reg_membase(inst, gpreg1, sreg, soffset + size, 2);
                    x86_mov_membase_reg(inst, dreg, doffset + size, gpreg1, 2);
                }

                while(size >= 1)
                {
                    size--;
                    x86_mov_reg_membase(inst, gpreg1, sreg, soffset + size, 1);
                    x86_mov_membase_reg(inst, dreg, doffset + size, gpreg1, 1);
                }
            }
        }
        else
        {
            inst = jite_memory_copy(inst, dreg, doffset, sreg, soffset, size, (int)(gpreg1));
        }
        if(save_gpreg1 && (sreg == X86_ESP || dreg == X86_ESP))
        {
            x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 16);
        }
        release_one_gp_reg(inst);
    }
    
    return inst;
}


unsigned char *masm_mov_membase_reg(unsigned char *inst, unsigned char basereg, unsigned int offset, unsigned char sreg, jit_type_t type)
{
    if(sreg >= 16) sreg -= 16;

    type = jit_type_remove_tags(type);
    int typeKind = jit_type_get_kind(type);
    switch(typeKind)
    {
        CASE_USE_WORD
        {
            x86_mov_membase_reg(inst, basereg, offset, sreg, 4);
        }
        break;
        CASE_USE_LONG
        {
            x86_mov_membase_reg(inst, basereg, offset, sreg, 4);
            x86_mov_membase_reg(inst, basereg, offset + 4, jite_gp_regs_map[gp_reg_map[sreg] + 1].reg, 4);
        }
        break;
        case JIT_TYPE_FLOAT32:
        {
            sse_movss_membase_xmreg(inst, basereg, offset, sreg);
        }
        break;
        case JIT_TYPE_FLOAT64:
        {
            sse2_movsd_membase_xmreg(inst, basereg, offset, sreg);
        }
        break;
        case JIT_TYPE_NFLOAT:
        {
            if(sizeof(jit_nfloat) == sizeof(jit_float64))
            {
                sse2_movsd_membase_xmreg(inst, basereg, offset, sreg);
            }
        }
        break;
    }
    return inst;
}

unsigned char *masm_mov_reg_membase(unsigned char *inst, unsigned char dreg, unsigned char basereg, unsigned int offset, jit_type_t type)
{
    type = jit_type_remove_tags(type);
    int typeKind = jit_type_get_kind(type);

    if(dreg >= 16) dreg -= 16;

    switch(typeKind)
    {
        CASE_USE_WORD
        {
            x86_mov_reg_membase(inst, dreg, basereg, offset, 4);
        }
        break;
        CASE_USE_LONG
        {
            if(dreg != basereg)
            {
                x86_mov_reg_membase(inst, dreg, basereg, offset, 4);
                x86_mov_reg_membase(inst, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, basereg, offset + 4, 4);
            }
            else
            {
                x86_mov_reg_membase(inst, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, basereg, offset + 4, 4);
                x86_mov_reg_membase(inst, dreg, basereg, offset, 4);
            }
        }
        break;
        case JIT_TYPE_FLOAT32:
        {
            sse_movss_xmreg_membase(inst, dreg, basereg, offset);
        }
        break;
        case JIT_TYPE_FLOAT64:
        {
            sse2_movsd_xmreg_membase(inst, dreg, basereg, offset);
        }
        break;
        case JIT_TYPE_NFLOAT:
        {
            if(sizeof(jit_nfloat) == sizeof(jit_float64))
            {
                sse2_movsd_xmreg_membase(inst, dreg, basereg, offset);
            }
        }
        break;
    }
    return inst;
}

unsigned char *masm_mov_reg_reg(unsigned char *inst, unsigned char dreg, unsigned char sreg, jit_type_t type)
{
    type = jit_type_remove_tags(type);
    int typeKind = jit_type_get_kind(type);

    if(dreg >= 16) dreg -= 16;
    if(sreg >= 16) sreg -= 16;

    switch(typeKind)
    {
        CASE_USE_WORD
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
        }
        break;
        CASE_USE_LONG
        {
            if(dreg != sreg)
            {
                if(gp_reg_map[dreg] < gp_reg_map[sreg])
                {
                    x86_mov_reg_reg(inst, dreg, sreg, 4);
                    x86_mov_reg_reg(inst, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, jite_gp_regs_map[gp_reg_map[sreg] + 1].reg, 4);
                }
                else
                {
                    x86_mov_reg_reg(inst, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, jite_gp_regs_map[gp_reg_map[sreg] + 1].reg, 4);
                    x86_mov_reg_reg(inst, dreg, sreg, 4);
                }
            }
        }
        break;
        case JIT_TYPE_FLOAT32:
        {
            if(dreg != sreg) sse_movss_xmreg_xmreg(inst, dreg, sreg);
        }
        break;
        case JIT_TYPE_FLOAT64:
        {
            if(dreg != sreg) sse2_movsd_xmreg_xmreg(inst, dreg, sreg);
        }
        break;
        case JIT_TYPE_NFLOAT:
        {
            if(sizeof(jit_nfloat) == sizeof(jit_float64))
            {
                if(dreg != sreg) sse2_movsd_xmreg_xmreg(inst, dreg, sreg);
            }
        }
        break;
    }
    return inst;
}

unsigned char* masm_mov_membase_imm(unsigned char *inst, unsigned char basereg, unsigned int offset,  jit_nint address, jit_type_t type)
{
    type = jit_type_remove_tags(type);
    int typeKind = jit_type_get_kind(type);

    switch(typeKind)
    {
        CASE_USE_LONG
        {
            x86_mov_membase_imm(inst, basereg, offset, ((jit_int *)(address))[0], 4);
            x86_mov_membase_imm(inst, basereg, offset + 4, ((jit_int *)(address))[1], 4);
        }
        break;
        case JIT_TYPE_FLOAT32:
        {
            x86_mov_membase_imm(inst, basereg, offset, ((jit_int *)(address))[0], 4);
        }
        break;

        case JIT_TYPE_FLOAT64:
        {
            x86_mov_membase_imm(inst, basereg, offset, ((jit_int *)(address))[0], 4);
            x86_mov_membase_imm(inst, basereg, offset + 4, ((jit_int *)(address))[1], 4);
        }
        break;
        case JIT_TYPE_NFLOAT:
        {
            x86_mov_membase_imm(inst, basereg, offset, ((jit_int *)(address))[0], 4);
            x86_mov_membase_imm(inst, basereg, offset + 4, ((jit_int *)(address))[1], 4);
            if(sizeof(jit_nfloat) != sizeof(jit_float64))
            {
                x86_mov_membase_imm(inst, basereg, offset + 8, ((jit_int *)(address))[2], 4);
            }
        }
        break;
        default:
        {
            x86_mov_membase_imm(inst, basereg, offset, address, 4);
        }
        break;
    }

    return inst;
}

unsigned char* masm_mov_reg_imm(unsigned char *inst, unsigned char dreg, jit_nint address, jit_type_t type)
{
    if(dreg >= 16) dreg -= 16;

    type = jit_type_remove_tags(type);
    int typeKind = jit_type_get_kind(type);

    switch(typeKind)
    {
        CASE_USE_LONG
        {
            x86_mov_reg_imm(inst, dreg, ((jit_nint *)(address))[0]);
            x86_mov_reg_imm(inst, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, ((jit_nint *)(address))[1]);
        }
        break;
        case JIT_TYPE_FLOAT32:
        {
            x86_mov_membase_imm(inst, X86_ESP, -32, ((jit_nint *)(address))[0], 4);
            sse_movss_xmreg_membase(inst, dreg, X86_ESP, -32);
        }
        break;
        case JIT_TYPE_FLOAT64:
        {
            x86_mov_membase_imm(inst, X86_ESP, -32, ((jit_nint *)(address))[0], 4);
            x86_mov_membase_imm(inst, X86_ESP, -28, ((jit_nint *)(address))[1], 4);
            sse2_movsd_xmreg_membase(inst, dreg, X86_ESP, -32);
        }
        break;
        case JIT_TYPE_NFLOAT:
        {
            if(sizeof(jit_nfloat) == sizeof(jit_float64))
            {
                x86_mov_membase_imm(inst, X86_ESP, -32, ((jit_nint *)(address))[0], 4);
                x86_mov_membase_imm(inst, X86_ESP, -28, ((jit_nint *)(address))[1], 4);
                sse2_movsd_xmreg_membase(inst, dreg, X86_ESP, -32);
            }
        }
        break;
        default:
        {
            if(address != 0)
            {
                x86_mov_reg_imm(inst, dreg, address);
            }
            else
            {
                x86_alu_reg_reg(inst, X86_XOR, dreg, dreg);
            }
        }
        break;
    }

    return inst;
}

unsigned char *masm_alu_reg_reg(unsigned char *inst, unsigned int opc1, unsigned int opc2, unsigned int dreg, unsigned int sreg)
{
    x86_alu_reg_reg(inst, opc1, dreg, sreg);
    if(opc2) x86_alu_reg_reg(inst, opc2, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, jite_gp_regs_map[gp_reg_map[sreg] + 1].reg);

    return inst;
}

unsigned char *masm_alu_membase_imm(unsigned char *inst, unsigned int opc1, unsigned int opc2, unsigned int basereg, jit_nint offset, jit_nint address)
{
    if(opc2)
    {
        x86_alu_membase_imm(inst, opc1, basereg, offset, ((jit_nint *)(address))[0]);
        x86_alu_membase_imm(inst, opc2, basereg, offset + 4, ((jit_nint *)(address))[1]);
    }
    else
    {
        x86_alu_membase_imm(inst, opc1, basereg, offset, address);
    }

    return inst;
}

unsigned char *masm_alu_reg_imm(unsigned char *inst, unsigned int opc1, unsigned int opc2, unsigned int dreg, jit_nint address)
{
    if(opc2)
    {
        x86_alu_reg_imm(inst, opc1, dreg, ((jit_nint *)(address))[0]);
        x86_alu_reg_imm(inst, opc2, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, ((jit_nint *)(address))[1]);
    }
    else
    {
            x86_alu_reg_imm(inst, opc1, dreg, address);
    }

    return inst;
}

unsigned char *masm_alu_reg_membase(unsigned char *inst, unsigned int opc1, unsigned int opc2, unsigned int dreg, unsigned int basereg, unsigned int offset)
{
    x86_alu_reg_membase(inst, opc1, dreg, basereg, offset);
    if(opc2) x86_alu_reg_membase(inst, opc2, jite_gp_regs_map[gp_reg_map[dreg] + 1].reg, basereg, offset + 4);

    return inst;
}

unsigned char *masm_alu_membase_reg(unsigned char *inst, unsigned int opc1, unsigned int opc2, unsigned int basereg, unsigned int offset, unsigned int sreg)
{
    x86_alu_membase_reg(inst, opc1, basereg, offset, sreg);
    if(opc2) x86_alu_membase_reg(inst, opc2, basereg, offset + 4, jite_gp_regs_map[gp_reg_map[sreg] + 1].reg);

    return inst;
}

unsigned char* masm_mov_st0_value(unsigned char *inst, jit_value_t value)
{
    if(jite_value_in_reg(value))
    {
        jit_type_t type = jit_value_get_type(value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            case JIT_TYPE_FLOAT32:
            {
                sse_movss_membase_xmreg(inst, X86_ESP, -32, value->vreg->reg->reg);
                x86_fld_membase(inst, X86_ESP, -32, 0);
            }
            break;
            case JIT_TYPE_FLOAT64:
            {
                sse2_movsd_membase_xmreg(inst, X86_ESP, -32, value->vreg->reg->reg);
                x86_fld_membase(inst, X86_ESP, -32, 1);
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) == sizeof(jit_float64))
                {
                    sse2_movsd_membase_xmreg(inst, X86_ESP, -32, value->vreg->reg->reg);
                    x86_fld_membase(inst, X86_ESP, -32, 1);
                }
            }
            break;
        }
    }
    else if(jite_value_in_frame(value))
    {
        jit_type_t type = jit_value_get_type(value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            case JIT_TYPE_FLOAT32:
            {
                x86_fld_membase(inst, X86_EBP, value->vreg->frame->frame_offset, 0);
            }
            break;
            case JIT_TYPE_FLOAT64:
            {
                x86_fld_membase(inst, X86_EBP, value->vreg->frame->frame_offset, 1);
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) == sizeof(jit_float64))
                {
                    x86_fld_membase(inst, X86_EBP, value->vreg->frame->frame_offset, 1);
                }
            }
            break;
        }
    }
    else if(jit_value_is_constant(value))
    {
        inst = masm_mov_membase_imm(inst, X86_ESP, -32, value->address, jit_value_get_type(value));
        jit_type_t type = jit_value_get_type(value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            case JIT_TYPE_FLOAT32:
            {
                x86_fld_membase(inst, X86_ESP, -32, 0);
            }
            break;
            case JIT_TYPE_FLOAT64:
            {
                x86_fld_membase(inst, X86_ESP, -32, 1);
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) == sizeof(jit_float64))
                {
                    x86_fld_membase(inst, X86_ESP, -32, 1);
                }
            }
            break;
        }
    }

    return inst;
}


unsigned char *masm_mov_value_st0(unsigned char *inst, jit_value_t value)
{
    if(jite_value_in_reg(value))
    {
        jit_type_t type = jit_value_get_type(value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            case JIT_TYPE_FLOAT32:
            {
                x86_fst_membase(inst, X86_ESP, -32, 0, 1);
                sse_movss_xmreg_membase(inst, value->vreg->reg->reg, X86_ESP, -32);
            }
            break;
            case JIT_TYPE_FLOAT64:
            {
                x86_fst_membase(inst, X86_ESP, -32, 1, 1);
                sse2_movsd_xmreg_membase(inst, value->vreg->reg->reg, X86_ESP, -32);
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) == sizeof(jit_float64))
                {
                    x86_fst_membase(inst, X86_ESP, -32, 1, 1);
                    sse2_movsd_xmreg_membase(inst, value->vreg->reg->reg, X86_ESP, -32);
                }
            }
            break;
        }
    }
    else if(jite_value_in_frame(value))
    {
        jit_type_t type = jit_value_get_type(value);
        type = jit_type_remove_tags(type);
        int typeKind = jit_type_get_kind(type);
        switch(typeKind)
        {
            case JIT_TYPE_FLOAT32:
            {
                x86_fst_membase(inst, X86_EBP, value->vreg->frame->frame_offset, 0, 1);
            }
            break;
            case JIT_TYPE_FLOAT64:
            {
                x86_fst_membase(inst, X86_EBP, value->vreg->frame->frame_offset, 1, 1);
            }
            break;
            case JIT_TYPE_NFLOAT:
            {
                if(sizeof(jit_nfloat) == sizeof(jit_float64))
                {
                    x86_fst_membase(inst, X86_EBP, value->vreg->frame->frame_offset, 1, 1);
                }
                else
                {
                    x86_fst80_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
                }
            }
            break;
        }
    }
    else // the destination value is never used, so just pop the st0 register
    {
        x86_fstp(inst, 0);
    }

    return inst;
}


#define masm_ldiv_un(inst) \
    _masm_ldiv_un(inst); \
    set_gpreg_as_scratched(X86_EBX); \
    set_gpreg_as_scratched(X86_EDI); \
    set_gpreg_as_scratched(X86_ESI); \

unsigned char *_masm_ldiv_un(unsigned char *inst)
{
    unsigned char *patch1, *patch2, *patch3, *patch4;

    x86_test_reg_reg(inst, X86_ECX, X86_ECX);
    patch1 = inst;
    x86_branch8(inst, X86_CC_NZ, 0, 0);
    // JNZ $bigdivisor

    x86_alu_reg_reg(inst, X86_CMP, X86_EDX, X86_EBX);
    patch2 = inst;
    x86_branch8(inst, X86_CC_AE, 0, 0);

    // JAE $two_divs
    x86_div_reg(inst, X86_EBX, 0);
    x86_mov_reg_reg(inst, X86_EDX, X86_ECX, 4);
    patch3 = inst;
    x86_jump8(inst, 0);
    // jump to end

    x86_patch(patch2, inst);
    // two_divs
    x86_mov_reg_reg(inst, X86_ECX, X86_EAX, 4);
    x86_mov_reg_reg(inst, X86_EAX, X86_EDX, 4);
    x86_alu_reg_reg(inst, X86_XOR, X86_EDX, X86_EDX);
    x86_div_reg(inst, X86_EBX, 0);
    x86_xchg_reg_reg(inst, X86_EAX, X86_ECX, 4);
    x86_div_reg(inst, X86_EBX, 0);
    x86_mov_reg_reg(inst, X86_EDX, X86_ECX, 4);

    patch4 = inst;
    x86_jump8(inst, 0);
    // jump to end

    // big divisor
    x86_patch(patch1, inst);

    x86_mov_membase_reg(inst, X86_ESP, -4, X86_EAX, 4);
    x86_mov_membase_reg(inst, X86_ESP, -8, X86_EDX, 4);
    x86_mov_membase_reg(inst, X86_ESP, -12, X86_EBX, 4);

    x86_mov_reg_reg(inst, X86_EDI, X86_ECX, 4);
    x86_shift_reg_imm(inst, X86_SHR, X86_EDX, 1);
    x86_shift_reg_imm(inst, X86_RCR, X86_EAX, 1);
    x86_shift_reg_imm(inst, X86_ROR, X86_EDI, 1);
    x86_shift_reg_imm(inst, X86_RCR, X86_EBX, 1);
    x86_bsr_reg_reg(inst, X86_ECX, X86_ECX);

    x86_shrd_reg_cl(inst, X86_EBX, X86_EDI);
    x86_shrd_reg_cl(inst, X86_EAX, X86_EDX);
    x86_shift_reg(inst, X86_SHR, X86_EDX);
    x86_shift_reg_imm(inst, X86_ROL, X86_EDI, 0x1);
    x86_div_reg(inst, X86_EBX, 0);
    x86_mov_reg_membase(inst, X86_EBX, X86_ESP, -4, 4);
    x86_mov_reg_reg(inst, X86_ECX, X86_EAX, 4);
    x86_imul_reg_reg(inst, X86_EDI, X86_EAX);
    x86_mul_membase(inst, X86_ESP, -12, 0);
    x86_alu_reg_reg(inst, X86_ADD, X86_EDX, X86_EDI);
    x86_alu_reg_reg(inst, X86_SUB, X86_EBX, X86_EAX);
    x86_mov_reg_reg(inst, X86_EAX, X86_ECX, 4);
    x86_mov_reg_membase(inst, X86_ECX, X86_ESP, -8, 4);
    x86_alu_reg_reg(inst, X86_SBB, X86_ECX, X86_EDX);
    x86_alu_reg_imm(inst, X86_SBB, X86_EAX, 0);
    x86_alu_reg_reg(inst, X86_XOR, X86_EDX, X86_EDX);

    x86_patch(patch3, inst);
    x86_patch(patch4, inst);
    return inst;
}

#define masm_ldiv(inst) \
    _masm_ldiv(inst); \
    set_gpreg_as_scratched(X86_EBX); \
    set_gpreg_as_scratched(X86_EDI); \
    set_gpreg_as_scratched(X86_ESI); \

unsigned char *_masm_ldiv(unsigned char *inst)
{
    unsigned char *patch1, *patch2, *patch3, *patch4;
    x86_mov_reg_reg(inst, X86_ESI, X86_ECX, 4);
    x86_alu_reg_reg(inst, X86_XOR, X86_ESI, X86_EDX); // with high_dword
    x86_shift_reg_imm(inst, X86_SAR, X86_ESI, 31);
    x86_mov_reg_reg(inst, X86_EDI, X86_EDX, 4); // with high_dword
    x86_shift_reg_imm(inst, X86_SAR, X86_EDI, 31);

    x86_alu_reg_reg(inst, X86_XOR, X86_EAX, X86_EDI);
    x86_alu_reg_reg(inst, X86_XOR, X86_EDX, X86_EDI);
    x86_alu_reg_reg(inst, X86_SUB, X86_EAX, X86_EDI);
    x86_alu_reg_reg(inst, X86_SBB, X86_EDX, X86_EDI);
    x86_mov_reg_reg(inst, X86_EDI, X86_ECX, 4);
    x86_shift_reg_imm(inst, X86_SAR, X86_EDI, 31);
    x86_alu_reg_reg(inst, X86_XOR, X86_EBX, X86_EDI);
    x86_alu_reg_reg(inst, X86_XOR, X86_ECX, X86_EDI);
    x86_alu_reg_reg(inst, X86_SUB, X86_EBX, X86_EDI);
    x86_alu_reg_reg(inst, X86_SBB, X86_ECX, X86_EDI);
    patch1 = inst;
    x86_branch8(inst, X86_CC_NZ, 0, 0);
    // JNZ $bigdivisor

    x86_alu_reg_reg(inst, X86_CMP, X86_EDX, X86_EBX);
    patch2 = inst;
    x86_branch8(inst, X86_CC_AE, 0, 0);
    // JAE $two_divs
    x86_div_reg(inst, X86_EBX, 0);
    x86_mov_reg_reg(inst, X86_EDX, X86_ECX, 4);

    x86_alu_reg_reg(inst, X86_XOR, X86_EAX, X86_ESI);
    x86_alu_reg_reg(inst, X86_XOR, X86_EDX, X86_ESI);
    x86_alu_reg_reg(inst, X86_SUB, X86_EAX, X86_ESI);
    x86_alu_reg_reg(inst, X86_SBB, X86_EDX, X86_ESI);
    patch3 = inst;
    x86_jump8(inst, 0);
    // jump to end

    x86_patch(patch2, inst);
    // two_divs
    x86_mov_reg_reg(inst, X86_ECX, X86_EAX, 4);
    x86_mov_reg_reg(inst, X86_EAX, X86_EDX, 4);
    x86_alu_reg_reg(inst, X86_XOR, X86_EDX, X86_EDX);
    x86_div_reg(inst, X86_EBX, 0);
    x86_xchg_reg_reg(inst, X86_EAX, X86_ECX, 4);
    x86_div_reg(inst, X86_EBX, 0);
    x86_mov_reg_reg(inst, X86_EDX, X86_ECX, 4);

    patch4 = inst;
    x86_jump8(inst, 0);
    // jump to make sign

    // big divisor
    x86_patch(patch1, inst);
    x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 12);
    x86_mov_membase_reg(inst, X86_ESP, 0, X86_EAX, 4);
    x86_mov_membase_reg(inst, X86_ESP, 4, X86_EBX, 4);
    x86_mov_membase_reg(inst, X86_ESP, 8, X86_EDX, 4);
    x86_mov_reg_reg(inst, X86_EDI, X86_ECX, 4);
    x86_shift_reg_imm(inst, X86_SHR, X86_EDX, 1);
    x86_shift_reg_imm(inst, X86_RCR, X86_EAX, 1);
    x86_shift_reg_imm(inst, X86_ROR, X86_EDI, 1);
    x86_shift_reg_imm(inst, X86_RCR, X86_EBX, 1);
    x86_bsr_reg_reg(inst, X86_ECX, X86_ECX);

    x86_shrd_reg_cl(inst, X86_EBX, X86_EDI);
    x86_shrd_reg_cl(inst, X86_EAX, X86_EDX);
    x86_shift_reg(inst, X86_SHR, X86_EDX);
    x86_shift_reg_imm(inst, X86_ROL, X86_EDI, 0x1);
    x86_div_reg(inst, X86_EBX, 0);
    x86_mov_reg_membase(inst, X86_EBX, X86_ESP, 0, 4);
    x86_mov_reg_reg(inst, X86_ECX, X86_EAX, 4);
    x86_imul_reg_reg(inst, X86_EDI, X86_EAX);
    x86_mul_membase(inst, X86_ESP, 4, 0);
    x86_alu_reg_reg(inst, X86_ADD, X86_EDX, X86_EDI);
    x86_alu_reg_reg(inst, X86_SUB, X86_EBX, X86_EAX);
    x86_mov_reg_reg(inst, X86_EAX, X86_ECX, 4);
    x86_mov_reg_membase(inst, X86_ECX, X86_ESP, 8, 4);
    x86_alu_reg_reg(inst, X86_SBB, X86_ECX, X86_EDX);
    x86_alu_reg_imm(inst, X86_SBB, X86_EAX, 0);
    x86_alu_reg_reg(inst, X86_XOR, X86_EDX, X86_EDX);
    x86_alu_reg_imm(inst, X86_ADD, X86_ESP, 12);

    x86_patch(patch4, inst);
    // make sign
    x86_alu_reg_reg(inst, X86_XOR, X86_EAX, X86_ESI);
    x86_alu_reg_reg(inst, X86_XOR, X86_EDX, X86_ESI);
    x86_alu_reg_reg(inst, X86_SUB, X86_EAX, X86_ESI);
    x86_alu_reg_reg(inst, X86_SBB, X86_EDX, X86_ESI);

    x86_patch(patch3, inst);
    return inst;
}

unsigned char *masm_lshr_un(unsigned char *inst, unsigned char reg1, unsigned char reg2)
{
    unsigned char *patch;
    x86_alu_reg_imm(inst, X86_AND, X86_ECX, 0x3f);
    x86_alu_reg_imm(inst, X86_CMP, X86_ECX, 32);
    patch = inst;
    x86_branch8(inst, X86_CC_LT, 0, 0);
    x86_mov_reg_reg(inst, reg1, reg2, 4);
    x86_alu_reg_reg(inst, X86_XOR, reg2, reg2);
    x86_patch(patch, inst);
    x86_shrd_reg(inst, reg1, reg2);
    x86_shift_reg(inst, X86_SHR, reg2);
    return inst;
}

unsigned char *masm_lshl(unsigned char *inst, unsigned char reg1, unsigned char reg2)
{
    unsigned char *patch;
    x86_alu_reg_imm(inst, X86_AND, X86_ECX, 0x3f);
    x86_alu_reg_imm(inst, X86_CMP, X86_ECX, 32);
    patch = inst;
    x86_branch8(inst, X86_CC_C, 0, 0);
    x86_mov_reg_reg(inst, reg2, reg1, 4);
    x86_alu_reg_reg(inst, X86_XOR, reg1, reg1);
    x86_patch(patch, inst);
    x86_shld_reg(inst, reg2, reg1);
    x86_shift_reg(inst, X86_SHL, reg1);
    return inst;
}


unsigned char *masm_jump_indirect(unsigned char *inst, jit_value_t value)
{
    if(value->vreg && value->vreg->in_reg)
    {
        x86_jump_reg(inst, value->vreg->reg->reg);
    }
    else if(value->vreg && value->vreg->in_frame)
    {
        x86_jump_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
    }
    else
    {
        x86_jump_mem(inst, value->address);
    }
    return inst;
}

unsigned char *masm_call_indirect(unsigned char *inst, jit_value_t value)
{
    if(value->vreg && value->vreg->in_reg)
    {
        x86_call_reg(inst, value->vreg->reg->reg);
    }
    else if(value->vreg && value->vreg->in_frame)
    {
        x86_call_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
    }
    else
    {
        x86_call_mem(inst, value->address);
    }
    return inst;
}

unsigned char *masm_push_dword(unsigned char *inst, jit_value_t value)
{
    if(value->vreg && value->vreg->in_reg)
    {
        x86_push_reg(inst, value->vreg->reg->reg);
    }
    else if(value->vreg && value->vreg->in_frame)
    {
        x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
    }
    else
    {
        x86_push_imm(inst, value->address);
    }
    return inst;
}

unsigned char *masm_push_float32(unsigned char *inst, jit_value_t value)
{
    if(value->vreg && value->vreg->in_reg)
    {
        sse_pushss_xmreg(inst, value->vreg->reg->reg);
    }
    else if(value->vreg && value->vreg->in_frame)
    {
        x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
    }
    else
    {
        x86_push_imm(inst, ((jit_uint*)(value->address))[0]);
    }
    return inst;
}

unsigned char *masm_push_float64(unsigned char *inst, jit_value_t value)
{
    if(value->vreg && value->vreg->in_reg)
    {
        sse2_pushsd_xmreg(inst, value->vreg->reg->reg);
    }
    else if(value->vreg && value->vreg->in_frame)
    {
        x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset + 4);
        x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
    }
    else
    {
        x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 8);
        x86_mov_membase_imm(inst, X86_ESP, 4, ((jit_uint*)(value->address))[1], 4);
        x86_mov_membase_imm(inst, X86_ESP, 0, ((jit_uint*)(value->address))[0], 4);
    }
    return inst;
}

unsigned char *masm_push_nfloat(unsigned char *inst, jit_value_t value)
{
    if(value->vreg && value->vreg->in_reg)
    {
        sse2_pushsd_xmreg(inst, value->vreg->reg->reg);
    }
    else if(value->vreg && value->vreg->in_frame)
    {
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset + 8);
            x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset + 4);
            x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
        }
        else
        {
            x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset + 4);
            x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
        }
    }
    else
    {
        if(sizeof(jit_nfloat) != sizeof(jit_float64))
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 12);
            x86_mov_membase_imm(inst, X86_ESP, 8, ((jit_uint*)(value->address))[2], 4);
            x86_mov_membase_imm(inst, X86_ESP, 4, ((jit_uint*)(value->address))[1], 4);
            x86_mov_membase_imm(inst, X86_ESP, 0, ((jit_uint*)(value->address))[0], 4);

        }
        else
        {
            x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 8);
            x86_mov_membase_imm(inst, X86_ESP, 0, ((jit_uint*)(value->address))[0], 4);
            x86_mov_membase_imm(inst, X86_ESP, 4, ((jit_uint*)(value->address))[1], 4);
        }
    }
    return inst;
}

unsigned char *masm_push_qword(unsigned char *inst, jit_value_t value)
{
    if(value->vreg && value->vreg->in_reg)
    {
        x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 8);
        x86_mov_membase_reg(inst, X86_ESP, 0, value->vreg->reg->reg, 4);
        x86_mov_membase_reg(inst, X86_ESP, 4, jite_gp_regs_map[gp_reg_map[value->vreg->reg->reg] + 1].reg, 4);
    }
    else if(value->vreg && value->vreg->in_frame)
    {
        x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset + 4);
        x86_push_membase(inst, X86_EBP, value->vreg->frame->frame_offset);
    }
    else
    {
        x86_alu_reg_imm(inst, X86_SUB, X86_ESP, 8);
        x86_mov_membase_imm(inst, X86_ESP, 0, ((jit_uint*)(value->address))[0], 4);
        x86_mov_membase_imm(inst, X86_ESP, 4, ((jit_uint*)(value->address))[1], 4);
    }
    return inst;
}

/* TODO: Build a more generic algorithm */
unsigned char *masm_imul_reg_reg_imm(unsigned char *inst, unsigned int dreg, unsigned int sreg, jit_uint imm)
{
    /* Handle special cases of immediate multiplies */
    switch(imm)
    {
        case 0:
        {
            x86_clear_reg(inst, dreg);
        }
        break;

        case 1:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
        }
        break;

        case -1:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_neg_reg(inst, dreg);
        }
        break;

        case 2:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 1);
        }
        break;

        case 3:
        {
            /* lea reg, [reg + reg * 2] */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 1);
        }
        break;

        case 4:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 2);
        }
        break;

        case 5:
        {    
            /* lea reg, [reg + reg * 4] */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 2);
        }
        break;

        case 6:
        {
            /* lea reg, [reg + reg * 2]; add reg, reg */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 1);
            x86_alu_reg_reg(inst, X86_ADD, dreg, dreg);
        }
        break;
        
        case 7:
        {
            if(dreg != sreg)
            {
                x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_shift_reg_imm(inst, X86_SHL, dreg, 3);
                x86_alu_reg_reg(inst, X86_SUB, dreg, sreg);
            }
            else
            {
                x86_imul_reg_reg_imm(inst, dreg, sreg, imm);
            }
        }
        break;

        case 8:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 3);
        }
        break;

        case 9:
        {
            /* lea reg, [reg + reg * 8] */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 3);
        }
        break;

        case 10:
        {
            /* lea reg, [reg + reg * 4]; add reg, reg */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 2);
            x86_alu_reg_reg(inst, X86_ADD, dreg, dreg);
        }
        break;

        case 12:
        {
            /* lea reg, [reg + reg * 2]; shl reg, 2 */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 1);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 2);
        }
        break;

        case 15:
        {
            /* lea reg1, [reg1 + reg1 * 2]; lea reg1, [reg1 + reg1 * 4] */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 1);
            x86_lea_memindex(inst, dreg, dreg, 0, dreg, 2);
        }
        break;

        case 16:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 4);
        }
        break;

        case 17:
        {
            if(dreg != sreg)
            {
                x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_shift_reg_imm(inst, X86_SHL, dreg, 4);
                x86_alu_reg_reg(inst, X86_ADD, dreg, sreg);
            }
            else
            {
                x86_imul_reg_reg_imm(inst, dreg, sreg, imm);
            }
        }
        break;

        case 18:
        {
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 3);
            x86_alu_reg_reg(inst, X86_ADD, dreg, dreg);
        }
        break;

        case 20:
        {
            /* lea reg1, [reg1 + reg1 * 4]; shl reg1, 2 */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 2);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 2);
        }
        break;

        case 24:
        {
            /* lea reg1, [reg1 + reg1 * 2]; shl reg1, 3 */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 1);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 3);
        }
        break;

        case 25:
        {
            /* lea reg, [reg + reg * 4]; lea reg, [reg + reg * 4] */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 2);
            x86_lea_memindex(inst, dreg, dreg, 0, dreg, 2);
        }
        break;

        case 27:
        {
            /* lea reg1, [reg1 + reg1 * 2]; lea reg1, [reg1 + reg1 * 8] */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 1);
            x86_lea_memindex(inst, dreg, dreg, 0, dreg, 3);
        }
        break;

        case 32:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 5);
        }
        break;

        case 64:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 6);
        }
        break;

        case 100:
        {
            /* lea reg, [reg + reg * 4]; shl reg, 2;
               lea reg, [reg + reg * 4] */
            x86_lea_memindex(inst, dreg, sreg, 0, sreg, 2);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 2);
            x86_lea_memindex(inst, dreg, dreg, 0, dreg, 2);
        }
        break;

        case 128:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 7);
        }
        break;

        case 256:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 8);
        }
        break;

        case 512:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 9);
        }
        break;

        case 1024:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 10);
        }
        break;

        case 2048:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 11);
        }
        break;

        case 4096:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 12);
        }
        break;

        case 8192:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 13);
        }
        break;

        case 16384:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 14);
        }
        break;

        case 32768:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 15);
        }
        break;

        case 65536:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 16);
        }
        break;

        case 0x00020000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 17);
        }
        break;

        case 0x00040000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 18);
        }
        break;

        case 0x00080000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 19);
        }
        break;

        case 0x00100000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 20);
        }
        break;

        case 0x00200000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 21);
        }
        break;

        case 0x00400000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 22);
        }
        break;

        case 0x00800000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 23);
        }
        break;

        case 0x01000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 24);
        }
        break;

        case 0x02000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 25);
        }
        break;

        case 0x04000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 26);
        }
        break;

        case 0x08000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 27);
        }
        break;

        case 0x10000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 28);
        }
        break;

        case 0x20000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 29);
        }
        break;

        case 0x40000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 30);
        }
        break;

        case (jit_nint)0x80000000:
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHL, dreg, 31);
        }
        break;

        default:
        {
            x86_imul_reg_reg_imm(inst, dreg, sreg, imm);
        }
        break;
    }
    return inst;
}

unsigned char *jite_throw_builtin
        (unsigned char *inst, jit_function_t func, int type);

unsigned char *__masm_idiv_reg_reg_imm(unsigned char *inst, jit_function_t func, local_regs_allocator_t lrs, unsigned int dreg, unsigned int sreg, jit_int imm)
{
    unsigned int reg = gpreg1;
    unsigned int state = save_gpreg1;

    if(imm != 1 && imm != -1 && imm != 0)
    {
        long long unsigned int m;
        long long int l;
        unsigned int sh_post;
        unsigned int d_abs = abs(imm);
        count_32bit_reciprocal_params_signed(d_abs, &sh_post, &m, &l);

        // SRA: arithmetic shift right
        // SRL: shift right (logical)
        // XSIGN: -1 if x < 0; 0 if x>=0; short for SRA(x, N - 1)
        if(d_abs == ((long long unsigned int)1<<l))
        {
            // q = SRA(n + SRL(SRA(n, l - 1), N - l), l)
            // SRA is arithmetic right shift
            // SRL is logical right shift
            find_one_gp_reg_cond1(inst, dreg);
            // We consider that gpreg1 != dreg.
            x86_mov_reg_reg(inst, gpreg1, sreg, 4);
            x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SAR, dreg, l - 1);
            x86_shift_reg_imm(inst, X86_SHR, dreg, 32 - l);
            x86_alu_reg_reg(inst, X86_ADD, dreg, gpreg1);
            x86_shift_reg_imm(inst, X86_SAR, dreg, l);
            release_one_gp_reg(inst);
        }
        else if(m < ((long long unsigned int)1<<(32 - 1)))
        {
            // q = SRA(MULSH(m, n), sh_post) - XSIGN(n)
            
            if(dreg != X86_EAX && dreg != X86_EDX)
            {                
                x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)m);
                x86_mul_reg(inst, dreg, 1);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_shift_reg_imm(inst, X86_SAR, dreg, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, dreg);
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)m);
                x86_mul_reg(inst, gpreg1, 1);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, gpreg1);
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
                if(dreg != gpreg1) release_one_gp_reg(inst);
            }
        }
        else
        {
//            else if(log2ui($3) == 32)
//            {
//                // TODO OR REMOVE AFTER CHECK THAT IT CAN BE REMOVED SAFELY
//            }
            // q = SRA(n + MULSH(m - 2^N, n), sh_post) - XSIGN(n)

            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, ((unsigned int)(m - ((long long unsigned int)1 << 32))));
                x86_mul_reg(inst, dreg, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, dreg);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_shift_reg_imm(inst, X86_SAR, dreg, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, dreg);
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, ((unsigned int)(m - ((long long unsigned int)1 << 32))));
                x86_mul_reg(inst, gpreg1, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, gpreg1);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_shift_reg_imm(inst, X86_SAR, gpreg1, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, gpreg1);
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
                if(dreg != gpreg1) release_one_gp_reg(inst);
            }
        }
        if(imm < 0)
        {
            x86_neg_reg(inst, dreg);
        }
    }
    else if(imm == 1)
    {
        x86_mov_reg_reg(inst, dreg, sreg, 4);
    }
    else if(imm == -1)
    {
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, sreg, jit_min_int);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_mov_reg_reg(inst, dreg, sreg, 4);
        x86_neg_reg(inst, dreg);
    }
    else
    {
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }
    
    save_gpreg1 = state;
    gpreg1 = reg;

    return inst;
}

unsigned char *__masm_idiv_un_reg_reg_imm(unsigned char *inst, jit_function_t func, local_regs_allocator_t lrs, unsigned int dreg, unsigned int sreg, jit_uint imm)
{
    unsigned int reg = gpreg1;
    unsigned int state = save_gpreg1;

    if(imm != 1 && imm != 0)
    {
        long long unsigned int m;
        unsigned int sh_pre, sh_post;
        long long unsigned int l;
        count_32bit_reciprocal_params(imm, &sh_pre, &sh_post, &m, &l);
        if(imm == ((long long unsigned int)1 << l))
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHR, dreg, l);
        }
        else if(imm == 0x80000000)
        {
            if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SHR, dreg, 31);
        }
        else if(m >= ((long long unsigned int)1 << 32))
        {
            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL is logic shift right
                x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m - ((long long unsigned int)1 << 32)));
                x86_mul_reg(inst, dreg, 0);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EDX);
                x86_shift_reg_imm(inst, X86_SHR, dreg, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, gpreg1);
                if(sh_post - 1) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, (sh_post - 1));
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL is logic shift right
                x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m - ((long long unsigned int)1 << 32)));
                x86_mul_reg(inst, gpreg1, 0);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EDX);
                x86_shift_reg_imm(inst, X86_SHR, gpreg1, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, gpreg1);
                if(sh_post - 1) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, (sh_post - 1));
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
                if(dreg != gpreg1) release_one_gp_reg(inst);
            }
        }
        else if(log2ui(imm) == 32)
        {
            find_one_gp_reg_cond1(inst, dreg); // FIND A REGISTER NOT EQUAL TO dreg
            x86_alu_reg_imm(inst, X86_CMP, sreg, imm);
            x86_mov_reg_imm(inst, dreg, 0);
            x86_mov_reg_imm(inst, gpreg1, 1);
            x86_cmov_reg(inst, X86_CC_GE, 0, dreg, gpreg1);
            release_one_gp_reg(inst);
        }
        else
        {
            if(dreg != X86_EAX)
            {
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL logical right shift
                x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m));
                if(sh_pre) x86_shift_reg_imm(inst, X86_SHR, dreg, sh_pre);
                x86_mul_reg(inst, dreg, 0);
                if(sh_post) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, sh_post);
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
            }
            else
            {
                find_one_gp_reg_cond1(inst, X86_EAX);
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL logical right shift
                x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m));
                if(sh_pre) x86_shift_reg_imm(inst, X86_SHR, gpreg1, sh_pre);
                x86_mul_reg(inst, gpreg1, 0);
                if(sh_post) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, sh_post);
                x86_mov_reg_reg(inst, dreg, X86_EDX, 4);
                if(dreg != gpreg1) release_one_gp_reg(inst);
            }
        }
    }
    else if(imm == 1)
    {
        x86_mov_reg_reg(inst, dreg, sreg, 4);
    }
    else
    {
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }
    
    save_gpreg1 = state;
    gpreg1 = reg;
    
    return inst;
}

unsigned char *__masm_irem_reg_reg_imm(unsigned char *inst, jit_function_t func, local_regs_allocator_t lrs, unsigned int dreg, unsigned int sreg, jit_int imm)
{
    unsigned int reg = gpreg1;
    unsigned int state = save_gpreg1;

    if(imm != 1 && imm != -1 && imm != 0)
    {
        long long unsigned int m;
        long long int l;
        unsigned int sh_post;
        unsigned int d_abs = abs(imm);
        count_32bit_reciprocal_params_signed(d_abs, &sh_post, &m, &l);

        // SRA: arithmetic shift right
        // SRL: shift right (logical)
        // XSIGN: -1 if x < 0; 0 if x>=0; short for SRA(x, N - 1)
        if(d_abs == ((long long unsigned int)1<<l))
        {
            // q = SRA(n + SRL(SRA(n, l - 1), N - l), l)
            // SRA is arithmetic right shift
            // SRL is logical right shift
            find_one_gp_reg_cond1(inst, dreg);
            // We consider that gpreg1 != dreg.
            x86_mov_reg_reg(inst, gpreg1, sreg, 4);
            x86_mov_reg_reg(inst, dreg, sreg, 4);
            x86_shift_reg_imm(inst, X86_SAR, dreg, l - 1);
            x86_shift_reg_imm(inst, X86_SHR, dreg, 32 - l);
            x86_alu_reg_reg(inst, X86_ADD, dreg, gpreg1);
            x86_shift_reg_imm(inst, X86_SAR, dreg, l);
            if(imm < 0) x86_neg_reg(inst, dreg);
            x86_imul_reg_reg_imm(inst, dreg, dreg, imm);
            x86_alu_reg_reg(inst, X86_SUB, gpreg1, dreg);
            x86_mov_reg_reg(inst, dreg, gpreg1, 4);
            release_one_gp_reg(inst);
        }
        else if(m < ((long long unsigned int)1<<(32 - 1)))
        {
            // q = SRA(MULSH(m, n), sh_post) - XSIGN(n)
            
            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)m);
                x86_mul_reg(inst, dreg, 1);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_mov_reg_reg(inst, X86_EAX, dreg, 4);
                x86_shift_reg_imm(inst, X86_SAR, X86_EAX, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, X86_EAX);
                if(imm < 0) x86_neg_reg(inst, X86_EDX);
                x86_imul_reg_reg_imm(inst, X86_EDX, X86_EDX, imm);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EDX);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != sreg) x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)m);
                x86_mul_reg(inst, gpreg1, 1);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_mov_reg_reg(inst, X86_EAX, gpreg1, 4);
                x86_shift_reg_imm(inst, X86_SAR, X86_EAX, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, X86_EAX);
                if(imm < 0) x86_neg_reg(inst, X86_EDX);
                x86_imul_reg_reg_imm(inst, X86_EDX, X86_EDX, imm);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EDX);
                x86_mov_reg_reg(inst, dreg, gpreg1, 4);
                if(dreg != gpreg1) release_one_gp_reg(inst);
            }
        }
        else
        {
//            else if(log2ui(imm) == 32)
//            {
//                // TODO OR REMOVE AFTER CHECK THAT IT CAN BE REMOVED SAFELY
//            }
            // q = SRA(n + MULSH(m - 2^N, n), sh_post) - XSIGN(n)
            
            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, ((unsigned int)(m - ((long long unsigned int)1 << 32))));
                x86_mul_reg(inst, dreg, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, dreg);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_mov_reg_reg(inst, X86_EAX, dreg, 4);
                x86_shift_reg_imm(inst, X86_SAR, X86_EAX, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, X86_EAX);
                if(imm < 0) x86_neg_reg(inst, X86_EDX);
                x86_imul_reg_reg_imm(inst, X86_EDX, X86_EDX, imm);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EDX);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != sreg) x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, ((unsigned int)(m - ((long long unsigned int)1 << 32))));
                x86_mul_reg(inst, gpreg1, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, gpreg1);
                x86_shift_reg_imm(inst, X86_SAR, X86_EDX, sh_post);
                x86_mov_reg_reg(inst, X86_EAX, gpreg1, 4);
                x86_shift_reg_imm(inst, X86_SAR, X86_EAX, 31);
                x86_alu_reg_reg(inst, X86_SUB, X86_EDX, X86_EAX);
                if(imm < 0) x86_neg_reg(inst, X86_EDX);
                x86_imul_reg_reg_imm(inst, X86_EDX, X86_EDX, imm);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EDX);
                x86_mov_reg_reg(inst, dreg, gpreg1, 4);
                if(dreg != gpreg1) release_one_gp_reg(inst);
            }

        }
    }
    else if(imm == 1)
    {
        x86_mov_reg_imm(inst, dreg, 0);
    }
    else if(imm == -1)
    {
        unsigned char *patch;
        x86_alu_reg_imm(inst, X86_CMP, sreg, jit_min_int);
        patch = inst;
        x86_branch8(inst, X86_CC_NE, 0, 0);
        inst = jite_throw_builtin(inst, func, JIT_RESULT_ARITHMETIC);
        x86_patch(patch, inst);
        x86_mov_reg_imm(inst, dreg, 0);
    }
    else
    {
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }

    save_gpreg1 = state;
    gpreg1 = reg;

    return inst;
}

unsigned char *__masm_irem_un_reg_reg_imm(unsigned char *inst, jit_function_t func, local_regs_allocator_t lrs, unsigned int dreg, unsigned int sreg, jit_uint imm)
{
    unsigned int reg = gpreg1;
    unsigned int state = save_gpreg1;

    if(imm != 0 && imm != 1)
    {
        long long unsigned int m;
        unsigned int sh_pre, sh_post;
        long long unsigned int l;
        count_32bit_reciprocal_params(imm, &sh_pre, &sh_post, &m, &l);
        if(imm == ((long long unsigned int)1 << l))
        {
            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_reg(inst, X86_EAX, sreg, 4);
                x86_shift_reg_imm(inst, X86_SHR, X86_EAX, l);
                x86_mov_reg_imm(inst, X86_EDX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EAX);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != sreg) x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_reg(inst, X86_EAX, sreg, 4);
                x86_shift_reg_imm(inst, X86_SHR, X86_EAX, l);
                x86_mov_reg_imm(inst, X86_EDX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EAX);
                if(dreg != gpreg1) x86_mov_reg_reg(inst, dreg, gpreg1, 4);
                release_one_gp_reg(inst);
            }
        }
        else if(imm == 0x80000000)
        {
            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_reg(inst, X86_EAX, sreg, 4);
                x86_shift_reg_imm(inst, X86_SHR, X86_EAX, 31);
                    x86_mov_reg_imm(inst, X86_EDX, imm);
                    x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EAX);
            }
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != sreg) x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_reg(inst, X86_EAX, sreg, 4);
                x86_shift_reg_imm(inst, X86_SHR, X86_EAX, 31);
                x86_mov_reg_imm(inst, X86_EDX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EAX);
                if(dreg != gpreg1) x86_mov_reg_reg(inst, dreg, gpreg1, 4);
                release_one_gp_reg(inst);
            }
        }
        else if(m >= ((long long unsigned int)1 << 32))
        {
            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL is logic shift right
                if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m - ((long long unsigned int)1 << 32)));
                x86_mul_reg(inst, dreg, 0);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EDX);
                x86_shift_reg_imm(inst, X86_SHR, dreg, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, dreg);
                if(sh_post - 1) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, (sh_post - 1));
                x86_mov_reg_imm(inst, X86_EAX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EAX);
            }                
            else
            {
                find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL is logic shift right
                if(gpreg1 != sreg) x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m - ((long long unsigned int)1 << 32)));
                x86_mul_reg(inst, gpreg1, 0);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EDX);
                x86_shift_reg_imm(inst, X86_SHR, gpreg1, 1);
                x86_alu_reg_reg(inst, X86_ADD, X86_EDX, gpreg1);
                if(sh_post - 1) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, (sh_post - 1));
                x86_mov_reg_imm(inst, X86_EAX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EAX);
                if(dreg != gpreg1) x86_mov_reg_reg(inst, dreg, gpreg1, 4);
                release_one_gp_reg(inst);
            }
        }
        else if(log2ui(imm) == 32)
        {
            if(dreg != X86_EAX && dreg != X86_EDX)
            {
                if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_alu_reg_imm(inst, X86_CMP, sreg, imm);
                x86_mov_reg_imm(inst, X86_EAX, 0);
                x86_mov_reg_imm(inst, X86_EDX, 1);
                x86_cmov_reg(inst, X86_CC_GE, 0, X86_EAX, X86_EDX);
                x86_mov_reg_imm(inst, X86_EDX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EDX);
            }
            else
            {
                    find_one_gp_reg_cond2(inst, X86_EAX, X86_EDX);
                if(gpreg1 != sreg) x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_alu_reg_imm(inst, X86_CMP, sreg, imm);
                x86_mov_reg_imm(inst, X86_EAX, 0);
                x86_mov_reg_imm(inst, X86_EDX, 1);
                x86_cmov_reg(inst, X86_CC_GE, 0, X86_EAX, X86_EDX);
                x86_mov_reg_imm(inst, X86_EDX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EDX);
                if(dreg != gpreg1) x86_mov_reg_reg(inst, dreg, gpreg1, 4);
                release_one_gp_reg(inst);
            }
        }
        else
        {
            if(dreg != X86_EAX)
            {
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL logical right shift
                if(dreg != sreg) x86_mov_reg_reg(inst, dreg, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m));
                if(sh_pre) x86_shift_reg_imm(inst, X86_SHR, gpreg1, sh_pre);
                x86_mul_reg(inst, gpreg1, 0);
                if(sh_post) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, sh_post);
                x86_mov_reg_imm(inst, X86_EAX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, dreg, X86_EAX);
            }
            else
            {
                find_one_gp_reg_cond1(inst, X86_EAX);
                // q = SRL(MULUH(m, SRL(n, sh_pre)), sh_post)
                // SRL logical right shift
                if(gpreg1 != sreg) x86_mov_reg_reg(inst, gpreg1, sreg, 4);
                x86_mov_reg_imm(inst, X86_EAX, (unsigned int)(m));
                if(sh_pre) x86_shift_reg_imm(inst, X86_SHR, gpreg1, sh_pre);
                x86_mul_reg(inst, gpreg1, 0);
                if(sh_post) x86_shift_reg_imm(inst, X86_SHR, X86_EDX, sh_post);
                x86_mov_reg_imm(inst, X86_EAX, imm);
                x86_mul_reg(inst, X86_EDX, 0);
                x86_alu_reg_reg(inst, X86_SUB, gpreg1, X86_EAX);
                if(gpreg1 != dreg) x86_mov_reg_reg(inst, dreg, gpreg1, 4);
                release_one_gp_reg(inst);
            }
        }
    }
    else if(imm == 1)
    {
        x86_mov_reg_imm(inst, dreg, 0);
    }
    else
    {
        inst = jite_throw_builtin(inst, func, JIT_RESULT_DIVISION_BY_ZERO);
    }
    
    save_gpreg1 = state;
    gpreg1 = reg;

    return inst;
}

#define masm_idiv_reg_reg_imm(inst, dreg, sreg, imm) __masm_idiv_reg_reg_imm(inst, func, lrs, dreg, sreg, imm);

#define masm_idiv_un_reg_reg_imm(inst, dreg, sreg, imm) __masm_idiv_un_reg_reg_imm(inst, func, lrs, dreg, sreg, imm);

#define masm_irem_reg_reg_imm(inst, dreg, sreg, imm) __masm_irem_reg_reg_imm(inst, func, lrs, dreg, sreg, imm);

#define masm_irem_un_reg_reg_imm(inst, dreg, sreg, imm) __masm_irem_un_reg_reg_imm(inst, func, lrs, dreg, sreg, imm);

#define ROUND_STACK(size)    \
        (((size) + (sizeof(void *) - 1)) & ~(sizeof(void *) - 1))

#endif /* JIT_GEN_I486_MASM_H */
