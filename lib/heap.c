#include <string.h>
#include <stdlib.h>
#include "heap.h"

struct Heap{
	int n; /*number of elements*/
	int s; /*size of element*/
	int (*cmp)(const void*, const void*); /*compare function*/
	void* (*cpy)(void*, const void*, const size_t);
	char v[0]; /*element value*/
};

void* heap_alloc(int n, int s, 
				 int (*cm)(const void*, const void*), 
				 void* (*co)(void*, const void*, const size_t)
				){
	struct Heap* h;
	h = (struct Heap*)malloc(sizeof(struct Heap) + s * n);
	if(h == NULL)
		return NULL;
	h->n = 0;
	h->s = s;
	h->cmp = cm;
	h->cpy = (co == NULL? memcpy: co);
	return (void*)h;
}

void heap_free(void* h){
	free(h);
}

static char* heap_get(struct Heap*h, int n){
	return h->v + n * h->s;
}

void heap_push(void* ptr, const void* obj){
	struct Heap* h = ptr;
	int now;
	now = h->n;
	h->n++;/*does not check for size*/
	while(now != 0){
		int up = (now - 1) / 2;
		if(h->cmp(obj, heap_get(h, up)) >= 0)
			break;
		h->cpy(heap_get(h, now), heap_get(h, up), h->s);
		now = up;
	}
	h->cpy(heap_get(h, now), obj, h->s);
}

void heap_changemin(void* ptr, const void* obj){
	struct Heap* h = ptr;
	int now;
	now = 0;
	while(1){
		int down = now * 2 + 1;
		if(down >= h->n)
			break;
		if(down + 1 < h->n){/*get the smaller from down and down + 1*/
			if(h->cmp(heap_get(h, down), heap_get(h, down + 1)) >= 0)
				down++;
		}
		if(h->cmp(heap_get(h, down), obj) >= 0)
			break;
		h->cpy(heap_get(h, now), heap_get(h, down), h->s);
		now = down;
	}
	h->cpy(heap_get(h, now), obj, h->s);
}

void heap_pop(void* ptr){
	struct Heap* h = ptr;
	h->n--;/*does not check for size*/
	if(h->n > 0)
		heap_changemin(h, heap_get(h, h->n));
}

void heap_getmin(void* ptr, void* obj){
	struct Heap* h = ptr;
	h->cpy(obj, heap_get(h, 0), h->s);
}

int heap_isempty(void* ptr){
	struct Heap* h = ptr;
	return h->n == 0;
}
