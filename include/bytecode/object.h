#ifndef bryte_object_h

#define bryte_object_h

#include "../common.h"
#include "./value.h"

typedef enum{
    OBJ_STRING,
}ObjType;

struct Obj {
    ObjType type;
 };

 typedef struct{
    Obj obj;// --> extends Obj struct so some kind of inheritance
    /*having obj as StringObj's first field means that they are aligned
    So basically you can cast StringObj* to Obj* and call it's attributes (e.g type)*/
    int length;
    char* chars;

 }StringObj;




//reminder: OBJ_VAL has a type "obj" and the payload is a pointer the the object struct
#define OBJ_TYPE(value) (PAYLOAD_OBJ(value)->type)

static inline bool isObjType(Value value, ObjType type){
    return IS_OBJ(value) && OBJ_TYPE(value) == type;
}
//isObjType is not a macro cuz value is called twice, so compiler would just copy and paste the code so if we called isObjType(pop()) 
// pop() would be called twice, ig be careful
 #define IS_STRING(value) isObjType(value,OBJ_STRING)
#define PAYLOAD_STRING(value) ((StringObj *)(PAYLOAD_OBJ(value))) // --> converts value's obj* to stringobj*
#define PAYLOAD_CSTRING(value) (((StringObj*)(PAYLOAD_OBJ(value)))->chars)
StringObj* copyString(const char* chars, int length);

#endif