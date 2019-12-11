#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "aoc2019.h"

// static const char* filename = "./input/day6.txt";

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

struct orbit_pair_list {
	size_t num_pairs;
	struct orbit_pair** pairs;
};

static struct orbit_pair_list*
make_orbit_pair_list(void)
{
	struct orbit_pair_list* l = malloc(sizeof(struct orbit_pair_list));
	l->num_pairs = 11;
	l->pairs = malloc(sizeof(struct orbit_pair*) * l->num_pairs);

	// test input from website
	l->pairs[0]  = make_orbit_pair("COM", "BBB");
	l->pairs[1]  = make_orbit_pair("BBB", "CCC");
	l->pairs[2]  = make_orbit_pair("CCC", "DDD");
	l->pairs[3]  = make_orbit_pair("DDD", "EEE");
	l->pairs[4]  = make_orbit_pair("EEE", "FFF");
	l->pairs[5]  = make_orbit_pair("BBB", "GGG");
	l->pairs[6]  = make_orbit_pair("GGG", "HHH");
	l->pairs[7]  = make_orbit_pair("DDD", "III");
	l->pairs[8]  = make_orbit_pair("EEE", "JJJ");
	l->pairs[9]  = make_orbit_pair("JJJ", "KKK");
	l->pairs[10] = make_orbit_pair("KKK", "LLL");

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


struct planet {
	char* name;
	size_t orbits;
	size_t num_children;
	struct planet** children;
};

static struct planet*
make_planet_node(char* name, size_t orbits)
{
	struct planet* p = malloc(sizeof(struct planet));

	p->name = strdup(name);
	p->orbits = orbits;
	p->num_children = 0;
	p->children = NULL;

	return p;
}

static void
make_planet_children(struct planet* node, struct orbit_pair_list* list)
{
	for (size_t i = 0; i < list->num_pairs; i++) {
		struct orbit_pair* pair = list->pairs[i];

		if (strcmp(pair->src, node->name) == 0) {
			node->children = realloc(node->children, sizeof(struct planet*) + (node->num_children + 1));
			struct planet* child = make_planet_node(pair->dst, node->orbits + 1);
			node->children[node->num_children++] = child;
		}
	}

	for (size_t i = 0; i < node->num_children; i++) {
		make_planet_children(node->children[i], list);
	}
}

static struct planet*
make_planet_tree(char* start, struct orbit_pair_list* list)
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

	struct planet* root = make_planet_node(start, 0);
	make_planet_children(root, list);
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

static void
print_planet_tree(struct planet* node, int indent)
{
	printf("%*s, o: %zu, c: %zu\n", indent, node->name, node->orbits, node->num_children);
	for (size_t i = 0; i < node->num_children; i++) {
		print_planet_tree(node->children[i], indent + 5);
	}
}

static size_t
planet_tree_total_orbits(struct planet* node)
{
	size_t children_total = 0;

	for (size_t i = 0; i < node->num_children; i++) {
		children_total += planet_tree_total_orbits(node->children[i]);
	}

	return children_total + node->orbits;
}

void
day6(void)
{
	struct orbit_pair_list* list = make_orbit_pair_list();
	struct planet* com = make_planet_tree("COM", list);

	size_t total_orbits = planet_tree_total_orbits(com);
	printf("Day 6, Part 1: %zu\n", total_orbits);

	free_planet_tree(com);
	free_orbit_pair_list(list);
}
