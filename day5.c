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

	int p1_input = 1;
	int p1_output[10] = {0};

	intcode(memory_copy, &p1_input, 1, &p1_output[0], sizeof(p1_output));
	printf("Day 5, Part 1: %d\n", p1_output[9]);

	// Part 2
	memcpy(memory_copy, memory, sizeof(int) * memory_size);

	int p2_input = 5;
	int p2_output = 0;

	intcode(memory_copy, &p2_input, 1, &p2_output, 1);

	printf("Day 5, Part 2: %d\n", p2_output);

	free(memory);
}
