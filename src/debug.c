#include <stdio.h>
#include "../include/debug.h"


static int simpleInstruction(const char*, int);

void disassembleChunk(Chunk*chunk, const char *name){
    printf("== %s ==\n", name);
    for (int offset = 0; offset< chunk -> count;){
        offset = disassembleInstruction(chunk,offset);// basically increments offset according to size

    }
}
int disassembleInstruction (Chunk*chunk, int offset){
    printf("%04d ", offset);
    u_int8_t instruction = chunk->code[offset];
    switch(instruction){
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);

        case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknowd opcode %d\n", instruction);
            return offset+1;
    }
}
static int simpleInstruction(const char* name, int offset){
    printf("%s\n", name);
    return offset +1;
}

static int constantInstruction (const char* name, Chunk* chunk, int offset){
    int i = 2;
}