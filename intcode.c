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
	printf("\n");
	return input;
}

static inline int
left_val(int* memory, int opcode, int address)
{
	int mode = opcode % 1000 / 100;
	int literal = memory[address + 1];
	return mode ? literal : memory[literal];
}

static inline int
right_val(int* memory, int opcode, int address)
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
			case 1: {
				int store = memory[address + 3];
				int left = left_val(memory, opcode, address);
				int right = right_val(memory, opcode, address);
				memory[store] = left + right;
				address += 4;
				break;
			}
			case 2: {
				int store = memory[address + 3];
				int left = left_val(memory, opcode, address);
				int right = right_val(memory, opcode, address);
				memory[store] = left * right;
				address += 4;
				break;
			}
			case 3: {
				int store = memory[address + 1];
				int input = get_input();
				memory[store] = input;
				address += 2;
				break;
			}
			case 4: {
				int load = memory[address + 1];
				printf("OUTPUT: %d\n", memory[load]);
				address += 2;
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
