#define _DEFAULT_SOURCE

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "intcode.h"

static inline int
get_input(void)
{
	int input;
	printf("INPUT: ");
	scanf("%d", &input);
	return input;
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
intcode(int* memory)
{
	int address = 0;

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
				int input = get_input();
				memory[store] = input;
				address += 2;
				break;
			}
			// Output first value
			case 4: {
				int output = first_val(memory, opcode, address);
				printf("OUTPUT: %d\n", output);
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
