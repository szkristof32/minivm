#pragma once

#include <stdint.h>

typedef enum token_type
{
	token_type_none,
	token_type_keyword_lit,
	token_type_keyword_get,
	token_type_keyword_set,
	token_type_keyword_put,
	token_type_keyword_sub,
	token_type_keyword_cgt,
	token_type_keyword_jnz,
	token_type_keyword_inp,
	token_type_keyword_out,
	token_type_keyword_end,
	token_type_integer,
	token_type_square_brackets_open,
	token_type_square_brackets_close
} token_type;

typedef struct token_t
{
	token_type type;
	uint8_t* data;
} token_t;

typedef struct tokeniser_t
{
	const char* source;
	size_t source_size;

	token_t* tokens;
	size_t token_count;
	size_t pointer;
} tokeniser_t;

void tokeniser_tokenise(const char* source, tokeniser_t* out_tokeniser);
void tokeniser_destroy(tokeniser_t* tokeniser);
