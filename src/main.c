#include "../include/common.h"
#include "../include/chunk.h"
#include "../include/debug.h"
int main(int argc, const char* argv[]){
    Chunk chunk;
    initChunk(&chunk);
    writeChunk(&chunk, OP_RETURN);
    freeChunk(&chunk);

    return 0;
}