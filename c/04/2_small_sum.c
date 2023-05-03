
#include "util.h"

/*
 *              [ 0, 1, 2, 3 ]
 *               /          \
 *           [ 0, 1 ]    [ 2, 3 ]
 *            /    \      /    \
 *          [ 0 ] [ 1 ] [ 2 ] [ 3 ]
 */
int merge(int arr[], int l, int m, int r)
{
	int p1 = l;
	int p2 = m + 1;
	int idx = 0, res = 0;

	int help[r - l + 1];

	while (p1 <= m && p2 <= r) {
		res += arr[p1] < arr[p2] ? (r - p2 + 1) * arr[p1] : 0;
		help[idx++] = arr[p1] < arr[p2] ? arr[p1++] : arr[p2++];
	}

	while (p1 <= m) {
		help[idx++] = arr[p1++];
	}

	while (p2 <= r) {
		help[idx++] = arr[p2++];
	}

	for (int i = 0; i < (r - l + 1); i++) {
		arr[l + i] = help[i];
	}

	return res;
}

/*
 * arr[l..r] 既要排好序, 也要求小和返回
 * 所有 merge 时, 产生的小和, 累加
 * 左 排序  merge
 * 右 排序  merge
 * merge
 */
int process(int arr[], int l, int r)
{
	if (l == r) {
		return 0;
	}

	/* l < r */
	int m = l + ((r - l) >> 1);

	return process(arr, l, m)
		+ process(arr, m + 1, r)
		+ merge(arr, l, m, r);
}

int small_sum(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return 0;
	}

	return process(arr, 0, n - 1);
}

int comparator(int arr[], int n)
{
	int res = 0;

	if (arr == NULL || n < 2) {
		return 0;
	}

	for (int i = 1; i < n; i++) {
		for (int j = 0; j < i; j++) {
			res += arr[j] < arr[i] ? arr[j] : 0;
		}
	}

	return res;
}

int main(int argc, char *argv[])
{
	int max_size = 10;
	int max_value = 10;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, sizeof(arr1));

		int ss = small_sum(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("small sum: %d\n", ss);
		printf("---8<---------------------------------------------------\n\n");

		if (ss != comparator(arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
