/*
 * jit-function.h - Functions for manipulating functions blocks.
 *
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

#ifndef	_JIT_FUNCTION_H
#define	_JIT_FUNCTION_H

#include <jit/jit-common.h>

#ifdef	__cplusplus
extern	"C" {
#endif

jit_function_t jit_function_create
	(jit_context_t context, jit_type_t signature) JIT_NOTHROW;
jit_function_t jit_function_create_nested
	(jit_context_t context, jit_type_t signature,
	 jit_function_t parent) JIT_NOTHROW;
void jit_function_abandon(jit_function_t func) JIT_NOTHROW;
jit_context_t jit_function_get_context(jit_function_t func) JIT_NOTHROW;
jit_type_t jit_function_get_signature(jit_function_t func) JIT_NOTHROW;
int jit_function_set_meta
	(jit_function_t func, int type, void *data,
	 jit_meta_free_func free_data, int build_only) JIT_NOTHROW;
void *jit_function_get_meta(jit_function_t func, int type) JIT_NOTHROW;
void jit_function_free_meta(jit_function_t func, int type) JIT_NOTHROW;
jit_function_t jit_function_next
	(jit_context_t context, jit_function_t prev) JIT_NOTHROW;
jit_function_t jit_function_previous
	(jit_context_t context, jit_function_t prev) JIT_NOTHROW;
jit_block_t jit_function_get_entry(jit_function_t func) JIT_NOTHROW;
jit_block_t jit_function_get_current(jit_function_t func) JIT_NOTHROW;
jit_function_t jit_function_get_nested_parent(jit_function_t func) JIT_NOTHROW;
int jit_function_compile(jit_function_t func) JIT_NOTHROW;
int jit_function_recompile(jit_function_t func) JIT_NOTHROW;
int jit_function_is_compiled(jit_function_t func) JIT_NOTHROW;
void jit_function_set_recompilable(jit_function_t func) JIT_NOTHROW;
void jit_function_clear_recompilable(jit_function_t func) JIT_NOTHROW;
int jit_function_is_recompilable(jit_function_t func) JIT_NOTHROW;
int jit_function_compile_entry(jit_function_t func, void **entry_point) JIT_NOTHROW;
void jit_function_setup_entry(jit_function_t func, void *entry_point) JIT_NOTHROW;
void *jit_function_to_closure(jit_function_t func) JIT_NOTHROW;
jit_function_t jit_function_from_closure
	(jit_context_t context, void *closure) JIT_NOTHROW;
jit_function_t jit_function_from_pc
	(jit_context_t context, void *pc, void **handler) JIT_NOTHROW;
void *jit_function_to_vtable_pointer(jit_function_t func) JIT_NOTHROW;
jit_function_t jit_function_from_vtable_pointer
	(jit_context_t context, void *vtable_pointer) JIT_NOTHROW;
void jit_function_set_on_demand_compiler
	(jit_function_t func, jit_on_demand_func on_demand) JIT_NOTHROW;
jit_on_demand_func jit_function_get_on_demand_compiler(jit_function_t func) JIT_NOTHROW;
int jit_function_apply
	(jit_function_t func, void **args, void *return_area);
int jit_function_apply_vararg
	(jit_function_t func, jit_type_t signature, void **args, void *return_area);
void jit_function_set_optimization_level
	(jit_function_t func, unsigned int level) JIT_NOTHROW;
unsigned int jit_function_get_optimization_level
	(jit_function_t func) JIT_NOTHROW;
unsigned int jit_function_get_max_optimization_level(void) JIT_NOTHROW;
jit_function_t jit_function_create_cdecl_trampoline
	(jit_context_t context, jit_type_t signature) JIT_NOTHROW;
jit_abi_t jit_function_get_abi(jit_function_t func) JIT_NOTHROW;
int jit_function_extended_compiler_is_enabled(jit_function_t func) JIT_NOTHROW;
#ifdef	__cplusplus
};
#endif

#endif /* _JIT_FUNCTION_H */
