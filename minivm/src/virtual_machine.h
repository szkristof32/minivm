#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct machine_configuration_t
{
	size_t memory_size;
} machine_configuration_t;

typedef struct machine_t
{
	machine_configuration_t configuration;
	uint8_t* memory;
	size_t program_counter;
	
	uint8_t instruction_register;
	uint64_t flags_register;
} machine_t;

bool machine_create(const machine_configuration_t* configuration, machine_t* out_machine);
void machine_destroy(machine_t* machine);

void machine_run(machine_t* machine);
