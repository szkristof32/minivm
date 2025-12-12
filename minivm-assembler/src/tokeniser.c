#include "tokeniser.h"

#include "debugmalloc.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static char _tokeniser_peek(tokeniser_t* tokeniser, uint32_t offset);
static char _tokeniser_consume(tokeniser_t* tokeniser);
static void _tokeniser_push_token(tokeniser_t* tokeniser, token_t* token);

void tokeniser_tokenise(const char* source, tokeniser_t* out_tokeniser)
{
	memset(out_tokeniser, 0, sizeof(tokeniser_t));
	out_tokeniser->source = source;
	out_tokeniser->source_size = strlen(out_tokeniser->source);

	while (_tokeniser_peek(out_tokeniser, 0))
	{
		if (isalpha(_tokeniser_peek(out_tokeniser, 0)))
		{
			char buffer[16] = { 0 };
			size_t buf_ptr = 0;
			while (_tokeniser_peek(out_tokeniser, 0) && isalpha(_tokeniser_peek(out_tokeniser, 0)))
				buffer[buf_ptr++] = tolower(_tokeniser_consume(out_tokeniser));

#define TOKEN_KEYWORD(keyword) \
if (strcmp(buffer, #keyword) == 0) \
{ \
	token_t token = { 0 }; \
	token.type = token_type_keyword_##keyword; \
	_tokeniser_push_token(out_tokeniser, &token); \
	continue; \
}
			TOKEN_KEYWORD(lit);
			TOKEN_KEYWORD(get);
			TOKEN_KEYWORD(set);
			TOKEN_KEYWORD(put);
			TOKEN_KEYWORD(sub);
			TOKEN_KEYWORD(cgt);
			TOKEN_KEYWORD(jnz);
			TOKEN_KEYWORD(inp);
			TOKEN_KEYWORD(out);
			TOKEN_KEYWORD(end);
			TOKEN_KEYWORD(db);
		}
		if (isdigit(_tokeniser_peek(out_tokeniser, 0)))
		{
			uint8_t radix = 10;
			if (_tokeniser_peek(out_tokeniser, 0) == '0' && _tokeniser_peek(out_tokeniser, 1) == 'x')
			{
				radix = 16;
				_tokeniser_consume(out_tokeniser);
				_tokeniser_consume(out_tokeniser);
			}

			char buffer[255] = { radix };
			size_t buf_ptr = 1;
			while (_tokeniser_peek(out_tokeniser, 0) && (isdigit(_tokeniser_peek(out_tokeniser, 0)) ||
				tolower(_tokeniser_peek(out_tokeniser, 0)) >= 'a' && tolower(_tokeniser_peek(out_tokeniser, 0)) <= 'f'))
				buffer[buf_ptr++] = _tokeniser_consume(out_tokeniser);

			token_t token = { 0 };
			token.type = token_type_integer;
			token.data = (uint8_t*)malloc((buf_ptr + 1) * sizeof(char));
			memset(token.data, 0, (buf_ptr + 1) * sizeof(char));
			memcpy(token.data, buffer, buf_ptr * sizeof(char));

			_tokeniser_push_token(out_tokeniser, &token);
			continue;
		}
		if (_tokeniser_peek(out_tokeniser, 0) == '[')
		{
			token_t token = { 0 };
			token.type = token_type_square_brackets_open;

			_tokeniser_consume(out_tokeniser);
			_tokeniser_push_token(out_tokeniser, &token);
			continue;
		}
		if (_tokeniser_peek(out_tokeniser, 0) == ']')
		{
			token_t token = { 0 };
			token.type = token_type_square_brackets_close;

			_tokeniser_consume(out_tokeniser);
			_tokeniser_push_token(out_tokeniser, &token);
			continue;
		}
		if (_tokeniser_peek(out_tokeniser, 0) == ',')
		{
			token_t token = { 0 };
			token.type = token_type_comma;

			_tokeniser_consume(out_tokeniser);
			_tokeniser_push_token(out_tokeniser, &token);
			continue;
		}
		if (_tokeniser_peek(out_tokeniser, 0) == '"')
		{
			char buffer[256] = { 0 };
			size_t buf_ptr = 0;
			_tokeniser_consume(out_tokeniser);

			while (_tokeniser_peek(out_tokeniser, 0) && _tokeniser_peek(out_tokeniser, 0) != '"')
				buffer[buf_ptr++] = _tokeniser_consume(out_tokeniser);

			_tokeniser_consume(out_tokeniser);
			
			token_t token = { 0 };
			token.type = token_type_string_literal;
			token.data = (uint8_t*)malloc((buf_ptr + 1) * sizeof(char));
			memset(token.data, 0, (buf_ptr + 1) * sizeof(char));
			memcpy(token.data, buffer, buf_ptr * sizeof(char));

			_tokeniser_push_token(out_tokeniser, &token);
			continue;
		}
		_tokeniser_consume(out_tokeniser);
	}
}

void tokeniser_destroy(tokeniser_t* tokeniser)
{
	for (uint32_t i = 0; i < tokeniser->token_count; i++)
	{
		if (tokeniser->tokens[i].type == token_type_integer)
			free(tokeniser->tokens[i].data);
	}

	free(tokeniser->tokens);
}

char _tokeniser_peek(tokeniser_t* tokeniser, uint32_t offset)
{
	if (tokeniser->pointer + offset < tokeniser->source_size)
		return tokeniser->source[tokeniser->pointer + offset];
	return '\0';
}

char _tokeniser_consume(tokeniser_t* tokeniser)
{
	if (tokeniser->pointer < tokeniser->source_size)
		return tokeniser->source[tokeniser->pointer++];
	return '\0';
}

void _tokeniser_push_token(tokeniser_t* tokeniser, token_t* token)
{
	tokeniser->tokens = realloc(tokeniser->tokens, (tokeniser->token_count + 1) * sizeof(token_t));
	memcpy(&tokeniser->tokens[tokeniser->token_count], token, sizeof(token_t));
	tokeniser->token_count++;
}
