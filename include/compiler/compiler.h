#ifndef bryte_compiler_h

#define bryte_compiler_h
#include "../common.h"
#include "../bytecode/chunk.h"

bool compile(const char* source, Chunk*chunk);

#endif