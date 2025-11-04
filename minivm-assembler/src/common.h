#pragma once

#include <stdio.h>
#include "debugmalloc.h"

#define error(...) do { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } while (0);
