#include <stdio.h>
#include <string.h>

#include "../../include/memory.h"
#include "../../include/bytecode/object.h"
#include "../../include/bytecode/value.h"
#include "../../include/vm/vm.h"


// #define ALLOCATE_OBJ(type, objectType) ((type*) allocateObject(sizeof(type), objectType))

// static Obj * allocateObject (size_t size, ObjType type){
//     Obj* object = malloc(size);
//     object->type = type;
//     return object;
// }

//FOR THE RECORD, IDK WHY THE CODE ABOVE IS EVEN THERE WHY SO COMPLICATED, JUST CALL MALLOC: IT AIN'T THAT DEEP
static StringObj* allocateString(char* chars, int length){
    StringObj* string = malloc(sizeof(StringObj));
    string -> chars = chars;
    string ->length = length;
    return string;
}

StringObj* copyString(const char* chars, int length){
    char* heapChars = malloc(sizeof(char)*length+1);
    memcpy(heapChars,chars,length);
    heapChars[length] = '\0';
    return allocateString(heapChars,length);
}

void printObject(Value value){
    switch(OBJ_TYPE(value)){
        case OBJ_STRING:
        printf("%s", PAYLOAD_CSTRING(value));
    }
}

StringObj* makeObjWithString(char* chars, int length){
    return allocateString(chars,length);
}