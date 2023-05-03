
#include "util.h"

/* data[idx] 刚来的数, 往上 */
void heap_insert(int data[], int idx)
{
	while (data[idx] > data[(idx - 1) / 2]) {
		swap(data, idx, (idx - 1) / 2);
		idx = (idx - 1) / 2;
	}
}

/* data[idx] 位置的数, 能否往下移动 */
void heapify(int data[], int idx, int heap_size)
{
	int left = idx * 2 + 1;

	while (left < heap_size) {	/* 下方还有孩子的时候 */
		/*
		 * 两个孩子中, 谁的值大, 把下标给 largest
		 * 1）只有左孩子, left -> largest
		 * 2) 同时有左孩子和右孩子, 右孩子的值 <= 左孩子的值, left -> largest
		 * 3) 同时有左孩子和右孩子并且右孩子的值 > 左孩子的值, right -> largest
		 */
		int largest = left + 1 < heap_size && data[left + 1] > data[left] ? left + 1 : left;

		/* 父和较大的孩子之间, 谁的值大, 把下标给 largest */
		largest = data[largest] > data[idx] ? largest : idx;
		if (largest == idx) {
			break;
		}

		/* idx 和较大孩子, 要互换 */
		swap(data, largest, idx);

		idx = largest;
		left = idx * 2 + 1;
	}
}

void heap_sort(int data[], int n)
{
	if (data == NULL || n < 2) {
		return;
	}
#if 0
	/*
	 * 1) 调整为大根堆, data[0] 为最大值
	 * O(N*logN)
	 */
	for (int i = 0; i < n; i++) {	/* O(N) */
		heap_insert(data, i);	    /* O(logN) */
	}
#else
	/* O(N) */
	for (int i = n - 1; i >= 0; i--) {
		heapify(data, i, n);
	}
#endif

	/* 2) 将最大值交换到 n - 1 位置, 再余下数据的大根堆 */
	int heap_size = n;
	swap(data, 0, --heap_size);

	/* O(N*logN) */
	while (heap_size > 0) {	            /* O(N) */
		heapify(data, 0, heap_size);	    /* O(logN) */
		swap(data, 0, --heap_size);	    /* O(1) */
	}
}

int main(int argc, char *argv[])
{
	int max_size = 10;
	int max_value = 15;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, sizeof(arr1));

		heap_sort(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");

		qsort(arr2, actual_size, sizeof(arr2[0]), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
