#include <stdio.h>
#include "ansnum.h"

int main()
{
	ANS_Num num;
	ANS_init_str(&num, "46321AB", 12);

	ANS_print(&num);
	printf("  size: %d, capacity: %d", num.size, num.capacity);
	printf("\n");

	ANS_push(&num, '5');

	ANS_print(&num);
	printf("  size: %d, capacity: %d", num.size, num.capacity);
	printf("\n");

	ANS_push(&num, '5');

	ANS_print(&num);
	printf("  size: %d, capacity: %d", num.size, num.capacity);
	printf("\n");

	ANS_push(&num, '5');

	ANS_print(&num);
	printf("  size: %d, capacity: %d", num.size, num.capacity);
	printf("\n");

	ANS_push(&num, '5');

	ANS_print(&num);
	printf("  size: %d, capacity: %d", num.size, num.capacity);
	printf("\n");

	ANS_pop(&num);

	ANS_print(&num);
	printf("  size: %d, capacity: %d", num.size, num.capacity);
	printf("\n");

	ANS_delete(&num);
}