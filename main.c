#define _CRT_SECURE_NO_WARNINGS
#define MAX_FILE_NAME_C 100

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "ansnum.h"
#include "ansoperations.h"

int main()
{
	char file_name[MAX_FILE_NAME_C];
	char result_file_name[MAX_FILE_NAME_C];
	char operation;
	char c;

	int numeral_system1;
	int numeral_system2;
	int operations_counter;

	float clock_start;
	float clock_end;
	float elapsed_time;

	bool file_error;

	FILE* ifile;
	FILE* ofile;

	ANS_Num num1;
	ANS_Num num2;
	ANS_Num result;
	ANS_Num num;

	while (true)
	{
		operations_counter = 1;
		file_error = false;
		// picking the file
		printf("Calculator: Enter file name(without extension) or type \".\" to shut down the program.\n>>");
		scanf("%s", file_name);

		// exit case
		if (file_name[0] == '.')
			break;

		// preparing result file name
		strncpy(result_file_name, file_name, strlen(file_name));
		result_file_name[strlen(file_name)] = '\0';

		strcat(file_name, ".txt\0");
		strcat(result_file_name, "_result.txt\0");

		// open files
		ifile = fopen(file_name, "r+");

		if (ifile == NULL)
		{
			file_error = true;
			printf("Calculator: File does'n exit.\n");
		}

		if (!file_error)
		{
			ofile = fopen(result_file_name, "w");

			printf("Calculator: Input file -> %s\n", file_name);
			printf("Calculator: Output file -> %s\n", result_file_name);

			operation = fgetc(ifile);
			do
			{
				printf("Calculator: Solving operation %d...", operations_counter);

				// get current operation
				fprintf(ofile, "%c", operation);

				if (operation == '+' || operation == '*' || operation == '/' || operation == '^')
				{
					// arithmetic operations

					fprintf(ofile, "%c", fgetc(ifile));

					fscanf(ifile, "%d", &numeral_system1);
					fprintf(ofile, "%d", numeral_system1);

					// init number containers(allocate memory)
					ANS_init_cap(&num1, 2, numeral_system1);
					ANS_init_cap(&num2, 2, numeral_system1);
					ANS_init_blank(&result);

					fprintf(ofile, "%c", fgetc(ifile));
					fprintf(ofile, "%c", fgetc(ifile));

					// read 2 numbers
					c = fgetc(ifile);
					while ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
					{
						fprintf(ofile, "%c", c);
						ANS_push_front(&num1, c);
						c = fgetc(ifile);
					}
					ANS_reverse(&num1);
					fprintf(ofile, "%c", fgetc(ifile));

					fprintf(ofile, "\n");

					c = fgetc(ifile);
					while ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
					{
						fprintf(ofile, "%c", c);
						ANS_push_front(&num2, c);
						c = fgetc(ifile);
					}
					ANS_reverse(&num2);
					fgetc(ifile);
					fprintf(ofile, "\n\n");

					// do operations
					clock_start = clock();

					if (operation == '+')
						ANS_sum_withc(&num1, &num2, &result);
					else if (operation == '*')
						ANS_mult_withc(&num1, &num2, &result);
					else if (operation == '/')
						ANS_divide_withc(&num1, &num2, &result);
					else
						ANS_pow_withc(&num1, &num2, &result);

					clock_end = clock();

					elapsed_time = (float)(clock_end - clock_start) / CLOCKS_PER_SEC;

					printf(" elapsed time: %f seconds.\n", elapsed_time);

					// print result to the result file
					for (int i = result.size - 1; i >= 0; i--)
						fprintf(ofile, "%c", result.string[i]);
					fprintf(ofile, "\n");

					// free memory
					ANS_delete(&num1);
					ANS_delete(&num2);
					ANS_delete(&result);
				}
				else
				{
					// number conversions

					// get numeral systems
					c = fgetc(ifile);

					if (c != ' ')
					{
						fprintf(ofile, "%c", c);
						numeral_system1 = 10 + (c - '0');
						fgetc(ifile);
					}
					else
						numeral_system1 = (operation - '0');

					// init number container(allocate memory)
					ANS_init_cap(&num, 2, numeral_system1);

					fprintf(ofile, " ");
					fscanf(ifile, "%d", &numeral_system2);
					fprintf(ofile, "%d", numeral_system2);
					fgetc(ifile);
					fgetc(ifile);
					fprintf(ofile, "\n\n");

					c = fgetc(ifile);

					// read given number
					while ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
					{
						fprintf(ofile, "%c", c);
						ANS_push_front(&num, c);
						c = fgetc(ifile);
					}
					ANS_reverse(&num);
					fprintf(ofile, "\n\n");

					// convert the number to the correct numeral system
					clock_start = clock();
					ANS_convert(&num, numeral_system2);
					clock_end = clock();

					elapsed_time = (float)(clock_end - clock_start) / CLOCKS_PER_SEC;

					printf(" elapsed time: %f seconds.\n", elapsed_time);

					// print the result 
					for (int i = num.size - 1; i >= 0; i--)
						fprintf(ofile, "%c", num.string[i]);

					fprintf(ofile, "\n");
					fgetc(ifile);

					// free memory
					ANS_delete(&num);
				}

				operations_counter++;

				operation = fgetc(ifile);
			} while (operation != EOF);

			// close files and clear strings
			fclose(ifile);
			fclose(ofile);
		}
		memset(file_name, 0, MAX_FILE_NAME_C);
		memset(result_file_name, 0, MAX_FILE_NAME_C);
	}
	return 0;
}