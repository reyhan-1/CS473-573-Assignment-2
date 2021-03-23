#ifndef _MINHEAP_H
#define _MINHEAP_H

typedef struct {
	int id;
	float distance;
} key_type;
typedef struct _minheap* minheap;
minheap minheap_create();
minheap minheap_heapify(const key_type* array, int n);
void minheap_destroy(minheap);
key_type minheap_findmin(minheap);
void minheap_insert(minheap, key_type);
void minheap_deletemin(minheap);
int minheap_is_empty(minheap);
int minheap_size(minheap);
void minheap_clear(minheap);

#endif