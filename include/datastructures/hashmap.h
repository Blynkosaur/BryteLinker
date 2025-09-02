#ifndef bryte_hashmap_h
#define bryte_hashmap_h




#include "../bytecode/value.h"
#include "../common.h"

typedef struct{
StringObj* key;
Value value;
}Entry;

typedef struct {
    int count;
    int capacity;
    Entry** entries;
}Table;

void initTable(Table* table);
void freeTable(Table* table);
bool set(Table* table, Value value, StringObj* key );
Entry* lookUp(Table* table, char* key, uint32_t hash);


bool delete_entry(Table *table, StringObj *key);
#endif