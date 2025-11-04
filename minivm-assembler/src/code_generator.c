#include "code_generator.h"

#include "common.h"

#include <stdlib.h>
#include <string.h>

static void _code_generator_resize_code(code_generator_t* generator, size_t new_size);

static void _code_generator_generate_lit(code_generator_t* generator, node_lit_t* node);
static void _code_generator_generate_get(code_generator_t* generator, node_get_t* node);
static void _code_generator_generate_set(code_generator_t* generator, node_set_t* node);
static void _code_generator_generate_put(code_generator_t* generator, node_put_t* node);
static void _code_generator_generate_sub(code_generator_t* generator, node_sub_t* node);
static void _code_generator_generate_cgt(code_generator_t* generator, node_cgt_t* node);
static void _code_generator_generate_jnz(code_generator_t* generator, node_jnz_t* node);
static void _code_generator_generate_inp(code_generator_t* generator, node_inp_t* node);
static void _code_generator_generate_out(code_generator_t* generator, node_out_t* node);
static void _code_generator_generate_end(code_generator_t* generator, node_end_t* node);

void code_generator_generate(node_generic_t* nodes, size_t node_count, node_t first_node, code_generator_t* out_generator)
{
	memset(out_generator, 0, sizeof(code_generator_t));
	out_generator->nodes = nodes;
	out_generator->node_count = node_count;

	node_t current_node = first_node;

	while (current_node != -1)
	{
		node_generic_t* node = &out_generator->nodes[current_node];

		switch (node->node_type)
		{
			case node_type_lit:	_code_generator_generate_lit(out_generator, (node_lit_t*)&node->_padding); break;
			case node_type_get:	_code_generator_generate_get(out_generator, (node_get_t*)&node->_padding); break;
			case node_type_set:	_code_generator_generate_set(out_generator, (node_set_t*)&node->_padding); break;
			case node_type_put:	_code_generator_generate_put(out_generator, (node_put_t*)&node->_padding); break;
			case node_type_sub:	_code_generator_generate_sub(out_generator, (node_sub_t*)&node->_padding); break;
			case node_type_cgt:	_code_generator_generate_cgt(out_generator, (node_cgt_t*)&node->_padding); break;
			case node_type_jnz:	_code_generator_generate_jnz(out_generator, (node_jnz_t*)&node->_padding); break;
			case node_type_inp:	_code_generator_generate_inp(out_generator, (node_inp_t*)&node->_padding); break;
			case node_type_out:	_code_generator_generate_out(out_generator, (node_out_t*)&node->_padding); break;
			case node_type_end:	_code_generator_generate_end(out_generator, (node_end_t*)&node->_padding); break;
			default: continue;
		}

		current_node = node->next_node;
	}
}

void _code_generator_resize_code(code_generator_t* generator, size_t new_size)
{
	generator->code = realloc(generator->code, new_size);
	generator->code_size = new_size;
}

void _code_generator_generate_lit(code_generator_t* generator, node_lit_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 2);
	generator->code[generator->code_index++] = 0x0;

	node_integer_t* integer = (node_integer_t*)&generator->nodes[node->operand]._padding;
	generator->code[generator->code_index++] = atoi((char*)integer->value->data);
}

void _code_generator_generate_get(code_generator_t* generator, node_get_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 2);
	generator->code[generator->code_index++] = 0x1;

	node_integer_t* integer = (node_integer_t*)&generator->nodes[node->operand]._padding;
	generator->code[generator->code_index++] = atoi((char*)integer->value->data);
}

void _code_generator_generate_set(code_generator_t* generator, node_set_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 2);
	generator->code[generator->code_index++] = 0x2;

	node_integer_t* integer = (node_integer_t*)&generator->nodes[node->operand]._padding;
	generator->code[generator->code_index++] = atoi((char*)integer->value->data);
}

void _code_generator_generate_put(code_generator_t* generator, node_put_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 2);
	generator->code[generator->code_index++] = 0x3;

	node_integer_t* integer = (node_integer_t*)&generator->nodes[node->operand]._padding;
	generator->code[generator->code_index++] = atoi((char*)integer->value->data);
}

void _code_generator_generate_sub(code_generator_t* generator, node_sub_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 2);
	generator->code[generator->code_index++] = 0x4;

	node_integer_t* integer = (node_integer_t*)&generator->nodes[node->operand]._padding;
	generator->code[generator->code_index++] = atoi((char*)integer->value->data);
}

void _code_generator_generate_cgt(code_generator_t* generator, node_cgt_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 2);
	generator->code[generator->code_index++] = 0x5;

	node_integer_t* integer = (node_integer_t*)&generator->nodes[node->operand]._padding;
	generator->code[generator->code_index++] = atoi((char*)integer->value->data);
}

void _code_generator_generate_jnz(code_generator_t* generator, node_jnz_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 2);
	generator->code[generator->code_index++] = 0x6;

	node_integer_t* integer = (node_integer_t*)&generator->nodes[node->operand]._padding;
	generator->code[generator->code_index++] = atoi((char*)integer->value->data);
}

void _code_generator_generate_inp(code_generator_t* generator, node_inp_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 1);
	generator->code[generator->code_index++] = 0x7;
}

void _code_generator_generate_out(code_generator_t* generator, node_out_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 1);
	generator->code[generator->code_index++] = 0x8;
}

void _code_generator_generate_end(code_generator_t* generator, node_end_t* node)
{
	_code_generator_resize_code(generator, generator->code_size + 1);
	generator->code[generator->code_index++] = 0x9;
}
