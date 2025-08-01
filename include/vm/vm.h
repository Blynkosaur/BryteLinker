#ifndef bryte_vm_h
#define bryte_vm_h
#include "chunk.h"
typedef struct{
    Chunk*chunk;

}VM;
void initVM();
void freeVM();
#endif