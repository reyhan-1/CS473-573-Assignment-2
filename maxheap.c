#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "maxheap.h"

struct _maxheap
{
	key_type *array;
	int max_size;
	int cur_size;
};

maxheap maxheap_create()
{
	maxheap h = (maxheap)malloc(sizeof(struct _maxheap));
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

void maxheap_destroy(maxheap h)
{
	assert(h);
	free(h->array);
	free(h);
}

static void maxheap_double_capacity(maxheap h)
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

static void maxheap_swap(maxheap h, int i, int j)
{
	assert(h && i >= 1 && i <= h->cur_size &&
		   j >= 1 && j <= h->cur_size);
	key_type tmp = h->array[i];
	h->array[i] = h->array[j];
	h->array[j] = tmp;
}

static void maxheap_fixup(maxheap h, int k)
{
	assert(h && k >= 1 && k <= h->cur_size);
	while (k > 1 && h->array[k].distance > h->array[k / 2].distance)
	{
		maxheap_swap(h, k / 2, k);
		k /= 2;
	}
}

static void maxheap_fixdown(maxheap h, int k)
{
	assert(h);
	while (2 * k <= h->cur_size)
	{
		int j = 2 * k;
		if (j < h->cur_size && h->array[j + 1].distance > h->array[j].distance)
			j++;
		if (h->array[k].distance >= h->array[j].distance)
			break;
		maxheap_swap(h, k, j);
		k = j;
	}
}

void maxheap_insert(maxheap h, key_type key)
{
	assert(h);
	// make sure there is space
	if (h->cur_size == h->max_size)
		maxheap_double_capacity(h);
	// add at the bottom, as a leaf
	h->array[++h->cur_size] = key;
	// fix its position
	maxheap_fixup(h, h->cur_size);
}

key_type maxheap_findmax(maxheap h)
{
	if (maxheap_is_empty(h))
	{
		fprintf(stderr, "Heap is empty!\n");
		abort();
	}
	// max is always in first position
	return h->array[1];
}

void maxheap_deletemax(maxheap h)
{
	if (maxheap_is_empty(h))
	{
		fprintf(stderr, "Heap is empty!\n");
		abort();
	}
	// swap first with last
	maxheap_swap(h, 1, h->cur_size);
	// delete last
	h->cur_size--;
	// fixdown first
	maxheap_fixdown(h, 1);
}

int maxheap_size(maxheap h)
{
	assert(h);
	return h->cur_size;
}
int maxheap_is_empty(maxheap h)
{
	assert(h);
	return h->cur_size <= 0;
}

void maxheap_clear(maxheap h)
{
	assert(h);
	h->cur_size = 0;
}

maxheap maxheap_heapify(const key_type *array, int n)
{
	assert(array && n > 0);
	maxheap h = (maxheap)malloc(sizeof(struct _maxheap));
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
		maxheap_fixdown(h, k);
	return h;
}