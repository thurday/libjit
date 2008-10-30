#include <stdio.h>

struct int128
{
    jit_long high;
    jit_ulong low;
};

struct int128 shift_left(struct int128 value, jit_uint offset)
{
	struct int128 result;
	result.low = 0;
	result.high = 0;
	jit_ulong index = 0;
	for(index = 0; index < (128 - offset); index++)
	{
	    if((index<=63 && (value.low & ((jit_ulong)1 << index)))
	     || (index>=64 && (value.high & ((jit_ulong)1 << (index - 64)))))
	    {
		if(((index + offset) >= 0) && ((index + offset) <= 63))
		{
			result.low |= ((jit_ulong)1 << (index + offset));
		}
		else if(((index + offset) >= 64) && ((index + offset) <= 127))
		{
			result.high |= ((jit_ulong)1 << (index + offset - 64));
		}
	    }
	}

	return result;
}


struct int128 shift_right(struct int128 value, jit_uint offset)
{
	struct int128 result;
	result.low = 0;
	result.high = 0;
	jit_ulong index = 0;
	for(index = offset; index < 128; index++)
	{
	    if((index<=63 && (value.low & ((jit_ulong)1 << index)))
		|| (index>=64 && (value.high & ((jit_ulong)1 << (index - 64)))))
	    {
		if(((index - offset) >= 0) && ((index - offset) <= 63))
		{
			result.low |= ((jit_ulong)1 << (index - offset));
		}
		else if(((index - offset) >= 64) && ((index - offset) <= 127))
		{
			result.high |= ((jit_ulong)1 << (index - offset - 64));
		}
	    }
	}

	return result;
}


struct int128 add(struct int128 value1, struct int128 value2)
{
	// two's complement arithmetics
	struct int128 result = value1;
	int add_one = 0;
	if((value1.low + value2.low) <= value1.low) add_one = 1;
	result.low += value2.low;
	result.high += value2.high;
	result.high += add_one;
	return result;
}


struct int128 sub(struct int128 value1, struct int128 value2)
{
	struct int128 result = value1;
	int add_one = 0;

	if((value1.high >= 0 && value2.high >= 0) ||
	    (value1.high < 0 && value2.high < 0))
	{
		
	}
	else if(value1.high < 0)
	{
		if ((value1.high - value2.high) <= value1.high)
		{
			add_one = 1;
		}
	}
	else
	{
		if ((value1.high - value2.high) >= value1.high)
		{
			add_one = 1;
		}
	}

	result.low -= value2.low;
	result.high -= value2.high;
	result.high -= add_one;
	return result;
}


struct int128 or(struct int128 value1, struct int128 value2)
{	
	struct int128 value;
	value.high = value1.high | value2.high;
	value.low = value1.low | value2.low;	
	return value;
}

struct int128 and(struct int128 value1, struct int128 value2)
{	
	struct int128 value;
	value.high = value1.high & value2.high;
	value.low = value1.low & value2.low;	
	return value;
}


struct int128 xor(struct int128 value1, struct int128 value2)
{
	struct int128 value;
	value.high = value1.high ^ value2.high;
	value.low = value1.low ^ value2.low;
	return value;
}


struct int128 not(struct int128 value1)
{
	struct int128 value;
	value.high = ~value1.high;
	value.low = ~value1.low;
	return value;
}


struct int128 neg(struct int128 value1)
{
	struct int128 const_1;
	const_1.low = 1;
	const_1.high = 0;
	if(value1.high < 0)
	{
		struct int128 value = not(value1);
		value1 = add(value, const_1);
	}
	else if(value1.high >=0)
	{
		struct int128 value = not(value1);
		value1 = add(value, const_1);
	}
	return value1;
}


struct int128 abs_int128(struct int128 value1)
{
	struct int128 const_1;
	const_1.low = 1;
	const_1.high = 0;
	if(value1.high < 0)
	{
		struct int128 value = not(value1);
		value1 = add(value, const_1);
	}
	return value1;
}


int cmp(struct int128 value1, struct int128 value2)
{
	// is boolean in fact. TODO
	if(value1.high > value2.high) { return 1; }
	if(value1.high < value2.high) { return -1; }
	if(value1.low > value2.low) { return 1; }
	if(value1.low < value2.low) { return -1; }
	return 0;
}


struct int128 div_un(struct int128 d, struct int128 n)
{
	struct int128 q = d;
	struct int128 r;
	struct int128 const_1;
	const_1.low = 1;
	const_1.high = 0;
	r.low = 0;
	r.high = 0;
	int i = 0;

	struct int128 const_high = shift_left(const_1, 127);
	for(i = 1; i <= 128; i++)
	{
		jit_ulong temp = q.high & const_high.high;
		q = shift_left(q, 1);
		r = shift_left(r, 1);
		if(temp)
		{
			r = or(r, const_1);
		}

		if(cmp(r, n) >= 0)
		{
			r = sub(r, n);
			q = add(q, const_1);
		}
	}
	return q;
}


struct int128 div_int128_int128(struct int128 d, struct int128 n)
{
	struct int128 d_m = abs_int128(d);
	struct int128 n_m = abs_int128(n);
	struct int128 q = div_un(d_m, n_m);
	if ((d.high < 0 && n.high > 0) || (d.high > 0 && n.high < 0))
	{
		q = neg(q);
	}
	return q;
}

int log2int128(struct int128 value)
{
	int result = 0;
	int index = 1;

	int num_of_bits = 0;
	for(index = 0; index < 128; index++)
	{
		if ((index <=63 && (value.low & ((jit_ulong)1 << index)))
			|| (index >=64 && (value.high & ((jit_ulong)1 << (index - 64)))))
		{ 
			result = index; num_of_bits++;
		}
	}
	if(num_of_bits != 1) result+=1;
	return result;
}

jit_uint log2llui(jit_ulong value)
{
	jit_uint result = 0;
	jit_ulong index = 1, offset = 1;
	jit_uint num_of_bits = 0;
	for(index = 0; index < 64; index++)
	{
		if(value & offset) { result = index; num_of_bits++;}
		offset <<=1;
	}
	if(num_of_bits != 1) result+=1;
	return result;
}

jit_uint log2ui(jit_uint value)
{
	jit_uint result = 0;
	jit_uint index = 1, offset = 1;
	jit_uint num_of_bits = 0;
	for(index = 0; index < 32; index++)
	{
		if(value & offset) { result = index; num_of_bits++;}
		offset <<=1;
	}
	if(num_of_bits != 1) result+=1;
	return result;
}

struct int128 create_int128_from_llui(jit_ulong value)
{
	struct int128 temp;
	temp.low = value;
	temp.high = 0;
	return temp;
}

struct int128 create_int128_from_lli(jit_long value)
{
	struct int128 temp;
	if(value < 0)
	{
		value = value & ~((jit_ulong)1 << 63);
		temp.high = ((jit_ulong)1 << 63);
		temp.low = value;
	}
	else
	{
		temp.high = 0;
		temp.low = value;
	}
	return temp;
}

// Count params according to divcnst-pldi94.pdf by Torbjorn Granlund and Peter L. Montgomery
// "Division By Invariant Integers using Multiplication"
void count_64bit_reciprocal_params(jit_ulong d, 
	jit_uint *sh_pre,
	jit_uint *sh_post, struct int128 *m, int *l)
{
	struct int128 const_1;
	const_1.high = 0;
	const_1.low = 1;

	jit_uint e, l_dummy;
	jit_uint prec;
	prec = 64;

	*l = log2llui(d);
	*sh_post = *l;
	struct int128 m_low;
	struct int128 m_high;

	m_low = div_un(shift_left(const_1, 64 + *l), create_int128_from_llui(d));

	m_high = div_un(add(shift_left(const_1, 64 + *l), shift_left(const_1, 64 + *l - prec)),
			create_int128_from_llui(d));

	while((cmp(shift_right(m_low, 1),  shift_right(m_high, 1)) < 0) && (*sh_post > 0))
	{
		m_low = shift_right(m_low, 1);
		m_high = shift_right(m_high, 1);
		*sh_post = *sh_post - 1;
	}
	*m = m_high;

	if((cmp(*m, shift_left(const_1, 64)) >= 0)
	&& ((d | 1) != d))
	{
		struct int128 temp = and(sub(shift_left(const_1, 64), create_int128_from_llui(d)), create_int128_from_llui(d));
		e = log2int128(temp);
		*sh_pre = e;
		struct int128 d_odd = div_un(create_int128_from_llui(d), temp);

		l_dummy = log2int128(d_odd);
		*sh_post = l_dummy;

		prec = 64 - e;
		m_low = div_un(shift_left(const_1, 64 + l_dummy), d_odd);
		m_high = div_un(add(shift_left(const_1, 64 + l_dummy), shift_left(const_1, 64 + l_dummy - prec)),
			    d_odd);

		while((cmp(shift_right(m_low, 1), shift_right(m_high, 1)) < 0) && (*sh_post > 0))
		{
			m_low = shift_right(m_low, 1);
			m_high = shift_right(m_high, 1);
			*sh_post = *sh_post - 1;
		}
		*m = m_high;
	}
	else
	{
		*sh_pre = 0;
	}
}


void count_32bit_reciprocal_params(jit_uint d,
	jit_uint *sh_pre,
	jit_uint *sh_post, jit_ulong *m, jit_ulong *l)
{
	jit_uint e, l_dummy;
	jit_uint prec;
	prec = 32;

	*l = log2ui(d);
	*sh_post = *l;
	jit_ulong m_low;
	jit_ulong m_high;

	m_low = (((jit_ulong)1 << (32 + *l))) / d;

	m_high = (((jit_ulong)1 << (32 + *l)) + (((jit_ulong)1 << (32 + *l - prec)))) / d;

	while(((m_low >> 1) < (m_high >> 1)) && (*sh_post > 0))
	{
		m_low = (m_low >> 1);
		m_high = (m_high >> 1);
		*sh_post = *sh_post - 1;
	}
	*m = m_high;

	if(((*m >= (jit_ulong)1 << 32))
	&& ((d | 1) != d))
	{
		jit_ulong temp = (((((jit_ulong)1 << 32) - d)) & d);
		e = log2llui(temp);
		*sh_pre = e;
		jit_ulong d_odd = d / temp;

		l_dummy = log2llui(d_odd);
		*sh_post = l_dummy;

		prec = 32 - e;
		m_low = (((jit_ulong)1 << (32 + l_dummy)) / d_odd);
		m_high = (((((jit_ulong)1 << (32 + l_dummy)) + ((jit_ulong)1 << (32 + l_dummy - prec)))) / d_odd);

		while(((m_low >> 1) < (m_high >> 1)) && (*sh_post > 0))
		{
			m_low = (m_low >> 1);
			m_high = (m_high >> 1);
			*sh_post = *sh_post - 1;
		}
		*m = m_high;
	}
	else
	{
		*sh_pre = 0;
	}
}


void count_32bit_reciprocal_params_signed(int d,
	jit_uint *sh_post, jit_ulong *m, jit_long *l)
{
	jit_uint prec;
	prec = 31;

	*l = log2ui(d);
	*sh_post = *l;
	jit_ulong m_low;
	jit_ulong m_high;

	m_low = (((jit_ulong)1 << (32 + *l))) / d;

	m_high = (((jit_ulong)1 << (32 + *l)) + (((jit_ulong)1 << (32 + *l - prec)))) / d;

	while(((m_low >> 1) < (m_high >> 1)) && (*sh_post > 0))
	{
		m_low = (m_low >> 1);
		m_high = (m_high >> 1);
		*sh_post = *sh_post - 1;
	}
	*m = m_high;
}

