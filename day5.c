#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "aoc2019.h"
#include "intcode.h"

static const char* filename = "./input/day5.txt";

void
day5(void)
{
	int* memory = NULL;
	size_t memory_size = 0;

	// Setup
	FILE* file = fopen(filename, "r");
	assert(file);

	char* file_buffer = dump_file(file);
	assert(file_buffer);

	memory_size = parse_memory_size(file_buffer);
	assert(memory_size);

	memory = parse_memory(file_buffer, memory_size);
	assert(memory);

	free(file_buffer);
	fclose(file);

	int* memory_copy = malloc(sizeof(int) * memory_size);

	// Part 1
	memcpy(memory_copy, memory, sizeof(int) * memory_size);

	// inputs and outputs
	struct intcode_io* p1_input = init_intcode_io(1);
	struct intcode_io* p1_output = init_intcode_io(10);

	// input value
	p1_input->io[0] = 1;

	intcode(memory_copy, p1_input, p1_output);
	printf("Day 5, Part 1: %d\n", p1_output->io[9]);

	free_intcode_io(p1_input);
	free_intcode_io(p1_output);

	// Part 2
	memcpy(memory_copy, memory, sizeof(int) * memory_size);

	// inputs and outputs
	struct intcode_io* p2_input = init_intcode_io(1);
	struct intcode_io* p2_output = init_intcode_io(1);

	// input value
	p2_input->io[0] = 5;

	intcode(memory_copy, p2_input, p2_output);

	printf("Day 5, Part 2: %d\n", p2_output->io[0]);

	free_intcode_io(p2_input);
	free_intcode_io(p2_output);

	free(memory);
}
