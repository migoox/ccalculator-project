#ifndef ANSNUM_H_
#define ANSNUM_H_

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

struct ANS_Num_s;

typedef struct ANS_Num_s
{
	char* string;			// digits container, 1 digit is 1 byte
	short numeral_system;   // [2, 16] are allowed
	size_t size;		    // length of number(count of digits)
	size_t capacity;	    // real length of string(allocated memory)
} ANS_Num;

void ANS_init_blank(ANS_Num* n);
void ANS_init_cap(ANS_Num* n, size_t cap, short num_sys);
void ANS_init_str(ANS_Num* n, const char* num_str, short num_sys);

// frees memory
void ANS_delete(ANS_Num* n);

// sets all digits of number to 0
void ANS_clear(ANS_Num* n);

// changes capacity of the container
void ANS_change_capacity(ANS_Num* n, size_t new_cap);

// resizes to 1 and sets to 0
void ANS_reset(ANS_Num* n);

// resizes number
void ANS_resize(ANS_Num* n, size_t new_size);

// reverse notation of number
void ANS_reverse(ANS_Num* n);

// returns value stored on given index
char ANS_getat(ANS_Num* n, size_t index);

// modifies digit on given index
void ANS_setat(ANS_Num* n, size_t index, char new_digit);

// push digit at front of the number(in memory number is reversed, so this function pushes digit back)
void ANS_push_front(ANS_Num* n, char digit);

// the size is smaller by one
void ANS_pop(ANS_Num* n);

// copies content of source to destination container
void ANS_cpy(ANS_Num* source, ANS_Num* destination);

// deletes uneccesary zeros at front of the number
void ANS_fix(ANS_Num* n);

bool ANS_is_equal(ANS_Num* num1, ANS_Num* num2);

int ANS_chr_toint(char c);

char ANS_int_tochr(int digit);


#endif
