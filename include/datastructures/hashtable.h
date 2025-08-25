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
    Entry* entries;
}Table;

void initTable(Table* table);
void free(Table* table);
void insert(Table* table, StringObj* key, Value value );
void lookup(Table* table, StringObj* key);


#endif