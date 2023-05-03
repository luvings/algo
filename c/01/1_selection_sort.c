/*
 *              时间复杂度      额外空间复杂度      稳定性
 *  选择排序      O(N²)             O(1)              无
 *  冒泡排序      O(N²)             O(1)              有
 *  插入排序      O(N²)             O(1)              有
 *  归并排序    O(N*logN)           O(N)              有
 *  随机快排    O(N*logN)           O(logN)           无
 *  堆排序      O(N*logN)           O(1)              无
 *
 *  计数排序      O(N)              O(M)              有
 *  基数排序      O(N)              O(N)              有
 */

#include "util.h"

/*
 * e.g. [ 3, 3', 1, 5 ]
 */
void selection_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	/*
	 * 0 ~ n-1
	 * 1 ~ n-1
	 * 2 ~ n-1
	 * ..
	 * n-2 ~ n-1
	 */
	for (int i = 0; i < n - 1; i++) {
		int min_idx = i;

		/* i ~ n-1, find min idx */
		for (int j = i + 1; j < n; j++) {
			min_idx = arr[j] < arr[min_idx] ? j : min_idx;
		}

		if (i != min_idx) {
			swap(arr, i, min_idx);
		}
	}
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 20;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned int) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_length = 0;

		create_random_array(max_size, max_value, arr1, &actual_length);
		memcpy(arr2, arr1, sizeof(arr1));

		selection_sort(arr1, actual_length);

#if defined(__F_DBG__)
		printf("[%d]:\t", actual_length);
		print_array_data(arr2, actual_length);
		printf("sort:\t");
		print_array_data(arr1, actual_length);
		printf("---8<---------------------------------------------------\n\n");
#endif

		qsort(arr2, actual_length, sizeof(arr2[0]), cmp);

		if (!is_equal(arr1, arr2, actual_length)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
