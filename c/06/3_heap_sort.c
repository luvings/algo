
#include "util.h"

/* arr[index]刚来的数, 往上 */
void heap_insert(int arr[], int index)
{
	while (arr[index] > arr[(index - 1) / 2]) {
		swap(arr, index, (index - 1) / 2);
		index = (index - 1) / 2;
	}
}

/* arr[index]位置的数, 能否往下移动 */
void heapify(int arr[], int index, int heap_size)
{
	int left = index * 2 + 1;

	while (left < heap_size) {	/* 下方还有孩子的时候 */
		/*
		 * 两个孩子中, 谁的值大, 把下标给largest
		 * 1）只有左孩子, left -> largest
		 * 2) 同时有左孩子和右孩子, 右孩子的值<= 左孩子的值, left -> largest
		 * 3) 同时有左孩子和右孩子并且右孩子的值> 左孩子的值, right -> largest
		 */
		int largest = left + 1 < heap_size && arr[left + 1] > arr[left] ? left + 1 : left;

		/* 父和较大的孩子之间, 谁的值大, 把下标给largest */
		largest = arr[largest] > arr[index] ? largest : index;
		if (largest == index) {
			break;
		}

		/* index和较大孩子, 要互换 */
		swap(arr, largest, index);

		index = largest;
		left = index * 2 + 1;
	}
}

void heap_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}
#if 0
	/*
	 * 1) 调整为大根堆, arr[0] 为最大值
	 * O(N*logN)
	 */
	for (int i = 0; i < n; i++) {	/* O(N) */
		heap_insert(arr, i);	    /* O(logN) */
	}
#else
	/* O(N) */
	for (int i = n - 1; i >= 0; i--) {
		heapify(arr, i, n);
	}
#endif

	/* 2) 将最大值交换到 n - 1 位置, 再余下数据的大根堆 */
	int heap_size = n;
	swap(arr, 0, --heap_size);

	/* O(N*logN) */
	while (heap_size > 0) {	        /* O(N) */
		heapify(arr, 0, heap_size);	/* O(logN) */
		swap(arr, 0, --heap_size);	/* O(1) */
	}
}

int main(int argc, char *argv[])
{
	int max_size = 100;
	int max_value = 100;
	int times = 5;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		heap_sort(arr1, actual_size);
		qsort(arr2, actual_size, sizeof(int), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			printf("---8>--------------------------------------------------\n");
			printf("arr1:\t");
			print_array_data(arr1, actual_size);
			printf("arr2:\t");
			print_array_data(arr2, actual_size);
			printf("---8<---------------------------------------------------\n\n");
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
