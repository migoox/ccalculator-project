#include "ansnum.h"

void ANS_increment(ANS_Num* num)
{
	int carry = 0;
	int i = 0;
	while (num->size > i)
	{
		int digit1 = 0;
		int digit2 = 0;

		if (num->size > i)
			digit1 = ANS_chr_toint(num->string[i]);
		if (i == 0)
			digit2 = 1;

		int new_digit = (digit1 + digit2 + carry) % num->numeral_system;
		carry = (digit1 + digit2 + carry) / num->numeral_system;

		if (num->size > i)
			ANS_setat(num, i, ANS_int_tochr(new_digit));
		else
			ANS_push_front(num, ANS_int_tochr(new_digit));

		i++;
	}

	if (carry > 0)
	{
		if (num->size > i + 1)
			ANS_setat(num, i, ANS_int_tochr(carry));
		else
			ANS_push_front(num, ANS_int_tochr(carry));
	}
}

void ANS_sum_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_sum error: cannot add numbers in different numeral systems.");

	if (container->size > 0)
		ANS_delete(container);

	size_t new_size;
	if (num1->size > num2->size)
		new_size = num1->size;
	else
		new_size = num2->size;

	ANS_init_cap(container, new_size, num1->numeral_system);

	int carry = 0;
	int i = 0;
	while (num1->size > i || num2->size > i)
	{
		int digit1 = 0;
		int digit2 = 0;

		if (num1->size > i)
			digit1 = ANS_chr_toint(num1->string[i]);
		if (num2->size > i)
			digit2 = ANS_chr_toint(num2->string[i]);

		int new_digit = (digit1 + digit2 + carry) % num1->numeral_system;
		carry = (digit1 + digit2 + carry) / num1->numeral_system;

		ANS_push_front(container, ANS_int_tochr(new_digit));
		i++;
	}
	if (carry > 0)
		ANS_push_front(container, ANS_int_tochr(carry));
}

void ANS_sum(ANS_Num* num1, ANS_Num* num2)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_sum error: cannot add numbers in different numeral systems.");

	size_t new_size;
	if (num1->size > num2->size)
		new_size = num1->size;
	else
		new_size = num2->size;

	int carry = 0;
	int i = 0;
	while (num1->size > i || num2->size > i)
	{
		int digit1 = 0;
		int digit2 = 0;

		if (num1->size > i)
			digit1 = ANS_chr_toint(num1->string[i]);
		if (num2->size > i)
			digit2 = ANS_chr_toint(num2->string[i]);

		int new_digit = (digit1 + digit2 + carry) % num1->numeral_system;
		carry = (digit1 + digit2 + carry) / num1->numeral_system;

		if (num1->size > i)
			ANS_setat(num1, i, ANS_int_tochr(new_digit));
		else
			ANS_push_front(num1, ANS_int_tochr(new_digit));

		i++;
	}

	if (carry > 0)
	{
		if (num1->size > i + 1)
			ANS_setat(num1, i, ANS_int_tochr(carry));
		else
			ANS_push_front(num1, ANS_int_tochr(carry));
	}
}

void ANS_mult_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_sum error: cannot add numbers in different numeral systems.");

	if (container->size > 0)
		ANS_delete(container);

	ANS_Num* long_num;
	ANS_Num* short_num;

	size_t new_size = num2->size;

	if (num1->size > num2->size)
	{
		new_size = num1->size * 2;
		long_num = num1;
		short_num = num2;
	}
	else
	{
		new_size = num2->size * 2;
		long_num = num2;
		short_num = num1;
	}

	ANS_init_cap(container, new_size, num1->numeral_system);

	for (int i = 0; i < short_num->size; i++)
	{
		int carry = 0;
		int sum_carry = 0;
		int short_digit = ANS_chr_toint(short_num->string[i]);

		for (int j = 0; j < long_num->size; j++)
		{
			int long_digit = ANS_chr_toint(long_num->string[j]);

			int new_digit = (short_digit * long_digit + carry) % num1->numeral_system;
			carry = (short_digit * long_digit + carry) / num1->numeral_system;

			if (i == 0)
			{
				ANS_push_front(container, ANS_int_tochr(new_digit));
			}
			else
			{
				int digit1 = 0;
				int digit2 = new_digit;

				if (i + j < container->size)
					digit1 = ANS_chr_toint(ANS_getat(container, i + j));

				int new_digit_sum = (digit1 + digit2 + sum_carry) % num1->numeral_system;
				sum_carry = (digit1 + digit2 + sum_carry) / num1->numeral_system;

				if (i + j < container->size)
					ANS_setat(container, i + j, ANS_int_tochr(new_digit_sum));
				else
					ANS_push_front(container, ANS_int_tochr(new_digit_sum));
			}
		}
		int new_carry = carry + sum_carry;
		if (new_carry > 0)
		{
			if (i == 0)
			{
				ANS_push_front(container, ANS_int_tochr(carry));
			}
			else
			{
				int x = 0;
				do
				{
					int new_digit = new_carry % num1->numeral_system;

					if (i + long_num->size + x < container->size)
						ANS_setat(container, i + long_num->size + x, ANS_int_tochr(new_digit));
					else
						ANS_push_front(container, ANS_int_tochr(new_digit));

					new_carry /= num1->numeral_system;

					x++;
				} while (new_carry != 0);

			}
		}
	}
}

void ANS_pow_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_sum error: cannot add numbers in different numeral systems.");

	if (container->size > 0)
		ANS_delete(container);

	ANS_Num inum;
	ANS_init_str(&inum, "0", num1->numeral_system);

	ANS_Num helper;
	ANS_init_blank(&helper);

	ANS_cpy(num1, container);
	ANS_increment(&inum);
	while (!ANS_is_equal(&inum, num2))
	{
		ANS_mult_withc(num1, container, &helper);
		ANS_cpy(&helper, container);

		ANS_clear(&helper);
		helper.size = 0;

		ANS_increment(&inum);
	}

	ANS_delete(&inum);
	ANS_delete(&helper);
}

void ANS_vdivide_withc(ANS_Num* num1, unsigned long long value, ANS_Num* container)
{

}

void ANS_divide_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{

}


