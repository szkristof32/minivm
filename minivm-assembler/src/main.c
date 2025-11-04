#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "tokeniser.h"

#define error(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");

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

	FILE* file = fopen(filepath, "rb");
	if (!file)
	{
		error("Input file doesn't exists!");
		return -1;
	}

	fseek(file, 0, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buffer = (char*)malloc((size + 1) * sizeof(char));
	memset(buffer, 0, (size + 1) * sizeof(char));
	
	fread(buffer, sizeof(char), size, file);
	fclose(file);

	tokeniser_t tokeniser;
	tokeniser_tokenise(buffer, &tokeniser);

	free(buffer);
}
