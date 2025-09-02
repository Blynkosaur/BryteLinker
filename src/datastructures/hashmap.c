#include "../../include/bytecode/object.h"
#include "../../include/bytecode/value.h"
#include <math.h>
#include "../../include/memory.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/datastructures/hashmap.h"
#define PRIME 151
#define BASE_SIZE 32

static Entry DELETED_ENTRY = {.key = NULL, .value = 0};
void initTable(Table *table)
{

    table->capacity = 0;
    table->count = 0;
    table->entries = NULL;
}
static int hash(const char *string, const int prime, const int capacity)
{
    long hash = 0;
    int len = strlen(string);
    for (int i = 0; i < len; i++)
    {
        hash += (int)pow(prime, (len - i - 1)) * string[i];
        hash = hash % capacity;
    }
    return hash;
}
void freeTable(Table *table)
{
    for (int i = 0; i < table->capacity; i++)
    {
        if (table->entries[i] != &DELETED_ENTRY)
        {

            free(table->entries[i]);
        }
    }
    free(table->entries);
    initTable(table);
}
Entry *lookUp(Table *table, char*key,  uint32_t hash, int length) // returns the index
{
    int capacity = table->capacity;
    char *string_key = key;
    uint32_t init_hash = (hash) % capacity;
    Entry *current = (table->entries)[init_hash];
    for (int i = 0; i < table->capacity; i++)
    {
        int index = (hash + i) % table->capacity;
        Entry *current = table->entries[index];
        if (current == NULL)
        {
            return NULL;
        }
        if (current != &DELETED_ENTRY &&
            strcmp(current->key->chars, key) == 0 && strlen(current->key->chars) == length)
        {
            return current;
        }
    }
    return NULL;
}
static int get_index(Table *table, StringObj *key)
{
    uint32_t init_hash = key->hash % (table->capacity);
    int index = init_hash;
    Entry *current = table->entries[index];
    for (int i = 0; i < table->capacity; i++)
    {
        int index = (init_hash + i) % table->capacity;
        Entry *current = table->entries[index];
        if (current == NULL)
        {
            return -1;
        }
        if (current != &DELETED_ENTRY &&
            strcmp(current->key->chars, key->chars) == 0)
        {
            return index;
        }
    }

    return -1;
    // while (current != NULL)
    // {
    //     if (strlen(current->key->chars) && strcmp(key->chars, current->key->chars) == 0 && current != &DELETED_ENTRY)
    //     {
    //         return index;
    //     }
    //     index++;
    // }
    // return -1;
}
static void growTable(Table *table, int new_size)
{
    if (new_size < BASE_SIZE)
    {
        return;
    }
    Table new_table;
    initTable(&new_table);
    new_table.capacity = new_size;
    new_table.count = 0;
    new_table.entries = calloc(new_size, sizeof(Entry *));
    for (int i = 0; i < table->capacity; i++)
    {
        if (table->entries[i] != NULL && table->entries[i] != &DELETED_ENTRY)
        {
            set(&new_table, table->entries[i]->value, table->entries[i]->key);
        }
    }
    freeTable(table);

    *table = new_table;
}

bool set(Table *table, Value value, StringObj *key)
{
    if(table->capacity == 0){
        growTable(table, BASE_SIZE);
    }
    Entry *lookup = lookUp(table, key->chars, key->hash, key->length);
    if (lookup != NULL)
    { // just changing the value that's already there
        lookup->value = value;
        return false;
    }
    if ((table->count / table->capacity) > 0.7)
    {
        growTable(table, table->capacity * 2);
    }
    // TODO: EXPAND THE HASHMAP OR DECREASE ITS SIZE DYNAMICALLY I AM WAY TO TIRED FOR THIS SHIT
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->value = value;
    int init_index = key->hash % (table->capacity);

    while ((table->entries)[init_index] != NULL)
    {
        init_index = (init_index+1)%table->capacity;
    }
    table->entries[init_index] = new_entry;
    table->count++;
    return true;
}
bool delete_entry(Table *table, StringObj *key)
{
    int index = get_index(table, key);
    if (index < 0)
    {
        return false;
    }
    Entry *temp_ptr = table->entries[index];
    free(temp_ptr);
    table->entries[index] = &(DELETED_ENTRY);
    table->count--;
    // NOW RESIZE DOWN IF COUNT IS SMALLER THAN 10%
    if ((table->count / table->capacity) < 0.1)
    {
        growTable(table, table->capacity / 2);
    }
    return true;
}
