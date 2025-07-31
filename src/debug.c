#include <stdio.h>
#include "../include/debug.h"


static int simpleInstruction(const char*, int);
static int constantInstruction (const char* name, Chunk* chunk, int offset);
void disassembleChunk(Chunk*chunk, const char *name){
    printf("== %s ==\n", name);
    for (int offset = 0; offset< chunk -> count;){
        offset = disassembleInstruction(chunk,offset);// basically increments offset according to size

    }
}
int disassembleInstruction (Chunk*chunk, int offset){
    printf("%04d ", offset);
    if (offset>0 && chunk->lines[offset] == chunk->lines[offset-1]){
        printf("   | ");

    }else{
        printf("%4d ", chunk->lines[offset]);
    }
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
    u_int8_t constant = chunk->code[offset+1]; // constant is the index of the constant we want ig
    printf("%-16s %4d => ",name, constant);// prints: OP_CONSTANT (some index)
    printValue(chunk->constants.values[constant]); // prints the actual constant
    printf("\n");
    return offset +2;

}
