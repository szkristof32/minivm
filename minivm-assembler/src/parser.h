#pragma once

#include "tokeniser.h"

typedef size_t node_t;

typedef struct node_lit_t
{
	node_t operand;
} node_lit_t;

typedef struct node_get_t
{
	node_t operand;
} node_get_t;

typedef struct node_set_t
{
	node_t operand;
} node_set_t;

typedef struct node_put_t
{
	node_t operand;
} node_put_t;

typedef struct node_sub_t
{
	node_t operand;
} node_sub_t;

typedef struct node_cgt_t
{
	node_t operand;
} node_cgt_t;

typedef struct node_jnz_t
{
	node_t operand;
} node_jnz_t;

typedef struct node_inp_t
{
	uint8_t _padding;
} node_inp_t;

typedef struct node_out_t
{
	uint8_t _padding;
} node_out_t;

typedef struct node_end_t
{
	uint8_t _padding;
} node_end_t;

typedef struct node_integer_t
{
	token_t* value;
} node_integer_t;

typedef enum node_type
{
	node_type_none,
	node_type_lit,
	node_type_get,
	node_type_set,
	node_type_put,
	node_type_sub,
	node_type_cgt,
	node_type_jnz,
	node_type_inp,
	node_type_out,
	node_type_end,
	node_type_integer,
} node_type;

typedef struct node_generic_t
{
	token_type node_type;
	uint64_t _padding;
	node_t next_node;
} node_generic_t;

typedef struct parser_t
{
	token_t* tokens;
	size_t token_count;
	size_t token_index;

	node_generic_t* nodes;
	size_t node_count;
	node_t start_node;
} parser_t;

void parser_parse(token_t* tokens, size_t token_count, parser_t* out_parser);
