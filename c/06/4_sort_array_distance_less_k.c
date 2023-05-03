
#include "util.h"

#include <algorithm>
#include <iostream>
#include <vector>

void sort_array_distance_less_k(int arr[], int n, int k)
{
	if (arr == NULL || n < 2 || k == 0) {
		return;
	}

	std::vector < int > v1;

	int index = 0;
	/* 0...K-1 */
	for (; index <= std::min(n - 1, k - 1); index++) {
		v1.push_back(arr[index]);
		push_heap(v1.begin(), v1.end(), std::greater < int >());
	}

	/* 小根堆 */
	//make_heap(v1.begin(), v1.end(), std::greater < int >());

	int i = 0;
	for (; index < n; i++, index++) {
		v1.push_back(arr[index]);
		push_heap(v1.begin(), v1.end(), std::greater < int >());

		arr[i] = v1.front();

		pop_heap(v1.begin(), v1.end(), std::greater < int >());
		v1.pop_back();
	}

	while (!v1.empty()) {
		arr[i++] = v1.front();

		pop_heap(v1.begin(), v1.end(), std::greater < int >());
		v1.pop_back();
	}
}

int create_random_array_k(int max_size, int max_value, int k, int *array, int *actual_size)
{
	int size = (int)(RANDOM() * (max_size + 1));

	*actual_size = size;

	if (size == 0) {
		return 0;
	}

	for (int i = 0; i < size; i++) {
		/* [-? , +?] */
		array[i] = (int)((max_value + 1) * RANDOM()) - (int)(max_value * RANDOM());
	}

	/* 先排个序 */
	qsort(array, size, sizeof(array[0]), cmp);

	/*
	 * 然后开始随意交换, 但是保证每个数距离不超过 K
	 * swap[i] == true, 表示 i 位置已经参与过交换
	 * swap[i] == false, 表示 i 位置没有参与过交换
	 */
	bool b_swap[size];
	memset(b_swap, 0, sizeof(b_swap));

	for (int i = 0; i < size; i++) {
		// FIXME: use MIN will cause error ?

		int j = std::min(i + (int)(RANDOM() * (k + 1)), size - 1);
		if (!b_swap[i] && !b_swap[j]) {
			b_swap[i] = true;
			b_swap[j] = true;

			swap(array, i, j);
		}
	}

	return 0;
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

		int k = (int)(RANDOM() * (max_size + 1));

		create_random_array_k(max_size, max_value, k, arr1, &actual_size);
		memcpy(arr2, arr1, sizeof(arr1));

		sort_array_distance_less_k(arr1, actual_size, k);

		qsort(arr2, actual_size, sizeof(arr2[0]), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
