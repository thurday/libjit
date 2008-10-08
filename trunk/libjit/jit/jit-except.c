/*
 * jit-except.c - Exception handling functions.
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

#include "jit-internal.h"
#include "jit-rules.h"
#include <config.h>
#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif
#if defined(JIT_BACKEND_INTERP)
	#include "jit-interp.h"
#endif
#include <stdio.h>
#include "jit-setjmp.h"

/*@

@cindex jit-except.h

@*/

/*@
 * @deftypefun {void *} jit_exception_get_last (void)
 * Get the last exception object that occurred on this thread, or NULL
 * if there is no exception object on this thread.  As far as @code{libjit}
 * is concerned, an exception is just a pointer.  The precise meaning of the
 * data at the pointer is determined by the front end.
 * @end deftypefun
@*/
void *jit_exception_get_last(void)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		return control->last_exception;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun {void *} jit_exception_get_last_and_clear (void)
 * Get the last exception object that occurred on this thread and also
 * clear the exception state to NULL.  This combines the effect of
 * both @code{jit_exception_get_last} and @code{jit_exception_clear_last}.
 * @end deftypefun
@*/
void *jit_exception_get_last_and_clear(void)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		void *obj = control->last_exception;
		control->last_exception = 0;
		return obj;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun void jit_exception_set_last (void *object)
 * Set the last exception object that occurred on this thread, so that
 * it can be retrieved by a later call to @code{jit_exception_get_last}.
 * This is normally used by @code{jit_function_apply} to save the
 * exception object before returning to regular code.
 * @end deftypefun
@*/
void jit_exception_set_last(void *object)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		control->last_exception = object;
	}
}

/*@
 * @deftypefun void jit_exception_clear_last (void)
 * Clear the last exception object that occurred on this thread.
 * This is equivalent to calling @code{jit_exception_set_last}
 * with a parameter of NULL.
 * @end deftypefun
@*/
void jit_exception_clear_last(void)
{
	jit_exception_set_last(0);
}

/*@
 * @deftypefun void jit_exception_throw ({void *} object)
 * Throw an exception object within the current thread.  As far as
 * @code{libjit} is concerned, the exception object is just a pointer.
 * The precise meaning of the data at the pointer is determined
 * by the front end.
 *
 * Note: as an exception object works its way back up the stack,
 * it may be temporarily stored in memory that is not normally visible
 * to a garbage collector.  The front-end is responsible for taking steps
 * to "pin" the object so that it is uncollectable until explicitly
 * copied back into a location that is visible to the collector once more.
 * @end deftypefun
@*/
void jit_exception_throw(void *object)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		control->last_exception = object;
		if(control->setjmp_head)
		{
			control->backtrace_head = control->setjmp_head->trace;
			siglongjmp(control->setjmp_head->buf, 1);
		}
	}
}

/*@
 * @deftypefun void jit_exception_builtin (int exception_type)
 * This function is called to report a builtin exception.
 * The JIT will automatically embed calls to this function wherever a
 * builtin exception needs to be reported.
 *
 * When a builtin exception occurs, the current thread's exception
 * handler is called to construct an appropriate object, which is
 * then thrown.
 *
 * If there is no exception handler set, or the handler returns NULL,
 * then @code{libjit} will print an error message to stderr and cause
 * the program to exit with a status of 1.  You normally don't want
 * this behavior and you should override it if possible.
 *
 * The following builtin exception types are currently supported:
 *
 * @table @code
 * @vindex JIT_RESULT_OK
 * @item JIT_RESULT_OK
 * The operation was performed successfully (value is 1).
 *
 * @vindex JIT_RESULT_OVERFLOW
 * @item JIT_RESULT_OVERFLOW
 * The operation resulted in an overflow exception (value is 0).
 *
 * @vindex JIT_RESULT_ARITHMETIC
 * @item JIT_RESULT_ARITHMETIC
 * The operation resulted in an arithmetic exception.  i.e. an attempt was
 * made to divide the minimum integer value by -1 (value is -1).
 *
 * @vindex JIT_RESULT_DIVISION_BY_ZERO
 * @item JIT_RESULT_DIVISION_BY_ZERO
 * The operation resulted in a division by zero exception (value is -2).
 *
 * @vindex JIT_RESULT_COMPILE_ERROR
 * @item JIT_RESULT_COMPILE_ERROR
 * An error occurred when attempting to dynamically compile a function
 * (value is -3).
 *
 * @vindex JIT_RESULT_OUT_OF_MEMORY
 * @item JIT_RESULT_OUT_OF_MEMORY
 * The system ran out of memory while performing an operation (value is -4).
 *
 * @vindex JIT_RESULT_NULL_REFERENCE
 * @item JIT_RESULT_NULL_REFERENCE
 * An attempt was made to dereference a NULL pointer (value is -5).
 *
 * @vindex JIT_RESULT_NULL_FUNCTION
 * @item JIT_RESULT_NULL_FUNCTION
 * An attempt was made to call a function with a NULL function pointer
 * (value is -6).
 *
 * @vindex JIT_RESULT_CALLED_NESTED
 * @item JIT_RESULT_CALLED_NESTED
 * An attempt was made to call a nested function from a non-nested context
 * (value is -7).
 * @end table
 * @end deftypefun
@*/
void jit_exception_builtin(int exception_type)
{
	jit_exception_func handler;
	void *object;
	static const char * const messages[10] = {
		"Success",
		"Overflow during checked arithmetic operation",
		"Arithmetic exception (dividing the minimum integer by -1)",
		"Division by zero",
		"Error during function compilation",
		"Out of memory",
		"Null pointer dereferenced",
		"Null function pointer called",
		"Nested function called from non-nested context",
		"Array index out of bounds"
	};
	#define	num_messages	(sizeof(messages) / sizeof(const char *))

	/* Get the exception handler for this thread */
	handler = jit_exception_get_handler();

	/* Invoke the exception handler to create an appropriate object */
	if(handler)
	{
		object = (*handler)(exception_type);
		if(object)
		{
			jit_exception_throw(object);
		}
	}

	/* We don't have an exception handler, so print a message and exit */
	fputs("A builtin JIT exception could not be handled:\n", stderr);
	exception_type = -(exception_type - 1);
	if(exception_type >= 0 && exception_type < (int)num_messages)
	{
		fputs(messages[exception_type], stderr);
	}
	else
	{
		fprintf(stderr, "Unknown builtin exception %d",
				(-exception_type) + 1);
	}
	putc('\n', stderr);
	exit(1);
}

/*@
 * @deftypefun jit_exception_func jit_exception_set_handler (jit_exception_func handler)
 * Set the builtin exception handler for the current thread.
 * Returns the previous exception handler.
 * @end deftypefun
@*/
jit_exception_func jit_exception_set_handler
	(jit_exception_func handler)
{
	jit_exception_func previous;
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		previous = control->exception_handler;
		control->exception_handler = handler;
		return previous;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_exception_func jit_exception_get_handler (void)
 * Get the builtin exception handler for the current thread.
 * @end deftypefun
@*/
jit_exception_func jit_exception_get_handler(void)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		return control->exception_handler;
	}
	else
	{
		return 0;
	}
}

/*
 * Structure of a stack trace.
 */
struct jit_stack_trace
{
	unsigned int		size;
	void			   *items[1];
};

/*@
 * @deftypefun jit_stack_trace_t jit_exception_get_stack_trace (void)
 * Create an object that represents the current call stack.
 * This is normally used to indicate the location of an exception.
 * Returns NULL if a stack trace is not available, or there is
 * insufficient memory to create it.
 * @end deftypefun
@*/
jit_stack_trace_t jit_exception_get_stack_trace(void)
{
	jit_stack_trace_t trace = 0;
	unsigned int size = 0;
#if JIT_APPLY_BROKEN_FRAME_BUILTINS != 0
	jit_thread_control_t control;
	jit_backtrace_t top;
	jit_backtrace_t item;
	
	/* Count the number of items in the current thread's call stack */
	control = _jit_thread_get_control();
	if(!control)
	{
		return 0;
	}
	size = 0;
	top = control->backtrace_head;
	item = top;
	while(item != 0)
	{
		++size;
		item = item->parent;
	}

	/* Allocate memory for the stack trace */
	trace = (jit_stack_trace_t)jit_malloc
		(sizeof(struct jit_stack_trace) +
		 size * sizeof(void *) - sizeof(void *));
	if(!trace)
	{
		return 0;
	}
	trace->size = size;

	/* Populate the stack trace with the items we counted earlier */
	size = 0;
	item = top;
	while(item != 0)
	{
		trace->items[size] = item->pc;
		++size;
		item = item->parent;
	}
#else
	void *frame = jit_get_current_frame();

	/* Count the number of items in the current thread's call stack */
	while(frame != 0)
	{
		frame = jit_get_next_frame_address(frame);
		++size;
	}

	/* Allocate memory for the stack trace */
	trace = (jit_stack_trace_t)jit_malloc
		(sizeof(struct jit_stack_trace) +
		 size * sizeof(void *) - sizeof(void *));
	if(!trace)
	{
		return 0;
	}
	trace->size = size;

	/* Populate the stack trace with the items we counted earlier */
	size = 0;
	frame = jit_get_current_frame();
	while(frame != 0)
	{
		trace->items[size] = jit_get_return_address(frame);
		frame = jit_get_next_frame_address(frame);
		++size;
	}
#endif
	return trace;
}

/*@
 * @deftypefun {unsigned int} jit_stack_trace_get_size (jit_stack_trace_t trace)
 * Get the size of a stack trace.
 * @end deftypefun
@*/
unsigned int jit_stack_trace_get_size(jit_stack_trace_t trace)
{
	if(trace)
	{
		return trace->size;
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun jit_function_t jit_stack_trace_get_function (jit_context_t context, jit_stack_trace_t trace, {unsigned int} posn)
 * Get the function that is at position @code{posn} within a stack trace.
 * Position 0 is the function that created the stack trace.  If this
 * returns NULL, then it indicates that there is a native callout at
 * @code{posn} within the stack trace.
 * @end deftypefun
@*/
jit_function_t jit_stack_trace_get_function
	(jit_context_t context, jit_stack_trace_t trace, unsigned int posn)
{
	if(trace && posn < trace->size)
	{
		jit_cache_t cache = _jit_context_get_cache(context);
		if(cache)
		{
			return (jit_function_t)_jit_cache_get_method
				(cache, trace->items[posn], 0);
		}
	}
	return 0;
}

/*@
 * @deftypefun {void *} jit_stack_trace_get_pc (jit_stack_trace_t trace, {unsigned int} posn)
 * Get the program counter that corresponds to position @code{posn}
 * within a stack trace.  This is the point within the function
 * where execution had reached at the time of the trace.
 * @end deftypefun
@*/
void *jit_stack_trace_get_pc
	(jit_stack_trace_t trace, unsigned int posn)
{
	if(trace && posn < trace->size)
	{
		return trace->items[posn];
	}
	else
	{
		return 0;
	}
}

/*@
 * @deftypefun {unsigned int} jit_stack_trace_get_offset (jit_stack_trace_t trace, {unsigned int} posn)
 * Get the bytecode offset that is recorded for position @code{posn}
 * within a stack trace.  This will be @code{JIT_NO_OFFSET} if there
 * is no bytecode offset associated with @code{posn}.
 * @end deftypefun
@*/
unsigned int jit_stack_trace_get_offset
	(jit_context_t context, jit_stack_trace_t trace, unsigned int posn)
{
	if(trace && posn < trace->size)
	{
		jit_cache_t cache = _jit_context_get_cache(context);
		if(cache)
		{
			jit_function_t func = (jit_function_t) _jit_cache_get_method
				(cache, trace->items[posn], 0);
			if (func)
			{
#ifdef JIT_PROLOG_SIZE
				void *start = _jit_cache_get_start_method
					(cache, func->entry_point);
#else
				void *start = func->entry_point;
#endif
				unsigned long offset = trace->items[posn] - start;
				return _jit_cache_get_bytecode(cache, start, offset, 0);
			}
		}
	}
	return JIT_CACHE_NO_OFFSET;
}

/*@
 * @deftypefun void jit_stack_trace_free (jit_stack_trace_t trace)
 * Free the memory associated with a stack trace.
 * @end deftypefun
@*/
void jit_stack_trace_free(jit_stack_trace_t trace)
{
	if(trace)
	{
		jit_free(trace);
	}
}

void _jit_backtrace_push(jit_backtrace_t trace, void *pc)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		trace->parent = control->backtrace_head;
		trace->pc = pc;
		trace->security_object = 0;
		trace->free_security_object = 0;
		control->backtrace_head = trace;
	}
	else
	{
		trace->parent = 0;
		trace->pc = pc;
		trace->security_object = 0;
		trace->free_security_object = 0;
	}
}

void _jit_backtrace_pop(void)
{
	jit_thread_control_t control = _jit_thread_get_control();
	jit_backtrace_t trace;
	if(control)
	{
		trace = control->backtrace_head;
		if(trace)
		{
			control->backtrace_head = trace->parent;
			if(trace->security_object && trace->free_security_object)
			{
				(*(trace->free_security_object))(trace->security_object);
			}
		}
	}
}

void _jit_backtrace_set(jit_backtrace_t trace)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		control->backtrace_head = trace;
	}
}

void _jit_unwind_push_setjmp(jit_jmp_buf *jbuf)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control)
	{
		jbuf->trace = control->backtrace_head;
		jbuf->catch_pc = 0;
		jbuf->parent = control->setjmp_head;
		control->setjmp_head = jbuf;
	}
}

void _jit_unwind_pop_setjmp(void)
{
	jit_thread_control_t control = _jit_thread_get_control();
	if(control && control->setjmp_head)
	{
		control->backtrace_head = control->setjmp_head->trace;
		control->setjmp_head = control->setjmp_head->parent;
	}
}

void _jit_unwind_pop_and_rethrow(void)
{
	_jit_unwind_pop_setjmp();
	jit_exception_throw(jit_exception_get_last());
}
