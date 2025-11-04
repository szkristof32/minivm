#include "virtual_machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _lit(machine_t* machine);
static void _get(machine_t* machine);
static void _set(machine_t* machine);
static void _put(machine_t* machine);
static void _sub(machine_t* machine);
static void _cgt(machine_t* machine);
static void _jnz(machine_t* machine);
static void _inp(machine_t* machine);
static void _out(machine_t* machine);
static void _end(machine_t* machine);
static void _crash(machine_t* machine);

#define BIT(x) (1 << x)

typedef enum opcode_t
{
	opcode_lit,
	opcode_get,
	opcode_set,
	opcode_put,
	opcode_sub,
	opcode_cgt,
	opcode_jnz,
	opcode_inp,
	opcode_out,
	opcode_end
} opcode_t;

typedef enum flags_t
{
	flags_exit = BIT(0)
} flags_t;

bool machine_create(const machine_configuration_t* configuration, machine_t* out_machine)
{
	memset(out_machine, 0, sizeof(machine_t));
	memcpy(&out_machine->configuration, configuration, sizeof(machine_configuration_t));

	out_machine->memory = malloc(configuration->memory_size);
	if (!out_machine->memory)
		return false;

	return true;
}

void machine_destroy(machine_t* machine)
{
	free(machine->memory);
	memset(machine, 0, sizeof(machine_t));
}

void machine_run(machine_t* machine)
{
	machine->program_counter = 0x10;

	while (!machine->flags_register & flags_exit)
	{
		machine->instruction_register = machine->memory[machine->program_counter];

		switch (machine->instruction_register)
		{
			case opcode_lit:	_lit(machine); break;
			case opcode_get:	_get(machine); break;
			case opcode_set:	_set(machine); break;
			case opcode_put:	_put(machine); break;
			case opcode_sub:	_sub(machine); break;
			case opcode_cgt:	_cgt(machine); break;
			case opcode_jnz:	_jnz(machine); break;
			case opcode_inp:	_inp(machine); break;
			case opcode_out:	_out(machine); break;
			case opcode_end:	_end(machine); break;
			default:			_crash(machine); break;
		}
	}
}

void _lit(machine_t* machine)
{
	memcpy(&machine->memory[0], &machine->memory[machine->program_counter + sizeof(uint8_t)], sizeof(uint8_t));
	machine->program_counter += 2 * sizeof(uint8_t);
}

void _get(machine_t* machine)
{
	uint8_t address = machine->memory[machine->program_counter + sizeof(uint8_t)];
	memcpy(&machine->memory[0], &machine->memory[machine->memory[address]], sizeof(uint8_t));
	machine->program_counter += 2 * sizeof(uint8_t);
}

void _set(machine_t* machine)
{
	uint8_t address = machine->memory[machine->program_counter + sizeof(uint8_t)];
	memcpy(&machine->memory[address], &machine->memory[0], sizeof(uint8_t));
	machine->program_counter += 2 * sizeof(uint8_t);
}

void _put(machine_t* machine)
{
	uint8_t address = machine->memory[machine->program_counter + sizeof(uint8_t)];
	memcpy(&machine->memory[machine->memory[address]], &machine->memory[0], sizeof(uint8_t));
	machine->program_counter += 2 * sizeof(uint8_t);
}

void _sub(machine_t* machine)
{
	uint8_t address = machine->memory[machine->program_counter + sizeof(uint8_t)];
	uint8_t result = machine->memory[address] - machine->memory[0];
	machine->memory[0] = result;
	machine->program_counter += 2 * sizeof(uint8_t);
}

void _cgt(machine_t* machine)
{
	uint8_t address = machine->memory[machine->program_counter + sizeof(uint8_t)];
	if (machine->memory[0] > machine->memory[address])
		memset(&machine->memory[0], 0, sizeof(uint8_t));
	machine->program_counter += 2 * sizeof(uint8_t);
}

void _jnz(machine_t* machine)
{
	if (machine->memory[0] != 0)
		machine->program_counter = machine->memory[machine->program_counter + sizeof(uint8_t)];
	else
		machine->program_counter += 2 * sizeof(uint8_t);
}

void _inp(machine_t* machine)
{
	machine->memory[0] = getc(stdin);
	machine->program_counter += sizeof(uint8_t);
}

void _out(machine_t* machine)
{
	putc(machine->memory[0], stdout);
	machine->program_counter += sizeof(uint8_t);
}

void _end(machine_t* machine)
{
	machine->flags_register |= flags_exit;
	machine->program_counter += sizeof(uint8_t);
}

void _crash(machine_t* machine)
{
	printf("Segmentation fault!\n");
	machine->flags_register |= flags_exit;
}
