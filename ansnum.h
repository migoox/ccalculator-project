#ifndef ANSNUM_H_
#define ANSNUM_H_

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

struct ANS_Num_s;

typedef struct ANS_Num_s
{
	char* string; // digits container, 1 digit -> 1 byte
	short numeral_system; // [2, 16] are allowed
	size_t size; // length of number(count of digits)
	size_t capacity; // real length of string(allocated memory)
} ANS_Num;

void ANS_init_blank(ANS_Num* n);
void ANS_init_cap(ANS_Num* n, size_t cap, short num_sys);
void ANS_init_str(ANS_Num* n, const char* num_str, short num_sys);

void ANS_delete(ANS_Num* n);

void ANS_clear(ANS_Num* n);

void ANS_change_capacity(ANS_Num* n, size_t new_cap);

void ANS_resize(ANS_Num* n, size_t new_size);

char ANS_getat(ANS_Num* n, size_t index);

void ANS_setat(ANS_Num* n, size_t index, char new_digit);

// push digit at front of the number(in memory number is reversed, so this function pushes digit back)
void ANS_push_front(ANS_Num* n, char digit);

void ANS_pop(ANS_Num* n);

void ANS_print(ANS_Num* n);

void ANS_cpy(ANS_Num* n1, ANS_Num* n2);

int ANS_chr_toint(char c);

char ANS_int_tochr(int digit);

bool ANS_is_equal(ANS_Num* num1, ANS_Num* num2);

#endif
