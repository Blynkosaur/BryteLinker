#ifndef bryte_value_h

#define bryte_value_h
#include "../common.h"

typedef double Value;

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