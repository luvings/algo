
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

typedef struct heap_greater {
	int h_size;
	int (*comp)(int a, int b);

	std::vector < int > v;
	std::map < int, int > i_map;	/* (val, idx) */
} m_heap_greater_t;

int comp(int a, int b)
{
	return (a - b);
}

void swap(m_heap_greater_t *h, int i, int j)
{
	int o1 = h->v.at(i);
	int o2 = h->v.at(j);

	h->v[i] = o2;
	h->v[j] = o1;

	h->i_map[o1] = j;
	h->i_map[o2] = i;
}

void heap_insert(m_heap_greater_t *h, int idx)
{
	while (h->comp(h->v[idx], h->v[(idx - 1) / 2]) < 0) {
		swap(h, idx, (idx - 1) / 2);

		idx = (idx - 1) / 2;
	}
}

void heapify(m_heap_greater_t *h, int idx, int h_size)
{
	int left = idx * 2 + 1;
	while (left < h->h_size) {
		int least = ((left + 1) < h->h_size && h->comp(h->v[left + 1], h->v[left]) < 0) ? left + 1 : left;

		least = h->comp(h->v[idx], h->v[least]) < 0 ? idx : least;
		if (idx == least) {
			break;
		}

		swap(h, idx, least);
		idx = least;
		left = idx * 2 + 1;
	}
}

void push(m_heap_greater_t *h, int x)
{
	h->v.push_back(x);
	h->i_map.insert(std::make_pair(x, h->h_size));
	heap_insert(h, h->h_size++);
}

int pop(m_heap_greater_t *h)
{
	if (h->h_size == 0) {
		printf("heap has been empty\n");
		return -111;
	}

	int ans = h->v.at(0);

	swap(h, 0, h->h_size - 1);

	h->i_map.erase(h->i_map.find(ans));
	h->v.pop_back();
	h->h_size--;

	heapify(h, 0, h->h_size);

	return ans;
}

void resign(m_heap_greater_t *h, int idx)
{
	heap_insert(h, idx);

	heapify(h, idx, h->h_size);
}

void remove(m_heap_greater_t *h, int x)
{
	if (h->i_map.find(x) == h->i_map.end()) {
		printf("x doesn't exist\n");
		return;
	}

	int replace = h->v[h->h_size - 1];
	h->v.pop_back();
	h->h_size--;

	int idx = h->i_map[x];
	h->i_map.erase(h->i_map.find(x));

	printf("remove.. [%d] = %d\n", idx, x);

	if (x != replace) {
		h->v[idx] = replace;
		h->i_map[replace] = idx;

		resign(h, idx);
	}
}

/*
 * v.front() 返回首元素的引用
 * v.back()  返回尾元素的引用
 */
int peek(m_heap_greater_t *h)
{
	return h->v.front();
}

/*
 * 查找对象不是基础类型时, 需要该类提供重载 == 函数
 *
 * #include <algorithm>
 *
 * std::vector< int >::iterator iter = std::find(vec.begin(), vec.end(), x);
 * if (iter != vec.end()) {
 *      vec.erase(iter);
 * } else {
 *      // x doesn't exsit
 * }
 */
bool contains(m_heap_greater_t *h, int x)
{
	return (h->i_map.find(x) != h->i_map.end());
}

bool empty(m_heap_greater_t *h)
{
	if (h->v.size() != h->i_map.size()) {
		printf("oops: v.size(%ld) != i_map.size(%ld)\n", h->v.size(), h->i_map.size());
	}

	return h->h_size == 0;
}

bool full(m_heap_greater_t *h)
{
	//return h->h_size == HEAP_LIMIT;
	return false;
}

void print_heap_greater(std::vector < int > &v)
{
	std::cout << "[ ";
	for (std::vector < int >::iterator it = v.begin(); it != v.end(); it++) {
		std::cout << *it << ", ";
	}
	printf("]\n\n");
}

int main(int argc, char *argv[])
{
	m_heap_greater_t heap = {
		.h_size = 0,
		.comp = comp,
	};

	push(&heap, 9);
	push(&heap, 4);
	push(&heap, 7);
	push(&heap, 1);
	push(&heap, 5);

	print_heap_greater(heap.v);

	printf("peek = %d\n", peek(&heap));
	printf("contains(%d) = %d\n\n", 5, contains(&heap, 5));

	pop(&heap);
	printf("out pop:\n");
	print_heap_greater(heap.v);

	remove(&heap, 4);
	print_heap_greater(heap.v);

	remove(&heap, 4);

	printf("---8>--------------------------------------------------\n\n");

	m_heap_greater_t heap2 = {
		.h_size = 0,
		.comp = comp,
	};

	push(&heap2, 3);
	push(&heap2, 3);
	push(&heap2, 2);
	push(&heap2, 5);
	push(&heap2, 3);

	print_heap_greater(heap2.v);

	/*
	 * 此时堆被破坏了, 因为你擅自改了一个下标的数字
	 * 只需要调用 resign 方法, 就能让堆恢复
	 * 而且复杂度是 O(log N), 很快的
	 * 系统提供的堆做不到的, 加强堆可以
	 * 调用 resign 方法
	 */
	heap2.v[1] = 9;
	printf("after [1] -> %d, resign\n\n", heap2.v[1]);

	resign(&heap2, 1);

	print_heap_greater(heap2.v);

	return 0;
}
