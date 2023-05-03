
#include "util.h"

/*
 * 找满足 >=target 的最左位置
 */
int nearest_left_index(int arr[], int n, int target)
{
	if (arr == NULL || n == 0) {
		return -1;
	}

	int L = 0;
	int R = n - 1;
	int index = -1;

	while (L <= R) {
		int mid = L + ((R - L) >> 1);

		if (arr[mid] >= target) {
			index = mid;
			R = mid - 1;
		} else {
			L = mid + 1;
		}
	}

	return index;
}

int left_diagnosis(int arr[], int n, int target)
{
	if (arr == NULL || n == 0) {
		return -1;
	}

	for (int i = 0; i < n; i++) {
		if (arr[i] >= target) {
			return i;
		}
	}
	return -1;
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
		printf("(.. >= %d) nearest left idx = '%d'\n", target, nearest_left_index(arr1, actual_size, target));
		printf("---8<---------------------------------------------------\n\n");

		if (nearest_left_index(arr1, actual_size, target) != left_diagnosis(arr1, actual_size, target)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
