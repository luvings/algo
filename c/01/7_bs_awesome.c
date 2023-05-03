
#include "util.h"

int valley_floor_idx(int arr[], int n)
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

bool index_is_at_low_valley(int arr[], int n, int index)
{
	if (n <= 1) {
		return true;
	}
	if (index == 0) {
		return arr[index] < arr[index + 1];
	}
	if (index == n - 1) {
		return arr[index] < arr[index - 1];
	}

	return arr[index] < arr[index - 1] && arr[index] < arr[index + 1];
}

int create_random_array_ne(int max_size, int max_value, int *array, int *actual_size)
{
	int size = (int)(RANDOM() * (max_size + 1));

	*actual_size = size;

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
	int times = 10;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int actual_size = 0;

		create_random_array_ne(max_size, max_value, arr1, &actual_size);

		int idx = valley_floor_idx(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr1, actual_size);
		printf("valley floor idx = '%d'\n", idx);
		printf("---8<---------------------------------------------------\n\n");

		if (!index_is_at_low_valley(arr1, actual_size, idx)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
