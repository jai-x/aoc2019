#pragma once

char* dump_file(FILE*);
size_t parse_memory_size(char*);
int* parse_memory(char*, size_t);
void intcode(int*, int*, size_t, int*, size_t);
