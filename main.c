#define _CRT_SECURE_NO_WARNINGS
#define MAX_FILE_NAME_C 100
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ansnum.h"
#include "ansoperations.h"

int main()
{
	char file_name[MAX_FILE_NAME_C];
	printf("Calculator: Enter file name.\n>>");
	scanf("%s", file_name);


	// input
	FILE* file = fopen(file_name, "r+");


	fclose(file);
	memset(file_name, 0, MAX_FILE_NAME_C);

	return 0;
}