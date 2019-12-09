#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "aoc2019.h"
#include "intcode.h"

static const char* filename = "./input/day2.txt";

static char*
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

static size_t
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

static int*
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
