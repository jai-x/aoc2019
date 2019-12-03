#include <stdio.h>
#include <stdlib.h>

#include "aoc2019.h"

#define LINE_SIZE 256

static const char* filename = "./input/day1.txt";

static int
fuel(int mass)
{
	int fuel = (mass / 3) - 2;
	return (fuel > 0) ? fuel : 0;
}

void
day1(void)
{
	FILE* input = fopen(filename, "r");
	if (!input) {
		printf("Error opening file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	int total_fuel = 0;
	int sum_of_fuel = 0;

	char line[LINE_SIZE];

	while (fgets(line, LINE_SIZE, input)) {
		int mass = atoi(line);

		total_fuel += fuel(mass);

		while (fuel(mass)) {
			mass = fuel(mass);
			sum_of_fuel += mass;
		}
	}

	fclose(input);

	printf("Day 1, Part 1: %d\n", total_fuel);
	printf("Day 1, Part 2: %d\n", sum_of_fuel);
}
