#include <stdlib.h>
#include "../include/chunk.h"
#include "../include/memory.h"





void initChunk(Chunk* chunk){
    chunk->count = 0;
    chunk -> capacity = 0;
    chunk ->code = NULL;
}

void * grow_array(size_t size_of_type, void * pointer, int oldCount, int newCount){
    //FILL THIS SHIT OUT TOMORROW
}

void writeChunk(Chunk* chunk, u_int8_t byte){
    // grow everything
    if (chunk->capacity < chunk->count +1){
        int oldCapacity = chunk->capacity;
        chunk -> capacity = GROW_CAPACITY(oldCapacity);
        chunk-> code = growArray(sizeof(u_int8_t), chunk->code, oldCapacity, chunk->capacity);

    }
    chunk->code[chunk->count] = byte;
    chunk->count ++;

}
