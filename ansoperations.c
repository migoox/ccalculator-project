#include "ansoperations.h"

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
	while(num1->size > i || num2->size > i)
	{
		int digit1 = 0;
		int digit2 = 0;

		if (num1->size > i)
			digit1 = ANS_chr_toint(num1->string[i]);
		if (num2->size > i)
			digit2 = ANS_chr_toint(num2->string[i]);
		
		int new_digit = (digit1 + digit2 + carry) % num1->numeral_system;
		carry = (digit1 + digit2 + carry) / num1->numeral_system;

		ANS_push(container, ANS_int_tochr(new_digit));

		i++;
	}

	if (carry > 0)
		ANS_push(container, ANS_int_tochr(carry));
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
			ANS_push(num1, ANS_int_tochr(new_digit));

		i++;
	}

	if (carry > 0)
	{
		if (num1->size > i + 1)
			ANS_setat(num1, i, ANS_int_tochr(carry));
		else 
			ANS_push(num1, ANS_int_tochr(carry));
	}
}

void ANS_mult_withc(ANS_Num* num1, ANS_Num* num2, ANS_Num* container)
{

}
