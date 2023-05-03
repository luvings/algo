
#include <stdio.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

/* [0, 1) */
#define RANDOM()        (rand() / (RAND_MAX + 1.0))

#define HEAP_LIMIT      100
#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

typedef struct max_heap m_heap_t;

struct max_heap {
	int h_data[HEAP_LIMIT];
	int h_size;

	void (*push)(m_heap_t * h, int v);
	int (*pop)(m_heap_t * h);
	bool (*empty)(m_heap_t * h);
	bool (*full)(m_heap_t * h);
};

void swap(int *data, int i, int j)
{
	int tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}

/*
 * 新加进来的数, 现在停在了index位置, 请依次往上移动
 * 移动到0位置, 或者干不掉自己的父亲了, 停
 */
void heap_insert(int *h_data, int index)
{
	while (h_data[index] > h_data[(index - 1) / 2]) {
		swap(h_data, index, (index - 1) / 2);
		index = (index - 1) / 2;
	}
}

/*
 * 从index位置, 往下看, 不断的下沉
 * 停: 较大的孩子都不再比index 位置的数大, 已经没孩子了
 */
void heapify(int *h_data, int index, int h_size)
{
	int left = index * 2 + 1;

	while (left < h_size) {	/* 如果有左孩子, 有没有右孩子, 可能有可能没有! */
		/* 把较大孩子的下标, 给 largest */
		int largest = left + 1 < h_size && h_data[left + 1] > h_data[left] ? left + 1 : left;

		largest = h_data[largest] > h_data[index] ? largest : index;
		if (largest == index) {
			break;
		}

		/* index和较大孩子, 要互换 */
		swap(h_data, largest, index);

		index = largest;
		left = index * 2 + 1;
	}
}

void push(m_heap_t *h, int v)
{
	if (h->h_size == HEAP_LIMIT) {
		printf("heap full\n");
		return;
	}

	h->h_data[h->h_size] = v;

	heap_insert(h->h_data, h->h_size++);
}

/*
 * 用户此时, 让你返回最大值, 并且在大根堆中, 把最大值删掉
 * 剩下的数, 依然保持大根堆组织
 */
int pop(m_heap_t *h)
{
	int ans = h->h_data[0];

	swap(h->h_data, 0, --h->h_size);
	heapify(h->h_data, 0, h->h_size);

	return ans;
}

bool empty(m_heap_t *h)
{
	return h->h_size == 0;
}

bool full(m_heap_t *h)
{
	return h->h_size == HEAP_LIMIT;
}

int main(int argc, char *argv[])
{
	int arr[] = { 6, 1, 2, 5, 3, 4 };
	int len = ARRAY_SIZE(arr);

	//std::make_heap(a, a + len);
	std::vector < int >v1(arr, arr + len);

	/* 大根堆 */
	make_heap(v1.begin(), v1.end());
	/* 小根堆 */
	//make_heap(v1.begin(), v1.end(), std::greater < int >());

	for (std::vector < int >::iterator it = v1.begin(); it != v1.end(); it++) {
		printf("%d ", *it);
	}
	printf("\n");
	std::cout << "max heap top: " << v1.front() << '\n';

	v1.push_back(20);

	/*
	 * 实现细节:
	 * 1) 添加元素到vector的尾部
	 * 2) 重新调整堆
	 */
	push_heap(v1.begin(), v1.end());
	//push_heap(v1.begin(), v1.end(), std::greater < int >());
	std::cout << "max heap after push: " << v1.front() << '\n';

	/*
	 * 堆顶元素取出来, 放到了数组或vector容器的末尾, 用原来的末尾元素去替代,
	 * 然后end迭代器减1, 执行siftdown()下溯函数来重新调整堆
	 */
	pop_heap(v1.begin(), v1.end());
	//pop_heap(v1.begin(), v1.end(), std::greater < int >());
	v1.pop_back();
	std::cout << "max heap after pop: " << v1.front() << '\n';

	v1.clear();
	std::cout << "max heap empty: " << v1.empty() << '\n';

	int times = 100000;
	int max_value = 100;
	bool succeed = true;

	/* seed */
	srand((int)time(0));

	m_heap_t heap = {
		.h_data = { 0, },
		.h_size = 0,
		.push = push,
		.pop = pop,
		.empty = empty,
		.full = full,
	};

	v1.clear();

	for (int i = 0; i < times; i++) {
		int op_limit = (int)(RANDOM() * (HEAP_LIMIT * 2));
		//printf("op_limit: %d\n", op_limit);

		for (int j = 0; j < op_limit; j++) {
			if (heap.empty(&heap) != v1.empty()) {
				printf("oops..1\n");
				succeed = false;
				goto __bad;
			}
			if (heap.full(&heap) && (v1.size() != HEAP_LIMIT)) {
				printf("oops..2\n");
				succeed = false;
				goto __bad;
			}

			if (heap.empty(&heap)) {
				int value = (int)(RANDOM() * (max_value + 1));
				heap.push(&heap, value);

				v1.push_back(value);
				push_heap(v1.begin(), v1.end());
			} else if (heap.full(&heap)) {
				if (heap.pop(&heap) != v1.front()) {
					printf("oops..3\n");
					succeed = false;
					goto __bad;
				}

				pop_heap(v1.begin(), v1.end());
				v1.pop_back();
			} else {
				if (RANDOM() < 0.5) {
					int value = (int)(RANDOM() * (max_value + 1));
					heap.push(&heap, value);

					v1.push_back(value);
					push_heap(v1.begin(), v1.end());
				} else {
					if (heap.pop(&heap) != v1.front()) {
						printf("oops..4\n");
						succeed = false;
						goto __bad;
					}

					pop_heap(v1.begin(), v1.end());
					v1.pop_back();
				}
			}
		}
	}

__bad:
	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
