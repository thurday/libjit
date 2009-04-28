#ifndef    _JIT_LINEAR_SCAN_H
#define    _JIT_LINEAR_SCAN_H

#include <stdio.h>
#include <stdlib.h>
#include "jit-memory.h"
#include "jit-rules.h"
#include <config.h>
#include "jit-rules.h"

// #if defined(JITE_ENABLED)

#define JIT_MIN_USED 0

// #define JITE_DEBUG_ENABLED 1

// #define JITE_DUMP_LIVENESS_RANGES 1

#define LINEAR_SCAN_EURISTIC          0
#define NUMBER_OF_USE_EURISTIC        1
#define BYHAND_EURISTIC               2


typedef struct _jite_vreg *jite_vreg_t;
typedef struct _jite_instance *jite_instance_t;
typedef struct _jite_critical_point *jite_critical_point_t;
typedef struct _jite_list *jite_list_t;
typedef struct _jite_reg *jite_reg_t;
typedef struct _jite_frame *jite_frame_t;
typedef struct _jite_linked_list *jite_linked_list_t;

typedef struct _jite_call_params *jite_call_params_t;

struct _jite_call_params
{
    jit_abi_t abi;
    unsigned int num;
    jit_value_t args[1];
};

struct _jite_opcode
{
    unsigned int hash;
    unsigned char machine_code1;
    unsigned char machine_code2;
    unsigned char machine_code3;
    unsigned char machine_code4;
    unsigned char has_side_effect;

    unsigned char is_nop;
    unsigned char has_multiple_paths_flow;

    unsigned char dest_defined;;
    unsigned char dest_used;
    unsigned char value1_defined;
    unsigned char value1_used;
    unsigned char value2_used;
};

struct _jite_frame
{
    int frame_offset;
    unsigned int hash_code;
    int length;
};

struct _jite_reg
{
    unsigned char reg;
    unsigned char index;
    unsigned int hash_code;
    jite_vreg_t vreg;
    jite_vreg_t local_vreg;
    jite_frame_t temp_frame;
    jite_linked_list_t liveness;
};

struct _jite_critical_point
{
    char is_branch_target : 1;
    jite_list_t vregs_die;   /* Vregs which die at this critical point. */
    jite_list_t vregs_born;  /* Vregs which are born at this critical point. */
    unsigned int regs_state_1;
    unsigned int regs_state_2;
};

struct _jite_list
{
    void *item1;
    void *item2;
    void *item3;
    void *item4;
    jite_list_t next;
    jite_list_t prev;
};

struct _jite_instance
{
    jit_function_t func;
    jit_block_t block;
    jit_insn_iter_t iter;
    jit_insn_t insn;
    jite_critical_point_t cpoints_list;
    jite_linked_list_t vregs_list;
    unsigned int vregs_num;
    jite_list_t branch_list;
    unsigned int regs_state;
    jite_list_t frame_state;
    void *incoming_params_state;
    unsigned int scratch_regs;
    unsigned int scratch_frame;
    jite_linked_list_t reg_holes[32];
    int relative_sp_offset;
    jite_vreg_t *vregs_table;
    unsigned char register_allocator_euristic;
};

struct _jite_vreg
{
    jit_value_t value;
    jit_insn_t min_range;
    jit_insn_t max_range;
    jite_reg_t reg;
    jite_frame_t frame;
    unsigned int in_reg : 1;
    unsigned int in_frame : 1;
    short index;
    short weight;
    jite_linked_list_t liveness;
};

struct _jite_linked_list
{
    jite_linked_list_t next;
    jite_linked_list_t prev;
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


void jite_function_set_register_allocator_euristic(jit_function_t func, int type);

unsigned char jite_function_get_register_allocator_euristic(jit_function_t func);

void jite_debug_print_vregs_ranges(jit_function_t func);

int jite_count_items(jit_function_t func, jite_linked_list_t list);

int jite_type_num_params(jit_type_t signature);

jit_type_t jite_type_get_param(jit_type_t signature, int index);

jit_value_t jite_value_get_param(jit_function_t func, int index);

void jite_init(jit_function_t func);

void jite_reinit(jit_function_t func);


/* Create a new virtual register value, the vreg is born. */
jite_vreg_t jite_create_vreg(jit_value_t value);

/* Returns 1 if the value is already in a vreg. */
char jite_value_is_in_vreg(jit_value_t value);

/* Count a vreg weight */
void jite_value_set_weight(jit_value_t value, unsigned int weight);

void jite_value_set_weight_using_insn(jit_value_t value, jit_insn_t insn);

unsigned int jite_value_get_weight(jit_value_t value);

unsigned int jite_vreg_weight(jite_vreg_t vreg);

unsigned int jite_vreg_get_weight(jite_vreg_t vreg);

unsigned int jite_get_max_weight();

void jite_add_branch_target(jit_function_t func, jit_insn_t insn, jit_label_t label);

/* Compute values liveness period */

void jite_compute_liveness(jit_function_t func);

void jite_compute_values_weight_using_insn(jit_function_t func, jit_insn_t insn);

void jite_create_vregs_table(jit_function_t func);

void jite_compute_local_liveness(jit_function_t func);

void jite_compute_fast_liveness(jit_function_t func);

void jite_compute_full_liveness(jit_function_t func);

void jite_compute_values_weight(jit_function_t func);

/* Create a new jite instance. */
jite_instance_t jite_create_instance(jit_function_t func);

/* Destroy a jite instance. */
void jite_destroy_instance(jit_function_t func);

/* Create a new critical point. */
/* Critical points are points where values are born and die. */
void jite_create_critical_point(jit_function_t func, jit_insn_t insn);

/* Given two critical points that are united by a branch, we need to update the vregs liveness. */
/* We assume that the order of the jump is not important. */
/* It is as we have linked data about two time frames. */
unsigned char jite_update_liveness_for_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to);

void jite_add_fixup_branch(jit_function_t func, jit_insn_t insn_from, jit_insn_t insn_to);

#ifdef JITE_DEBUG_ENABLED
void jite_debug_print_vregs_ranges(jit_function_t func);
void jite_dump_registers(unsigned int buf, unsigned int ecx, unsigned int edx, unsigned int eax, unsigned int ebx, unsigned int edi, unsigned int esi, unsigned int ebp);
unsigned char* jite_generate_dump(unsigned char *inst, jit_gencode_t gen);
#endif

/* void jite_allocate_registers_and_locals(jit_function_t func); */

void jite_add_vreg_to_complex_list(jit_function_t func, jite_vreg_t vreg, jite_list_t list);

void jite_remove_vreg_from_complex_list(jit_function_t func, jite_vreg_t vreg, jite_list_t list);

void jite_clear_linked_list(jit_function_t func, jite_linked_list_t linked_list);

jite_linked_list_t jite_add_item_no_duplicate_to_linked_list(jit_function_t func, void *item, jite_linked_list_t linked_list);

jite_linked_list_t jite_insert_item_after_linked_list(jit_function_t func, void *item, jite_linked_list_t linked_list);

jite_linked_list_t jite_add_item_to_linked_list(jit_function_t func, void *item, jite_linked_list_t linked_list);

jite_linked_list_t jite_remove_item_from_linked_list(jit_function_t func, void *item, jite_linked_list_t linked_list);

jite_vreg_t jite_vregs_higher_criteria(jit_function_t func, jite_vreg_t vreg1, jite_vreg_t vreg2);

/* Make a CPU or a memory frame which is used by a certain virtual register to be available. */

void jite_free_registers(jit_function_t func, jite_list_t list);

void jite_free_reg(jit_function_t func, jite_vreg_t vreg);

void jite_free_frames(jit_function_t func, jit_insn_t insn);

void jite_free_vreg_frame(jit_function_t func, jite_vreg_t vreg);

void jite_free_frame(jit_function_t func, jite_frame_t frame);

int jite_count_vregs(jit_function_t func, jite_list_t list);

int jite_compile(jit_function_t func, void **entry_point);

void jite_preallocate_global_registers(jit_function_t func);

void jite_preallocate_registers_and_frames(jit_function_t func, jite_list_t list);

void jite_allocate_registers(jit_function_t func, jite_linked_list_t list);

void jite_allocate_registers_and_frames(jit_function_t func, jite_linked_list_t list);

unsigned char* jite_allocate_local_registers_for_input(unsigned char *inst, jit_function_t func, jite_vreg_t vreg, jite_vreg_t vreg1, jite_vreg_t vreg2);

unsigned char* jite_restore_temporary_frame(unsigned char *inst, jit_function_t func, unsigned int regMask);

unsigned char* jite_restore_local_registers(unsigned char *inst, jit_function_t func, unsigned int regMask);

unsigned char* jite_restore_local_frame(unsigned char *inst, jit_function_t func, jite_vreg_t vreg);

unsigned char* jite_restore_local_vreg(unsigned char *inst, jit_function_t func, jite_vreg_t vreg);

void gen_start_block(jit_gencode_t gen, jit_block_t block);

void *gen_prolog(jit_gencode_t gen, jit_function_t func, void *buf);

void gen_epilog(jit_gencode_t gen, jit_function_t func);

void gen_start_block(jit_gencode_t gen, jit_block_t block);

void gen_end_block(jit_gencode_t gen, jit_block_t block);

void jite_compile_block(jit_gencode_t gen, jit_function_t func, jit_block_t block);

void jite_compute_register_holes(jit_function_t func);

unsigned char jite_vreg_is_in_register_hole(jit_function_t func, jite_vreg_t vreg, unsigned int regIndex);

unsigned char *jite_call_internal(jit_gencode_t gen, unsigned char *buf, unsigned int num, jit_value_t *args, void *func_address, jit_value_t return_value, int call_type);

unsigned int jite_value_in_reg(jit_value_t value);

unsigned int jite_value_in_frame(jit_value_t value);


int jite_regIndex2reg(jit_function_t func, int regIndex, jit_type_t type);

unsigned char *jite_emit_trampoline_for_internal_abi(jit_gencode_t gen, unsigned char *buf,
                jit_type_t signature, void *func_address, int call_type);

unsigned char *jite_emit_function_call(jit_gencode_t gen, unsigned char *buf, jit_function_t func,
                                       void *func_address, jit_value_t indirect_ptr, int call_type);

unsigned char jite_insn_has_side_effect(jit_insn_t insn);
unsigned char jite_insn_has_multiple_paths_flow(jit_insn_t insn);
unsigned char jite_insn_dest_defined(jit_insn_t insn);
unsigned char jite_insn_dest_used(jit_insn_t insn);
unsigned char jite_insn_value1_defined(jit_insn_t insn);
unsigned char jite_insn_value1_used(jit_insn_t insn);
unsigned char jite_insn_value2_used(jit_insn_t insn);

#define NORMAL_CALL              1
#define INDIRECT_CALL            2
#define TAIL_CALL                3
#define INDIRECT_TAIL_CALL       4

#endif
// #else

// int jite_regIndex2reg(jit_function_t func, int regIndex, jit_type_t type) { return 1; }

// #endif
