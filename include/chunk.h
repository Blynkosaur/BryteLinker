#ifndef bryte_chunk_h

#define bryte_chunk_h
#include "common.h"

typedef enum {
    OP_RETURN,
} OpCode;

typedef struct{
    int count;
    int capacity;
    uint8_t* code;
} Chunk;

void initChunk(Chunk* chunk);

void writeChunk(Chunk* chunk, u_int8_t byte);

void freeChunk(Chunk*chunk);
#endif