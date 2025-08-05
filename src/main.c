#include "../include/common.h"
#include "../include/bytecode/chunk.h"
#include "../include/debug.h"
#include "../include/vm/vm.h"
int main(int argc, const char* argv[]){
    initVM();
    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 1.2);// adds constant to the constant array
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant,123);// writes the constant's index to the byte chunk
    /*
    So after everything ts looks like 
    OP_CONSTANT 0 OP_RETURN
    CST_ARRAY = {1.2} --> THE 1.2 IS AT INDEX 0 SO THE OPERATION CAN KNOW WHAT INDEX
    TO ACCESS NEXT
    */
   constant = addConstant(&chunk,3.4);
   writeChunk(&chunk, OP_CONSTANT, 123);
   writeChunk(&chunk, constant, 123);

   writeChunk(&chunk, OP_ADD, 123);
   constant = addConstant(&chunk,5.6);
   writeChunk(&chunk, OP_CONSTANT,123);
   writeChunk(&chunk, constant, 123);

   writeChunk(&chunk, OP_DIVIDE, 123);
   
   writeChunk(&chunk, OP_NEGATE, 123);
    writeChunk(&chunk, OP_RETURN,123);
    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);
    freeChunk(&chunk);
    freeVM();

    return 0;
}