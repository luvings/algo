
#include "util.h"

void selection_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	/*
	 * 0 ~ n-1
	 * 1 ~ n-1
	 * 2 ~ n-1
	 * ...
	 * n-2 ~ n-1
	 */
	for (int i = 0; i < n - 1; i++) {
		int min_index = i;

		/* i ~ n-1, find minimum index */
		for (int j = i + 1; j < n; j++) {
			min_index = arr[j] < arr[min_index] ? j : min_index;
		}

		if (i != min_index)
			swap(arr, i, min_index);
	}
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 20;
	int times = 10;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		selection_sort(arr1, actual_size);
#if defined(__F_DBG__)
		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");
#endif

		qsort(arr2, actual_size, sizeof(int), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
