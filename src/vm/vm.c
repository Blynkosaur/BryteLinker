#include "../../include/common.h"
#include "../../include/vm/vm.h"
#include "../../include/debug.h"
#include "../../include/compiler/compiler.h"
#include <stdarg.h>
#include <stdio.h>


VM vm;

static Value peek(int distance){
    return vm.stackTop[-1-distance];
}
static void resetStack(){
    vm.stackTop = vm.stack;
}
static void runtimeError(const char* format, ...){
    va_list args;
    va_start(args,format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip -vm.chunk->code -1;
    int line = vm.chunk -> lines[instruction];
    fprintf(stderr, "[line %d] in script \n", line);
    resetStack();
}


static InterpretResult run(){
    #define READ_BYTE() (*vm.ip++)// dereferences vm.ip (index pointer) and moves the pointer more
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    #define BINARY_OP(valueType,op) do{\
        if (!IS_NUMBER(peek(0))|| !IS_NUMBER(peek(1))){\
            runtimeError("Operands have to be numbers.");\
        }\
        double a = PAYLOAD_NUMBER(pop());\
        double b = PAYLOAD_NUMBER(pop());\
        push(valueType(b op a));\
    } while(false)
    printf("\n-----INTERPRETING-----\n\n");
    for (;;){
         #ifdef DEBUG_TRACE_EXECUTION // only for debugging
        disassembleInstruction(vm.chunk, (int)(vm.ip-vm.chunk->code));
        
        
        #endif
        uint8_t instruction;
        switch(instruction = READ_BYTE()){
            case OP_CONSTANT:{
                Value constant = READ_CONSTANT();
                printf("PUSHED: ");
                printValue(constant);
                push(constant);
                printf("\n");
                break;

            }
            case OP_ADD:{
                BINARY_OP(NUMBER_VAL,+);
                break;
            }
            case OP_SUBSTRACT:{
                BINARY_OP(NUMBER_VAL,-);
                break;
            }
            case OP_MULITPLY:{
                BINARY_OP(NUMBER_VAL,*);
                break;
            }
            case OP_DIVIDE:{
                BINARY_OP(NUMBER_VAL,/);
                break;
            }
            case OP_NEGATE:{
                
                // push(-pop());]
                if(!IS_NUMBER(peek(0))){
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                //stackTop = pointer to a value
                (*(vm.stackTop-1)).payload.number *= -1; // goofy aaah shit
                break;
            }
            case OP_NULL:{
                push(NULL_VAL);
                break;
            }
            case OP_FALSE:{
                push(BOOL_VAL(false));
                break;
            }
            case OP_TRUE:{
                push(BOOL_VAL(true));
                break;
            }
            case OP_RETURN:{
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
            default:{
                runtimeError("Unknown opcode %d", instruction);
                return INTERPRET_RUNTIME_ERROR;
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
InterpretResult interpret(const char* source){
    Chunk chunk;
    initChunk(&chunk);
    if(!compile(source,&chunk)){
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;
    InterpretResult result = run();
    freeChunk(&chunk);
    return result;
    
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