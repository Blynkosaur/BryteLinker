#ifndef bryte_chunk_h

#define bryte_chunk_h
#include "../common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct{
    int count;
    int capacity;
    int* lines;
    ValueArray constants;
    uint8_t* code;
} Chunk;

void initChunk(Chunk* chunk);

void writeChunk(Chunk* chunk, u_int8_t byte, int line);

void freeChunk(Chunk*chunk);

int addConstant(Chunk*, Value value);
#endif