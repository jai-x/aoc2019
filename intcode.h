#pragma once

struct intcode_io {
	size_t num_io;
	int* io;
};

struct intcode_io* init_intcode_io(size_t);
void free_intcode_io(struct intcode_io*);

char* dump_file(FILE*);
size_t parse_memory_size(char*);
int* parse_memory(char*, size_t);

void intcode(int*, struct intcode_io*, struct intcode_io*);
