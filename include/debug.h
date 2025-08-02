#ifndef bryte_debug_h
#define bryte_debug_h

#include "bytecode/chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);

#endif