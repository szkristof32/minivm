#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "tokeniser.h"
#include "parser.h"
#include "code_generator.h"

int main(int argc, char** argv)
{
	const char* filepath = NULL;
	const char* output = "out.mvm";

	for (uint32_t i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			if (i + 1 >= argc)
			{
				error("No filepath specified!");
				return -1;
			}

			output = argv[i + 1];
			i++;
			continue;
		}
		filepath = argv[i];
	}

	FILE* file_in = fopen(filepath, "rb");
	if (!file_in)
	{
		error("Input file doesn't exists!");
		return -1;
	}

	fseek(file_in, 0, SEEK_END);
	size_t size = ftell(file_in);
	fseek(file_in, 0, SEEK_SET);

	char* buffer = (char*)malloc((size + 1) * sizeof(char));
	memset(buffer, 0, (size + 1) * sizeof(char));

	fread(buffer, sizeof(char), size, file_in);
	fclose(file_in);

	tokeniser_t tokeniser;
	tokeniser_tokenise(buffer, &tokeniser);

	parser_t parser;
	parser_parse(tokeniser.tokens, tokeniser.token_count, &parser);

	code_generator_t generator;
	code_generator_generate(parser.nodes, parser.node_count, parser.start_node, &generator);

	FILE* file_out = fopen(output, "wb");
	if (!file_in)
	{
		error("Cannot open output file!");
		goto cleanup;
	}

	fwrite(generator.code, sizeof(uint8_t), generator.code_size, file_out);
	fflush(file_out);
	fclose(file_out);

cleanup:
	free(generator.code);
	free(parser.nodes);
	tokeniser_destroy(&tokeniser);
	free(buffer);
}
