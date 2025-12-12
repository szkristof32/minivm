#include "parser.h"

#include "common.h"

#include <stdlib.h>
#include <string.h>

static token_t* _parser_peek(parser_t* parser, uint32_t offset);
static token_t* _parser_consume(parser_t* parser);
static node_t _parser_push_node_generic(parser_t* parser, node_generic_t* node, node_type type);

#define _parser_push_node(parser, node, type) _parser_push_node_generic(parser, (node_generic_t*)node, type)

static node_t _parser_parse_lit(parser_t* parser);
static node_t _parser_parse_get(parser_t* parser);
static node_t _parser_parse_set(parser_t* parser);
static node_t _parser_parse_put(parser_t* parser);
static node_t _parser_parse_sub(parser_t* parser);
static node_t _parser_parse_cgt(parser_t* parser);
static node_t _parser_parse_jnz(parser_t* parser);
static node_t _parser_parse_inp(parser_t* parser);
static node_t _parser_parse_out(parser_t* parser);
static node_t _parser_parse_end(parser_t* parser);
static node_t _parser_parse_db(parser_t* parser);
static node_t _parser_parse_integer(parser_t* parser);
static node_t _parser_parse_string_literal(parser_t* parser);

void parser_parse(token_t* tokens, size_t token_count, parser_t* out_parser)
{
	memset(out_parser, 0, sizeof(parser_t));
	out_parser->tokens = tokens;
	out_parser->token_count = token_count;

	node_t prev_node = -1;

	while (_parser_peek(out_parser, 0))
	{
		node_t node = -1;

		switch (_parser_peek(out_parser, 0)->type)
		{
			case token_type_keyword_lit: node = _parser_parse_lit(out_parser); break;
			case token_type_keyword_get: node = _parser_parse_get(out_parser); break;
			case token_type_keyword_set: node = _parser_parse_set(out_parser); break;
			case token_type_keyword_put: node = _parser_parse_put(out_parser); break;
			case token_type_keyword_sub: node = _parser_parse_sub(out_parser); break;
			case token_type_keyword_cgt: node = _parser_parse_cgt(out_parser); break;
			case token_type_keyword_jnz: node = _parser_parse_jnz(out_parser); break;
			case token_type_keyword_inp: node = _parser_parse_inp(out_parser); break;
			case token_type_keyword_out: node = _parser_parse_out(out_parser); break;
			case token_type_keyword_end: node = _parser_parse_end(out_parser); break;
			case token_type_keyword_db:	 node = _parser_parse_db(out_parser);  break;
			default: continue;
		}

		if (prev_node == -1)
		{
			out_parser->start_node = node;
		}
		else
		{
			node_generic_t* prev = &out_parser->nodes[prev_node];
			prev->next_node = node;
		}
		prev_node = node;
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
	parser->nodes[parser->node_count].next_node = -1;
	memcpy(&parser->nodes[parser->node_count]._padding, node, sizeof(node->_padding));
	return parser->node_count++;
}

node_t _parser_parse_lit(parser_t* parser)
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
	return _parser_push_node(parser, &lit_node, node_type_lit);
}

node_t _parser_parse_get(parser_t* parser)
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
	return _parser_push_node(parser, &get_node, node_type_get);
}

node_t _parser_parse_set(parser_t* parser)
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
	return _parser_push_node(parser, &set_node, node_type_set);
}

node_t _parser_parse_put(parser_t* parser)
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
	return _parser_push_node(parser, &put_node, node_type_put);
}

node_t _parser_parse_sub(parser_t* parser)
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
	return _parser_push_node(parser, &sub_node, node_type_sub);
}

node_t _parser_parse_cgt(parser_t* parser)
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
	return _parser_push_node(parser, &cgt_node, node_type_cgt);
}

node_t _parser_parse_jnz(parser_t* parser)
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
	return _parser_push_node(parser, &jnz_node, node_type_jnz);
}

node_t _parser_parse_inp(parser_t* parser)
{
	_parser_consume(parser);
	node_inp_t inp_node = { 0 };
	return _parser_push_node(parser, &inp_node, node_type_inp);
}

node_t _parser_parse_out(parser_t* parser)
{
	_parser_consume(parser);
	node_out_t out_node = { 0 };
	return _parser_push_node(parser, &out_node, node_type_out);
}

node_t _parser_parse_end(parser_t* parser)
{
	_parser_consume(parser);
	node_end_t end_node = { 0 };
	return _parser_push_node(parser, &end_node, node_type_end);
}

node_t _parser_parse_db(parser_t* parser)
{
	_parser_consume(parser);
	if (!_parser_peek(parser, 0) ||
		(_parser_peek(parser, 0)->type != token_type_integer &&
		_parser_peek(parser, 0)->type != token_type_string_literal))
		return -1;

	token_t* token = _parser_peek(parser, 0);
	node_t first_operand = -1;
	node_t current_operand = -1;
	size_t operand_count = 0;

	switch (token->type)
	{
		case token_type_integer:		first_operand = _parser_parse_integer(parser); break;
		case token_type_string_literal:	first_operand = _parser_parse_string_literal(parser); break;
		default: break;
	}

	current_operand = first_operand;

	operand_count++;

	while (_parser_peek(parser, 0) && _parser_peek(parser, 0)->type == token_type_comma)
	{
		_parser_consume(parser);
		if (_parser_peek(parser, 0) && (
			_parser_peek(parser, 0)->type == token_type_integer ||
			_parser_peek(parser, 0)->type == token_type_string_literal))
		{
			token = _parser_peek(parser, 0);

			node_t operand = -1;
			switch (token->type)
			{
				case token_type_integer:		operand = _parser_parse_integer(parser); break;
				case token_type_string_literal:	operand = _parser_parse_string_literal(parser); break;
				default: break;
			}
			operand_count++;

			node_generic_t* last_operand = &parser->nodes[current_operand];
			last_operand->next_node = operand;
			current_operand = operand;
		}
	}

	node_db_t db_node = { 0 };
	db_node.operands = first_operand;
	db_node.operand_count = operand_count;
	return _parser_push_node(parser, &db_node, node_type_db);
}

node_t _parser_parse_integer(parser_t* parser)
{
	token_t* token = _parser_consume(parser);
	node_integer_t integer_node = { 0 };
	integer_node.value = token;
	return _parser_push_node(parser, &integer_node, node_type_integer);
}

node_t _parser_parse_string_literal(parser_t* parser)
{
	token_t* token = _parser_consume(parser);
	node_string_t string_node = { 0 };
	string_node.value = token;
	return _parser_push_node(parser, &string_node, node_type_string);
}
