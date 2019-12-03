#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "aoc2019.h"

#define MAX_LINE_SIZE 1500

static const char* filename = "./input/day3.txt";

// Point struct
struct point { int x; int y; };

static void
point_print(struct point p)
{
	printf("Point, x: %d, y: %d\n", p.x, p.y);
}

// Path struct
struct path { struct point begin; struct point end; };

static void
path_print(struct path p)
{
	printf("Path, begin.x: %d, begin.y: %d, end.x: %d, end.y: %d\n",
			p.begin.x, p.begin.y, p.end.x, p.end.y);
}

static bool
path_intersect(struct path a, struct path b, struct point* intersect)
{
	(void)a;
	(void)b;
	(void)intersect;
	return false;
}

static size_t
num_paths(char* wire_str)
{
	size_t num_commas = 0;
	for (size_t i = 0; i < strlen(wire_str); i++) {
		if (wire_str[i] == ',') {
			num_commas++;
		}
	}
	return num_commas + 1;
}

static void
make_paths(struct path* paths, char* wire_str)
{
	char* wire_str_tail = wire_str;
	char* token = NULL;
	size_t pos = 0;

	while ((token = strsep(&wire_str_tail, ","))) {
		char direction = token[0];
		int movement = atoi(&token[1]);

		// This path starts at the end of the the last path
		struct point this_begin = (pos == 0) ?
			(struct point) { 0, 0 } :
			paths[pos - 1].end;

		// Apply movement offset to the beginning point of this path to get the end point
		struct point this_end = this_begin;

		switch (direction) {
		case 'R':
			this_end.x += movement;
			break;
		case 'L':
			this_end.x -= movement;
			break;
		case 'U':
			this_end.y += movement;
			break;
		case 'D':
			this_end.y -= movement;
			break;
		}

		paths[pos++] = (struct path) { this_begin, this_end };
	}
}

void
day3(void)
{
	// Open file
	FILE* input = fopen(filename, "r");
	if (!input) {
		printf("Error opening file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	// Buffer space
	char line[MAX_LINE_SIZE] = "";
	size_t line_size = 0;

	// Allocate first wire string
	fgets(line, MAX_LINE_SIZE, input);
	line_size = strlen(line);
	char* wire_1_str = malloc(sizeof(char) * line_size);
	memcpy(wire_1_str, line, line_size);

	// Allocate second wire string
	fgets(line, MAX_LINE_SIZE, input);
	line_size = strlen(line);
	char* wire_2_str = malloc(sizeof(char) * line_size);
	memcpy(wire_2_str, line, line_size);

	// Close file
	fclose(input);

	// First wire string into wire paths
	size_t wire_1_num_paths = num_paths(wire_1_str);
	struct path* wire_1_paths = malloc(sizeof(struct path) * wire_1_num_paths);
	make_paths(wire_1_paths, wire_1_str);

	// Second wire string into wire paths
	size_t wire_2_num_paths = num_paths(wire_2_str);
	struct path* wire_2_paths = malloc(sizeof(struct path) * wire_2_num_paths);
	make_paths(wire_2_paths, wire_2_str);

	// Deallocate wire strings
	free(wire_1_str);
	free(wire_2_str);

	// Part 1

	// Part 2

	// Deallocate wire paths
	free(wire_1_paths);
	free(wire_2_paths);
}
