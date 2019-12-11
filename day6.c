#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "aoc2019.h"

static const char* filename = "./input/day6.txt";

// Orbit pair struct
struct orbit_pair {
	char* src;
	char* dst;
};

static struct orbit_pair*
make_orbit_pair(char* src, char* dst)
{
	struct orbit_pair* p = malloc(sizeof(struct orbit_pair));
	p->src = strdup(src);
	p->dst = strdup(dst);
	return p;
}

// Orbit pair list
struct orbit_pair_list {
	size_t num_pairs;
	struct orbit_pair** pairs;
};

// Orbit pair list functions
static struct orbit_pair_list*
make_orbit_pair_list(FILE* file)
{
	struct orbit_pair_list* l = malloc(sizeof(struct orbit_pair_list));

	// stack allocated char array so no need for free
	char line[9];

	// find the number of lines
	while (fgets(line, sizeof(line), file)) {
		l->num_pairs++;
	}
	rewind(file);

	// allocate based on number of lines
	l->pairs = malloc(sizeof(struct orbit_pair*) * l->num_pairs);

	// parse the pairs
	size_t pair_count = 0;
	while (fgets(line, sizeof(line), file)) {
		// remove newline char
		line[7] = '\0';

		// make substrings from the line
		char* dst = line;
		char* src = strsep(&dst, ")");

		l->pairs[pair_count++] = make_orbit_pair(src, dst);
	}

	return l;
}

static void
free_orbit_pair_list(struct orbit_pair_list* l)
{
	for (size_t i = 0; i < l->num_pairs; i++) {
		free(l->pairs[i]->src);
		free(l->pairs[i]->dst);
		free(l->pairs[i]);
	}
	free(l->pairs);
	free(l);
}

static void
print_orbit_pair_list(struct orbit_pair_list* l)
{
	for (size_t i = 0; i < l->num_pairs; i++) {
		printf("%s -> %s\n", l->pairs[i]->src, l->pairs[i]->dst);
	}
}

// Planet tree node
struct planet {
	char* name;
	size_t orbits;
	size_t num_children;
	struct planet** children;
	struct planet* parent;
};

/*
static void
print_planet(struct planet* p)
{
	printf("%s -> o: %zu, c: %zu\n", p->name, p->orbits, p->num_children);
}
*/

// Planet tree functions
static struct planet*
make_planet_node(char* name, size_t orbits, struct planet* parent)
{
	struct planet* p = malloc(sizeof(struct planet));

	p->name = strdup(name);
	p->orbits = orbits;
	p->num_children = 0;
	p->children = NULL;
	p->parent = parent;

	return p;
}

static void
make_planet_children(struct orbit_pair_list* list, struct planet* node)
{
	for (size_t i = 0; i < list->num_pairs; i++) {
		struct orbit_pair* pair = list->pairs[i];

		if (strcmp(pair->src, node->name) == 0) {
			node->children = realloc(node->children, sizeof(struct planet*) + (node->num_children + 1));
			struct planet* child = make_planet_node(pair->dst, node->orbits + 1, node);
			node->children[node->num_children++] = child;
		}
	}

	for (size_t i = 0; i < node->num_children; i++) {
		make_planet_children(list, node->children[i]);
	}
}

static struct planet*
make_planet_tree(struct orbit_pair_list* list, char* start)
{
	bool exists = false;

	for (size_t i = 0; i < list->num_pairs; i++) {
		struct orbit_pair* pair = list->pairs[i];

		if (strcmp(pair->src, start) == 0) {
			exists = true;
		}
	}

	if (!exists) {
		printf("Starting point does not exist in orbit pairs: %s\n", start);
		print_orbit_pair_list(list);
		return NULL;
	}

	struct planet* root = make_planet_node(start, 0, NULL);
	make_planet_children(list, root);
	return root;
}

static void
free_planet_tree(struct planet* node)
{
	for (size_t i = 0; i < node->num_children; i++) {
		free_planet_tree(node->children[i]);
	}

	free(node->children);
	free(node->name);
	free(node);
}

/*
static void
print_planet_tree(struct planet* node, int indent)
{
	printf("%*s", indent, "");
	print_planet(node);

	for (size_t i = 0; i < node->num_children; i++) {
		print_planet_tree(node->children[i], indent + 2);
	}
}
*/

// Task functions
static size_t
total_orbits(struct planet* node)
{
	size_t children_total = 0;

	for (size_t i = 0; i < node->num_children; i++) {
		children_total += total_orbits(node->children[i]);
	}

	return children_total + node->orbits;
}

static struct planet*
find_planet(struct planet* node, char* to_find)
{
	if (strcmp(node->name, to_find) == 0) {
		return node;
	}

	for (size_t i = 0; i < node->num_children; i++) {
		struct planet* result = find_planet(node->children[i], to_find);
		if (result) {
			return result;
		}
	}

	return NULL;
}

static struct planet*
find_common_parent(struct planet* a, struct planet* b)
{
	if (a == b) {
		return a;
	}

	struct planet* a_parent = a->parent;
	struct planet* b_parent = b->parent;

	while (a_parent) {
		while (b_parent) {
			if (a_parent == b_parent) {
				return a_parent;
			}

			b_parent = b_parent->parent;
		}

		a_parent = a_parent->parent;
		b_parent = b->parent;
	}

	return NULL;
}

void
day6(void)
{
	FILE* file = fopen(filename, "r");
	assert(file);

	struct orbit_pair_list* list = make_orbit_pair_list(file);
	struct planet* com_tree = make_planet_tree(list, "COM");

	fclose(file);

	printf("Day 6, Part 1: %zu\n", total_orbits(com_tree));

	struct planet* you = find_planet(com_tree, "YOU");
	struct planet* san = find_planet(com_tree, "SAN");

	struct planet* ys_parent = find_common_parent(you, san);

	struct planet* ys_tree = make_planet_tree(list, ys_parent->name);

	struct planet* ys_you = find_planet(ys_tree, "YOU");
	struct planet* ys_san = find_planet(ys_tree, "SAN");

	// Subtract 2, to exclude the counted orbit of YOU and SAN themselves
	printf("Day 6, Part 2: %zu\n", ys_you->orbits + ys_san-> orbits - 2);

	free_planet_tree(ys_tree);
	free_planet_tree(com_tree);
	free_orbit_pair_list(list);
}
