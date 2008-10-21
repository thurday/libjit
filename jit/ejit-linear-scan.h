#if defined(__i386) || defined(__i386__) || defined(_M_IX86)
#ifndef    _JIT_LINEAR_SCAN_H
#define    _JIT_LINEAR_SCAN_H
#include <stdio.h>
#include <stdlib.h>
#include "jit-memory.h"
#include "jit-rules.h"

#if !defined(JIT_BACKEND_INTERP) && (defined(__i386) && defined(__i386__) || defined(_M_IX86))
#define EJIT_ENABLED
#endif

// #define EJIT_DEBUG_ENABLED 1

typedef struct _ejit_vreg *ejit_vreg_t;
typedef struct _ejit_instance *ejit_instance_t;
typedef struct _ejit_critical_point *ejit_critical_point_t;
typedef struct _ejit_list *ejit_list_t;
typedef struct _ejit_reg *ejit_reg_t;
typedef struct _ejit_frame *ejit_frame_t;
typedef struct _ejit_linked_list *ejit_linked_list_t;
typedef struct _ejit_call_params *ejit_call_params_t;

struct _ejit_call_params
{
    jit_abi_t abi;
    unsigned int num;
    jit_value_t args[1];
};

struct _ejit_opcode
{
    unsigned int hash;
    unsigned char machine_code1;
    unsigned char machine_code2;
    unsigned char machine_code3;
    unsigned char machine_code4;
};

struct _ejit_frame
{
    int frame_offset;
    unsigned int hash_code;
    int length;
};

struct _ejit_reg
{
    unsigned char reg;
    unsigned char index;
    int min_life_range;
    int max_life_range;
    int min_weight_range;
    int max_weight_range;
    unsigned int hash_code;
    ejit_vreg_t vreg;
    ejit_vreg_t local_vreg;
    ejit_frame_t temp_frame;
};

struct _ejit_critical_point
{
    char is_branch_target : 1;
    ejit_list_t vregs_die;   // Vregs which die at this critical point.
    ejit_list_t vregs_born;  // Vregs which are born at this critical point.
                // it has the info about the numeric order of the opcode for ejit.
    unsigned int regs_state_1;
    unsigned int regs_state_2;
};

struct _ejit_list
{
    void *item1;
    void *item2;
    void *item3;
    void *item4;
    ejit_list_t next;
    ejit_list_t prev;
};

struct _ejit_instance
{
    jit_function_t func;
    jit_block_t block;
    jit_insn_iter_t iter;
    jit_insn_t insn;
    ejit_critical_point_t cpoints_list;
    ejit_linked_list_t vregs_list;
    short vregs_num;
    ejit_list_t branch_list;
    unsigned int regs_state;
    ejit_list_t frame_state;
    void *incoming_params_state;
    unsigned int scratch_regs;
    unsigned int scratch_frame;
    ejit_linked_list_t reg_holes[32];
    int relative_sp_offset;
};

struct _ejit_vreg
{
    jit_value_t value;
    jit_insn_t min_range;
    jit_insn_t max_range;
    ejit_reg_t reg;
    ejit_frame_t frame;
    unsigned int in_reg : 1;
    unsigned int in_frame : 1;
    short index;
    short weight;
};

struct _ejit_linked_list
{
    ejit_linked_list_t next;
    ejit_linked_list_t prev;
    void *item;
};

#ifndef JIT_NATIVE64_INT64
#define CASE_USE_WORD \
        case JIT_TYPE_SBYTE: \
        case JIT_TYPE_UBYTE: \
        case JIT_TYPE_SHORT: \
        case JIT_TYPE_USHORT: \
        case JIT_TYPE_INT: \
        case JIT_TYPE_UINT: \
        case JIT_TYPE_NINT: \
        case JIT_TYPE_NUINT: \
        case JIT_TYPE_PTR:

#define CASE_USE_FLOAT \
        case JIT_TYPE_FLOAT32: \
        case JIT_TYPE_FLOAT64: \
        case JIT_TYPE_NFLOAT:

#define CASE_USE_LONG \
        case JIT_TYPE_LONG: \
        case JIT_TYPE_ULONG:
#else
#define CASE_USE_WORD \
        case JIT_TYPE_SBYTE: \
        case JIT_TYPE_UBYTE: \
        case JIT_TYPE_SHORT: \
        case JIT_TYPE_USHORT: \
        case JIT_TYPE_INT: \
        case JIT_TYPE_UINT: \
        case JIT_TYPE_NINT: \
        case JIT_TYPE_NUINT: \
        case JIT_TYPE_PTR:   \
        case JIT_TYPE_LONG: \
        case JIT_TYPE_ULONG:

#define CASE_USE_FLOAT \
        case JIT_TYPE_FLOAT32: \
        case JIT_TYPE_FLOAT64: \
        case JIT_TYPE_NFLOAT:

#define CASE_USE_LONG
#endif



int ejit_count_items(jit_function_t func, ejit_linked_list_t list);

int ejit_type_num_params(jit_type_t signature);

jit_type_t ejit_type_get_param(jit_type_t signature, int index);

jit_value_t ejit_value_get_param(jit_function_t func, int index);

char ejit_is_virtual_reference(jit_value_t value);

jit_value_t ejit_find_work_value(jit_insn_t insn, jit_value_t value);

void ejit_init(jit_function_t func);

void ejit_reinit(jit_function_t func);


// Create a new virtual register value, the vreg is born.
ejit_vreg_t ejit_create_vreg(jit_function_t func, jit_value_t value);

// Returns 1 if the value is already in a vreg.
char ejit_value_is_in_vreg(jit_function_t func, jit_value_t value);

// Count a vreg weight
short ejit_vreg_weight(jit_function_t func, ejit_vreg_t vreg);

void ejit_add_branch_target(jit_function_t func, jit_insn_t insn, jit_label_t label);

// Compute values liveness period
void ejit_compute_liveness(jit_function_t func);

// Create a new ejit instance.
ejit_instance_t ejit_create_instance(jit_function_t func);

// Destroy a ejit instance.
void ejit_destroy_instance(jit_function_t func);

// Create a new critical point.
// Critical points are points where values are born and die.
void ejit_create_critical_point(jit_function_t func, jit_insn_t insn);

// Given two critical points that are united by a branch, we need to update the vregs liveness.
// We assume that the order of the jump is not important.
// It is as we have linked data about two time frames.
unsigned char ejit_update_liveness_for_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to);

void ejit_add_fixup_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to);

#ifdef EJIT_DEBUG_ENABLED
void ejit_debug_print_vregs_ranges(jit_function_t func);
void ejit_dump_registers(unsigned int buf, unsigned int ecx, unsigned int edx, unsigned int eax, unsigned int ebx, unsigned int edi, unsigned int esi, unsigned int ebp);
unsigned char* ejit_generate_dump(unsigned char *inst, jit_gencode_t gen);
#endif

// void ejit_allocate_registers_and_locals(jit_function_t func);

void ejit_add_vreg_to_complex_list(jit_function_t func, ejit_vreg_t vreg, ejit_list_t list);

void ejit_remove_vreg_from_complex_list(jit_function_t func, ejit_vreg_t vreg, ejit_list_t list);

unsigned char ejit_add_item_to_linked_list(jit_function_t func, void *item, ejit_linked_list_t linked_list);

unsigned char ejit_remove_item_from_linked_list(jit_function_t func, void *item, ejit_linked_list_t linked_list);

ejit_vreg_t ejit_vregs_higher_criteria(jit_function_t func, ejit_vreg_t vreg1, ejit_vreg_t vreg2);

// Make a CPU or a memory frame which is used by a certain virtual register to be available.

void ejit_free_registers(jit_function_t func, ejit_list_t list);

void ejit_free_reg(jit_function_t func, ejit_vreg_t vreg);

void ejit_free_frames(jit_function_t func, ejit_list_t list);

void ejit_free_vreg_frame(jit_function_t func, ejit_vreg_t vreg);

void ejit_free_frame(jit_function_t func, ejit_frame_t frame);

int ejit_count_vregs(jit_function_t func, ejit_list_t list);

int ejit_compile(jit_function_t func, void **entry_point);

void ejit_preallocate_global_registers(jit_function_t func);

void ejit_preallocate_registers_and_frames(jit_function_t func, ejit_list_t list);

unsigned char* ejit_allocate_registers_and_frames(unsigned char *inst, jit_function_t func, ejit_linked_list_t list);

unsigned char* ejit_allocate_local_registers_for_input(unsigned char *inst, jit_function_t func, ejit_vreg_t vreg, ejit_vreg_t vreg1, ejit_vreg_t vreg2);

unsigned char* ejit_restore_local_registers(unsigned char *inst, jit_function_t func, unsigned int regMask);

unsigned char* ejit_restore_local_frame(unsigned char *inst, jit_function_t func, ejit_vreg_t vreg);

unsigned char* ejit_restore_local_vreg(unsigned char *inst, jit_function_t func, ejit_vreg_t vreg);

void gen_start_block(jit_gencode_t gen, jit_block_t block);

void *gen_prolog(jit_gencode_t gen, jit_function_t func, void *buf);

void gen_epilog(jit_gencode_t gen, jit_function_t func);

void gen_start_block(jit_gencode_t gen, jit_block_t block);

void gen_end_block(jit_gencode_t gen, jit_block_t block);

void ejit_compile_block(jit_gencode_t gen, jit_function_t func, jit_block_t block);

void ejit_compute_register_holes(jit_function_t func);

unsigned char ejit_vreg_is_in_register_hole(jit_function_t func, ejit_vreg_t vreg, unsigned int regIndex);

unsigned char *ejit_call_internal(jit_gencode_t gen, unsigned char *buf, unsigned int num, jit_value_t *args, void *func_address, jit_value_t return_value, int call_type);

unsigned int ejit_value_in_reg(jit_value_t value);

unsigned int ejit_value_in_frame(jit_value_t value);

int ejit_regIndex2reg(jit_function_t func, int regIndex, jit_type_t type);

#define NORMAL_CALL              1
#define INDIRECT_CALL            2
#define TAIL_CALL                3
#define INDIRECT_TAIL_CALL       4

#endif
#else

int ejit_regIndex2reg(jit_function_t func, int regIndex, jit_type_t type) { return 1; }

#endif
