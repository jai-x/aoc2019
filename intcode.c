#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#include "intcode.h"

void
intcode(int* memory)
{
	size_t address = 0;

	while (true) {
		int opcode = memory[address];

		switch (opcode) {
			case 1: {
				int left  = memory[address + 1];
				int right = memory[address + 2];
				int store = memory[address + 3];
				memory[store] = memory[left] + memory[right];
				address += 4;
				break;
			}
			case 2: {
				int left  = memory[address + 1];
				int right = memory[address + 2];
				int store = memory[address + 3];
				memory[store] = memory[left] * memory[right];
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
