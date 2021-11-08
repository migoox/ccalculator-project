#include "ansnum.h"
#include "ansoperations.h"

void ANS_increment(ANS_Num* num)
{
	int carry = 0;
	int i = 0;
	int digit1 = 0;
	int digit2 = 0;
	int new_digit;

	while (num->size > i)
	{
		// prepare two digits
		digit1 = 0;
		digit2 = 0;

		if (num->size > i)
			digit1 = ANS_chr_toint(num->string[i]);
		if (i == 0)
			digit2 = 1;

		// create new digit
		new_digit = (digit1 + digit2 + carry) % num->numeral_system;
		carry = (digit1 + digit2 + carry) / num->numeral_system;

		// set digit
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

	int carry = 0;
	int i = 0;
	int digit1 = 0;
	int digit2 = 0;
	size_t new_size;

	if (container->size > 0)
		ANS_delete(container);

	if (num1->size > num2->size)
		new_size = num1->size;
	else
		new_size = num2->size;

	ANS_init_cap(container, new_size, num1->numeral_system);

	while (num1->size > i || num2->size > i)
	{
		digit1 = 0;
		digit2 = 0;

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

	ANS_Num* long_num;
	ANS_Num* short_num;
	int carry;
	int sum_carry;

	int short_digit;
	int long_digit;

	int new_digit;

	int sum_digit1;
	int sum_digit2;
	int new_digit_sum;

	int new_carry;

	int x;

	size_t new_size = num2->size;

	if (container->size > 0)
		ANS_delete(container);

	if (num2->size == 1 && num2->string[0] == '0')
	{
		// multiplication by zero returns 0
		ANS_init_str(container, "0", num1->numeral_system);
		return;
	}

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

	// prepare container(allocate memory)
	ANS_init_cap(container, new_size, num1->numeral_system);

	// main loop
	for (int i = 0; i < short_num->size; i++)
	{
		carry = 0;
		sum_carry = 0;
		short_digit = ANS_chr_toint(short_num->string[i]);

		for (int j = 0; j < long_num->size; j++)
		{
			// get digits
			long_digit = ANS_chr_toint(long_num->string[j]);

			new_digit = (short_digit * long_digit + carry) % num1->numeral_system;
			carry = (short_digit * long_digit + carry) / num1->numeral_system;

			// set digit
			if (i == 0)
			{
				ANS_push_front(container, ANS_int_tochr(new_digit));
			}
			else
			{
				// sum with previous digit
				sum_digit1 = 0;
				sum_digit2 = new_digit;

				if (i + j < container->size)
					sum_digit1 = ANS_chr_toint(ANS_getat(container, i + j));

				new_digit_sum = (sum_digit1 + sum_digit2 + sum_carry) % num1->numeral_system;
				sum_carry = (sum_digit1 + sum_digit2 + sum_carry) / num1->numeral_system;

				// set digit
				if (i + j < container->size)
					ANS_setat(container, i + j, ANS_int_tochr(new_digit_sum));
				else
					ANS_push_front(container, ANS_int_tochr(new_digit_sum));
			}
		}
		// combine sum carry and mult carry
		new_carry = carry + sum_carry;
		if (new_carry > 0)
		{
			if (i == 0)
			{
				ANS_push_front(container, ANS_int_tochr(carry));
			}
			else
			{
				x = 0;
				do
				{
					new_digit = new_carry % num1->numeral_system;

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

void ANS_convert(ANS_Num* num, short numeral_system)
{
	if (num->numeral_system == numeral_system)
		return;

	assert((numeral_system <= 16 && numeral_system >= 2) && "ANS_convert error: numeral system isn't right(should be between 2 and 16)");

	// required containers
	ANS_Num mult;
	ANS_Num result;
	ANS_Num helper;
	ANS_Num curr_digit;

	int digit = 0;
	int rest = 0;

	// init containers
	ANS_init_str(&mult, "0", numeral_system);
	ANS_init_str(&result, "0", numeral_system);
	ANS_init_cap(&helper, 2, numeral_system);
	ANS_init_cap(&curr_digit, 2, numeral_system);

	// prepare multiplier
	for (int i = 0; i < num->numeral_system; i++)
	{
		ANS_increment(&mult);
	}

	// main loop
	for (int i = num->size - 1; i >= 0; i--)
	{
		ANS_mult_withc(&result, &mult, &helper);

		// convert current digit to number in output numeral system and sum it
		digit = ANS_chr_toint(ANS_getat(num, i));
		rest = 0;
		while (digit > 0)
		{
			rest = digit % numeral_system;
			digit = digit / numeral_system;
			ANS_push_front(&curr_digit, ANS_int_tochr(rest));
		}
		ANS_sum(&helper, &curr_digit);

		// copy content from helper container to result
		ANS_cpy(&helper, &result);
		ANS_resize(&curr_digit, 0);
	}

	// copy content of result to given number container
	ANS_delete(num);
	ANS_init_blank(num);
	ANS_fix(&result);
	ANS_cpy(&result, num);

	// free memory
	ANS_delete(&mult);
	ANS_delete(&result);
	ANS_delete(&helper);
	ANS_delete(&curr_digit);
}

bool ANS_sub(ANS_Num* num1, ANS_Num* num2)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_sum error: cannot sub numbers in different numeral systems.");
	
	bool loan = false;
	int digit1;
	int digit2;
	int result;

	if (num1->size < num2->size)
	{
		// second number is bigger than the first one, return 0
		ANS_reset(num1);
		return false;
	}

	for (int i = 0; i < num1->size; i++)
	{
		// prepare digits
		digit1 = ANS_chr_toint(num1->string[i]);
		digit2 = 0;

		if (i < num2->size)
			digit2 = ANS_chr_toint(num2->string[i]);

		// count result
		result = digit1 - digit2;

		if (loan)
			result--;

		if (result < 0)
		{
			result += num1->numeral_system;
			loan = true;
		}
		else
			loan = false;

		// set digit
		ANS_setat(num1, i, ANS_int_tochr(result));
	}

	if (loan)
	{
		// second number is bigger than the first one, return 0
		ANS_reset(num1);
		return false;
	}
	
	ANS_fix(num1);

	return true;
}

void ANS_pow_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_sum error: cannot add numbers in different numeral systems.");

	ANS_Num inum;
	ANS_Num helper;


	if (num2->size == 1 && num2->string[0] == '0')
	{
		// pow with zero exponent returns 1
		ANS_init_str(container, "1", num1->numeral_system);
		return;
	}

	if (container->size > 0)
		ANS_delete(container);

	ANS_init_str(&inum, "0", num1->numeral_system);
	ANS_init_blank(&helper);
	ANS_cpy(num1, container);
	ANS_increment(&inum);

	// main loop
	while (!ANS_is_equal(&inum, num2))
	{
		ANS_mult_withc(num1, container, &helper);
		ANS_cpy(&helper, container);
		ANS_reset(&helper);
		ANS_increment(&inum);
	}

	// free memory
	ANS_delete(&inum);
	ANS_delete(&helper);
}

void ANS_divide_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_divide_withc error: cannot add numbers in different numeral systems.");
	assert(!(num2->size == 1 && num2->string[0] == '0') && "ANS_divide_withc error: cannot divide by zero.");

	ANS_Num num1cpy;

	if (container->size > 0)
		ANS_delete(container);

	ANS_init_str(container, "0", num1->numeral_system);
	ANS_init_blank(&num1cpy);
	ANS_cpy(num1, &num1cpy);

	while (!(num1cpy.size == 1 && num1cpy.string[0] == '0'))
		if (ANS_sub(&num1cpy, num2))
			ANS_increment(container);

	ANS_delete(&num1cpy);
}

void ANS_modulo_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{
	assert(num1->numeral_system == num2->numeral_system && "ANS_divide_withc error: cannot add numbers in different numeral systems.");
	assert(!(num2->size == 1 && num2->string[0] == '0') && "ANS_divide_withc error: cannot divide by zero.");

	ANS_Num num1cpy;

	if (container->size > 0)
		ANS_delete(container);

	ANS_init_str(container, "0", num1->numeral_system);
	ANS_init_blank(&num1cpy);
	ANS_cpy(num1, &num1cpy);

	while (!(num1cpy.size == 1 && num1cpy.string[0] == '0'))
	{
		if (ANS_sub(&num1cpy, num2))
		{
			ANS_cpy(&num1cpy, container);
		}
	}

	ANS_delete(&num1cpy);
}
