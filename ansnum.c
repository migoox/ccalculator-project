#include "ansnum.h"

void ANS_init_blank(ANS_Num* n)
{
	n->capacity = 0;
	n->size = 0;
	n->numeral_system = 10;
	n->string = NULL;
}

void ANS_init_cap(ANS_Num* n, size_t cap, short num_sys)
{
	n->capacity = cap;
	n->size = 0;
	n->numeral_system = num_sys;

	n->string = (char*)calloc(cap * sizeof(char));
}

void ANS_init_str(ANS_Num* n, const char* num_str, short num_sys)
{
	bool correct_num = true;
	size_t size = 0;

	while (num_str[size] != '\0')
	{
		if (ANS_chr_toint(num_str[size]) >= num_sys)
		{
			correct_num = false;
			break;
		}
		size++;
	}

	assert(correct_num && "ANS_init_str function error: given number contains digits which are not par of given number system.");

	n->capacity = size;
	n->size = size;
	n->numeral_system = num_sys;

	n->string = (char*)calloc(size * sizeof(char));

	for (size_t i = size - 1, j = 0; j < size; i--, j++)
		n->string[i] = num_str[j];
}

void ANS_delete(ANS_Num* n)
{
	free(n->string);
	n->capacity = 0;
	n->size = 0;
	n->numeral_system = 10;
}

void ANS_clear(ANS_Num* n)
{
	for (size_t i = 0; i < n->size; i++)
	{
		n->string[i] = '0';
	}
}

void ANS_change_capacity(ANS_Num* n, size_t new_cap)
{
	if (new_cap == n->capacity)
		return;

	char* new_ptr = (char*)calloc(new_cap * sizeof(char));
	for (size_t i = 0; i < n->size; i++)
	{
		if (n->size > new_cap)
		{
			n->size = new_cap;
			break;
		}
		new_ptr[i] = n->string[i];
	}

	free(n->string);
	n->capacity = new_cap;
	n->string = new_ptr;
}

char ANS_getat(ANS_Num* n, size_t index)
{
	return n->string[index];
}

void ANS_setat(ANS_Num* n, size_t index, char new_digit)
{
	n->string[index] = new_digit;
}

void ANS_push_front(ANS_Num* n, char digit)
{
	if (n->size + 1 >= n->capacity)
	{
		size_t new_capacity = n->capacity + n->capacity / 2 + 1;
		if (n->size <= 0)
			new_capacity = 2;

		ANS_change_capacity(n, new_capacity);
	}

	n->string[n->size++] = digit;
}

void ANS_pop(ANS_Num* n)
{
	n->string[n->size--] = '0';
}

void ANS_print(ANS_Num* n)
{
	for (size_t i = 0; i < n->size; i++)
	{
		printf("%c", n->string[n->size - i - 1]);
	}
	printf("|%d", n->numeral_system);
	printf("\n");
}

void ANS_cpy(ANS_Num* n1, ANS_Num* n2)
{
	if (n2->size == n1->size)
	{
		for (int i = 0; i < n1->size; i++)
		{
			n2->string[i] = n1->string[i];
		}
	}
	else if (n2->size > n1->size)
	{
		ANS_clear(n2);
		for (int i = 0; i < n1->size; i++)
		{
			n2->string[i] = n1->string[i];
		}
	}
	else
	{
		if (n2->capacity < n1->size)
		{
			// realloc
			if (n2->size > 0)
				ANS_delete(n2);

			ANS_init_cap(n2, n1->capacity, n1->numeral_system);

			for (int i = 0; i < n1->size; i++)
			{
				n2->string[i] = n1->string[i];
			}
		}
		else
		{
			for (int i = 0; i < n1->size; i++)
			{
				n2->string[i] = n1->string[i];
			}
		}
	}

	n2->numeral_system = n1->numeral_system;
	n2->size = n1->size;
}

int ANS_chr_toint(char c)
{
	int i;

	bool incorrect_digit = false;
	if (c >= '0' && c <= '9')
	{
		i = c - '0';
	}
	else if (c >= 'A' && c <= 'F')
	{
		i = c - 'A' + 10;
	}
	else if (c >= 'a' && c <= 'f')
	{
		i = c - 'a' + 10;
	}
	else
	{
		incorrect_digit = true;
	}

	assert(!incorrect_digit && "ANS_chr_toint error: Imposible to convert char digit to decimal integer, incorrect input.");

	return i;
}

char ANS_int_tochr(int digit)
{
	char c;

	bool incorrect_digit = false;
	if (digit >= 0 && digit <= 9)
	{
		c = digit + '0';
	}
	else if (digit >= 10 && digit <= 15)
	{
		c = digit + 'A' - 10;
	}
	else
	{
		incorrect_digit = true;
	}

	assert(!incorrect_digit && "ANS_int_tochr error: Imposible to convert char digit to decimal integer, incorrect input.");

	return c;
}

bool ANS_is_equal(ANS_Num* num1, ANS_Num* num2)
{
	ANS_Num* short_num;
	ANS_Num* long_num;

	if (num1->size > num2->size)
	{
		short_num = num2;
		long_num = num1;
	}
	else
	{
		short_num = num1;
		long_num = num2;
	}

	for (int i = 0, j = 0; i < long_num->size; i++, j++)
	{
		if (j >= short_num->size)
		{
			if (ANS_getat(long_num, i) != '0')
				return false;
		}
		else
		{

			if (ANS_getat(short_num, j) != ANS_getat(long_num, i))
				return false;
		}
	}

	return true;
}