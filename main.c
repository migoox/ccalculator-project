#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "ansnum.h"
#include "ansoperations.h"

int main()
{
	int numeral_system = 10;
	char num1str[1000], num2str[1000];

	printf("Number 1: ");
	scanf("%s", num1str);
	printf("Number 2: ");
	scanf("%s", num2str);

	printf("\nInput: \n");

	ANS_Num num;
	ANS_init_str(&num, &num1str, numeral_system);
	ANS_print(&num);
	printf("\n");
	ANS_Num num2;
	ANS_init_str(&num2, &num2str, numeral_system);
	ANS_print(&num2);
	printf("\n");


	/*ANS_Num container;
	ANS_init_blank(&container);

	ANS_sum_withc(&num, &num2, &container);
	printf("sum: ");
	ANS_print(&container);

	ANS_delete(&container);

	ANS_init_blank(&container);

	ANS_mult_withc(&num, &num2, &container);
	printf("mult: ");
	ANS_print(&container);

	ANS_delete(&container);

	ANS_init_blank(&container);

	ANS_pow_withc(&num, &num2, &container);
	printf("pow: ");
	ANS_print(&container);

	ANS_delete(&container);
	*/
	printf("sub: ");
	ANS_sub(&num, &num2);
	ANS_print(&num);

	ANS_delete(&num);
	ANS_delete(&num2);

	printf("\n\n");

	main();

	return 0;
}