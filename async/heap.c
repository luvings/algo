#include "heap.h"

Heap createHeap(HeapEqual equal, HeapType type) {
	Heap heap = newHeap();
	if (heap == NULL) {
		return NULL;
	}
	heap->equal = equal;
	heap->list = (HeapItem)malloc(sizeof(struct heapItem) * 10);
	heap->size = 0;
	heap->maxSize = 10;
	heap->type = type;
	heap->insert = insert;
	heap->remove = heapRemoveRoot;
	return heap;
}

void insert(Heap heap, let item) {
	if (heap->size >= heap->maxSize) {
		heap->maxSize *= 2;
		HeapItem list = realloc(heap->list, sizeof(struct heapItem) * heap->maxSize);
		if (list == NULL) {
			return;
		}
		heap->list = list;
	}
	heap->list[heap->size].data = item;
	int index = heap->size;
	heap->size++;
	while (index != 0) {
		int parent = (index - 1) / 2;
		int result = heap->equal(heap->list[index].data, heap->list[parent].data);
		if ((heap->type == MaxHeap && result > 0) || (heap->type == MinHeap && result < 0)) {
			struct heapItem temp = heap->list[parent];
			heap->list[parent] = heap->list[index];
			heap->list[index] = temp;
		}
		index = parent;
	}
}

let heapRemoveRoot(Heap heap) {
	if (heap->size != 0) {
		heap->size--;
		if (heap->size == 0) return  heap->list[0].data;
		struct heapItem head = heap->list[0];
		struct heapItem tail = heap->list[heap->size];

		int index = 0;
		int target = 2 * index + 1;
		int equalResut = heap->type == MinHeap ? 1 : -1;
		while (target <= heap->size - 1) {
			if (target < heap->size - 1 && heap->equal(heap->list[target].data, heap->list[target + 1].data) == equalResut) {
				target++;
			}

			if (heap->equal(tail.data, heap->list[target].data) == -equalResut) break;

			heap->list[index] = heap->list[target];
			index = target;
			target = 2 * index + 1;
		}
		heap->list[index] = tail;
		return head.data;
	}
	if (heap->size < heap->maxSize) {
		heap->maxSize /= 2;
		HeapItem list = realloc(heap->list, sizeof(struct heapItem) * heap->maxSize);
		heap->list = list;
	}
	return NULL;
}

void freeHeap(Heap * heap) {
	free((*heap)->list);
	free(*heap);
	*heap = NULL;
}