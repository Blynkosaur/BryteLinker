#if !defined bryte_memory_h

#define bryte_memory_h

#define GROW_CAPACITY(oldCap) ((oldCap) < 8? 8: (oldCap)*2)
void * grow_array(int size_of_type, void * pointer, int oldCount, int newCount);
#endif