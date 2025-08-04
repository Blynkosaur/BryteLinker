#ifndef bryte_vm_h
#define bryte_vm_h
#include "../bytecode/chunk.h"
#include "../../include/bytecode/value.h"
#define STACK_MAX 256
typedef struct{
    Chunk*chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value *stackTop;

}VM;

void push(Value value);
Value pop();


typedef enum{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR

}InterpretResult;
InterpretResult interpret(Chunk* chunk);
void initVM();
void freeVM();
#endif