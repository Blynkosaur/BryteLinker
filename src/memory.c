#include "../include/memory.h"
#include "../include/bytecode/object.h"
#include "../include/vm/vm.h"
#include <stdio.h>

static void freeObject(Obj *object) {
  // reminder Obj --> {.type, .next}
  // StringObj --> {.obj, .chars, .length,}
  switch (object->type) {
  case OBJ_STRING: {
    StringObj *string = (StringObj *)object;
    printf("Freed string: %s\n", string->chars);
    free(string->chars);
    free(object);
    break;
  }
  case OBJ_FUNCION: {
    FunctionObj *function = (FunctionObj *)object;
    printf("Freed function: %s\n", function->name->chars);
    freeChunk(&function->chunk);
    free(function->name->chars);
    free(function->name);
  }
  }
}
void freeObjects() {
  Obj *head = vm.objectsHead;
  while (head != NULL) {
    Obj *temp_next = head->next;
    freeObject(head);
    head = temp_next;
  }
}
