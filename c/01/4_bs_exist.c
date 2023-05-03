
#include "util.h"

bool bin_search(int arr[], int n, int tgt)
{
	if (arr == NULL || n == 0) {
		return false;
	}

	int l = 0;
	int r = n - 1;

	while (l <= r) {
		int mid = l + ((r - l) >> 1);

		if (arr[mid] == tgt) {
			return true;
		} else if (arr[mid] > tgt) {
			r = mid - 1;
		} else {
			l = mid + 1;
		}
	}

	return false;
}

bool exist(int arr[], int n, int tgt)
{
	if (arr == NULL || n == 0) {
		return false;
	}

	for (int i = 0; i < n; i++) {
		if (arr[i] == tgt) {
			return true;
		}
	}
	return false;
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

		create_random_array(max_size, max_value, arr1, &actual_length);
		qsort(arr1, actual_length, sizeof(arr1[0]), cmp);

		int tgt = (int)((max_value + 1) * RANDOM());

		printf("[%d]:\t", actual_length);
		print_array_data(arr1, actual_length);
		printf("(%2d) exist ? '%s'\n", tgt, bin_search(arr1, actual_length, tgt) ? "true" : "false");
		printf("---8<---------------------------------------------------\n\n");

		if (bin_search(arr1, actual_length, tgt) != exist(arr1, actual_length, tgt)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
