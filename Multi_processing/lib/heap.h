#include <string.h>
/*size_t*/

/*n = max number of elements, s = size of element, cm = compare function, co = copy function*/
void* heap_alloc(int n, int s,
				 int (*cm)(const void*,const void*), 
				 void* (*co)(void*, const void*, const size_t)
				);

void heap_free(void* h);
/*insert obj into heap*/
void heap_push(void* ptr, const void* obj);

/*h->v[0] = obj and adjust its position*/
void heap_changemin(void* ptr, const void* obj);

/*delete min*/
void heap_pop(void* ptr);

/*copy min to obj*/
void heap_getmin(void* ptr, void* obj);

int heap_isempty(void* ptr);
