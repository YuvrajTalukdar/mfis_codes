#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void bin_print64(unsigned long long int v, unsigned long long int print_len)
{
	unsigned long long int shift_cnt, mask;
	shift_cnt = print_len - 1;
	mask = 1ULL << shift_cnt;
	while(mask)
	{
		printf("%llu", ((v & mask) >> shift_cnt));
		mask = mask >> 1;
		shift_cnt--;
	}
}

unsigned int majority_fn(unsigned int b1, unsigned int b2, unsigned int b3)
{
	return ((b1 & b2) ^ (b2 & b3) ^ (b3 ^ b1));
}

unsigned int exct_bit(unsigned int *lfsr, unsigned int bit_no)
{
	return ((((1 << bit_no) & *lfsr) >> bit_no)  % 2);
}

unsigned int lfsr1_fn(unsigned int clk_bit, unsigned int init_bit, unsigned int *lfsr)
{
	unsigned int i, cnst =  0x72000, lfsrlsb;
	if(*lfsr == 0)
		*lfsr = 1;
	if(clk_bit)
	{
		lfsrlsb = 0;
		/*printf("lfsr1 = %0x\n", *lfsr);
		bin_print64(*lfsr, 19);
		printf("\n");*/
		*lfsr ^= init_bit;
		for(i = 0; i < 19; i++)
		{
			lfsrlsb ^= (((cnst >> i) % 2) ? ((*lfsr >> i) % 2) : 0);
		}
		*lfsr = (*lfsr << 1) ^ lfsrlsb;
		//printf("lfsr1 = ");
		//bin_print64(*lfsr, 19);
		//printf("\n");
		return exct_bit(lfsr,19);
	}
	return exct_bit(lfsr,18);
}

unsigned int lfsr2_fn(unsigned int clk_bit, unsigned int init_bit, unsigned int *lfsr)
{
	unsigned int i, cnst =  0x300000, lfsrlsb;
	if(*lfsr == 0)
		*lfsr = 1;
	if(clk_bit)
	{
		lfsrlsb = 0;
		/*printf("lfsr1 = %0x\n", *lfsr);
		bin_print64(*lfsr, 22);
		printf("\n");*/
		*lfsr ^= init_bit;
		for(i = 0; i < 22; i++)
		{
			lfsrlsb ^= (((cnst >> i) % 2) ? ((*lfsr >> i) % 2) : 0);
		}
		*lfsr = (*lfsr << 1) ^ lfsrlsb;
		//printf("lfsr1 = ");
		//bin_print64(*lfsr, 22);
		//printf("\n");
		return exct_bit(lfsr,22);
	}
	return exct_bit(lfsr,21);
}

unsigned int lfsr3_fn(unsigned int clk_bit, unsigned int init_bit, unsigned int *lfsr)
{
	unsigned int i, cnst =  0x700080, lfsrlsb;
	if(*lfsr == 0)
		*lfsr = 1;
	if(clk_bit)
	{
		lfsrlsb = 0;
		/*printf("lfsr1 = %0x\n", *lfsr);
		bin_print64(*lfsr, 23);
		printf("\n");*/
		*lfsr ^= init_bit;
		for(i = 0; i < 23; i++)
		{
			lfsrlsb ^= (((cnst >> i) % 2) ? ((*lfsr >> i) % 2) : 0);
		}
		*lfsr = (*lfsr << 1) ^ lfsrlsb;
		//printf("lfsr1 = ");
		//bin_print64(*lfsr, 23);
		//printf("\n");
		return exct_bit(lfsr,23);
	}
	return exct_bit(lfsr,22);
}

unsigned int lfsr4_fn(unsigned int clk_bit, unsigned int init_bit, unsigned int *lfsr)
{
	unsigned int i, cnst =  0x10400, lfsrlsb;
	if(*lfsr == 0)
		*lfsr = 1;
	if(clk_bit)
	{
		lfsrlsb = 0;
		/*printf("lfsr1 = %0x\n", *lfsr);
		bin_print64(*lfsr, 17);
		printf("\n");*/
		*lfsr ^= init_bit;
		for(i = 0; i < 17; i++)
		{
			lfsrlsb ^= (((cnst >> i) % 2) ? ((*lfsr >> i) % 2) : 0);
		}
		*lfsr = (*lfsr << 1) ^ lfsrlsb;
		//printf("lfsr1 = ");
		//bin_print64(*lfsr, 17);
		//printf("\n");
		return exct_bit(lfsr,17);
	}
	return exct_bit(lfsr,16);
}

void init_fn(unsigned *r1, unsigned *r2, unsigned *r3, unsigned *r4)
{
	unsigned long long i, key = 0x1234567890, iv  = 0x123456;
	for(i = 0; i < 64; i++)
	{
		lfsr1_fn(1, key % 2, r1);
		lfsr2_fn(1, key % 2, r2);
		lfsr3_fn(1, key % 2, r3);
		lfsr4_fn(1, key % 2, r4);
		key = key >> 1ULL;
	}
	for(i = 0; i < 22; i++)
	{
		lfsr1_fn(1, iv % 2, r1);
		lfsr2_fn(1, iv % 2, r2);
		lfsr3_fn(1, iv % 2, r3);
		lfsr4_fn(1, iv % 2, r4);
		iv = iv >> 1ULL;
	}
}

void init_enc(unsigned *r1, unsigned *r2, unsigned *r3, unsigned *r4)
{
	int i;
	unsigned int mj_bit, r1_bit, r2_bit, r3_bit;
	for(i = 0; i < 99; i++)
	{
		r1_bit = 0;
		r2_bit = 0;
		r3_bit = 0;
		mj_bit = majority_fn(exct_bit(r4,3), exct_bit(r4,7), exct_bit(r4,10));
		if(mj_bit == exct_bit(r4,10))
			r1_bit = lfsr1_fn(1, 0, r1);
		if(mj_bit == exct_bit(r4,3))
			r2_bit = lfsr2_fn(1, 0, r2);
		if(mj_bit == exct_bit(r4,7))
			r3_bit = lfsr3_fn(1, 0, r3);
		lfsr4_fn(1, 0, r4);
	}
}

void encrypt(unsigned *r1, unsigned *r2, unsigned *r3, unsigned *r4)
{
	static unsigned int mj_bit, r1_bit, r2_bit, r3_bit;
	mj_bit = majority_fn(exct_bit(r4,3), exct_bit(r4,7), exct_bit(r4,10));
	if(mj_bit == exct_bit(r4,10))
	{
		r1_bit = lfsr1_fn(1, 0, r1);
		r1_bit ^= majority_fn(exct_bit(r1,12), !exct_bit(r1,14), exct_bit(r1,15));
	}
	if(mj_bit == exct_bit(r4,3))
	{
		r2_bit = lfsr2_fn(1, 0, r2);
		r2_bit ^= majority_fn(exct_bit(r2,9), exct_bit(r2,13), !exct_bit(r2,16));
	}
	if(mj_bit == exct_bit(r4,7))
	{
		r3_bit = lfsr3_fn(1, 0, r3);
		r3_bit ^= majority_fn(!exct_bit(r3,13), exct_bit(r3,16), exct_bit(r3,18));
	}
	lfsr4_fn(1, 0, r4);
	printf("%u", r1_bit ^ r2_bit ^ r3_bit);
}

void a5by2_enc()
{
	unsigned int i, j, k, *r1 = 0, *r2 = 0, *r3 = 0, *r4 = 0;
	
	r1 = (unsigned int *)malloc(sizeof(unsigned int));
	r2 = (unsigned int *)malloc(sizeof(unsigned int));
	r3 = (unsigned int *)malloc(sizeof(unsigned int));
	r4 = (unsigned int *)malloc(sizeof(unsigned int));
	
	init_fn(r1, r2, r3, r4);
	init_enc(r1, r2, r3, r4);
	for(i = 0; i < 114; i++)
	{
		encrypt(r1, r2, r3, r4);
	}
	printf("\n");
}

int main()
{
	a5by2_enc();
	return 0;
}
