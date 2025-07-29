#include "../include/common.h"
#include "../include/chunk.h"
#include "../include/debug.h"
int main(int argc, const char* argv[]){
    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 1.2);// adds constant to the constant array
    writeChunk(&chunk, "OP_CONSTANT");
    writeChunk(&chunk, constant);// writes the constant's index to the byte chunk
    /*
    So after everything ts looks like 
    OP_CONSTANT 0 OP_RETURN
    CST_ARRAY = {1.2} --> THE 1.2 IS AT INDEX 0 SO THE OPERATION CAN KNOW WHAT INDEX
    TO ACCESS NEXT
    */
    writeChunk(&chunk, OP_RETURN);
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);

    return 0;
}