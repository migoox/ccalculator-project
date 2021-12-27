#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#include "ansnum.h"
#include "ansoperations.h"

#define MAX_FILE_NAME_SIZE 64
#define MAX_ERROR_MSG_SIZE 64

bool do_operation(FILE* input_file, FILE* output_file, int operation_counter);

void create_current_output_file_name(char* input_file_path, char* output_file_path_container, int operation_counter);

int main()
{
	char input_file_path[MAX_FILE_NAME_SIZE];
	char output_file_path[MAX_FILE_NAME_SIZE];
	char output_type_choice;

	FILE* input_file;
	FILE* current_output_file;

	bool single_file_output;

	int operation_counter;

	while (true)
	{
		operation_counter = 0;
		single_file_output = false;

		// file name input
		printf("Calculator: Enter txt file name(without extension) or type \".\" to shut down the program.\n>>");
		scanf("%s", input_file_path);

		// exit case
		if (input_file_path[0] == '.')
			break;

		// repairing file name input
		strcat(input_file_path, ".txt\0");

		// opening input file
		input_file = fopen(input_file_path, "r");

		// checking if the file exists
		if (input_file == NULL)
		{
			printf("Calculator: File doesn't exist.\n");
			continue;
		}

		// single file output choice
		printf("Calculator: Do you want single file output? Y/N\n>>");
		scanf(" %c", &output_type_choice);

		if (output_type_choice == 'Y' || output_type_choice == 'y')
			single_file_output = true;

		// single output case
		if (single_file_output)
		{
			// prepare output name
			strncpy(output_file_path, input_file_path, strlen(input_file_path) - 4);
			output_file_path[strlen(input_file_path) - 4] = '\0';
			strcat(output_file_path, "_result.txt\0");

			// open(create) output file
			current_output_file = fopen(output_file_path, "w");

			// do operations loop
			do
			{
				operation_counter++;
			} while (do_operation(input_file, current_output_file, operation_counter));

			// close output file
			fclose(current_output_file);
		}
		else
		{
			while (fgetc(input_file) != EOF)
			{
				// go back
				fseek(input_file, -1, SEEK_CUR);

				operation_counter++;

				create_current_output_file_name(input_file_path, output_file_path, operation_counter);

				// do operation and save it in new file
				current_output_file = fopen(output_file_path, "w");

				do_operation(input_file, current_output_file, operation_counter);

				fclose(current_output_file);
			}
		}
		// close input file
		fclose(input_file);
	}
	return 0;
}

bool do_operation(FILE* input_file, FILE* output_file, int operation_counter)
{
	char current_operation;
	char operation_error_msg[MAX_ERROR_MSG_SIZE];
	char temp;

	bool operation_error;

	int numeral_system;
	int numeral_system2;

	float clock_start;
	float clock_end;
	float elapsed_time;

	ANS_Num num1;
	ANS_Num num2;
	ANS_Num result;
	ANS_Num num;

	// initialize error flag
	operation_error = false;

	current_operation = fgetc(input_file);

	if (current_operation == EOF)
		return false;

	printf("Calculator: Working on operation %d...", operation_counter);

	// print current operation in output_file
	fprintf(output_file, "%c", current_operation);

	// arithmetic operations
	if (current_operation == '+' || current_operation == '*' || current_operation == '/' || current_operation == '%' || current_operation == '^')
	{
		fprintf(output_file, "%c", fgetc(input_file));

		fscanf(input_file, "%d", &numeral_system);
		fprintf(output_file, "%d", numeral_system);

		// init number containers(allocate memory)
		ANS_init_cap(&num1, 2, numeral_system);
		ANS_init_cap(&num2, 2, numeral_system);
		ANS_init_blank(&result);

		fprintf(output_file, "%c", fgetc(input_file));
		fprintf(output_file, "%c", fgetc(input_file));

		// read 2 numbers
		temp = fgetc(input_file);
		while ((temp >= '0' && temp <= '9') || (temp >= 'A' && temp <= 'F') || (temp >= 'a' && temp <= 'f'))
		{
			fprintf(output_file, "%c", temp);
			ANS_push_front(&num1, temp);
			temp = fgetc(input_file);
		}
		ANS_reverse(&num1);
		ANS_fix(&num1);
		fprintf(output_file, "%c", fgetc(input_file));

		fprintf(output_file, "\n");

		temp = fgetc(input_file);
		while ((temp >= '0' && temp <= '9') || (temp >= 'A' && temp <= 'F') || (temp >= 'a' && temp <= 'f'))
		{
			fprintf(output_file, "%c", temp);
			ANS_push_front(&num2, temp);
			temp = fgetc(input_file);
		}
		ANS_reverse(&num2);
		ANS_fix(&num2);
		fgetc(input_file);
		fgetc(input_file);
		fprintf(output_file, "\n\n");


		if (numeral_system > 16 || numeral_system < 2)
		{
			strcpy(operation_error_msg, "ERROR: Invalid numeral system");
			operation_error = true;
		}
		else
		{
			for (size_t i = 0; i < num1.size; i++)
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
				for (size_t i = 0; i < num2.size; i++)
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
				if (num2.size == 1 && ANS_getat(&num2, 0) == '0' && (current_operation == '/' || current_operation == '%'))
				{
					strcpy(operation_error_msg, "ERROR: Can't divide by 0");
					operation_error = true;
				}
			}
		}

		// do operations
		clock_start = (float)clock();
		if (!operation_error)
		{
			if (current_operation == '+')
				ANS_sum_withc(&num1, &num2, &result);
			else if (current_operation == '*')
				ANS_mult_withc(&num1, &num2, &result);
			else if (current_operation == '/')
				ANS_divide_withc(&num1, &num2, &result);
			else if (current_operation == '%')
				ANS_modulo_withc(&num1, &num2, &result);
			else
				ANS_pow_withc(&num1, &num2, &result);
		}
		clock_end = (float)clock();

		elapsed_time = (float)(clock_end - clock_start) / CLOCKS_PER_SEC;
		printf(" elapsed time: %f seconds.\n", elapsed_time);

		// print result to the result file
		if (!operation_error)
			for (int i = result.size - 1; i >= 0; i--)
				fprintf(output_file, "%c", result.string[i]);
		else
			fprintf(output_file, "%s", operation_error_msg);

		fprintf(output_file, "\n\n");

		// free memory
		ANS_delete(&num1);
		ANS_delete(&num2);
		ANS_delete(&result);
	}
	// number conversions
	else if (current_operation - '0' >= 0 && current_operation - '0' <= 9)
	{

		// get numeral systems
		temp = fgetc(input_file);

		if (temp != ' ')
		{
			fprintf(output_file, "%c", temp);
			numeral_system = 10 + (temp - '0');
			fgetc(input_file);
		}
		else
			numeral_system = (current_operation - '0');

		// init number container(allocate memory)
		ANS_init_cap(&num, 2, numeral_system);

		fprintf(output_file, " ");
		fscanf(input_file, "%d", &numeral_system2);
		fprintf(output_file, "%d", numeral_system2);
		fgetc(input_file);
		fgetc(input_file);
		fprintf(output_file, "\n\n");

		temp = fgetc(input_file);

		// read given number
		while ((temp >= '0' && temp <= '9') || (temp >= 'A' && temp <= 'F') || (temp >= 'a' && temp <= 'f'))
		{
			fprintf(output_file, "%c", temp);
			ANS_push_front(&num, temp);
			temp = fgetc(input_file);
		}
		ANS_reverse(&num);
		ANS_fix(&num);
		fgetc(input_file);
		fprintf(output_file, "\n\n");

		// convert the number to the correct numeral system

		// error checking
		if (numeral_system > 16 || numeral_system < 2 || numeral_system2 > 16 || numeral_system2 < 2)
		{
			strcpy(operation_error_msg, "ERROR: Invalid numeral system");
			operation_error = true;
		}
		else
		{
			for (size_t i = 0; i < num.size; i++)
			{
				int digit = ANS_chr_toint(ANS_getat(&num, i));
				if (!(digit >= 0 && digit < num.numeral_system))
				{
					strcpy(operation_error_msg, "ERROR: Number contains invalid digit(s).");
					operation_error = true;
				}
			}
		}

		// conversion
		clock_start = (float)clock();
		if (!operation_error)
			ANS_convert(&num, numeral_system2);
		clock_end = (float)clock();

		elapsed_time = (float)(clock_end - clock_start) / CLOCKS_PER_SEC;
		printf(" elapsed time: %f seconds.\n", elapsed_time);

		// print the result or error message
		if (!operation_error)
			for (int i = num.size - 1; i >= 0; i--)
				fprintf(output_file, "%c", num.string[i]);
		else
			fprintf(output_file, "%s", operation_error_msg);

		fprintf(output_file, "\n\n");
		fgetc(input_file);

		// free memory
		ANS_delete(&num);
	}
	else
	{
		operation_error = true;
		fprintf(output_file, "\n");
		strcpy(operation_error_msg, "ERROR: Invalid operation");
		fprintf(output_file, "%s", operation_error_msg);
		printf("\nCalculator: The program was interrupted by invalid operation sign.\n");
		// incorrect operation breaks whole loop
		return false;
	}

	return true;
}

void create_current_output_file_name(char* input_file_path, char* output_file_path_container, int operation_counter)
{
	char result[MAX_FILE_NAME_SIZE];
	char number[MAX_FILE_NAME_SIZE];
	char temp;
	int temp_digit;
	int size;

	// getting file name without extension and saving it in result container
	strncpy(result, input_file_path, strlen(input_file_path) - 4);
	result[strlen(input_file_path) - 4] = '\0';

	strcat(result, "_result[\0");

	size = 0;
	do
	{
		temp_digit = operation_counter % 10;
		number[size++] = temp_digit + '0';
	} while (operation_counter /= 10);

	number[size] = '\0';

	for (int i = 0, j = size - 1; i < j; i++, j--)
	{
		temp = number[i];
		number[i] = number[j];
		number[j] = temp;
	}

	strcat(result, number);

	strcat(result, "].txt\0");

	strncpy(output_file_path_container, result, strlen(result));
	output_file_path_container[strlen(result)] = '\0';
}
