#define _DEFAULT_SOURCE

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "intcode.h"

char*
dump_file(FILE* file) {
	// Get num bytes
	fseek(file, 0, SEEK_END);
	size_t file_bytes = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	// Allocate buffer and dump file
	char* file_buffer = malloc(sizeof(char) * file_bytes);
	fread(file_buffer, sizeof(char), file_bytes, file);

	return file_buffer;
}

size_t
parse_memory_size(char* line)
{
	size_t num_commas = 0;
	for (size_t i = 0; i < strlen(line); i++) {
		if (line[i] == ',') {
			num_commas++;
		}
	}

	return num_commas + 1;
}

int*
parse_memory(char* line, size_t memory_size)
{
	int* memory = malloc(sizeof(int) * memory_size);

	char* line_remainder = line;
	char *token = NULL;
	size_t pos = 0;

	while ((token = strsep(&line_remainder, ","))) {
		memory[pos++] = atoi(token);
	}

	return memory;
}

static inline int
first_val(int* memory, int opcode, int address)
{
	int mode = opcode % 1000 / 100;
	int literal = memory[address + 1];
	return mode ? literal : memory[literal];
}

static inline int
second_val(int* memory, int opcode, int address)
{
	int mode = opcode % 10000 / 1000;
	int literal = memory[address + 2];
	return mode ? literal : memory[literal];
}

void
intcode(int* memory, int* input, size_t input_size, int* output, size_t output_size)
{
	int address = 0;

	size_t input_count = 0;
	size_t output_count = 0;

	while (true) {
		int opcode = memory[address];

		int inst = opcode % 100;

		switch (inst) {
			// Add left value right value into third value position
			case 1: {
				int store = memory[address + 3];
				int first = first_val(memory, opcode, address);
				int second = second_val(memory, opcode, address);
				memory[store] = first + second;
				address += 4;
				break;
			}
			// Muliply first value and second into third value position
			case 2: {
				int store = memory[address + 3];
				int first = first_val(memory, opcode, address);
				int second = second_val(memory, opcode, address);
				memory[store] = first * second;
				address += 4;
				break;
			}
			// Read input into first value position
			case 3: {
				int store = memory[address + 1];
				assert(input);
				assert(input_count < input_size);
				memory[store] = input[input_count++];
				address += 2;
				break;
			}
			// Output first value
			case 4: {
				assert(output);
				assert(output_count < output_size);
				output[output_count++] = first_val(memory, opcode, address);
				address += 2;
				break;
			}
			// Jump to second value if first value is not zero
			case 5: {
				int val = first_val(memory, opcode, address);
				int addr = second_val(memory, opcode, address);
				address = (val != 0) ? addr : address + 3;
				break;
			}
			// Jump to second value if first value is zero
			case 6: {
				int val = first_val(memory, opcode, address);
				int addr = second_val(memory, opcode, address);
				address = (val == 0) ? addr : address + 3;
				break;
			}
			// If first value is less than the second value store 1 in third position, else store 0
			case 7: {
				int store = memory[address + 3];
				int first = first_val(memory, opcode, address);
				int second = second_val(memory, opcode, address);
				memory[store] = (first < second) ? 1 : 0;
				address += 4;
				break;
			}
			// If first value is equal to the second value store 1 in third position, else store 0
			case 8: {
				int store = memory[address + 3];
				int first = first_val(memory, opcode, address);
				int second = second_val(memory, opcode, address);
				memory[store] = (first == second) ? 1 : 0;
				address += 4;
				break;
			}
			case 99: {
				return;
			}
			default: {
				printf("Unknown opcode: %d\n", opcode);
			}
		}
	}
}
