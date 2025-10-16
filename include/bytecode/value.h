#ifndef bryte_value_h

#define bryte_value_h
#include "../common.h"

typedef enum { VAL_BOOL, VAL_NULL, VAL_NUMBER, VAL_OBJ } ValueType;

typedef struct Obj Obj;

typedef struct StringObj StringObj;

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
    Obj *obj;
  } payload;

} Value;
// just for fun this is a pointer to a function that doesnt return anything:
// -->typedef void (*ptr) ()
#define BOOL_VAL(value)                                                        \
  ((Value){.type = VAL_BOOL, .payload = {.boolean = value}})
#define NULL_VAL ((Value){.type = VAL_NULL, .payload = {.number = 0}})
#define NUMBER_VAL(value)                                                      \
  ((Value){.type = VAL_NUMBER, .payload = {.number = value}})
//-->object is a pointer to type Obj
#define OBJ_VAL(object)                                                        \
  ((Value){.type = VAL_OBJ, .payload = {.obj = (Obj *)object}})

#define PAYLOAD_BOOL(value) ((value).payload.boolean)
#define PAYLOAD_NUMBER(value) ((value).payload.number)
#define PAYLOAD_OBJ(value) ((value).payload.obj)

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_NULL(value) ((value).type == VAL_NULL)
#define IS_OBJ(value) ((value).type == VAL_OBJ)

// value is either a bool Value (struct) or NULL
#define MAKE_NOT(value)                                                        \
  (IS_NULL(value) || (IS_BOOL(value) && !PAYLOAD_BOOL(value)))
typedef struct {
  int capacity;
  int count;
  Value *values;

} ValueArray;

void initValueArray(ValueArray *);
void writeValueArray(ValueArray *, Value);
void freeValueArray(ValueArray *);
bool isEqual(Value a, Value b);

void printValue(Value hhalue);

#endif
