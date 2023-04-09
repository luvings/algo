#ifndef __HEAP_H__
#define __HEAP_H__

#include"dataStruct.h"

typedef struct heap *Heap;

enum _HeapType {
	MaxHeap, MinHeap
};

typedef enum _HeapType HeapType;

// 判等函数类型
typedef int(*HeapEqual)(let item1, let item2);

typedef void(*HeapInsert)(Heap heap, let item);

typedef let(*HeapRemove)(Heap heap);

typedef struct heapItem {
	let data;
}*HeapItem;

typedef struct heap {
	HeapItem list;
	int size;
	int maxSize;
	HeapEqual equal;
	HeapType type;
	HeapInsert insert;
	HeapRemove remove;
}*Heap;

#define newHeap() NEW(struct heap)

Heap createHeap(HeapEqual equal, HeapType type);

static void insert(Heap heap, let item);

static let heapRemoveRoot(Heap heap);

void freeHeap(Heap * heap);

#endif // !__HEAP_H__
