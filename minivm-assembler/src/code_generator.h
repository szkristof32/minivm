#pragma once

#include "parser.h"

typedef struct code_generator_t
{
	node_generic_t* nodes;
	size_t node_count;

	uint8_t* code;
	size_t code_size;
	size_t code_index;
} code_generator_t;

void code_generator_generate(node_generic_t* nodes, size_t node_count, node_t first_node, code_generator_t* out_generator);
