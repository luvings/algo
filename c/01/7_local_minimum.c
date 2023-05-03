
#include "util.h"

/*
 * 局部最小值
 */
int local_minimum_idx(int arr[], int n)
{
	if (arr == NULL || n == 0) {
		return -1;
	}

	if (n == 1 || arr[0] < arr[1]) {
		return 0;
	}
	if (arr[n - 1] < arr[n - 2]) {
		return n - 1;
	}

	int left = 1;
	int right = n - 2;
	int mid = 0;

	while (left < right) {
		mid = (left + right) / 2;

		if (arr[mid] > arr[mid - 1]) {
			right = mid - 1;
		} else if (arr[mid] > arr[mid + 1]) {
			left = mid + 1;
		} else {
			return mid;
		}
	}

	return left;
}

bool is_at_valley(int arr[], int n, int idx)
{
	if (n <= 1) {
		return true;
	}
	if (idx == 0) {
		return arr[idx] < arr[idx + 1];
	}
	if (idx == n - 1) {
		return arr[idx] < arr[idx - 1];
	}

	return arr[idx] < arr[idx - 1] && arr[idx] < arr[idx + 1];
}

int create_random_array_ne(int max_size, int max_value, int *array, int *actual_length)
{
	int size = (int)(RANDOM() * (max_size + 1));

	*actual_length = size;

	array[0] = (int)((max_value + 1) * RANDOM());

	for (int i = 1; i < size; i++) {
		do {
			array[i] = (int)((max_value + 1) * RANDOM());
		} while (array[i] == array[i - 1]);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int max_size = 20;
	int max_value = 20;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned int) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int actual_length = 0;

		create_random_array_ne(max_size, max_value, arr1, &actual_length);

		int idx = local_minimum_idx(arr1, actual_length);

		printf("[%d]:\t", actual_length);
		print_array_data(arr1, actual_length);
		printf("local minimum idx = '%d'\n", idx);
		printf("---8<---------------------------------------------------\n\n");

		if (!is_at_valley(arr1, actual_length, idx)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
