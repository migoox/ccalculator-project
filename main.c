#define _CRT_SECURE_NO_WARNINGS
#define MAX_FILE_NAME_SIZE 64
#define MAX_ERROR_MSG_SIZE 64

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "ansnum.h"
#include "ansoperations.h"

int main()
{
	// file names
	char file_name[MAX_FILE_NAME_SIZE];
	char result_file_name[MAX_FILE_NAME_SIZE];

	// current operation
	char operation;

	// helper container
	char c;

	// input numeral systems
	int numeral_system1;
	int numeral_system2;

	int operation_counter;

	// clock
	float clock_start;
	float clock_end;
	float elapsed_time;

	// file reading error status
	bool file_error;

	// operation error status
	bool operation_error;

	// operation error messages
	char operation_error_incorrect_system[MAX_ERROR_MSG_SIZE] = 
		"ERROR: Numeral system should be between 2 and 16";

	char operation_error_msg[MAX_ERROR_MSG_SIZE];

	// input and output file
	FILE* ifile;
	FILE* ofile;

	// number containers
	ANS_Num num1;
	ANS_Num num2;
	ANS_Num result;
	ANS_Num num;

	while (true)
	{
		// initialize values
		operation_counter = 1;
		file_error = false;

		// choosing the file
		printf("Calculator: Enter file name(without extension) or type \".\" to shut down the program.\n>>");
		scanf("%s", file_name);

		// exit case
		if (file_name[0] == '.')
			break;

		// preparing result file name
		strncpy(result_file_name, file_name, strlen(file_name));
		result_file_name[strlen(file_name)] = '\0';

		strcat(file_name, ".txt\0");\
		strcat(result_file_name, "_result.txt\0");

		// open files
		ifile = fopen(file_name, "r");

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
				// initialize error flag
				operation_error = false;

				printf("Calculator: Working on operation %d...", operation_counter);

				// get current operation
				fprintf(ofile, "%c", operation);

				// arithmetic operations
				if (operation == '+' || operation == '*' || operation == '/' || operation == '%' || operation == '^')
				{
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
					ANS_fix(&num1);
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
					ANS_fix(&num2);
					fgetc(ifile);
					fgetc(ifile);
					fprintf(ofile, "\n\n");

					// check errors
					if (numeral_system1 > 16 || numeral_system1 < 2)
					{
						strcpy(operation_error_msg, "ERROR: Numeral system should be between 2 and 16(both inclusive)");
						operation_error = true;
					}
					else
					{
						for (int i = 0; i < num1.size; i++)
						{
							int digit = ANS_chr_toint(ANS_getat(&num1, i));
							if (!(digit >= 0 && digit < num1.numeral_system))
							{
								strcpy(operation_error_msg, "ERROR: First number contains invalid digit(s).");
								operation_error = true;
							}
						}

						if (!operation_error)
						{
							for (int i = 0; i < num2.size; i++)
							{
								int digit = ANS_chr_toint(ANS_getat(&num2, i));
								if (!(digit >= 0 && digit < num2.numeral_system))
								{
									strcpy(operation_error_msg, "ERROR: Second number contains invalid digit(s).");
									operation_error = true;
								}
							}
						}
						if (!operation_error)
						{
							if (num2.size == 1 && ANS_getat(&num2, 0) == '0' && (operation == '/' || operation == '%'))
							{
								strcpy(operation_error_msg, "ERROR: Can't divide by 0");
								operation_error = true;
							}
						}
					}

					// do operations
					clock_start = clock();
					if (!operation_error)
					{
						if (operation == '+')
							ANS_sum_withc(&num1, &num2, &result);
						else if (operation == '*')
							ANS_mult_withc(&num1, &num2, &result);
						else if (operation == '/')
							ANS_divide_withc(&num1, &num2, &result);
						else if (operation == '%')
							ANS_modulo_withc(&num1, &num2, &result);
						else
							ANS_pow_withc(&num1, &num2, &result);
					}
					clock_end = clock();

					elapsed_time = (float)(clock_end - clock_start) / CLOCKS_PER_SEC;
					printf(" elapsed time: %f seconds.\n", elapsed_time);

					// print result to the result file
					if(!operation_error)
						for (int i = result.size - 1; i >= 0; i--)
							fprintf(ofile, "%c", result.string[i]);
					else
						fprintf(ofile, "%s", operation_error_msg);

					fprintf(ofile, "\n\n");

					// free memory
					ANS_delete(&num1);
					ANS_delete(&num2);
					ANS_delete(&result);
				}
				// number conversions
				else if (operation - '0' >= 0 && operation - '0' <= 9)
				{

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
					ANS_fix(&num);
					fgetc(ifile);
					fprintf(ofile, "\n\n");

					// convert the number to the correct numeral system

					// error checking
					if (numeral_system1 > 16 || numeral_system1 < 2 || numeral_system2 > 16 || numeral_system2 < 2)
					{
						strcpy(operation_error_msg, "ERROR: Numeral system should be between 2 and 16(both inclusive)");
						operation_error = true;
					}
					else
					{
						for (int i = 0; i < num1.size; i++)
						{
							int digit = ANS_chr_toint(ANS_getat(&num1, i));
							if (!(digit >= 0 && digit < num1.numeral_system))
							{
								strcpy(operation_error_msg, "ERROR: First number contains invalid digit(s).");
								operation_error = true;
							}
						}
					}

					// conversion
					clock_start = clock();
					if(!operation_error)
						ANS_convert(&num, numeral_system2);
					clock_end = clock();

					elapsed_time = (float)(clock_end - clock_start) / CLOCKS_PER_SEC;
					printf(" elapsed time: %f seconds.\n", elapsed_time);

					// print the result or error message
					if (!operation_error)
						for (int i = num.size - 1; i >= 0; i--)
							fprintf(ofile, "%c", num.string[i]);
					else
						fprintf(ofile, "%s", operation_error_msg);

					fprintf(ofile, "\n\n");
					fgetc(ifile);

					// free memory
					ANS_delete(&num);
				}
				else
				{
					operation_error = true;
					fprintf(ofile, "\n");
					strcpy(operation_error_msg, "ERROR: Invalid operation");
					fprintf(ofile, "%s", operation_error_msg);
					printf("\nCalculator: Invalid operation sign interrupted further operation.\n");
					// incorrect operation breaks whole loop
					break;
				}
				// increment operation counter
				operation_counter++;

				operation = fgetc(ifile);
			} while (operation != EOF);

			// close files and clear strings
			fclose(ifile);
			fclose(ofile);
		}
		memset(file_name, 0, MAX_FILE_NAME_SIZE);
		memset(result_file_name, 0, MAX_FILE_NAME_SIZE);
	}
	return 0;
}