#include "common.h"
#include "vm/vm.h"

VM vm;

InterpretResult interpret(Chunk*chunk){
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();

}
static InterpretResult run(){
    #define READ_BYTE() (*vm.ip++)// dereferences vm.ip and moves the pointer more
    for (;;){
        uint8_t instruction;
        switch (instruction = READ_BYTE()){
            case OP_RETURN:{
                return INTERPRET_OK;
            }

        }
    }
    #undef READ_BYTE
}
void initVM(){

}

void freeVM(){

}