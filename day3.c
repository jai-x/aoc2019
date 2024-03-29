#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "aoc2019.h"

#define MAX_LINE_SIZE 1500
#define MAX_PATH_SIZE 170000

static const char* filename = "./input/day3.txt";

// Point struct
typedef struct { int x; int y; } vec2i;

/* Debug
static void
vec2i_print(vec2i v)
{
	printf("vec2i, x: %d, y: %d\n", v.x, v.y);
}
*/

static inline vec2i
vec2i_add(vec2i a, vec2i b)
{
	return (vec2i) { a.x + b.x, a.y + b.y };
}

static inline bool
vec2i_eql(vec2i a, vec2i b)
{
	return (a.x == b.x) && (a.y == b.y);
}

static inline bool
vec2i_origin(vec2i v)
{
	return (v.x == 0) && (v.y == 0);
}

static inline int
vec2i_origin_manhatten(vec2i v)
{
	return abs(v.x) + abs(v.y);
}

static inline int
min(int a, int b)
{
	return (a > b) ? b : a;
}

static size_t
make_path(char* wire_str, vec2i* path)
{
	path[0] = (vec2i) { 0, 0 };
	size_t current_vec = 1;

	char* wire_str_tail = wire_str;
	char* token = NULL;

	while ((token = strsep(&wire_str_tail, ","))) {
		// Get movement size
		int movement_size = atoi(&token[1]);

		// Determine movement direction
		vec2i movement_offset;
		char direction = token[0];

		switch (direction) {
		case 'R':
			movement_offset = (vec2i) { 1, 0 };
			break;
		case 'L':
			movement_offset = (vec2i) { -1, 0 };
			break;
		case 'U':
			movement_offset = (vec2i) { 0, 1 };
			break;
		case 'D':
			movement_offset = (vec2i) { 0, -1 };
			break;
		default:
			printf("Unknown movement: %s\n", token);
			exit(EXIT_FAILURE);
		}

		while (movement_size > 0) {
			if (current_vec > MAX_PATH_SIZE) {
				printf("Not enough path space! Allocate more: %lu\n", current_vec);
				exit(EXIT_FAILURE);
			}

			path[current_vec] = vec2i_add(path[current_vec - 1], movement_offset);
			current_vec++;
			movement_size--;
		}
	}

	return current_vec;
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

	// First wire string into wire path
	vec2i* wire_1_path = malloc(sizeof(vec2i) * MAX_PATH_SIZE);
	size_t wire_1_path_size = make_path(wire_1_str, wire_1_path);

	// Second wire string into wire path
	vec2i* wire_2_path = malloc(sizeof(vec2i) * MAX_PATH_SIZE);
	size_t wire_2_path_size = make_path(wire_2_str, wire_2_path);

	// Deallocate wire strings
	free(wire_1_str);
	free(wire_2_str);

	// Part 1 & 2
	int min_distance = INT_MAX;
	int min_steps = INT_MAX;

	// Very brute force!
	for (size_t i = 1; i < wire_1_path_size; i++) {
		for (size_t j = 1; j < wire_2_path_size; j++) {
			// Ignore if coordinate is the origin
			if (vec2i_origin(wire_1_path[i]) || vec2i_origin(wire_2_path[j])) {
				continue;
			}

			// Intersection found
			if (vec2i_eql(wire_1_path[i], wire_2_path[j])) {
				// Store minimal manhatten distance from origin to this intersection
				min_distance = min(min_distance, vec2i_origin_manhatten(wire_1_path[i]));
				// Store minimum combined steps to reach this intersection
				min_steps = min(min_steps, (int) i + (int) j);
			}
		}
	}

	printf("Day 3, Part 1: %d\n", min_distance);
	printf("Day 3, Part 2: %d\n", min_steps);

	// Deallocate wire path
	free(wire_1_path);
	free(wire_2_path);
}
