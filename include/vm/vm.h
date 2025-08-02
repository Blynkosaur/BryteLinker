#ifndef bryte_vm_h
#define bryte_vm_h
#include "chunk.h"
typedef struct{
    Chunk*chunk;
    uint8_t* ip;

}VM;
typedef enum{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR

}InterpretResult;
InterpretResult interpret(Chunk* chunk);
void initVM();
void freeVM();
#endif