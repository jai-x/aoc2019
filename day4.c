#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "aoc2019.h"

static bool
increasing(char* num_str)
{
	for (int i = 1; i < 6; i++) {
		if (num_str[i - 1] > num_str[i]) {
			return false;
		}
	}
	return true;
}


static bool
adjacent(char* num_str)
{
	for (int i = 1; i < 6; i++) {
		if (num_str[i - 1] == num_str[i]) {
			return true;
		}
	}
	return false;
}

static bool
pair_adjacent(char* num_str) {
	int groupings[6] = {
		0, // singles
		0, // pairs
		0, // triples
		0, // quads
		0, // quints
		0, // sixts
	};
	int group_type = 0;

	for (int i = 1; i < 6; i++) {
		if (num_str[i - 1] == num_str[i]) {
			group_type++;
		} else {
			groupings[group_type]++;
			group_type = 0;
		}
	}
	groupings[group_type]++;

	/*
	printf("singles: %d\n", groupings[0]);
	printf("pairs:   %d\n", groupings[1]);
	printf("triples: %d\n", groupings[2]);
	printf("quads:   %d\n", groupings[3]);
	printf("quints:  %d\n", groupings[4]);
	printf("sixts:   %d\n", groupings[5]);
	*/

	// At least one pair grouping present
	return groupings[1] > 0;
}

void
day4(void)
{
	// Inputs
	const int lower = 178416;
	const int upper = 676461;

	char num_str[7];
	int part_1_combos = 0;
	int part_2_combos = 0;

	for (int num = lower; num < upper; num++) {
		sprintf(num_str, "%d", num);

		bool inc = increasing(num_str);

		// Part 1
		if (inc && adjacent(num_str)) {
			part_1_combos++;
		}
	
		// Part 2
		if (inc && pair_adjacent(num_str)) {
			part_2_combos++;
		}
	}

	printf("Day 4, Part 1: %d\n", part_1_combos);
	printf("Day 4, Part 2: %d\n", part_2_combos);
}
