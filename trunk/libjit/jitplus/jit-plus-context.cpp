/*
 * jit-plus-context.cpp - C++ wrapper for JIT contexts.
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

#include <jit/jit-plus.h>

/*@

The @code{jit_context} class provides a C++ counterpart to the
C @code{jit_context_t} type.  @xref{Initialization}, for more
information on creating and managing contexts.

@*/

/*@
 * @defop Constructor jit_context jit_context ()
 * Construct a new JIT context.  This is equivalent to calling
 * @code{jit_context_create} in the C API.  The raw C context is
 * destroyed when the @code{jit_context} object is destructed.
 * @end defop
@*/
jit_context::jit_context()
{
	jit_init();
	context = jit_context_create();
	copied = 0;
}

/*@
 * @defop Constructor jit_context jit_context (jit_context_t context)
 * Construct a new JIT context by wrapping up an existing raw C context.
 * This is useful for importing a context from third party C code
 * into a program that prefers to use C++.
 *
 * When you this form of construction, @code{jit_context_destroy}
 * will not be called on the context when the @code{jit_context}
 * object is destructed.  You will need to arrange for that manually.
 * @end defop
@*/
jit_context::jit_context(jit_context_t context)
{
	this->context = context;
	this->copied = 1;
}

/*@
 * @defop Destructor jit_context ~jit_context ()
 * Destruct a JIT context.
 * @end defop
@*/
jit_context::~jit_context()
{
	if(!copied)
	{
		jit_context_destroy(context);
	}
}

/*@
 * @deftypemethod jit_context jit_context_t raw () const
 * Get the raw C context pointer that underlies this object.
 * @end deftypemethod
@*/
