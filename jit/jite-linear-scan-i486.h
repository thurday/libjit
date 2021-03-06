#include <config.h>
#include "jit-rules.h"

#if defined(JITE_ENABLED) && !defined(JIT_BACKEND_INTERP)

#include "jite-linear-scan.h"
#include "jit-gen-i486-simd.h"
/* Item = {reg,    index,          hash_code,      vreg,    local_vreg, liveness}.
          1 param  2 param         3 param         4 param  5 param     6 param */

struct _jite_reg jite_gp_regs_map[] =
        {
        {X86_EAX, 0, 0x1,  0, 0, 0, 0},
        {X86_EDX, 1, 0x2,  0, 0, 0, 0},
        {X86_ECX, 2, 0x4,  0, 0, 0, 0},
        {X86_EBX, 3, 0x8,  0, 0, 0, 0},
        {X86_EDI, 4, 0x10, 0, 0, 0, 0},
        {X86_ESI, 5, 0x20, 0, 0, 0, 0}
        };

#define LOCAL_REGISTERS_HASH (0x1 | 0x2 | 0x4)

struct _jite_reg jite_xmm_regs_map[] =
        {
        {XMM0, 0, 0x40,   0, 0, 0, 0},
        {XMM1, 1, 0x80,   0, 0, 0, 0},
        {XMM2, 2, 0x100,  0, 0, 0, 0},
        {XMM3, 3, 0x200,  0, 0, 0, 0},
        {XMM4, 4, 0x400,  0, 0, 0, 0},
        {XMM5, 5, 0x800,  0, 0, 0, 0},
        {XMM6, 6, 0x1000, 0, 0, 0, 0},
        {XMM7, 7, 0x2000, 0, 0, 0, 0}
        };

/* In the following table all opcodes are aranged according to their own integer code, which means that
   JIT_OP_TRUCT_SBYTE has a 0x1 integer code.
   hash
   machine_code1     machine_code2  machine_code3                 machine_code4
   has_side_effect   is_nop         has_branching
   dest_defined      dest_used      value1_defined                value1_used    value2_used */

struct _jite_opcode jite_opcodes_map[] =
	{/* OPCODE                        OBJECT CODE PARAMS  	 OBJECT CODE PROPERTIES    VALUES PROPERTIES */
	{JIT_OP_NOP,                      0, 0, 0, 0, 		  	0, 1, 0,           0, 0, 0, 0, 0},
	{JIT_OP_TRUNC_SBYTE,              0, 0, 0, 0,   		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_TRUNC_UBYTE,              0, 0, 0, 0,   	 	0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_TRUNC_SHORT,              0, 0, 0, 0,   	 	0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_TRUNC_USHORT,             0, 0, 0, 0,    		0, 0, 0,	   1, 0, 0, 1, 0},
	{JIT_OP_TRUNC_INT,                0, 0, 0, 0,    		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_TRUNC_UINT,               0, 0, 0, 0,   		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_SBYTE,              0, 0, 0, 0,   		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_UBYTE,              0, 0, 0, 0, 			1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_SHORT,              0, 0, 0, 0, 			1, 0, 0,  	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_USHORT,             0, 0, 0, 0,    		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_INT,                0, 0, 0, 0,    		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_UINT,               0, 0, 0, 0,    		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_LOW_WORD,                 0, 0, 0, 0,    		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_EXPAND_INT,               0, 0, 0, 0,  			0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_EXPAND_UINT,              0, 0, 0, 0,    		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_LOW_WORD,           0, 0, 0, 0,    		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_SIGNED_LOW_WORD,    0, 0, 0, 0,   		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_LONG,               0, 0, 0, 0,    		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_ULONG,              0, 0, 0, 0,    		1, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_NFLOAT_TO_INT,            0, 0, 0, 0,    		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_NFLOAT_TO_UINT,           0, 0, 0, 0,   		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_NFLOAT_TO_LONG,           0, 0, 0, 0,    		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_NFLOAT_TO_ULONG,          0, 0, 0, 0,    		0, 0, 0,   	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_NFLOAT_TO_INT,      0, 0, 0, 0,   		1, 0, 0,  	   1, 0, 0, 1, 0},
	{JIT_OP_CHECK_NFLOAT_TO_UINT,     0, 0, 0, 0,    		1, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_CHECK_NFLOAT_TO_LONG,     0, 0, 0, 0,    		1, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_CHECK_NFLOAT_TO_ULONG,    0, 0, 0, 0,   		1, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_INT_TO_NFLOAT,            0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_UINT_TO_NFLOAT,           0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_LONG_TO_NFLOAT,           0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_ULONG_TO_NFLOAT,          0, 0, 0, 0,   		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_NFLOAT_TO_FLOAT32,        0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_NFLOAT_TO_FLOAT64,        0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_FLOAT32_TO_NFLOAT,        0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_FLOAT64_TO_NFLOAT,        0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 0},
	{JIT_OP_IADD,               X86_ADD, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IADD_OVF,           X86_ADD, 0, 0, 0,   		1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IADD_OVF_UN,        X86_ADD, 0, 0, 0,    		1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_ISUB,                     0, 0, 0, 0,    		0, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_ISUB_OVF,                 0, 0, 0, 0,   		1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_ISUB_OVF_UN,              0, 0, 0, 0,    		1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IMUL,                     0, 0, 0, 0,   		0, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IMUL_OVF,                 0, 0, 0, 0,   		1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IMUL_OVF_UN,              0, 0, 0, 0,    		1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IDIV,                     0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_IDIV_UN,                  0, 0, 0, 0,			1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IREM,                     0, 0, 0, 0, 			1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_IREM_UN,                  0, 0, 0, 0, 			1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_INEG,                     0, 0, 0, 0, 			0, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_LADD,         X86_ADD, X86_ADC, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LADD_OVF,     X86_ADD, X86_ADC, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LADD_OVF_UN,  X86_ADD, X86_ADC, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LSUB,         X86_SUB, X86_SBB, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LSUB_OVF,     X86_SUB, X86_SBB, 0, 0, 			1, 0, 0,	   1, 0, 0, 1, 1},
	{JIT_OP_LSUB_OVF_UN,  X86_SUB, X86_SBB, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LMUL,                     0, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LMUL_OVF,                 0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LMUL_OVF_UN, 		  0, 0, 0, 0,			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LDIV, 			  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LDIV_UN, 		  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LREM,			  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LREM_UN, 		  0, 0, 0, 0,			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LNEG,			  0, 0, 0, 0,			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_FADD,	            0xF3, 0x58, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_FSUB,		    0xF3, 0x5C, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_FMUL, 		    0xF3, 0x59, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_FDIV, 		    0xF3, 0x5E, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_FREM, 		          0, 0, 0, 0,			1, 0, 0,	   1, 0, 0, 1, 1},
	{JIT_OP_FREM_IEEE, 		  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_FNEG, 			  0, 0, 0, 0, 			0, 0, 0,	   1, 0, 0, 1, 0},
	{JIT_OP_DADD, 		    0xF2, 0x58, 0, 0, 			0, 0, 0,	   1, 0, 0, 1, 1},
	{JIT_OP_DSUB, 		    0xF2, 0x5C, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_DMUL, 		    0xF2, 0x59, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_DDIV, 		    0xF2, 0x5E, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_DREM, 			  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_DREM_IEEE, 		  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_DNEG, 			  0, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 0},
	{JIT_OP_NFADD, 			  0, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_NFSUB, 			  0, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_NFMUL, 			  0, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_NFDIV, 			  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_NFREM, 			  0, 0, 0, 0, 			1, 0, 0,           1, 0, 0, 1, 1},
	{JIT_OP_NFREM_IEEE, 		  0, 0, 0, 0, 			1, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_NFNEG, 			  0, 0, 0, 0,			0, 0, 0, 	   1, 0, 0, 1, 0},
	{JIT_OP_IAND, 		    X86_AND, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_IOR, 		    X86_OR,  0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_IXOR, 		    X86_XOR, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_INOT, 			  1, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 0},
	{JIT_OP_ISHL, 		    X86_SHL, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_ISHR,		    X86_SAR, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_ISHR_UN, 	    X86_SHR, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LAND,	      X86_AND, X86_AND, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LOR, 	      X86_OR,  X86_OR,  0, 0,			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LXOR, 	      X86_XOR, X86_XOR, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LNOT,			  1, 1, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 0},
	{JIT_OP_LSHL, 			  0, 0, 0, 0, 			0, 0, 0,	   1, 0, 0, 1, 1},
	{JIT_OP_LSHR, 			  0, 0, 0, 0, 			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_LSHR_UN,		  0, 0, 0, 0,			0, 0, 0, 	   1, 0, 0, 1, 1},
	{JIT_OP_BR,                       0, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 0},
	{JIT_OP_BR_IFALSE,    0x74 /* eq */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 0},
	{JIT_OP_BR_ITRUE,     0x75 /* ne */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_IEQ,       0x74 /* eq */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_INE,       0x75 /* ne */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_ILT,       0x7C /* lt */,    0x7F /* gt */,    0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_ILT_UN,    0x72 /* lt_un */, 0x77 /* gt_un */, 0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_ILE,       0x7E /* le */,    0x7D /* ge */,    0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_ILE_UN,    0x76 /* le_un */, 0x73 /* ge_un */, 0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_IGT,       0x7F /* gt */,    0x7C /* lt */,    0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_IGT_UN,    0x77 /* gt_un */, 0x72 /* lt_un */, 0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_IGE,       0x7D /* ge */,    0x7E /* le */,    0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_IGE_UN,    0x73 /* ge_un */, 0x76 /* le_un */, 0, 0, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_LFALSE,    0x74 /* eq */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 0},
	{JIT_OP_BR_LTRUE,     0x75 /* ne */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 0},
	{JIT_OP_BR_LEQ,       0x74 /* eq */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_LNE,       0x75 /* ne */, 0, 0, 0,                   0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_LLT,       0x7C /* lt */,    0x72 /* lt_un */, 0x7F /* gt */,    0x77 /* gt_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_LLT_UN,    0x72 /* lt_un */, 0x72 /* lt_un */, 0x77 /* gt_un */, 0x77 /* gt_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_LLE,       0x7C /* lt */,    0x76 /* le_un */, 0x7F /* gt */,    0x73 /* ge_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_LLE_UN,    0x72 /* lt_un */, 0x76 /* le_un */, 0x77 /* gt_un */, 0x73 /* ge_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_LGT,       0x7F /* gt */,    0x77 /* gt_un */, 0x7C /* lt */,    0x72 /* lt_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_LGT_UN,    0x77 /* gt_un */, 0x77 /* gt_un */, 0x72 /* lt_un */, 0x72 /* lt_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_LGE,       0x7F /* gt */   , 0x73 /* ge_un */, 0x7C /* lt */,    0x76 /* le_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_LGE_UN,    0x77 /* gt_un */, 0x73 /* ge_un */, 0x72 /* lt_un */, 0x76 /* le_un */, 0, 0, 1,     0, 0, 0, 1, 1},
	{JIT_OP_BR_FEQ,       0, 0, 0, 0,                               0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FNE,       0, 0, 0, 0,                               0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FLT,     0x72 /* lt_un */, 0x77 /* gt */, 0x0, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FLE,     0x76 /* le_un */, 0x73 /* ge */, 0x0, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FGT,     0x77 /* gt_un */, 0x72 /* lt */, 0x0, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FGE,     0x73 /* ge_un */, 0x76 /* le */, 0x0, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FEQ_INV,   0, 0, 0, 0,                               0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FNE_INV,   0, 0, 0, 0,                               0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_FLT_INV, 0x72 /* lt_un */, 0x77 /* gt_un */, 0x0, 0x2F, 0, 0, 1,        0, 0, 0, 1, 1},
	{JIT_OP_BR_FLE_INV, 0x76 /* le_un */, 0x73 /* ge_un */, 0x0, 0x2F, 0, 0, 1,        0, 0, 0, 1, 1},
	{JIT_OP_BR_FGT_INV, 0x77 /* gt_un */, 0x72 /* lt_un */, 0x0, 0x2F, 0, 0, 1,        0, 0, 0, 1, 1},
	{JIT_OP_BR_FGE_INV, 0x73 /* ge_un */, 0x76 /* le_un */, 0x0, 0x2F, 0, 0, 1,        0, 0, 0, 1, 1},
	{JIT_OP_BR_DEQ, 0, 0, 0, 0, 0, 0, 1,                                               0, 0, 0, 1, 1},
	{JIT_OP_BR_DNE, 0, 0, 0, 0, 0, 0, 1,                                               0, 0, 0, 1, 1},
	{JIT_OP_BR_DLT, 0x72 /* lt_un */, 0x77 /* gt_un */, 0x66, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_DLE, 0x76 /* le_un */, 0x73 /* ge_un */, 0x66, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_DGT, 0x77 /* gt_un */, 0x72 /* lt_un */, 0x66, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_DGE, 0x73 /* ge_un */, 0x76 /* le_un */, 0x66, 0x2F, 0, 0, 1,           0, 0, 0, 1, 1},
	{JIT_OP_BR_DEQ_INV, 0, 0, 0, 0, 0, 0, 1,                                           0, 0, 0, 1, 1},
	{JIT_OP_BR_DNE_INV, 0, 0, 0, 0, 0, 0, 1,                                           0, 0, 0, 1, 1},
	{JIT_OP_BR_DLT_INV, 0x72 /* lt_un */, 0x77 /* gt_un */, 0x66, 0x2F, 0, 0, 1,       0, 0, 0, 1, 1},
	{JIT_OP_BR_DLE_INV, 0x76 /* le_un */, 0x73 /* ge_un */, 0x66, 0x2F, 0, 0, 1,       0, 0, 0, 1, 1},
	{JIT_OP_BR_DGT_INV, 0x77 /* gt_un */, 0x72 /* lt_un */, 0x66, 0x2F, 0, 0, 1,       0, 0, 0, 1, 1},
	{JIT_OP_BR_DGE_INV, 0x73 /* ge_un */, 0x76 /* le_un */, 0x66, 0x2F, 0, 0, 1,       0, 0, 0, 1, 1},
	{JIT_OP_BR_NFEQ, 0, 0, 0, 0, 0, 0, 1,                                              0, 0, 0, 1, 1},
	{JIT_OP_BR_NFNE, 0, 0, 0, 0, 0, 0, 1,                                              0, 0, 0, 1, 1},
	{JIT_OP_BR_NFLT, 0, 0, 0, 0, 0, 0, 1,                                              0, 0, 0, 1, 1},
	{JIT_OP_BR_NFLE, 0, 0, 0, 0, 0, 0, 1,                                              0, 0, 0, 1, 1},
	{JIT_OP_BR_NFGT, 0, 0, 0, 0, 0, 0, 1,                                              0, 0, 0, 1, 1},
	{JIT_OP_BR_NFGE, 0, 0, 0, 0, 0, 0, 1,                                              0, 0, 0, 1, 1},
	{JIT_OP_BR_NFEQ_INV, 0, 0, 0, 0, 0, 0, 1,                                          0, 0, 0, 1, 1},
	{JIT_OP_BR_NFNE_INV, 0, 0, 0, 0, 0, 0, 1,                                          0, 0, 0, 1, 1},
	{JIT_OP_BR_NFLT_INV, 0, 0, 0, 0, 0, 0, 1,                                          0, 0, 0, 1, 1},
	{JIT_OP_BR_NFLE_INV, 0, 0, 0, 0, 0, 0, 1,                                          0, 0, 0, 1, 1},
	{JIT_OP_BR_NFGT_INV, 0, 0, 0, 0, 0, 0, 1,                                          0, 0, 0, 1, 1},
	{JIT_OP_BR_NFGE_INV, 0, 0, 0, 0, 0, 0, 1,                                          0, 0, 0, 1, 1},
	{JIT_OP_ICMP, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_ICMP_UN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 1},
	{JIT_OP_LCMP, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_LCMP_UN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 1},
	{JIT_OP_FCMPL, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 1},
	{JIT_OP_FCMPG, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 1},
	{JIT_OP_DCMPL, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 1},
	{JIT_OP_DCMPG, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 1},
	{JIT_OP_NFCMPL, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 1},
	{JIT_OP_NFCMPG, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 1},
	{JIT_OP_IEQ,    X86_CC_EQ, X86_CC_EQ, 0, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_INE,    X86_CC_NE, X86_CC_NE, 0, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_ILT,    X86_CC_LT, X86_CC_GT, 1, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_ILT_UN, X86_CC_B, X86_CC_A, 0, 0, 0, 0, 0,                                 1, 0, 0, 1, 1},
	{JIT_OP_ILE,    X86_CC_LE, X86_CC_GE, 1, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_ILE_UN, X86_CC_BE, X86_CC_AE, 0, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_IGT,    X86_CC_GT, X86_CC_LT, 1, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_IGT_UN, X86_CC_A, X86_CC_B, 0, 0, 0, 0, 0,                                 1, 0, 0, 1, 1},
	{JIT_OP_IGE,    X86_CC_GE, X86_CC_LE, 1, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_IGE_UN, X86_CC_AE, X86_CC_BE, 0, 0, 0, 0, 0,                               1, 0, 0, 1, 1},
	{JIT_OP_LEQ, 0, 0, 0, 0, 0, 0, 0,                                                  1, 0, 0, 1, 1},
	{JIT_OP_LNE, 0, 0, 0, 0, 0, 0, 0,                                                  1, 0, 0, 1, 1},
	{JIT_OP_LLT, 0, 0, 0, 0, 0, 0, 0,                                                  1, 0, 0, 1, 1},
	{JIT_OP_LLT_UN, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 1},
	{JIT_OP_LLE, 0, 0, 0, 0, 0, 0, 0,                                                  1, 0, 0, 1, 1},
	{JIT_OP_LLE_UN, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 1},
	{JIT_OP_LGT, 0, 0, 0, 0, 0, 0, 0,                                                  1, 0, 0, 1, 1},
	{JIT_OP_LGT_UN, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 1},
	{JIT_OP_LGE, 0, 0, 0, 0, 0, 0, 0,                                                  1, 0, 0, 1, 1},
	{JIT_OP_LGE_UN, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 1},
	{JIT_OP_FEQ,     X86_CC_EQ, X86_CC_EQ, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FNE,     X86_CC_NE, X86_CC_NE, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FLT,     X86_CC_B,  X86_CC_A,  0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FLE,     X86_CC_BE, X86_CC_AE, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FGT,     X86_CC_A,  X86_CC_B , 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FGE,     X86_CC_AE, X86_CC_BE, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FEQ_INV, X86_CC_EQ, X86_CC_EQ, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FNE_INV, X86_CC_NE, X86_CC_NE, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FLT_INV, X86_CC_B,  X86_CC_A,  0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FLE_INV, X86_CC_BE, X86_CC_AE, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FGT_INV, X86_CC_A,  X86_CC_B,  0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_FGE_INV, X86_CC_AE, X86_CC_BE, 0x0, 0x2F, 0, 0, 0,                         1, 0, 0, 1, 1},
	{JIT_OP_DEQ,     X86_CC_EQ, X86_CC_EQ, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DNE,     X86_CC_NE, X86_CC_NE, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DLT,     X86_CC_B,  X86_CC_A,  0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DLE,     X86_CC_BE, X86_CC_AE, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DGT,     X86_CC_A,  X86_CC_B,  0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DGE,     X86_CC_AE, X86_CC_BE, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DEQ_INV, X86_CC_EQ, X86_CC_EQ, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DNE_INV, X86_CC_NE, X86_CC_NE, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DLT_INV, X86_CC_B,  X86_CC_A,  0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DLE_INV, X86_CC_BE, X86_CC_AE, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DGT_INV, X86_CC_A,  X86_CC_B,  0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_DGE_INV, X86_CC_AE, X86_CC_BE, 0x66, 0x2F, 0, 0, 0,                        1, 0, 0, 1, 1},
	{JIT_OP_NFEQ, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_NFNE, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_NFLT, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_NFLE, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_NFGT, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_NFGE, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_NFEQ_INV, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 1},
	{JIT_OP_NFNE_INV, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 1},
	{JIT_OP_NFLT_INV, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 1},
	{JIT_OP_NFLE_INV, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 1},
	{JIT_OP_NFGT_INV, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 1},
	{JIT_OP_NFGE_INV, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 1},
	{JIT_OP_IS_FNAN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_IS_FINF, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_IS_FFINITE, 0, 0, 0, 0, 0, 0, 0,                                           1, 0, 0, 1, 0},
	{JIT_OP_IS_DNAN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_IS_DINF, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_IS_DFINITE, 0, 0, 0, 0, 0, 0, 0,                                           1, 0, 0, 1, 0},
	{JIT_OP_IS_NFNAN, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 0},
	{JIT_OP_IS_NFINF, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 0},
	{JIT_OP_IS_NFFINITE, 0, 0, 0, 0, 0, 0, 0,                                          1, 0, 0, 1, 0},
	{JIT_OP_FACOS, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FASIN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FATAN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FATAN2, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_FCEIL, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FCOS, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_FCOSH, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FEXP, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_FFLOOR, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_FLOG, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_FLOG10, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_FPOW, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_FRINT, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FROUND, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_FSIN, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_FSINH, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FSQRT, 0xF3, 0x51, 0, 0, 0, 0, 0,                                          1, 0, 0, 1, 0},
	{JIT_OP_FTAN,  0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FTANH, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DACOS, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DASIN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DATAN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DATAN2, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_DCEIL, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DCOS, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_DCOSH, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DEXP, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_DFLOOR, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_DLOG, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_DLOG10, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_DPOW, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_DRINT, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DROUND, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_DSIN, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_DSINH, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DSQRT, 0xF2, 0x51, 0, 0, 0, 0, 0,                                          1, 0, 0, 1, 0},
	{JIT_OP_DTAN, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_DTANH, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFACOS, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFASIN, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFATAN, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFATAN2, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_NFCEIL, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFCOS, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFCOSH, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFEXP, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFFLOOR, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_NFLOG, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFLOG10, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_NFPOW, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFRINT, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFROUND, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_NFSIN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFSINH, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFSQRT, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_NFTAN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFTANH, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_IABS, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_LABS, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_FABS, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_DABS, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 0},
	{JIT_OP_NFABS, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_IMIN, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_IMIN_UN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 1},
	{JIT_OP_LMIN, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_LMIN_UN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 1},
	{JIT_OP_FMIN, 0xF3, 0x5D, 0, 0, 0, 0, 0,                                           1, 0, 0, 1, 1},
	{JIT_OP_DMIN, 0xF2, 0x5D, 0, 0, 0, 0, 0,                                           1, 0, 0, 1, 1},
	{JIT_OP_NFMIN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 1},
	{JIT_OP_IMAX, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_IMAX_UN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 1},
	{JIT_OP_LMAX, 0, 0, 0, 0, 0, 0, 0,                                                 1, 0, 0, 1, 1},
	{JIT_OP_LMAX_UN, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 1},
	{JIT_OP_FMAX, 0xF3, 0x5F, 0, 0, 0, 0, 0,                                           1, 0, 0, 1, 1},
	{JIT_OP_DMAX, 0xF2, 0x5F, 0, 0, 0, 0, 0,                                           1, 0, 0, 1, 1},
	{JIT_OP_NFMAX, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 1},
	{JIT_OP_ISIGN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 1},
	{JIT_OP_LSIGN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_FSIGN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_DSIGN, 0, 0, 0, 0, 0, 0, 0,                                                1, 0, 0, 1, 0},
	{JIT_OP_NFSIGN, 0, 0, 0, 0, 0, 0, 0,                                               1, 0, 0, 1, 0},
	{JIT_OP_CHECK_NULL, 0, 0, 0, 0, 1, 0, 0,                                           0, 0, 0, 1, 0},
	{JIT_OP_CALL, 0, 0, 0, 0, 1, 0, 0,                                                 0, 0, 0, 0, 0},
	{JIT_OP_CALL_TAIL, 0, 0, 0, 0, 1, 0, 0,                                            0, 0, 0, 0, 0},
	{JIT_OP_CALL_INDIRECT, 0, 0, 0, 0, 1, 0, 0,                                        0, 0, 0, 0, 0},
	{JIT_OP_CALL_INDIRECT_TAIL, 0, 0, 0, 0, 1, 0, 0,                                   0, 0, 0, 0, 0},
	{JIT_OP_CALL_VTABLE_PTR, 0, 0, 0, 0, 1, 0, 0,                                      0, 0, 0, 0, 0},
	{JIT_OP_CALL_VTABLE_PTR_TAIL, 0, 0, 0, 0, 1, 0, 0,                                 0, 0, 0, 0, 0},
	{JIT_OP_CALL_EXTERNAL, 0, 0, 0, 0, 1, 0, 0,                                        0, 0, 0, 0, 0},
	{JIT_OP_CALL_EXTERNAL_TAIL, 0, 0, 0, 0, 1, 0, 0,                                   0, 0, 0, 0, 0},
	{JIT_OP_RETURN, 0, 0, 0, 0, 1, 0, 0,                                               0, 0, 0, 0, 0},
	{JIT_OP_RETURN_INT, 0, 0, 0, 0, 1, 0, 0,                                           0, 0, 0, 1, 0},
	{JIT_OP_RETURN_LONG, 0, 0, 0, 0, 1, 0, 0,                                          0, 0, 0, 1, 0},
	{JIT_OP_RETURN_FLOAT32, 0, 0, 0, 0, 1, 0, 0,                                       0, 0, 0, 1, 0},
	{JIT_OP_RETURN_FLOAT64, 0, 0, 0, 0, 1, 0, 0,                                       0, 0, 0, 1, 0},
	{JIT_OP_RETURN_NFLOAT, 0, 0, 0, 0, 1, 0, 0,                                        0, 0, 0, 1, 0},
	{JIT_OP_RETURN_SMALL_STRUCT, 0, 0, 0, 0, 1, 0, 0,                                  0, 0, 0, 1, 0},
	{JIT_OP_SETUP_FOR_NESTED, 0, 0, 0, 0, 1, 0, 0,                                     0, 0, 0, 0, 0},
	{JIT_OP_SETUP_FOR_SIBLING, 0, 0, 0, 0, 1, 0, 0,                                    0, 0, 0, 0, 0},
	{JIT_OP_IMPORT, 0, 0, 0, 0, 1, 0, 0,                                               0, 0, 0, 0, 0},
	{JIT_OP_THROW, 0, 0, 0, 0, 1, 0, 0,                                                0, 0, 0, 1, 0},
	{JIT_OP_RETHROW, 0, 0, 0, 0, 0, 0, 0,                                              0, 0, 0, 1, 0},
	{JIT_OP_LOAD_PC, 0, 0, 0, 0, 0, 0, 0,                                              1, 0, 0, 1, 0},
	{JIT_OP_LOAD_EXCEPTION_PC, 0, 0, 0, 0, 0, 0, 0,                                    1, 0, 0, 1, 0},
	{JIT_OP_ENTER_FINALLY, 0, 0, 0, 0, 1, 0, 0,                                        0, 0, 0, 0, 0},
	{JIT_OP_LEAVE_FINALLY, 0, 0, 0, 0, 1, 0, 1,                                        0, 0, 0, 0, 0},
	{JIT_OP_CALL_FINALLY, 0, 0, 0, 0, 1, 0, 1,                                         0, 0, 0, 0, 0},
	{JIT_OP_ENTER_FILTER, 0, 0, 0, 0, 1, 0, 0,                                         0, 0, 0, 0, 0},
	{JIT_OP_LEAVE_FILTER, 0, 0, 0, 0, 1, 0, 0,                                         0, 0, 0, 0, 0},
	{JIT_OP_CALL_FILTER, 0, 0, 0, 0, 1, 0, 0,                                          0, 0, 0, 0, 0},
	{JIT_OP_CALL_FILTER_RETURN, 0, 0, 0, 0, 1, 0, 0,                                   0, 0, 0, 0, 0},
	{JIT_OP_ADDRESS_OF_LABEL, 0, 0, 0, 0, 0, 0, 0,                                     1, 0, 0, 1, 0},
	{JIT_OP_COPY_LOAD_SBYTE, 0, 0, 0, 0, 0, 0, 0,                                      1, 0, 0, 1, 0},
	{JIT_OP_COPY_LOAD_UBYTE, 0, 0, 0, 0, 0, 0, 0,                                      1, 0, 0, 1, 0},
	{JIT_OP_COPY_LOAD_SHORT, 0, 0, 0, 0, 0, 0, 0,                                      1, 0, 0, 1, 0},
	{JIT_OP_COPY_LOAD_USHORT, 0, 0, 0, 0, 0, 0, 0,                                     1, 0, 0, 1, 0},
	{JIT_OP_COPY_INT, 0, 0, 0, 0, 0, 0, 0,                                             1, 0, 0, 1, 0},
	{JIT_OP_COPY_LONG, 0, 0, 0, 0, 0, 0, 0,                                            1, 0, 0, 1, 0},
	{JIT_OP_COPY_FLOAT32, 0, 0, 0, 0, 0, 0, 0,                                         1, 0, 0, 1, 0},
	{JIT_OP_COPY_FLOAT64, 0, 0, 0, 0, 0, 0, 0,                                         1, 0, 0, 1, 0},
	{JIT_OP_COPY_NFLOAT, 0, 0, 0, 0, 0, 0, 0,                                          1, 0, 0, 1, 0},
	{JIT_OP_COPY_STRUCT, 0, 0, 0, 0, 0, 0, 0,                                          1, 0, 0, 1, 0},
	{JIT_OP_COPY_STORE_BYTE, 0, 0, 0, 0, 0, 0, 0,                                      1, 0, 0, 1, 0},
	{JIT_OP_COPY_STORE_SHORT, 0, 0, 0, 0, 0, 0, 0,                                     1, 0, 0, 1, 0},
	{JIT_OP_ADDRESS_OF, 0, 0, 0, 0, 0, 0, 0,                                           1, 0, 0, 1, 0},
	{JIT_OP_INCOMING_REG, 0, 0, 0, 0, 0, 0, 0,                                         0, 0, 0, 0, 0},
	{JIT_OP_INCOMING_FRAME_POSN, 0, 0, 0, 0, 0, 0, 0,                                  0, 0, 0, 0, 0},
	{JIT_OP_OUTGOING_REG, 0, 0, 0, 0, 1, 0, 0,                                         0, 0, 0, 1, 0},
	{JIT_OP_OUTGOING_FRAME_POSN, 0, 0, 0, 0, 1, 0, 0,                                  0, 0, 0, 1, 0},
	{JIT_OP_RETURN_REG, 0, 0, 0, 0, 1, 0, 0,                                           0, 0, 1, 0, 0},
	{JIT_OP_PUSH_INT, 0, 0, 0, 0, 1, 0, 0,                                             0, 0, 0, 1, 0},
	{JIT_OP_PUSH_LONG, 0, 0, 0, 0, 1, 0, 0,                                            0, 0, 0, 1, 0},
	{JIT_OP_PUSH_FLOAT32, 0, 0, 0, 0, 1, 0, 0,                                         0, 0, 0, 1, 0},
	{JIT_OP_PUSH_FLOAT64, 0, 0, 0, 0, 1, 0, 0,                                         0, 0, 0, 1, 0},
	{JIT_OP_PUSH_NFLOAT, 0, 0, 0, 0, 1, 0, 0,                                          0, 0, 0, 1, 0},
	{JIT_OP_PUSH_STRUCT, 0, 0, 0, 0, 1, 0, 0,                                          0, 0, 0, 1, 0},
	{JIT_OP_POP_STACK, 0, 0, 0, 0, 1, 0, 0,                                            0, 0, 0, 0, 0},
	{JIT_OP_FLUSH_SMALL_STRUCT, 0, 0, 0, 0, 1, 0, 0,                                   0, 0, 0, 1, 0},
	{JIT_OP_SET_PARAM_INT, 0, 0, 0, 0, 1, 0, 0,                                        0, 0, 0, 1, 0},
	{JIT_OP_SET_PARAM_LONG, 0, 0, 0, 0, 1, 0, 0,                                       0, 0, 0, 1, 0},
	{JIT_OP_SET_PARAM_FLOAT32, 0, 0, 0, 0, 1, 0, 0,                                    0, 0, 0, 1, 0},
	{JIT_OP_SET_PARAM_FLOAT64, 0, 0, 0, 0, 1, 0, 0,                                    0, 0, 0, 1, 0},
	{JIT_OP_SET_PARAM_NFLOAT, 0, 0, 0, 0, 1, 0, 0,                                     0, 0, 0, 1, 0},
	{JIT_OP_SET_PARAM_STRUCT, 0, 0, 0, 0, 1, 0, 0,                                     0, 0, 0, 1, 0},
	{JIT_OP_PUSH_RETURN_AREA_PTR, 0, 0, 0, 0, 1, 0, 0,                                 0, 0, 0, 1, 0},
	{JIT_OP_LOAD_RELATIVE_SBYTE, 0, 0, 0, 0, 0, 0, 0,                                  1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_UBYTE, 0, 0, 0, 0, 0, 0, 0,                                  1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_SHORT, 0, 0, 0, 0, 0, 0, 0,                                  1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_USHORT, 0, 0, 0, 0, 0, 0, 0,                                 1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_INT, 0, 0, 0, 0, 0, 0, 0,                                    1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_LONG, 0, 0, 0, 0, 0, 0, 0,                                   1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_FLOAT32, 0, 0, 0, 0, 0, 0, 0,                                1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_FLOAT64, 0, 0, 0, 0, 0, 0, 0,                                1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_NFLOAT, 0, 0, 0, 0, 0, 0, 0,                                 1, 0, 0, 1, 1},
	{JIT_OP_LOAD_RELATIVE_STRUCT, 0, 0, 0, 0, 0, 0, 0,                                 1, 0, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_BYTE, 0, 0, 0, 0, 1, 0, 0,                                  0, 1, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_SHORT, 0, 0, 0, 0, 1, 0, 0,                                 0, 1, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_INT, 0, 0, 0, 0, 1, 0, 0,                                   0, 1, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_LONG, 0, 0, 0, 0, 1, 0, 0,                                  0, 1, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_FLOAT32, 0, 0, 0, 0, 1, 0, 0,                               0, 1, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_FLOAT64, 0, 0, 0, 0, 1, 0, 0,                               0, 1, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_NFLOAT, 0, 0, 0, 0, 1, 0, 0,                                0, 1, 0, 1, 1},
	{JIT_OP_STORE_RELATIVE_STRUCT, 0, 0, 0, 0, 1, 0, 0,                                0, 1, 0, 1, 1},
	{JIT_OP_ADD_RELATIVE, X86_ADD, 0, 0, 0, 0, 0, 0,                                   1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_SBYTE, 0, 0, 0, 0, 0, 0, 0,                                   1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_UBYTE, 0, 0, 0, 0, 0, 0, 0,                                   1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_SHORT, 0, 0, 0, 0, 0, 0, 0,                                   1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_USHORT, 0, 0, 0, 0, 0, 0, 0,                                  1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_INT, 0, 0, 0, 0, 0, 0, 0,                                     1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_LONG, 0, 0, 0, 0, 0, 0, 0,                                    1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_FLOAT32, 0, 0, 0, 0, 0, 0, 0,                                 1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_FLOAT64, 0, 0, 0, 0, 0, 0, 0,                                 1, 0, 0, 1, 1},
	{JIT_OP_LOAD_ELEMENT_NFLOAT, 0, 0, 0, 0, 0, 0, 0,                                  1, 0, 0, 1, 1},
	{JIT_OP_STORE_ELEMENT_BYTE, 0, 0, 0, 0, 1, 0, 0,                                   0, 1, 0, 1, 1},
	{JIT_OP_STORE_ELEMENT_SHORT, 0, 0, 0, 0, 1, 0, 0,                                  0, 1, 0, 1, 1},
	{JIT_OP_STORE_ELEMENT_INT, 0, 0, 0, 0, 1, 0, 0,                                    0, 1, 0, 1, 1},
	{JIT_OP_STORE_ELEMENT_LONG, 0, 0, 0, 0, 1, 0, 0,                                   0, 1, 0, 1, 1},
	{JIT_OP_STORE_ELEMENT_FLOAT32, 0, 0, 0, 0, 1, 0, 0,                                0, 1, 0, 1, 1},
	{JIT_OP_STORE_ELEMENT_FLOAT64, 0, 0, 0, 0, 1, 0, 0,                                0, 1, 0, 1, 1},
	{JIT_OP_STORE_ELEMENT_NFLOAT, 0, 0, 0, 0, 1, 0, 0,                                 0, 1, 0, 1, 1},
	{JIT_OP_MEMCPY, 0, 0, 0, 0, 1, 0, 0,                                               0, 0, 0, 1, 1},
	{JIT_OP_MEMMOVE, 0, 0, 0, 0, 1, 0, 0,                                              0, 0, 0, 1, 1},
	{JIT_OP_MEMSET, 0, 0, 0, 0, 1, 0, 0,                                               0, 0, 0, 1, 1},
	{JIT_OP_ALLOCA, 0, 0, 0, 0, 1, 0, 0,                                               0, 0, 0, 1, 0},
	{JIT_OP_MARK_OFFSET, 0, 0, 0, 0, 1, 0, 0,                                          0, 0, 0, 0, 0},
	{JIT_OP_MARK_BREAKPOINT, 0, 0, 0, 0, 1, 1, 0,                                      0, 0, 0, 0, 0},
	{JIT_OP_JUMP_TABLE, 0, 0, 0, 0, 0, 0, 1,                                           0, 0, 0, 1, 0},
	{JIT_OP_TAIL_CALL, 0, 0, 0, 0, 1, 0, 0,                                            0, 0, 0, 0, 0}
	};

struct _incoming_params_state
{
    unsigned char gp_index;
    unsigned char mmx_index;
    unsigned char xmm_index;
    unsigned char frame_index;
};

typedef struct _incoming_params_state *incoming_params_state_t;

void jite_allocate_large_frame(jit_function_t func, jite_frame_t frame, int size);

void jite_allocate_large_frame_cond6(jit_function_t func, jite_frame_t frame, int size, jite_frame_t frame1, jite_frame_t frame2, jite_frame_t frame3, jite_frame_t frame4, jite_frame_t frame5, jite_frame_t frame6);

void jite_allocate_frame(jit_function_t func, jite_frame_t frame);

void jite_occupy_frame(jit_function_t func, jite_frame_t frame);

unsigned char jite_gp_reg_is_free(jit_function_t func, int index);

unsigned char jite_xmm_reg_is_free(jit_function_t func, int index);

unsigned char jite_mmx_reg_is_free(jit_function_t func, int index);

unsigned char jite_gp_reg_index_is_free(void *handler, int index);

unsigned char jite_xmm_reg_index_is_free(void *handler, int index);

unsigned char jite_mmx_reg_index_is_free(void *handler, int index);

unsigned char jite_register_is_free(jit_function_t func, int reg, jit_value_t value);

unsigned char jite_regIndex_is_free(jit_function_t func, int regIndex, jit_value_t value);

void gen_start_block(jit_gencode_t gen, jit_block_t block);

void *gen_prolog(jit_gencode_t gen, jit_function_t func, void *buf);

unsigned char *restore_callee_saved_registers(unsigned char *inst, jit_function_t func);

void gen_epilog(jit_gencode_t gen, jit_function_t func);

void gen_start_block(jit_gencode_t gen, jit_block_t block);

void gen_end_block(jit_gencode_t gen, jit_block_t block);

void jite_compile_block(jit_gencode_t gen, jit_function_t func, jit_block_t block);

unsigned char *jite_jump_to_epilog
    (jit_gencode_t gen, unsigned char *inst, jit_block_t block);

unsigned char *jite_throw_builtin
        (unsigned char *inst, jit_function_t func, int type);

unsigned int jite_stack_depth_used(jit_function_t func);

void jite_compute_registers_holes(jit_function_t func);

unsigned char jite_vreg_is_in_register_liveness(jit_function_t func, jite_vreg_t vreg, unsigned int regIndex);

unsigned char jite_vreg_is_in_register_liveness_ignore_vreg(jit_function_t func, jite_vreg_t vreg, unsigned int regIndex, jite_vreg_t ignoreVreg);

unsigned char jite_vreg_is_in_register_hole(jit_function_t func, jite_vreg_t vreg, unsigned int regIndex);

unsigned int jite_register_pair(unsigned int reg);

unsigned int jite_index_register_pair(unsigned int index);

jite_reg_t jite_object_register_pair(jite_reg_t reg);

unsigned char *jite_allocate_local_register(unsigned char *inst, jit_function_t func, jite_vreg_t value, jite_vreg_t value1, jite_vreg_t value2, unsigned char bUsage, unsigned int fRegCond, int typeKind, unsigned int *regFound);

int jite_x86reg_to_reg(int reg);

#endif
