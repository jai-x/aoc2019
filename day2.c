#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2019.h"

static const char* filename = "./input/day2.txt";

static void
intcode(int* memory)
{
	size_t address = 0;
	while (memory[address] != 99) {
		int op = memory[address];
		int left = memory[address+1];
		int right = memory[address+2];
		int store = memory[address+3];

		if (op == 1) {
			memory[store] = memory[left] + memory[right];
		}

		if (op == 2) {
			memory[store] = memory[left] * memory[right];
		}

		address += 4;
	}
}

void
day2(void)
{
	// Open file
	FILE* input_file = fopen(filename, "r");
	if (!input_file) {
		printf("Error opening file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	// Get num bytes
	fseek(input_file, 0, SEEK_END);
	size_t input_num_bytes = (size_t)ftell(input_file);
	fseek(input_file, 0, SEEK_SET);

	// Allocate to buffer and close file
	char* input_buffer = malloc(sizeof(char) * input_num_bytes);
	fread(input_buffer, sizeof(char), input_num_bytes, input_file);
	fclose(input_file);

	// Get memory entries
	size_t num_commas = 0;
	for (size_t i = 0; i < input_num_bytes; i++) {
		if (input_buffer[i] == ',') {
			num_commas++;
		}
	}
	size_t memory_size = num_commas + 1;

	// Allocate program memory
	int* memory = malloc(sizeof(int) * memory_size);

	// Read into array and free buffer
	char* buffer_remainder = input_buffer;
	char* token = NULL;
	int read_pos = 0;
	while ((token = strsep(&buffer_remainder, ","))) {
		memory[read_pos++] = atoi(token);
	}
	free(input_buffer);

	// Allocate for copy of memory
	int* memory_copy = malloc(sizeof(int) * memory_size);

	// Part 1
	memcpy(memory_copy, memory, sizeof(int) * memory_size);
	memory_copy[1] = 12;
	memory_copy[2] = 2;
	intcode(memory_copy);
	printf("Day 2, Part 1: %d\n", memory_copy[0]);

	// Part 2
	const int target = 19690720;

	for (int noun = 0; noun < 99; noun++) {
		for (int verb = 0; verb < 99; verb++) {
				memcpy(memory_copy, memory, sizeof(int) * memory_size);

				memory_copy[1] = noun;
				memory_copy[2] = verb;

				intcode(memory_copy);

				if (memory_copy[0] == target) {
					printf("Day 2, Part 2: %d\n", 100 * noun + verb);
					noun = 100;
					verb = 100;
				}
		}
	}

	free(memory_copy);
	free(memory);
}
