#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/bytecode/object.h"
#include "../../include/bytecode/value.h"
#include "../../include/datastructures/hashmap.h"

void printAll(Table* table){
    if (table->entries == NULL || table->capacity == 0) {
        printf("Table is empty\n");
        return;
    }
    
    Entry** entries = table->entries;
    for (int i = 0; i < table->capacity; i++){
        if(entries[i] != NULL && entries[i]->key != NULL){
            printf("[ ");
            printf("%s %p", entries[i]->key->chars, entries[i] );
            printf(" ]");
        }
    }
    printf("\n");
}
uint32_t hashFunc(const char* key, int length){
    uint32_t hash =2166136261u;
    for (int i = 0; i < length; i++){
        hash ^= (uint8_t) key[i];
        hash *= 16777619;
    }
    return hash;
}


int main(void){
    Table* table = malloc(sizeof(Table));
    initTable(table);
    while(1){
        int command;
        char args[256];  // Use a fixed-size buffer instead of dynamic allocation
        printf("1) set value \n 2) lookup value \n 3) delete item");
        printf("> ");
        
        scanf("%d %255s", &command, args);  // Read into fixed buffer
        
        StringObj* obj = malloc(sizeof(StringObj));
        obj->chars = strdup(args);  // Make a copy of the string
        obj->hash = hashFunc(args, strlen(args));
        obj->length = strlen(args);
        switch (command){
            case 1: {
                printf("Setting entry with key: %s\n", args);
                bool result = set(table, NULL_VAL, obj);
                printf("Set result: %s\n", result ? "true" : "false");
                printf("Table capacity: %d, count: %d\n", table->capacity, table->count);
                break;
            }
            case 2:{
                Entry* found = lookUp(table, args, obj->hash, obj->length);
                printf("Lookup result: %p\n", found);
                break;
            }
            case 3: {
                bool result = delete_entry(table, obj);
                printf("Delete result: %s\n", result ? "true" : "false");
                break;
            }
        }
        printAll(table);
        
        
    }
}