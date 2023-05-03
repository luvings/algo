
#include "util.h"

bool bin_search(int arr[], int n, int target)
{
	if (arr == NULL || n == 0) {
		return false;
	}

	int L = 0;
	int R = n - 1;

	while (L <= R) {
		int mid = L + ((R - L) >> 1);

		if (arr[mid] == target) {
			return true;
		} else if (arr[mid] > target) {
			R = mid - 1;
		} else {
			L = mid + 1;
		}
	}

	return false;
}

bool exist(int arr[], int n, int target)
{
	if (arr == NULL || n == 0) {
		return false;
	}

	for (int i = 0; i < n; i++) {
		if (arr[i] == target) {
			return true;
		}
	}
	return false;
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

		create_random_array(max_size, max_value, arr1, &actual_size);
		qsort(arr1, actual_size, sizeof(int), cmp);

		int target = (int)((max_value + 1) * RANDOM());

		printf("[%d]:\t", actual_size);
		print_array_data(arr1, actual_size);
		printf("(%2d) exist ? '%s'\n", target, bin_search(arr1, actual_size, target) ? "true" : "false");
		printf("---8<---------------------------------------------------\n\n");

		if (bin_search(arr1, actual_size, target) != exist(arr1, actual_size, target)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
