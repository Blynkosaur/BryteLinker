#include "../../include/common.h"
#include "../../include/vm/vm.h"
#include "../../include/debug.h"
#include <stdio.h>


VM vm;


static InterpretResult run(){
    #define READ_BYTE() (*vm.ip++)// dereferences vm.ip and moves the pointer more
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    #define BINARY_OP(op) do{\
        Value a = pop();\
        Value b = pop();\
        push(b op a);\
    } while(false)
    printf("\n-----INTERPRETING-----\n\n");
    for (;;){
         #ifdef DEBUG_TRACE_EXECUTION // only for debugging
        disassembleInstruction(vm.chunk, (int)(vm.ip-vm.chunk->code));
        
        
        #endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()){
            case OP_CONSTANT:{
                Value constant = READ_CONSTANT();
                printf("PUSHED: ");
                printValue (constant);
                push(constant);
                printf("\n");
                break;

            }
            case OP_ADD:{
                BINARY_OP(+);
                break;
            }
            case OP_SUBSTRACT:{
                BINARY_OP(-);
                break;
            }
            case OP_MULITPLY:{
                BINARY_OP(*);
                break;
            }
            case OP_DIVIDE:{
                BINARY_OP(/);
                break;
            }
            case OP_NEGATE:{
                
                // push(-pop());
                *(vm.stackTop-1) *= -1;
                break;
            }
            case OP_RETURN:{
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }

        }
        #ifdef DEBUG_TRACE_EXECUTION
        printf("\n         ");

        for(Value*slot = vm.stack; slot<vm.stackTop; slot++){
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n\n");
        #endif
    

    }
    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef BINARY_OP
}
InterpretResult interpret(Chunk*chunk){
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run(); 

}
static void resetStack(){
    vm.stackTop = vm.stack;
}
void initVM(){
    resetStack();

}

void push(Value value){
    *(vm.stackTop) = value;
    vm.stackTop++;

}
Value pop(){
    vm.stackTop--;//never explicitly removes the last element
    Value value = *(vm.stackTop);
    return value;


}
void freeVM(){

}
// typedef struct{
//     Chunk*chunk;
//     uint8_t* ip;
//     Value stack[STACK_MAX];
//     Value *stackTop;

// }VM;