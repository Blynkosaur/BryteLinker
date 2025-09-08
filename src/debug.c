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

        case OP_PRINT:
            return simpleInstruction("OP_PRINT", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE",offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD",offset);
        case OP_SUBSTRACT:
            return simpleInstruction("OP_SUBSTRACT",offset);
        case OP_MULITPLY:
            return simpleInstruction("OP_MULTIPLY",offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE",offset);
        case OP_NOT:
            return simpleInstruction("OP_NOT",offset);
        case OP_NULL:
            return simpleInstruction("OP_NULL",offset);
        case OP_TRUE:
            return simpleInstruction("OP_TRUE", offset);
        case OP_FALSE:
            return simpleInstruction("OP_FALSE", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_EQUAL:
            return simpleInstruction("OP_EQUAL", offset);
        case OP_GREATER:
            return simpleInstruction("OP_GREATER", offset);
        case OP_LESS:
            return simpleInstruction("OP_LESS", offset);

        
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
