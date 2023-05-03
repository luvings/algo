
#include "util.h"

void insertion_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	/*
	 * 0    v (有序)
	 * 1    v
	 * 2    v
	 * ..
	 * n-1  v
	 */
	for (int i = 1; i < n; i++) {
		/*
		 * j..0, looking ahead,
		 *
		 * if (j >=0 && arr[j] > arr[j+1]), then swap
		 */
		for (int j = i - 1; j >= 0 && arr[j] > arr[j + 1]; j--) {
			swap(arr, j, j + 1);
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

		insertion_sort(arr1, actual_length);

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
