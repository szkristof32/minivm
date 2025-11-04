#include "parser.h"

#include "common.h"

#include <stdlib.h>
#include <string.h>

static token_t* _parser_peek(parser_t* parser, uint32_t offset);
static token_t* _parser_consume(parser_t* parser);
static node_t _parser_push_node_generic(parser_t* parser, node_generic_t* node, node_type type);

#define _parser_push_node(parser, node, type) _parser_push_node_generic(parser, (node_generic_t*)node, type)

static void _parser_parse_lit(parser_t* parser);
static void _parser_parse_get(parser_t* parser);
static void _parser_parse_set(parser_t* parser);
static void _parser_parse_put(parser_t* parser);
static void _parser_parse_sub(parser_t* parser);
static void _parser_parse_cgt(parser_t* parser);
static void _parser_parse_jnz(parser_t* parser);
static void _parser_parse_inp(parser_t* parser);
static void _parser_parse_out(parser_t* parser);
static void _parser_parse_end(parser_t* parser);

void parser_parse(token_t* tokens, size_t token_count, parser_t* out_parser)
{
	memset(out_parser, 0, sizeof(parser_t));
	out_parser->tokens = tokens;
	out_parser->token_count = token_count;

	while (_parser_peek(out_parser, 0))
	{
		switch (_parser_peek(out_parser, 0)->type)
		{
			case token_type_keyword_lit: _parser_parse_lit(out_parser); break;
			case token_type_keyword_get: _parser_parse_get(out_parser); break;
			case token_type_keyword_set: _parser_parse_set(out_parser); break;
			case token_type_keyword_put: _parser_parse_put(out_parser); break;
			case token_type_keyword_sub: _parser_parse_sub(out_parser); break;
			case token_type_keyword_cgt: _parser_parse_cgt(out_parser); break;
			case token_type_keyword_jnz: _parser_parse_jnz(out_parser); break;
			case token_type_keyword_inp: _parser_parse_inp(out_parser); break;
			case token_type_keyword_out: _parser_parse_out(out_parser); break;
			case token_type_keyword_end: _parser_parse_end(out_parser); break;
			default: continue;
		}
	}
}

token_t* _parser_peek(parser_t* parser, uint32_t offset)
{
	if (parser->token_index + offset < parser->token_count)
		return &parser->tokens[parser->token_index + offset];
	return NULL;
}

token_t* _parser_consume(parser_t* parser)
{
	if (parser->token_index < parser->token_count)
		return &parser->tokens[parser->token_index++];
	return NULL;
}

node_t _parser_push_node_generic(parser_t* parser, node_generic_t* node, node_type type)
{
	parser->nodes = realloc(parser->nodes, (parser->node_count + 1) * sizeof(node_generic_t));
	parser->nodes[parser->node_count].node_type = type;
	memcpy(&parser->nodes[parser->node_count]._padding, node, sizeof(node->_padding));
	return parser->node_count++;
}

void _parser_parse_lit(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) || _parser_peek(parser, 0)->type != token_type_integer)
		error("Error! Expected integer token!");

	token_t* int_token = _parser_consume(parser);

	node_integer_t int_node = { 0 };
	int_node.value = int_token;
	node_t int_node_ref = _parser_push_node(parser, &int_node, node_type_integer);

	node_lit_t lit_node = { 0 };
	lit_node.operand = int_node_ref;
	_parser_push_node(parser, &lit_node, node_type_lit);
}

void _parser_parse_get(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) || _parser_peek(parser, 0)->type != token_type_integer)
		error("Error! Expected integer token!");

	token_t* int_token = _parser_consume(parser);

	node_integer_t int_node = { 0 };
	int_node.value = int_token;
	node_t int_node_ref = _parser_push_node(parser, &int_node, node_type_integer);

	node_get_t get_node = { 0 };
	get_node.operand = int_node_ref;
	_parser_push_node(parser, &get_node, node_type_get);
}

void _parser_parse_set(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) || _parser_peek(parser, 0)->type != token_type_integer)
		error("Error! Expected integer token!");

	token_t* int_token = _parser_consume(parser);

	node_integer_t int_node = { 0 };
	int_node.value = int_token;
	node_t int_node_ref = _parser_push_node(parser, &int_node, node_type_integer);

	node_set_t set_node = { 0 };
	set_node.operand = int_node_ref;
	_parser_push_node(parser, &set_node, node_type_set);
}

void _parser_parse_put(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) || _parser_peek(parser, 0)->type != token_type_integer)
		error("Error! Expected integer token!");

	token_t* int_token = _parser_consume(parser);

	node_integer_t int_node = { 0 };
	int_node.value = int_token;
	node_t int_node_ref = _parser_push_node(parser, &int_node, node_type_integer);

	node_put_t put_node = { 0 };
	put_node.operand = int_node_ref;
	_parser_push_node(parser, &put_node, node_type_put);
}

void _parser_parse_sub(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) || _parser_peek(parser, 0)->type != token_type_integer)
		error("Error! Expected integer token!");

	token_t* int_token = _parser_consume(parser);

	node_integer_t int_node = { 0 };
	int_node.value = int_token;
	node_t int_node_ref = _parser_push_node(parser, &int_node, node_type_integer);

	node_sub_t sub_node = { 0 };
	sub_node.operand = int_node_ref;
	_parser_push_node(parser, &sub_node, node_type_sub);
}

void _parser_parse_cgt(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) || _parser_peek(parser, 0)->type != token_type_integer)
		error("Error! Expected integer token!");

	token_t* int_token = _parser_consume(parser);

	node_integer_t int_node = { 0 };
	int_node.value = int_token;
	node_t int_node_ref = _parser_push_node(parser, &int_node, node_type_integer);

	node_cgt_t cgt_node = { 0 };
	cgt_node.operand = int_node_ref;
	_parser_push_node(parser, &cgt_node, node_type_cgt);
}

void _parser_parse_jnz(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) || _parser_peek(parser, 0)->type != token_type_integer)
		error("Error! Expected integer token!");

	token_t* int_token = _parser_consume(parser);

	node_integer_t int_node = { 0 };
	int_node.value = int_token;
	node_t int_node_ref = _parser_push_node(parser, &int_node, node_type_integer);

	node_jnz_t jnz_node = { 0 };
	jnz_node.operand = int_node_ref;
	_parser_push_node(parser, &jnz_node, node_type_jnz);
}

void _parser_parse_inp(parser_t* parser)
{
	_parser_consume(parser);
	node_inp_t inp_node = { 0 };
	_parser_push_node(parser, &inp_node, node_type_inp);
}

void _parser_parse_out(parser_t* parser)
{
	_parser_consume(parser);
	node_out_t out_node = { 0 };
	_parser_push_node(parser, &out_node, node_type_out);
}

void _parser_parse_end(parser_t* parser)
{
	_parser_consume(parser);
	node_end_t end_node = { 0 };
	_parser_push_node(parser, &end_node, node_type_end);
}
