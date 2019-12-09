#include <stdlib.h>
#include <stdio.h>

#include "aoc2019.h"

int
main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Usage: ./aoc2019 {day_num}\n");
		return EXIT_FAILURE;
	}

	char day = argv[1][0];
	switch (day) {
	case '1':
		day1();
		break;
	case '2':
		day2();
		break;
	case '3':
		printf("TODO: Make Day 3 faster...\n");
		day3();
		break;
	case '4':
		day4();
		break;
	case '5':
		day5();
		break;
	default:
		printf("No solution for Day %c\n", day);
	}
	return EXIT_SUCCESS;
}
