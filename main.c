#include <stdio.h>
#include "ansnum.h"
#include "ansoperations.h"

int main()
{
	ANS_Num num;
	
	ANS_init_str(&num, "9169", 10);
	ANS_print(&num);

	ANS_Num num2;
	ANS_init_str(&num2, "952", 10);
	ANS_print(&num2);

	ANS_sum(&num, &num2);
	ANS_print(&num);

	ANS_delete(&num);
	ANS_delete(&num2);
}