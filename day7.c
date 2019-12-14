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

static int
amplifiers(int phase[5], int* memory, size_t memory_size)
{
	// memory for each amp
	int* amp_memory[5];

	// allocate and copy from original mem
	for (size_t amp = 0; amp < 5; amp++) {
		amp_memory[amp] = calloc(memory_size, sizeof(int));
		memcpy(amp_memory[amp], memory, memory_size);
	}

	// allocate i/o for amps
	struct intcode_io* input = init_intcode_io(2);
	struct intcode_io* output = init_intcode_io(1);

	int signal = 0;

	// loop all amps
	for (size_t amp = 0; amp < 5; amp++) {
		// assign phase and signal to input
		input->io[0] = phase[amp];
		input->io[1] = signal;

		// run program for amp
		intcode(amp_memory[amp], input, output);

		// assign output to signal for use on next amp
		signal = output->io[0];
	}

	// free i/o for amps
	free_intcode_io(input);
	free_intcode_io(output);

	// free memory for each amp
	for (size_t amp = 0; amp < 5; amp++) {
		free(amp_memory[amp]);
	}

	return signal;
}

static int
permutations(int phase[5], size_t start, size_t end, int* memory, size_t memory_size)
{
	if (start == end) {
		return amplifiers(phase, memory, memory_size);
	}

	int max_signal = 0;

	for (size_t i = start; i <= end; i++) {
		swap(&phase[start], &phase[i]);

		int signal = permutations(phase, start + 1, end, memory, memory_size);

		max_signal = max(max_signal, signal);

		swap(&phase[start], &phase[i]);
	}

	return max_signal;
}


void
day7(void)
{
	size_t memory_size = 0;
	int* memory = NULL;
	FILE* file = NULL;
	char* file_buffer;

	file = fopen(filename, "r");
	assert(file);

	file_buffer = dump_file(file);
	assert(file_buffer);

	memory_size = parse_memory_size(file_buffer);
	assert(memory_size);

	memory = parse_memory(file_buffer, memory_size);
	assert(memory);

	// Part 1
	int p1_phase[5] = { 0, 1, 2, 3, 4 };
	int p1_max_signal = permutations(p1_phase, 0, 4, memory, memory_size);
	printf("Day 7, Part 1: %d\n", p1_max_signal);

	free(memory);
	free(file_buffer);
	fclose(file);
}
