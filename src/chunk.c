#include <stdlib.h>
#include "../include/chunk.h"
#include "../include/memory.h"





void initChunk(Chunk* chunk){
    chunk->count = 0;
    chunk -> capacity = 0;
    chunk ->code = NULL;
}

void * grow_array(size_t size_of_type, void * pointer, int oldCount, int newCount){
    //to use: pointer = grow_array smt machin chouette grosse merde
    if (newCount == 0){
        free(pointer);
        return NULL;
    }
    pointer = realloc(pointer, (newCount*size_of_type));
    if(pointer == NULL){
        exit(1);
    }
    return pointer;

}
void freeChunk(Chunk*chunk){
    free(chunk->code);
    initChunk(chunk);
    
}

void writeChunk(Chunk* chunk, u_int8_t byte){
    // grow everything
    if (chunk->capacity < chunk->count +1){
        int oldCapacity = chunk->capacity;
        chunk -> capacity = GROW_CAPACITY(oldCapacity);
        chunk-> code = grow_array(sizeof(u_int8_t), chunk->code, oldCapacity, chunk->capacity);
        // chunk->code is a pointer to an array of int
    }
    chunk->code[chunk->count] = byte;
    chunk->count ++;

}
