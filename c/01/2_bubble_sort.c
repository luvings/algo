
#include "util.h"

void bubble_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	/*
	 * 0 ~ n-1
	 * 0 ~ n-2
	 * 0 ~ n-3
	 * ..
	 * 0 ~ 1
	 */
	for (int e = n - 1; e > 0; e--) {
		/*
		 * 0, 1
		 * 1, 2
		 * ..
		 * e-1 ~ e
		 */
		for (int i = 0; i < e; i++) {
			if (arr[i] > arr[i + 1]) {
				swap(arr, i, i + 1);
			}
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

		bubble_sort(arr1, actual_length);

		printf("[%d]:\t", actual_length);
		print_array_data(arr2, actual_length);
		printf("sort:\t");
		print_array_data(arr1, actual_length);
		printf("---8<---------------------------------------------------\n\n");

		qsort(arr2, actual_length, sizeof(arr2[0]), cmp);

		if (!is_equal(arr1, arr2, actual_length)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
