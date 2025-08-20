#ifndef bryte_value_h

#define bryte_value_h
#include "../common.h"


typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER
} ValueType;

typedef struct{
    ValueType type;
    union{
        bool boolean;
        double number;
    }payload;

}Value;

#define BOOL_VAL(value) ((Value) {.type = VAL_BOOL , .payload = {.boolean = value}})
#define NIL_VAL ((Value) {.type = VAL_NIL, .payload = {.number = 0}})
#define NUMBER_VAL(value) ((Value) {.type = VAL_NUMBER, .payload = {.number = value}})
typedef struct{
    int capacity;
    int count;
    Value* values;

} ValueArray;

void initValueArray(ValueArray*);
void writeValueArray(ValueArray*, Value);
void freeValueArray(ValueArray*);

void printValue(Value value);

#endif