#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "intcode.h"

static int
get_input(void)
{
	int input;
	printf("INPUT: ");
	scanf("%d", &input);
	printf("\n");
	return input;
}

void
intcode(int* memory)
{
	size_t address = 0;

	while (true) {
		int opcode = memory[address];

		int inst = opcode % 100;

		int left_mode = opcode % 1000 / 100;
		int right_mode = opcode % 10000 / 1000;

		int left_val = left_mode ? memory[address + 1] : memory[memory[address + 1]];
		int right_val = right_mode ? memory[address + 2] : memory[memory[address + 2]];

		switch (inst) {
			case 1: {
				int store = memory[address + 3];
				memory[store] = left_val + right_val;
				address += 4;
				break;
			}
			case 2: {
				int store = memory[address + 3];
				memory[store] = left_val * right_val;
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
