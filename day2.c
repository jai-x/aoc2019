#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "aoc2019.h"
#include "intcode.h"

static const char* filename = "./input/day2.txt";

void
day2(void)
{
	// Open file and dump to buffer
	FILE* file = fopen(filename, "r");
	assert(file);
	char* file_buffer = dump_file(file);
	fclose(file);

	// Parse memory
	size_t memory_size = parse_memory_size(file_buffer);
	int* memory = parse_memory(file_buffer, memory_size);
	free(file_buffer);

	// Allocate for copy of memory
	int* memory_copy = malloc(sizeof(int) * memory_size);

	// Part 1
	memcpy(memory_copy, memory, sizeof(int) * memory_size);

	// Inputs by modifying memory addresses
	memory_copy[1] = 12;
	memory_copy[2] = 2;

	intcode(memory_copy, NULL, NULL);

	printf("Day 2, Part 1: %d\n", memory_copy[0]);

	// Part 2
	const int target = 19690720;

	for (int noun = 0; noun < 99; noun++) {
		for (int verb = 0; verb < 99; verb++) {
				memcpy(memory_copy, memory, sizeof(int) * memory_size);

				// Inputs by modifying memory addresses
				memory_copy[1] = noun;
				memory_copy[2] = verb;

				intcode(memory_copy, NULL, NULL);

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
