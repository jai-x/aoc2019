#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "aoc2019.h"
#include "intcode.h"

static const char* filename = "./input/day5.txt";

static char*
dump_file(FILE* file)
{
	// Get num bytes
	fseek(file, 0, SEEK_END);
	size_t file_bytes = (size_t)ftell(file);
	fseek(file, 0, SEEK_SET);

	// Allocate buffer and dump
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
	// End Setup

	intcode(memory);

	free(memory);
}
