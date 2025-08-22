#include "../../include/bytecode/value.h"
#include "../../include/memory.h"
#include "../../include/bytecode/object.h"
#include <string.h>


#include <stdio.h>

void initValueArray(ValueArray* array){
    array->capacity = 0;
    array->count = 0;
    array->values = NULL;
}

void writeValueArray(ValueArray*array, Value value){
    // if the capacity is the same as count then we expand it
    
    if (array->capacity == array->count){
        // grow capacity
        int oldCap = array->capacity;
        array->capacity = GROW_CAPACITY(array->capacity);
        array->values = grow_array(sizeof(Value),array->values, oldCap, array->capacity);

    }
    // now to add the elements themselves
    array->values[array->count] = value;
    array->count += 1;
    
}
void freeValueArray(ValueArray*array){
    //first free the values, then free the struct itself
    //by initializing it again
    free(array->values);
    initValueArray(array);
}

bool isEqual(Value a, Value b){
    if (a.type != b.type){
        return false;
    }
    switch (a.type){
        case VAL_BOOL: return PAYLOAD_BOOL(a) == PAYLOAD_BOOL(b);
        case VAL_NULL: return true;
        case VAL_NUMBER: return PAYLOAD_NUMBER(a) == PAYLOAD_NUMBER(b);

        case VAL_OBJ:{
            StringObj* aString = PAYLOAD_STRING(a);
            StringObj* bString = PAYLOAD_STRING(b);
            return aString->length == bString->length && strcmp(aString->chars,bString->chars)==0 ;

        }

        default: return false;
    }
}


void printValue(Value value){
    switch (value.type){
        case VAL_BOOL:
            printf(value.payload.boolean? "true": "false");
            break;
        case VAL_NULL: printf("NULL"); break;
        case VAL_NUMBER:
            printf("%g", PAYLOAD_NUMBER(value));
        case VAL_OBJ:
            printObject(value);
    }
    
}