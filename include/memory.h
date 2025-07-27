#if !defined bryte_memory_h
#include <stdlib.h>
#define bryte_memory_h

#define GROW_CAPACITY(oldCap) ((oldCap) < 8? 8: (oldCap)*2)
void * grow_array(size_t size_of_type, void * pointer, int oldCount, int newCount);
#endif