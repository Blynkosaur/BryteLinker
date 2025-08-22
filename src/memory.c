#include "../include/memory.h"
#include "../include/vm/vm.h"
#include "../include/bytecode/object.h"
#include <stdio.h>

static void freeObject(Obj* object){
    //reminder Obj --> {.type, .next}
    //StringObj --> {.obj, .chars, .length,}
    switch (object->type){
        case OBJ_STRING:{
            StringObj* string = (StringObj*)object;
            printf("Freed string: %s\n", string->chars);
            free(string->chars);
            free(object);
            break;
        }
    }
}
 void freeObjects(){
    Obj* head = vm.objectsHead;
    while (head != NULL){
        Obj* temp_next = head->next;
        freeObject(head);
        head = temp_next;
    }
}

