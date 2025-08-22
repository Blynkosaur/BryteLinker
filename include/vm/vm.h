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
    Obj* objectsHead;

}VM;

extern VM vm;

void push(Value value);
Value pop();


typedef enum{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR

}InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);

#endif