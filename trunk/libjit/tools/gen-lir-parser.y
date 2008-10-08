%{
/*
 * gen-lir-parser.y - Bison grammar for the "lir-rules" program.
 *
 * Copyright (C) 2006  Southern Storm Software, Pty Ltd.
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

#include <config.h>
#include <stdio.h>
#ifdef HAVE_STRING_H
	#include <string.h>
#elif defined(HAVE_STRINGS_H)
	#include <strings.h>
#endif
#ifdef HAVE_STDLIB_H
	#include <stdlib.h>
#endif

/*
 * Imports from the lexical analyser.
 */
extern int yylex(void);
extern void yyrestart(FILE *file);
#ifdef YYTEXT_POINTER
extern char *yytext;
#else
extern char yytext[];
#endif

/*
 * Current file and line number.
 */
extern char *gensel_filename;
extern long gensel_linenum;

/*
 * Report error message.
 */
static void
gensel_error_message(char *filename, long linenum, char *msg)
{
	fprintf(stderr, "%s(%ld): %s\n", filename, linenum, msg);
}

/*
 * Report error message and exit.
 */
static void
gensel_error(char *filename, long linenum, char *msg)
{
	gensel_error_message(filename, linenum, msg);
	exit(1);
}

/*
 * Report error messages from the parser.
 */
static void
yyerror(char *msg)
{
	gensel_error_message(gensel_filename, gensel_linenum, msg);
}

/*
 * Instruction type for the "inst" variable.
 */
static char *gensel_inst_type = "unsigned char *";
static int gensel_new_inst_type = 0;

/*
 * Amount of space to reserve for the primary instruction output.
 */
// static int gensel_reserve_space = 32;
// static int gensel_reserve_more_space = 128;

/*
 * Maximal number of input values in a pattern.
 */
#define MAX_INPUT				3

/*
 * Maximal number of scratch registers in a pattern.
 */
#define MAX_SCRATCH				6

/*
 * Maximal number of pattern elements.
 */
#define MAX_PATTERN				(MAX_INPUT + MAX_SCRATCH)

/*
 * Rule Options.
 */
#define	GENSEL_OPT_TERNARY			1
#define GENSEL_OPT_BRANCH			2
#define GENSEL_OPT_NOTE				3
#define	GENSEL_OPT_COPY				4
#define GENSEL_OPT_COMMUTATIVE			5
#define	GENSEL_OPT_STACK			6
#define GENSEL_OPT_X87_ARITH			7
#define GENSEL_OPT_X87_ARITH_REVERSIBLE		8

#define	GENSEL_OPT_MANUAL			9
#define	GENSEL_OPT_MORE_SPACE			10

/*
 * Pattern values.
 */
#define GENSEL_PATT_ANY				1
#define	GENSEL_PATT_REG				2
#define	GENSEL_PATT_LREG			3
#define	GENSEL_PATT_IMM				4
#define	GENSEL_PATT_IMMZERO			5
#define	GENSEL_PATT_IMMS8			6
#define	GENSEL_PATT_IMMU8			7
#define	GENSEL_PATT_IMMS16			8
#define	GENSEL_PATT_IMMU16			9
#define	GENSEL_PATT_LOCAL			10
#define	GENSEL_PATT_FRAME			11
#define	GENSEL_PATT_SCRATCH			12
#define	GENSEL_PATT_CLOBBER			13
#define	GENSEL_PATT_IF				14
#define	GENSEL_PATT_SPACE			15

/*
 * Value types.
 */
#define GENSEL_VALUE_STRING			1
#define GENSEL_VALUE_REGCLASS			2
#define	GENSEL_VALUE_ALL			3
#define GENSEL_VALUE_CLOBBER			4
#define GENSEL_VALUE_EARLY_CLOBBER		5

/*
 * Option value.
 */
typedef struct gensel_value *gensel_value_t;
struct gensel_value
{
	int			type;
	void			*value;
	gensel_value_t		next;
};

/*
 * Option information.
 */
typedef struct gensel_option *gensel_option_t;
struct gensel_option
{
	int			option;
	gensel_value_t		values;
	gensel_option_t		next;
};

/*
 * Information about clauses.
 */
typedef struct gensel_clause *gensel_clause_t;
struct gensel_clause
{
	int			dest;
	gensel_option_t		pattern;
	char			*filename;
	long			linenum;
	char			*code;
	gensel_clause_t		next;
};

static char *gensel_args[] = {
	"dest", "value1", "value2"
};
static char *gensel_imm_args[] = {
	"dest->address", "value1->address", "value2->address"
};
// static char *gensel_reg_names[] = {
// 	"reg", "reg2", "reg3", "reg4", "reg5", "reg6", "reg7", "reg8", "reg9"
// };
// static char *gensel_other_reg_names[] = {
// 	"other_reg", "other_reg2", "other_reg3"
// };
// static char *gensel_imm_names[] = {
//	"imm_value", "imm_value2", "imm_value3"
// };
// static char *gensel_local_names[] = {
//	"local_offset", "local_offset2", "local_offset3"
// };

static char *gensel_value_names[] = {
	"param[0]", "param[1]", "param[2]"
};



/*
 * Create a value.
 */
static gensel_value_t
gensel_create_value(int type)
{
	gensel_value_t vp;

	vp = (gensel_value_t) malloc(sizeof(struct gensel_value));
	if(!vp)
	{
		exit(1);
	}

	vp->type = type;
	vp->value = 0;
	vp->next = 0;

	return vp;
}

/*
 * Create string value.
 */
static gensel_value_t
gensel_create_string_value(char *value)
{
	gensel_value_t vp;

	vp = gensel_create_value(GENSEL_VALUE_STRING);
	vp->value = value;

	return vp;
}

/*
 * Create an option.
 */
static gensel_option_t
gensel_create_option(int option, gensel_value_t values)
{
	gensel_option_t op;

	op = (gensel_option_t) malloc(sizeof(struct gensel_option));
	if(!op)
	{
		exit(1);
	}

	op->option = option;
	op->values = values;
	op->next = 0;
	return op;
}

/*
 * Create a register pattern element.
 */
static gensel_option_t
gensel_create_register(
	int flags,
	gensel_value_t value,
	gensel_value_t values)
{
	if(flags)
	{
		value->next = gensel_create_value(flags);
		value->next->next = values;
	}
	else
	{
		value->next = values;
	}

	return gensel_create_option(
		GENSEL_PATT_REG,
		value);
}

/*
 * Create a scratch register pattern element.
 */
static gensel_option_t
gensel_create_scratch(
	gensel_value_t values)
{
	return gensel_create_option(GENSEL_PATT_SCRATCH, values);
}

/*
 * Free a list of values.
 */
static void
gensel_free_values(gensel_value_t values)
{
	gensel_value_t next;
	while(values)
	{
		next = values->next;
		if(values->type == GENSEL_VALUE_STRING)
		{
			free(values->value);
		}
		free(values);
		values = next;
	}
}

/*
 * Free a list of options.
 */
static void
gensel_free_options(gensel_option_t options)
{
	gensel_option_t next;
	while(options)
	{
		next = options->next;
		gensel_free_values(options->values);
		free(options);
		options = next;
	}
}

/*
 * Free a list of clauses.
 */
static void
gensel_free_clauses(gensel_clause_t clauses)
{
	gensel_clause_t next;
	while(clauses != 0)
	{
		next = clauses->next;
		gensel_free_options(clauses->pattern);
		free(clauses->code);
		free(clauses);
		clauses = next;
	}
}

/*
 * Look for the option.
 */
static gensel_option_t
gensel_search_option(gensel_option_t options, int tag)
{
	while(options && options->option != tag)
	{
		options = options->next;
	}
	return options;
}

/*
 * Declare the register variables that are needed for a set of clauses.
 */
static void
gensel_declare_regs(gensel_clause_t clauses, gensel_option_t options)
{
	gensel_option_t pattern;
	int regs, max_regs;
	int other_regs_mask;
	int imms, max_imms;
	int locals, max_locals;
	int scratch, others;

	max_regs = 0;
	other_regs_mask = 0;
	max_imms = 0;
	max_locals = 0;
	while(clauses != 0)
	{
		regs = 0;
		imms = 0;
		locals = 0;
		others = 0;
		scratch = 0;
		pattern = clauses->pattern;
		while(pattern)
		{
			switch(pattern->option)
			{
			case GENSEL_PATT_ANY:
				++others;
				break;

			case GENSEL_PATT_REG:
				++regs;
				break;

			case GENSEL_PATT_LREG:
				other_regs_mask |= (1 << regs);
				++regs;
				break;

			case GENSEL_PATT_IMMZERO:
				++others;
				break;

			case GENSEL_PATT_IMM:
			case GENSEL_PATT_IMMS8:
			case GENSEL_PATT_IMMU8:
			case GENSEL_PATT_IMMS16:
			case GENSEL_PATT_IMMU16:
				++imms;
				break;

			case GENSEL_PATT_LOCAL:
			case GENSEL_PATT_FRAME:
				++locals;
				break;

			case GENSEL_PATT_SCRATCH:
				++scratch;
			}
			pattern = pattern->next;
		}
		if((regs + imms + locals + others) > MAX_INPUT)
		{
			gensel_error(
				clauses->filename,
				clauses->linenum,
				"too many input args in the pattern");
		}
		if(scratch > MAX_SCRATCH)
		{
			gensel_error(
				clauses->filename,
				clauses->linenum,
				"too many scratch args in the pattern");
		}
		if(max_regs < (regs + scratch))
		{
			max_regs = regs + scratch;
		}
		if(max_imms < imms)
		{
			max_imms = imms;
		}
		if(max_locals < locals)
		{
			max_locals = locals;
		}
		clauses = clauses->next;
	}
}

/*
 * Check if the pattern contains any registers.
 */
static int
gensel_contains_registers(gensel_option_t pattern)
{
	while(pattern)
	{
		switch(pattern->option)
		{
		case GENSEL_PATT_REG:
		case GENSEL_PATT_LREG:
		case GENSEL_PATT_SCRATCH:
		case GENSEL_PATT_CLOBBER:
			return 1;
		}
		pattern = pattern->next;
	}
	return 0;
}

/*
 * Returns first register in the pattern if any.
 */
static gensel_option_t
gensel_get_first_register(gensel_option_t pattern)
{
	while(pattern)
	{
		switch(pattern->option)
		{
		case GENSEL_PATT_REG:
		case GENSEL_PATT_LREG:
			return pattern;
		}
		pattern = pattern->next;
	}
	return 0;
}

static void
gensel_init_names(int count, char *names[], char *other_names[])
{
	int index;
	for(index = 0; index < count; index++)
	{
		if(names)
		{
			names[index] = "undefined";
		}
		if(other_names)
		{
			other_names[index] = "undefined";
		}
	}

}

static void
gensel_build_arg_index(
	gensel_option_t pattern,
	int count,
	char *names[],
	char *other_names[],
	int ternary,
	int free_dest)
{
	int index;

	gensel_init_names(count, names, other_names);

	index = 0;
	while(pattern)
	{
		switch(pattern->option)
		{
		case GENSEL_PATT_ANY:
			++index;
			break;

		case GENSEL_PATT_REG:
		case GENSEL_PATT_LREG:
		case GENSEL_PATT_LOCAL:
		case GENSEL_PATT_FRAME:
		case GENSEL_PATT_IMMZERO:
		case GENSEL_PATT_IMM:
		case GENSEL_PATT_IMMS8:
		case GENSEL_PATT_IMMU8:
		case GENSEL_PATT_IMMS16:
		case GENSEL_PATT_IMMU16:
			if(ternary || free_dest)
			{
				if(index < 3)
				{
					names[index] = gensel_args[index];
				}
			}
			else
			{
				if(index < 2)
				{
					names[index] = gensel_args[index + 1];
				}
			}
			++index;
			break;
		}
		pattern = pattern->next;
	}
}

static void
gensel_build_imm_arg_index(
	gensel_option_t pattern,
	int count,
	char *names[],
	char *other_names[],
	int ternary,
	int free_dest)
{
	int index;

	gensel_init_names(count, names, other_names);

	index = 0;
	while(pattern)
	{
		switch(pattern->option)
		{
		case GENSEL_PATT_ANY:
		case GENSEL_PATT_REG:
		case GENSEL_PATT_LREG:
		case GENSEL_PATT_LOCAL:
		case GENSEL_PATT_FRAME:
		case GENSEL_PATT_IMMZERO:
			++index;
			break;

		case GENSEL_PATT_IMM:
		case GENSEL_PATT_IMMS8:
		case GENSEL_PATT_IMMU8:
		case GENSEL_PATT_IMMS16:
		case GENSEL_PATT_IMMU16:
			if(ternary || free_dest)
			{
				if(index < 3)
				{
					names[index] = gensel_imm_args[index];
				}
			}
			else
			{
				if(index < 2)
				{
					names[index] = gensel_imm_args[index + 1];
				}
			}
			++index;
			break;
		}
		pattern = pattern->next;
	}
}

/*
 * Build index of input value names.
 */
static void
gensel_build_var_index(
	gensel_option_t pattern,
	char *names[MAX_PATTERN],
	char *other_names[MAX_PATTERN])
{
	int regs, imms, locals, index;

	gensel_init_names(MAX_PATTERN, names, other_names);

	regs = 0;
	imms = 0;
	locals = 0;
	index = 0;
	while(pattern)
	{
		switch(pattern->option)
		{
		case GENSEL_PATT_ANY:
			++index;
			break;

		case GENSEL_PATT_REG:
			names[index] = gensel_value_names[index];
			++regs;
			++index;
			break;

		case GENSEL_PATT_LREG:
			names[index] = gensel_value_names[index];
			other_names[index] = gensel_value_names[index];
			++regs;
			++index;
			break;

		case GENSEL_PATT_IMMZERO:
			++index;
			break;

		case GENSEL_PATT_IMM:
		case GENSEL_PATT_IMMS8:
		case GENSEL_PATT_IMMU8:
		case GENSEL_PATT_IMMS16:
		case GENSEL_PATT_IMMU16:
			names[index] = gensel_value_names[index];
			++imms;
			++index;
			break;

		case GENSEL_PATT_LOCAL:
		case GENSEL_PATT_FRAME:
			names[index] = gensel_value_names[index];
			++locals;
			++index;
			break;

		case GENSEL_PATT_SCRATCH:
			names[index] = gensel_value_names[index];
			++regs;
			++index;
		}
		pattern = pattern->next;
	}
}

/*
 * Output the code.
 */
static void
gensel_output_code(
	gensel_option_t pattern,
	char *code,
	char *names[MAX_PATTERN],
	char *other_names[MAX_PATTERN],
	int free_dest,
	int in_line)
{
	char first;
	int index;
	
	/* Output the clause code */
	if(!in_line)
	{
		printf("\t");
	}
	while(*code != '\0')
	{
		first = '1';
		if(*code == '$' && code[1] >= first && code[1] < (first + MAX_PATTERN))
		{
			index = code[1] - first;
			printf(names[index]);
			code += 2;
		}
		else if(*code == '%' && code[1] >= first && code[1] < (first + MAX_PATTERN))
		{
			index = code[1] - first;
			printf(other_names[index]);
			code += 2;
		}
		else if(*code == '\n')
		{
			putc(*code, stdout);
			++code;
		}
		else
		{
			putc(*code, stdout);
			++code;
		}
	}
	if(!in_line)
	{
		printf("\n\tbreak;\n");
	}
}

/*
 * Output the code within a clause.
 */
static void
gensel_output_clause_code(
	gensel_clause_t clause,
	char *names[MAX_PATTERN],
	char *other_names[MAX_PATTERN],
	int free_dest)
{
	/* Output the line number information from the original file */
#if 0
	printf("#line %ld \"%s\"\n", clause->linenum, clause->filename);
#endif

	gensel_output_code(clause->pattern, clause->code, names, other_names, free_dest, 0);
}

static void
gensel_output_register(char *name, gensel_value_t values)
{
}

/*
 * Output value initialization code.
 */
static void
gensel_output_register_pattern(char *name, gensel_option_t pattern)
{
	gensel_output_register(name, pattern->values->next);
}

/*
 * Output the clauses for a rule.
 */
static void gensel_output_clauses(gensel_clause_t clauses, gensel_option_t options)
{
	char *name;
	char *args[MAX_INPUT];
	char *names[MAX_PATTERN];
	char *other_names[MAX_PATTERN];
	gensel_clause_t clause;
	gensel_option_t pattern;
//	gensel_option_t space, more_space;
	gensel_value_t values;
	int regs, imms, locals, scratch, index;
	int first, seen_option;
	int ternary, free_dest;
	int contains_registers;

	/* If the clause is manual, then output it as-is */
	if(gensel_search_option(options, GENSEL_OPT_MANUAL))
	{
		gensel_init_names(MAX_PATTERN, names, other_names);
		gensel_output_clause_code(clauses, names, other_names, 0);
		return;
	}

	clause = clauses;
	contains_registers = 0;
	while(clause)
	{
		contains_registers = gensel_contains_registers(clause->pattern);
		if(contains_registers)
		{
			break;
		}
		clause = clause->next;
	}

	gensel_declare_regs(clauses, options);

	ternary = (0 != gensel_search_option(options, GENSEL_OPT_TERNARY));

	/* Output the clause checking and dispatching code */
	clause = clauses;
	first = 1;
	unsigned int state, offset;
	while(clause)
	{
		contains_registers = gensel_contains_registers(clause->pattern);
		free_dest = clause->dest;

		gensel_build_arg_index(clause->pattern, 3, args, 0, ternary, free_dest);

		if(first)
		{
			// printf("\tif(");
			printf("\tswitch(state)\n\t\{\n");
//			printf("\tcase");
		}
		else
		{
			//printf("\telse if(");
//			printf("\tcase");
		}

		index = 0;
		seen_option = 0;
		pattern = clause->pattern;
		state = 0;
		offset = 1;
		while(pattern)
		{
			switch(pattern->option)
			{
			case GENSEL_PATT_ANY:
				++index;
				break;

			case GENSEL_PATT_REG:
			case GENSEL_PATT_LREG:
				/* Do not check if the value is in
				   a register as the allocator will
				   load them anyway as long as other
				   conditions are met. */
				// if(seen_option)
				// {
				//	printf(" && ");
				// }
				// printf("(state & 0x%x)", (0x1 << (4 * index)));//args[index]);
				// state += (0x2 * offset);

				seen_option = 1;
				offset *= 3;
				++index;
				break;

			case GENSEL_PATT_IMM:
				// if(seen_option)
				// {
				// 	printf(" && ");
				// }
				// printf("(state & 0x%x)", (0x4 << (4 * index)));//args[index]);
				state += (0x2 * offset);
				seen_option = 1;
				offset *= 3;
				++index;
			break;
				
			case GENSEL_PATT_IMMZERO:
				if(seen_option)
				{
					printf(" && ");
				}
				printf("%s->is_nint_constant && ", args[index]);
				printf("%s->address == 0", args[index]);
				seen_option = 1;
				++index;
				break;

			case GENSEL_PATT_IMMS8:
				if(seen_option)
				{
					printf(" && ");
				}
				printf("%s->is_nint_constant && ", args[index]);
				printf("%s->address >= -128 && ", args[index]);
				printf("%s->address <= 127", args[index]);
				seen_option = 1;
				++index;
				break;

			case GENSEL_PATT_IMMU8:
				if(seen_option)
				{
					printf(" && ");
				}
				printf("%s->is_nint_constant && ", args[index]);
				printf("%s->address >= 0 && ", args[index]);
				printf("%s->address <= 255", args[index]);
				seen_option = 1;
				++index;
				break;

			case GENSEL_PATT_IMMS16:
				if(seen_option)
				{
					printf(" && ");
				}
				printf("%s->is_nint_constant && ", args[index]);
				printf("%s->address >= -32768 && ", args[index]);
				printf("%s->address <= 32767", args[index]);
				seen_option = 1;
				++index;
				break;

			case GENSEL_PATT_IMMU16:
				if(seen_option)
				{
					printf(" && ");
				}
				printf("%s->is_nint_constant && ", args[index]);
				printf("%s->address >= 0 && ", args[index]);
				printf("%s->address <= 65535", args[index]);
				seen_option = 1;
				++index;
				break;

			case GENSEL_PATT_LOCAL:
				// if(seen_option)
				// {
				// 	printf(" && ");
				// }
				// printf("%s->vreg && %s->vreg->in_frame",
				//       args[index], args[index]);
				// printf("(state & 0x%x)", (0x2 << (4 * index)));
				state += (0x1 * offset);
				seen_option = 1;
				offset *= 3;
				++index;
				break;

			case GENSEL_PATT_FRAME:
				if(seen_option)
				{
					printf(" && ");
				}
				printf("!%s->is_constant", args[index]);
				seen_option = 1;
				++index;
				break;

			case GENSEL_PATT_IF:
				if(seen_option)
				{
					printf(" && ");
				}
				printf("(");
				gensel_build_imm_arg_index(
					clause->pattern, MAX_PATTERN,
					names, other_names, ternary, free_dest);
				gensel_output_code(
					clause->pattern,
					pattern->values->value,
					names, other_names, free_dest, 1);
				printf(")");
				seen_option = 1;
				break;
			}
			pattern = pattern->next;
		}
		if(!seen_option)
		{
			printf("\tdefault:");
		}
		else
		{
			printf("\tcase 0x%x:", state);
		}
		printf("\n");
		if(contains_registers)
		{
			seen_option = 0;
			if(gensel_search_option(options, GENSEL_OPT_STACK))
			{
				if(seen_option)
				{
					printf(" | ");
				}
				else
				{
					seen_option = 1;
				}
				printf("_JIT_REGS_STACK");
			}
			if(gensel_search_option(options, GENSEL_OPT_COMMUTATIVE))
			{
				if(seen_option)
				{
					printf(" | ");
				}
				else
				{
					seen_option = 1;
				}
				printf("_JIT_REGS_COMMUTATIVE");
			}
			/* x87 options */
			if(gensel_search_option(options, GENSEL_OPT_X87_ARITH))
			{
				if(seen_option)
				{
					printf(" | ");
				}
				else
				{
					seen_option = 1;
				}
				printf("_JIT_REGS_X87_ARITH");
			}
			else if(gensel_search_option(options, GENSEL_OPT_X87_ARITH_REVERSIBLE))
			{
				if(seen_option)
				{
					printf(" | ");
				}
				else
				{
					seen_option = 1;
				}
				printf("_JIT_REGS_X87_ARITH_REVERSIBLE");
			}

			if(!(ternary || free_dest
			     || gensel_search_option(options, GENSEL_OPT_NOTE)
			     || gensel_search_option(options, GENSEL_OPT_BRANCH)))
			{
				pattern = gensel_get_first_register(clause->pattern);
				gensel_output_register("dest", 0);
			}
		}

		regs = 0;
		index = 0;
		scratch = 0;
		pattern = clause->pattern;
		while(pattern)
		{
			switch(pattern->option)
			{
			case GENSEL_PATT_ANY:
				++index;
				break;

			case GENSEL_PATT_REG:
			case GENSEL_PATT_LREG:
				gensel_output_register_pattern(args[index], pattern);
				++regs;
				++index;
				break;

			case GENSEL_PATT_IMMZERO:
			case GENSEL_PATT_IMM:
			case GENSEL_PATT_IMMS8:
			case GENSEL_PATT_IMMU8:
			case GENSEL_PATT_IMMS16:
			case GENSEL_PATT_IMMU16:
			case GENSEL_PATT_LOCAL:
				++index;
				break;

			case GENSEL_PATT_FRAME:
				++index;
				break;

			case GENSEL_PATT_SCRATCH:
				if(pattern->values->next && pattern->values->next->value)
				{
					name = pattern->values->next->value;
				}
				++regs;
				++scratch;
				++index;
				break;

			case GENSEL_PATT_CLOBBER:
				values = pattern->values;
				while(values)
				{
					if(!values->value)
					{
						continue;
					}
					switch(values->type)
					{
					case GENSEL_VALUE_STRING:
						name = values->value;
						break;

					case GENSEL_VALUE_REGCLASS:
						break;

					case GENSEL_VALUE_ALL:
						break;
					}
					values = values->next;
				}
				break;
			}
			pattern = pattern->next;
		}

		regs = 0;
		imms = 0;
		locals = 0;
		index = 0;
		scratch = 0;

		gensel_build_var_index(clause->pattern, names, other_names);
		gensel_output_clause_code(clause, names, other_names, free_dest);

		/* Copy "inst" back into the generation context */
		if(gensel_new_inst_type)
		{
		}

		first = 0;
		clause = clause->next;
	}
	// printf("\telse printf(\"An error 0x\%\%x\", insn->opcode);\n");
	printf("\t}\n");
}

/*
 * List of opcodes that are supported by the input rules.
 */
static char **supported = 0;
static char **supported_options = 0;
static int num_supported = 0;

/*
 * Add an opcode to the supported list.
 */
static void gensel_add_supported(char *name, char *option)
{
	supported = (char **)realloc
		(supported, (num_supported + 1) * sizeof(char *));
	if(!supported)
	{
		exit(1);
	}
	supported[num_supported] = name;
	supported_options = (char **)realloc
		(supported_options, (num_supported + 1) * sizeof(char *));
	if(!supported_options)
	{
		exit(1);
	}
	supported_options[num_supported++] = option;
}

/*
 * Output the list of supported opcodes.
 */
static void gensel_output_supported(void)
{
	int index;
	for(index = 0; index < num_supported; ++index)
	{
		if(supported_options[index])
		{
			if(supported_options[index][0] == '!')
			{
				printf("#ifndef %s\n", supported_options[index] + 1);
			}
			else
			{
				printf("#ifdef %s\n", supported_options[index]);
			}
			printf("case %s:\n", supported[index]);
			printf("#endif\n");
		}
		else
		{
			printf("case %s:\n", supported[index]);
		}
	}
	printf("\treturn 1;\n\n");
}

%}

/*
 * Define the structure of yylval.
 */
%union {
	int			tag;
	char			*name;
	struct gensel_value	*value;
	struct gensel_option	*option;
	struct
	{
		char	*filename;
		long	linenum;
		char	*block;

	}	code;
	struct
	{
		struct gensel_value	*head;
		struct gensel_value	*tail;

	}	values;
	struct
	{
		struct gensel_option	*head;
		struct gensel_option	*tail;

	}	options;
	struct
	{
		struct gensel_clause	*head;
		struct gensel_clause	*tail;

	}	clauses;
}

/*
 * Primitive lexical tokens and keywords.
 */
%token IDENTIFIER		"an identifier"
%token CODE_BLOCK		"a code block"
%token LITERAL			"literal string"
%token K_PTR			"`->'"
%token K_ANY			"any value"
%token K_ALL			"all registers"
%token K_IMM			"immediate value"
%token K_IMMZERO		"immediate zero value"
%token K_IMMS8			"immediate signed 8-bit value"
%token K_IMMU8			"immediate unsigned 8-bit value"
%token K_IMMS16			"immediate signed 16-bit value"
%token K_IMMU16			"immediate unsigned 16-bit value"
%token K_LOCAL			"local variable"
%token K_FRAME			"local variable forced out into the stack frame"
%token K_NOTE			"`note'"
%token K_TERNARY		"`ternary'"
%token K_BRANCH			"`branch'"
%token K_COPY			"`copy'"
%token K_COMMUTATIVE		"`commutative'"
%token K_IF			"`if'"
%token K_CLOBBER		"`clobber'"
%token K_SCRATCH		"`scratch'"
%token K_SPACE			"`space'"
%token K_STACK			"`stack'"
%token K_X87_ARITH		"`x87_arith'"
%token K_X87_ARITH_REVERSIBLE	"`x87_arith_reversible'"
%token K_INST_TYPE		"`%inst_type'"
%token K_REG_CLASS		"`%reg_class'"
%token K_LREG_CLASS		"`%lreg_class'"

/* deperecated keywords */

%token K_MANUAL			"`manual'"
%token K_MORE_SPACE		"`more_space'"

/*
 * Define the yylval types of the various non-terminals.
 */
%type <code>			CODE_BLOCK
%type <name>			IDENTIFIER LITERAL
%type <name>			IfClause IdentifierList Literal
%type <tag>			OptionTag InputTag DestFlag RegFlag
%type <clauses>			Clauses Clause
%type <options>			Options Pattern
%type <option>			Option PatternElement
%type <values>			ValuePair ClobberList ClobberSpec
%type <value>			Value ClobberEntry RegClass

%expect 0

%error-verbose

%start Start
%%

Start
	: /* empty */
	| Rules
	;

Rules
	: Rule
	| Rules Rule
	;

Rule
	: IdentifierList IfClause ':' Options Clauses {
			if($2)
			{
				if(($2)[0] == '!')
				{
					printf("#ifndef %s\n\n", $2 + 1);
				}
				else
				{
					printf("#ifdef %s\n\n", $2);
				}
			}
			printf("case %s:\n{\n", $1);
			gensel_output_clauses($5.head, $4.head);
			printf("}\nbreak;\n\n");
			if($2)
			{
				printf("#endif /* %s */\n\n", $2);
			}
			gensel_free_clauses($5.head);
			gensel_add_supported($1, $2);
		}
	| K_INST_TYPE IDENTIFIER	{
			gensel_inst_type = $2;
			gensel_new_inst_type = 1;
		}
	| K_REG_CLASS IDENTIFIER IDENTIFIER {
			// gensel_create_regclass($2, $3, 0);
		}
	| K_LREG_CLASS IDENTIFIER IDENTIFIER {
			// gensel_create_regclass($2, $3, 1);
		}
	;

IdentifierList
	: IDENTIFIER			{ $$ = $1; }
	| IdentifierList ',' IDENTIFIER	{
			char *result = (char *)malloc(strlen($1) + strlen($3) + 16);
			if(!result)
			{
				exit(1);
			}
			strcpy(result, $1);
			strcat(result, ":\ncase ");
			strcat(result, $3);
			free($1);
			free($3);
			$$ = result;
		}
	;

IfClause
	: /* empty */			{ $$ = 0; }
	| '(' IDENTIFIER ')'		{ $$ = $2; }
	;

Options
	: /* empty */			{
			$$.head = 0;
			$$.tail = 0;
		}
	| Option			{
			$$.head = $1;
			$$.tail = $1;
		}
	| Options ',' Option		{
			$1.tail->next = $3;
			$$.head = $1.head;
			$$.tail = $3;
		}
	;

Option
	: OptionTag			{
			$$ = gensel_create_option($1, 0);
		}
	;

Clauses
	: Clause			{ $$ = $1; }
	| Clauses Clause		{
			$1.tail->next = $2.head;
			$$.head = $1.head;
			$$.tail = $2.tail;
		}
	;

Clause
	: '[' DestFlag Pattern ']' K_PTR CODE_BLOCK {
			gensel_clause_t clause;
			clause = (gensel_clause_t)malloc(sizeof(struct gensel_clause));
			if(!clause)
			{
				exit(1);
			}
			clause->dest = $2;
			clause->pattern = $3.head;
			clause->filename = $6.filename;
			clause->linenum = $6.linenum;
			clause->code = $6.block;
			clause->next = 0;
			$$.head = clause;
			$$.tail = clause;
		}
	;

Pattern
	: /* empty */			{
			$$.head = 0;
			$$.tail = 0;
		}
	| PatternElement		{
			$$.head = $1;
			$$.tail = $1;
		}
	| Pattern ',' PatternElement	{
			$1.tail->next = $3;
			$$.head = $1.head;
			$$.tail = $3;
		}
	;
	
PatternElement
	: InputTag			{
			$$ = gensel_create_option($1, 0);
		}
	| RegFlag RegClass		{
			$$ = gensel_create_register($1, $2, 0);
		}
	| RegFlag RegClass '(' Value ')'	{
			$$ = gensel_create_register($1, $2, $4);
		}
	| RegFlag RegClass '(' ValuePair ')'	{
			$$ = gensel_create_register($1, $2, $4.head);
		}
	| K_SCRATCH RegClass {
		$$ = gensel_create_scratch($2);
		}
	| K_SCRATCH RegClass '(' Value ')' {
			$$ = gensel_create_scratch($2);
		}
	| K_CLOBBER '(' ClobberSpec ')'	{
			$$ = gensel_create_option(GENSEL_PATT_CLOBBER, $3.head);
		}
	| K_IF '(' Value ')'		{
			$$ = gensel_create_option(GENSEL_PATT_IF, $3);
		}
	| K_SPACE '(' Value ')'		{
			$$ = gensel_create_option(GENSEL_PATT_SPACE, $3);
		}
	;

ClobberSpec
	: K_ALL				{
			$$.head = $$.tail = gensel_create_value(GENSEL_VALUE_ALL);
		}
	| ClobberList			{
			$$ = $1;
		}
	;

ClobberList
	: ClobberEntry			{
			$$.head = $1;
			$$.tail = $1;
		}
	| ClobberList ',' ClobberEntry	{
			$1.tail->next = $3;
			$$.head = $1.head;
			$$.tail = $3;
		}
	;

ClobberEntry
	: RegClass			{ $$ = $1; }
	| Value				{ $$ = $1; }
	;

RegClass
	: IDENTIFIER			{
		}
	;

Value
	: Literal			{
			$$ = gensel_create_string_value($1);
		}
	;

ValuePair
	: Value	':' Value		{
			$1->next = $3;
			$$.head = $1;
			$$.tail = $3;
		}
	;

OptionTag
	: K_TERNARY			{ $$ = GENSEL_OPT_TERNARY; }
	| K_BRANCH			{ $$ = GENSEL_OPT_BRANCH; }
	| K_NOTE			{ $$ = GENSEL_OPT_NOTE; }
	| K_COPY			{ $$ = GENSEL_OPT_COPY; }
	| K_COMMUTATIVE			{ $$ = GENSEL_OPT_COMMUTATIVE; }
	| K_STACK			{ $$ = GENSEL_OPT_STACK; }
	| K_X87_ARITH			{ $$ = GENSEL_OPT_X87_ARITH; }
	| K_X87_ARITH_REVERSIBLE	{ $$ = GENSEL_OPT_X87_ARITH_REVERSIBLE; }

	/* deprecated: */
	| K_MANUAL			{ $$ = GENSEL_OPT_MANUAL; }
	| K_MORE_SPACE			{ $$ = GENSEL_OPT_MORE_SPACE; }
	;

InputTag
	: K_IMM				{ $$ = GENSEL_PATT_IMM; }
	| K_IMMZERO			{ $$ = GENSEL_PATT_IMMZERO; }
	| K_IMMS8			{ $$ = GENSEL_PATT_IMMS8; }
	| K_IMMU8			{ $$ = GENSEL_PATT_IMMU8; }
	| K_IMMS16			{ $$ = GENSEL_PATT_IMMS16; }
	| K_IMMU16			{ $$ = GENSEL_PATT_IMMU16; }
	| K_LOCAL			{ $$ = GENSEL_PATT_LOCAL; }
	| K_FRAME			{ $$ = GENSEL_PATT_FRAME; }
	| K_ANY				{ $$ = GENSEL_PATT_ANY; }
	;

DestFlag
	: /* empty */			{ $$ = 0; }
	| '='				{ $$ = 1; }
	;

RegFlag
	: /* empty */			{ $$ = 0; }
	| '*'				{ $$ = GENSEL_VALUE_CLOBBER; }
	| '+'				{ $$ = GENSEL_VALUE_EARLY_CLOBBER; }
	;

Literal
	: LITERAL			{ $$ = $1; }
	| Literal LITERAL		{
			char *cp = malloc(strlen($1) + strlen($2) + 1);
			if(!cp)
			{
				exit(1);
			}
			strcpy(cp, $1);
			strcat(cp, $2);
			free($1);
			free($2);
			$$ = cp;
		}
	;

%%

#define	COPYRIGHT_MSG	\
" * Copyright (C) 2004  Southern Storm Software, Pty Ltd.\n" \
" *\n" \
" * This program is free software; you can redistribute it and/or modify\n" \
" * it under the terms of the GNU General Public License as published by\n" \
" * the Free Software Foundation; either version 2 of the License, or\n" \
" * (at your option) any later version.\n" \
" *\n" \
" * This program is distributed in the hope that it will be useful,\n" \
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n" \
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n" \
" * GNU General Public License for more details.\n" \
" *\n" \
" * You should have received a copy of the GNU General Public License\n" \
" * along with this program; if not, write to the Free Software\n" \
" * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA\n"
 
int main(int argc, char *argv[])
{
	FILE *file;
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s input.sel >output.slc\n", argv[0]);
		return 1;
	}
	file = fopen(argv[1], "r");
	if(!file)
	{
		perror(argv[1]);
		return 1;
	}
	printf("/%c Automatically generated from %s - DO NOT EDIT %c/\n",
		   '*', argv[1], '*');
	printf("/%c\n%s%c/\n\n", '*', COPYRIGHT_MSG, '*');
	printf("#if defined(JIT_INCLUDE_RULES)\n\n");
	gensel_filename = argv[1];
	gensel_linenum = 1;
	yyrestart(file);
	if(yyparse())
	{
		fclose(file);
		return 1;
	}
	fclose(file);
	printf("#elif defined(JIT_INCLUDE_SUPPORTED)\n\n");
	gensel_output_supported();
	printf("#endif\n");
	return 0;
}
