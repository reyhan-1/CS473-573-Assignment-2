#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minheap.h"

struct _minheap
{
	key_type *array;
	int max_size;
	int cur_size;
};

minheap minheap_create()
{
	minheap h = (minheap)malloc(sizeof(struct _minheap));
	if (h == NULL)
	{
		fprintf(stderr, "Not enough memory!\n");
		abort();
	}
	h->max_size = 64;
	h->cur_size = 0;
	h->array = (key_type *)malloc(
		sizeof(key_type) * (h->max_size + 1));
	if (h->array == NULL)
	{
		fprintf(stderr, "Not enough memory!\n");
		abort();
	}
	return h;
}

void minheap_destroy(minheap h)
{
	assert(h);
	free(h->array);
	free(h);
}

static void minheap_double_capacity(minheap h)
{
	// create double the array
	int new_max_size = 2 * h->max_size;
	key_type *new_array = (key_type *)malloc(
		sizeof(key_type) * (new_max_size + 1));
	if (new_array == NULL)
	{
		fprintf(stderr, "Not enough memory!\n");
		abort();
	}
	/* copy old elements to new array */
	for (int i = 1; i <= h->cur_size; i++)
	{
		new_array[i] = h->array[i];
	}
	/* free old array and place new in position */
	free(h->array);
	h->array = new_array;
	h->max_size = new_max_size;
}

static void minheap_swap(minheap h, int i, int j)
{
	assert(h && i >= 1 && i <= h->cur_size &&
		   j >= 1 && j <= h->cur_size);
	key_type tmp = h->array[i];
	h->array[i] = h->array[j];
	h->array[j] = tmp;
}

static void minheap_fixup(minheap h, int k)
{
	assert(h && k >= 1 && k <= h->cur_size);
	while (k > 1 && h->array[k].distance < h->array[k / 2].distance)
	{
		minheap_swap(h, k / 2, k);
		k /= 2;
	}
}

static void minheap_fixdown(minheap h, int k)
{
	assert(h);
	while (2 * k <= h->cur_size)
	{
		int j = 2 * k;
		if (j < h->cur_size && h->array[j + 1].distance < h->array[j].distance)
			j++;
		if (h->array[k].distance <= h->array[j].distance)
			break;
		minheap_swap(h, k, j);
		k = j;
	}
}

void minheap_insert(minheap h, key_type key)
{
	assert(h);
	// make sure there is space
	if (h->cur_size == h->max_size)
		minheap_double_capacity(h);
	// add at the bottom, as a leaf
	h->array[++h->cur_size] = key;
	// fix its position
	minheap_fixup(h, h->cur_size);
}

key_type minheap_findmin(minheap h)
{
	if (minheap_is_empty(h))
	{
		fprintf(stderr, "Heap is empty!\n");
		abort();
	}
	// min is always in first position
	return h->array[1];
}

void minheap_deletemin(minheap h)
{
	if (minheap_is_empty(h))
	{
		fprintf(stderr, "Heap is empty!\n");
		abort();
	}
	// swap first with last
	minheap_swap(h, 1, h->cur_size);
	// delete last
	h->cur_size--;
	// fixdown first
	minheap_fixdown(h, 1);
}

int minheap_size(minheap h)
{
	assert(h);
	return h->cur_size;
}
int minheap_is_empty(minheap h)
{
	assert(h);
	return h->cur_size <= 0;
}

void minheap_clear(minheap h)
{
	assert(h);
	h->cur_size = 0;
}

minheap minheap_heapify(const key_type *array, int n)
{
	assert(array && n > 0);
	minheap h = (minheap)malloc(sizeof(struct _minheap));
	if (h == NULL)
	{
		fprintf(stderr, "Not enough memory!\n");
		abort();
	}
	h->max_size = n;
	h->cur_size = 0;
	h->array = (key_type *)malloc(sizeof(key_type) * (h->max_size + 1));
	if (h->array == NULL)
	{
		fprintf(stderr, "Not enough memory!\n");
		abort();
	}
	h->cur_size = n;
	for (int k = 0; k < n; k++)
		h->array[k + 1] = array[k];
	for (int k = (h->max_size + 1) / 2; k > 0; k--)
		minheap_fixdown(h, k);
	return h;
}