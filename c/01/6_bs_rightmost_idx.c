
#include "util.h"

/*
 * 找满足 <=tgt 的最右位置
 */
int rightmost_idx(int arr[], int n, int tgt)
{
	if (arr == NULL || n == 0) {
		return -1;
	}

	int l = 0;
	int r = n - 1;
	int idx = -1;

	while (l <= r) {
		int mid = l + ((r - l) >> 1);

		if (arr[mid] <= tgt) {
			idx = mid;
			l = mid + 1;
		} else {
			r = mid - 1;
		}
	}

	return idx;
}

int rightmost_diag(int arr[], int n, int tgt)
{
	if (arr == NULL || n == 0) {
		return -1;
	}

	for (int i = n - 1; i >= 0; i--) {
		if (arr[i] <= tgt) {
			return i;
		}
	}
	return -1;
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

		int tgt = (int)(max_value * RANDOM()) - (int)((max_value + 1) * RANDOM());

		printf("[%d]:\t", actual_length);
		print_array_data(arr1, actual_length);
		printf("(.. <= %d) rightmost idx = '%d'\n", tgt, rightmost_idx(arr1, actual_length, tgt));
		printf("---8<---------------------------------------------------\n\n");

		if (rightmost_idx(arr1, actual_length, tgt) != rightmost_diag(arr1, actual_length, tgt)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
