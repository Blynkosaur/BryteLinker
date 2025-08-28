#include "../../include/bytecode/object.h"
#include "../../include/bytecode/value.h"
#include <math.h>
#include "../../include/memory.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/datastructures/hashtable.h"
#define PRIME 151
#define BASE_SIZE 32

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
    free(table->entries);
    initTable(table);
}
static Entry DELETED_ENTRY = {.key = NULL, .value = 0};
static Entry *lookUp(Table *table, StringObj *key) // returns the index
{
    int capacity = table->capacity;
    char *string_key = key->chars;
    uint32_t hash = (key->hash) % capacity;
    Entry *current = (table->entries)[hash];
    while (current != NULL)
    {
        char *entry_key = current->key->chars;

        if (strlen(entry_key) == strlen(key) && strcmp(key, entry_key) == 0 && current != &DELETED_ENTRY)
        {
            return current;
        } // compare key from the current entry to the one as a parameter
        else
        {
            current++;
        }
    }
    return NULL;
}
static int get_index(Table *table, StringObj *key)
{
    uint32_t init_hash = key->hash % (table->capacity);
    int index = init_hash;
    Entry *current = table->entries[index];

    while (current != NULL)
    {
        if (strlen(current->key->chars) && strcmp(key, current->key->chars) == 0 && current != &DELETED_ENTRY)
        {
            return index;
        }
        index++;
    }
    return -1;
}
void growTable(Table *table, int new_size)
{
    if (new_size < BASE_SIZE)
    {
        return table;
    }
    Table *new_table = malloc(sizeof(Table));
    new_table->capacity = new_size;
    new_table->count = 0;
    new_table->entries = calloc(new_size, sizeof(Entry));

    return new_table;
}
bool set(Table *table, Value value, StringObj *key)
{
    Entry *lookup = lookUp(table, key);
    if (lookup != NULL)
    { // just changing the value that's already there
        lookup->value = value;
        return true;
    }
    // TODO: EXPAND THE HASHMAP OR DECREASE ITS SIZE DYNAMICALLY I AM WAY TO TIRED FOR THIS SHIT
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->value = value;
    int init_index = key->hash % (table->capacity);
    while ((table->entries)[init_index] != NULL)
    {
        init_index++;
    }
    table->entries[init_index] = new_entry;
}
bool delete(Table *table, StringObj *key)
{
    int index = get_index(table, key);
    Entry *temp_ptr = table->entries[index];
    free(temp_ptr);
    table->entries[index] = &(DELETED_ENTRY);
    // NOW RESIZE DOWN IF COUNT IS SMALLER THAN 10%
}
