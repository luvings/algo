
#include "util.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

void sort_array_distance_less_k(int arr[], int n, int k)
{
	if (arr == NULL || n < 2 || k == 0) {
		return;
	}

	std::vector < int >v1;

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
	qsort(array, size, sizeof(int), cmp);

	/*
	 * 然后开始随意交换, 但是保证每个数距离不超过K
	 * swap[i] == true, 表示i位置已经参与过交换
	 * swap[i] == false, 表示i位置没有参与过交换
	 */
	bool if_swap[size];
	memset(if_swap, 0, size * sizeof(bool));

	for (int i = 0; i < size; i++) {
		// FIXME: use MIN will cause error ?

		int j = std::min(i + (int)(RANDOM() * (k + 1)), size - 1);
		if (!if_swap[i] && !if_swap[j]) {
			if_swap[i] = true;
			if_swap[j] = true;

			swap(array, i, j);
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int max_size = 100;
	int max_value = 100;
	int times = 1000;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		int k = (int)(RANDOM() * (max_size + 1));

		create_random_array_k(max_size, max_value, k, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		sort_array_distance_less_k(arr1, actual_size, k);
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
