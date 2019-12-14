#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "aoc2019.h"
#include "intcode.h"

static const char* filename = "./input/day7.txt";

static inline void
swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline int
max(int a, int b)
{
	return (a > b) ? a : b;
}

static inline void
print_array_5(int array[5])
{
	printf("%d, %d, %d, %d, %d\n", array[0], array[1], array[2], array[3], array[4]);
}

// Globals ;_;
int max_signal = 0;
int* memory = NULL;
int* memory_copy = NULL;
size_t memory_size = 0;

static void
amplifiers(int phase[5])
{
	struct intcode_io* input = init_intcode_io(2);
	struct intcode_io* output = init_intcode_io(1);

	int signal = 0;

	for (size_t amp = 0; amp < 5; amp++) {
		input->io[0] = phase[amp];
		input->io[1] = signal;

		memcpy(memory_copy, memory, sizeof(int) * memory_size);
		intcode(memory_copy, input, output);

		signal = output->io[0];
	}

	max_signal = max(max_signal, signal);

	free_intcode_io(input);
	free_intcode_io(output);
}

static void
permutations(int phase[5], size_t start, size_t end)
{
	if (start == end) {
		amplifiers(phase);
		return;
	}

	for (size_t i = start; i <= end; i++) {
		swap(&phase[start], &phase[i]);
		permutations(phase, start + 1, end);
		swap(&phase[start], &phase[i]);
	}
}

void
day7(void)
{
	FILE* file = fopen(filename, "r");
	assert(file);
	char* file_buffer = dump_file(file);
	fclose(file);

	memory_size = parse_memory_size(file_buffer);
	assert(memory_size);

	memory = parse_memory(file_buffer, memory_size);
	assert(memory);

	memory_copy = malloc(sizeof(int) * memory_size);

	free(file_buffer);

	// Part 1
	int phase[5] = { 0, 1, 2, 3, 4 };
	permutations(phase, 0, 4);

	printf("Day 7, Part 1: %d\n", max_signal);

	free(memory_copy);
	free(memory);
}
