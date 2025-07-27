#include "../include/value.h"
#include "../include/memory.h"


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