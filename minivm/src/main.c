#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "virtual_machine.h"

#define error(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");

int main(int argc, char** argv)
{
	const char* filepath = NULL;
	machine_configuration_t configuration = { 0 };
	configuration.memory_size = 256;

	for (uint32_t i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--file") == 0)
		{
			if (i + 1 >= argc)
			{
				error("No filepath specified!");
				return -1;
			}

			filepath = argv[i + 1];
			i++;
			continue;
		}
		if (strcmp(argv[i], "--memory") == 0)
		{
			if (i + 1 >= argc)
			{
				error("No memory size specified!");
				return -1;
			}

			configuration.memory_size = atoi(argv[i + 1]);
			i++;
			continue;
		}
	}

	if (filepath == NULL)
	{
		error("No input file specified!");
		return -1;
	}

	machine_t machine;
	machine_create(&configuration, &machine);

	{
		FILE* file = fopen(filepath, "rb");
		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		fseek(file, 0, SEEK_SET);
		fread(&machine.memory[0x10], sizeof(uint8_t), size, file);
		fclose(file);
	}

	machine_run(&machine);

	machine_destroy(&machine);
}
