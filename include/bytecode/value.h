#ifndef bryte_value_h

#define bryte_value_h
#include "../common.h"


typedef enum {
    VAL_BOOL,
    VAL_NULL,
    VAL_NUMBER
} ValueType;

typedef struct{
    ValueType type;
    union{
        bool boolean;
        double number;
    }payload;

}Value;
// just for fun this is a pointer to a function that doesnt return anything: -->typedef void (*ptr) ()
#define BOOL_VAL(value) ((Value) {.type = VAL_BOOL , .payload = {.boolean = value}})
#define NULL_VAL ((Value) {.type = VAL_NULL, .payload = {.number = 0}})
#define NUMBER_VAL(value) ((Value) {.type = VAL_NUMBER, .payload = {.number = value}})


#define PAYLOAD_BOOL(value) ((value).payload.boolean)
#define PAYLOAD_NUMBER(value) ((value).payload.number)

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_NULL(value) ((value).type == VAL_NULL)

//value is either a bool Value (struct) or NULL
#define MAKE_NOT(value) (IS_NULL(value) || (IS_BOOL(value) && !value.payload.boolean))
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