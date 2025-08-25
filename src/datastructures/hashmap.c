#include "../../include/bytecode/object.h"
#include "../../include/bytecode/value.h"
#include "../../include/memory.h"
#include <stdlib.h>
#include <string.h>
#include "../../include/datastructures/hashtable.h"


void initTable(Table * table){

    table->capacity = 0;
    table->count = 0;
    table->entries = NULL;
}

void freeTable(Table*table){
    free(table->entries);
    initTable(table);
}

