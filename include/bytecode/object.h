#ifndef bryte_object_h

#define bryte_object_h
#include "../common.h"
#include "./chunk.h"
#include "./value.h"

typedef enum {
  OBJ_FUNCTION,
  OBJ_STRING,
} ObjType;

struct Obj {
  ObjType type;
  Obj *next; // --> intrusive linkedlist whatever that is. Cuz we're not making
             // another linkedlist
};

struct StringObj {
  Obj obj; // --> extends Obj struct so some kind of inheritance
  /*having obj as StringObj's first field means that they are aligned
  So basically you can cast StringObj* to Obj* and call it's attributes (e.g
  type)*/
  int length;
  char *chars;
  uint32_t hash;
};
typedef struct {
  Obj obj;
  int param_count;
  Chunk chunk;
  StringObj *name;

} FunctionObj;
FunctionObj *makeFunction();
typedef Value (*NativeFn)(int argCount, Value *args);
typedef struct {
  Obj obj;
  NativeFn function;

} ObjNative;

void printObject(Value value);

// reminder: OBJ_VAL has a type "obj" and the payload is a pointer the the
// object struct
#define OBJ_TYPE(value) (PAYLOAD_OBJ(value)->type)

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && OBJ_TYPE(value) == type;
}
// isObjType is not a macro cuz value is called twice, so compiler would just
// copy and paste the code so if we called isObjType(pop())
//  pop() would be called twice, ig be careful
#define IS_STRING(value) isObjType(value, OBJ_STRING)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define PAYLOAD_STRING(value)                                                  \
  ((StringObj *)(PAYLOAD_OBJ(value))) // --> converts value's obj* to stringobj*
#define PAYLOAD_CSTRING(value) (((StringObj *)(PAYLOAD_OBJ(value)))->chars)
#define PAYLOAD_FUNCTION(value) ((FunctionObj*)PAYLOAD_OBJ(value)
StringObj *copyString(const char *chars, int length);

StringObj *makeObjWithString(char *chars, int length);
FunctionObj *makeFunction();
#endif
