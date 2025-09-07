#include "../../include/common.h"
#include "../../include/vm/vm.h"
#include "../../include/debug.h"
#include "../../include/compiler/compiler.h"
#include "../../include/bytecode/object.h"
#include "../../include/memory.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


VM vm;

static Value peek(int distance)
{
    return vm.stackTop[-1 - distance];
}
static void resetStack()
{
    vm.stackTop = vm.stack;
}
static void runtimeError(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->code - 1;
    int line = vm.chunk->lines[instruction];
    fprintf(stderr, "[line %d] in script \n", line);
    resetStack();
}
static bool isFalsey(Value value) {
  return IS_NULL(value) || (IS_BOOL(value) && !PAYLOAD_BOOL(value));
}

static void concatenate(){
    StringObj * last = PAYLOAD_STRING(pop());
    StringObj * second_last = PAYLOAD_STRING(pop());
    int new_len = last->length  + second_last->length;
    char* concat = malloc(sizeof(char)*new_len +1);
    strcpy(concat, second_last->chars);
    strcat(concat, last -> chars);
    concat[new_len] = '\0';

    StringObj* result = makeObjWithString(concat,new_len);
    push(OBJ_VAL(result));
    

}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++) // dereferences vm.ip (index pointer) and moves the pointer more
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op)                          \
    do                                                    \
    {                                                     \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)))   \
        {                                                 \
            runtimeError("Operands have to be numbers."); \
        }                                                 \
        double a = PAYLOAD_NUMBER(pop());                 \
        double b = PAYLOAD_NUMBER(pop());                 \
        push(valueType(b op a));                          \
    } while (false)

    printf("\n-----INTERPRETING-----\n\n");
    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION // only for debugging
        disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));

#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            printf("PUSHED: ");
            printValue(constant);
            push(constant);
            printf("\n");
            break;
        }
        case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
        case OP_LESS: BINARY_OP(BOOL_VAL, <); break;
        case OP_ADD:
        {   
            if(IS_STRING(peek(0)) && IS_STRING(peek(1))){
                concatenate();
                
            }else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))){
                double last = PAYLOAD_NUMBER(pop());
                double second_last = PAYLOAD_NUMBER(pop());
                push(NUMBER_VAL(last+second_last));

            }else{
                runtimeError("Operands must be two numbers (addition) or two strings (concatenation)");
                return INTERPRET_RUNTIME_ERROR;
            }
            // BINARY_OP(NUMBER_VAL, +);
            break;
        }
        case OP_SUBSTRACT:
        {
            BINARY_OP(NUMBER_VAL, -);
            break;
        }
        case OP_MULITPLY:
        {
            BINARY_OP(NUMBER_VAL, *);
            break;
        }
        case OP_DIVIDE:
        {   
            
            BINARY_OP(NUMBER_VAL, /);
            break;
        }
        case OP_NEGATE:
        {

            // push(-pop());]
            if (!IS_NUMBER(peek(0)))
            {
                runtimeError("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            // stackTop = pointer to a value
            (*(vm.stackTop - 1)).payload.number *= -1; // goofy aaah shit
            break;
        }
        case OP_NULL:
        {
            push(NULL_VAL);
            break;
        }
        case OP_FALSE:
        {
            push(BOOL_VAL(false));
            break;
        }
        case OP_TRUE:
        {
            push(BOOL_VAL(true));
            break;
        }
        case OP_EQUAL:{
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(isEqual(a,b)));
            break;
        }
        case OP_NOT:
        {
            Value last = pop();
            Value new = BOOL_VAL(MAKE_NOT(last));
            push(new);
            break;
        }
        case OP_PRINT:
        {
            printValue(pop());
            printf("\n");
            break;
        }
        case OP_RETURN:
        {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
        }
#ifdef DEBUG_TRACE_EXECUTION
        printf("\n         ");
        printf("VM stack:");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++)
        {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n\n");
        printf("VM table: ");
        if ((&vm.strings)->entries == NULL || (&vm.strings)->capacity == 0) {
        printf("Table is empty\n");
        continue;   
    }
    
    Entry** entries = vm.strings.entries;
    for (int i = 0; i < vm.strings.capacity; i++){
        if(entries[i] != NULL && entries[i]->key != NULL){
            printf("[ ");
            printf("%s %p %p", entries[i]->key->chars, entries[i]->key, entries[i] );
            printf(" ]");
        }
    }
    printf("\n");
#endif
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}
InterpretResult interpret(const char *source)
{
    Chunk chunk;
    initChunk(&chunk);
    if (!compile(source, &chunk))
    {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;
    InterpretResult result = run();
    freeChunk(&chunk);
    return result;
}

void initVM()
{   
    vm.objectsHead = NULL;
    resetStack();
    initTable(&vm.strings);
}

void push(Value value)
{
    *(vm.stackTop) = value;
    vm.stackTop++;
}
Value pop()
{
    vm.stackTop--; // never explicitly removes the last element
    Value value = *(vm.stackTop);
    return value;
}
void freeVM()
{
    freeObjects();
    freeTable(&vm.strings);
}
// typedef struct{
//     Chunk*chunk;
//     uint8_t* ip;
//     Value stack[STACK_MAX];
//     Value *stackTop;

// }VM;
