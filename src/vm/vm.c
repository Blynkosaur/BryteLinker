#include "../../include/common.h"
#include "../../include/vm/vm.h"
#include "../../include/debug.h"
#include <stdio.h>


VM vm;


static InterpretResult run(){
    #define READ_BYTE() (*vm.ip++)// dereferences vm.ip and moves the pointer more
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    for (;;){
        #ifdef DEBUG_TRACE_EXECUTION
        disassembleInstruction(vm.chunk, (int)(vm.ip-vm.chunk->code));
        #endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()){
            case OP_CONSTANT:{
                Value constant = READ_CONSTANT();
                printValue (constant);
                printf("\n");
                break;

            }
            case OP_RETURN:{
                return INTERPRET_OK;
            }

        }
    }
    #undef READ_BYTE
    #undef READ_CONSTANT
}
InterpretResult interpret(Chunk*chunk){
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run(); 

}
void initVM(){

}

void freeVM(){

}