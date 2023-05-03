
#include "util.h"

int merge(int arr[], int l, int m, int r)
{
	/* [l..m] [m + 1..r] */
	int res = 0;

	/* 目前囊括进来的数, 是从 [m + 1, window_r) */
	int window_r = m + 1;

	/*
	 *              [ 4, 6, 0, 1 ]
	 *               /          \
	 *           [ 4, 6 ]    [ 0, 1 ]
	 *            /    \      /    \
	 *          [ 4 ] [ 6 ] [ 0 ] [ 1 ]
	 *
	 *  final:  window_r = 4, m = 1  =>  res += wr - (m + 1)
     *              v
     *           exclusive
	 *
	 */
	for (int i = l; i <= m; i++) {
		while (window_r <= r && (long)arr[i] > (long)arr[window_r] * 2) {
			window_r++;
		}

		res += window_r - (m + 1);
	}

	int i = 0;
	int p1 = l;
	int p2 = m + 1;
	int help[r - l + 1];

	while (p1 <= m && p2 <= r) {
		help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
	}

	while (p1 <= m) {
		help[i++] = arr[p1++];
	}

	while (p2 <= r) {
		help[i++] = arr[p2++];
	}

	for (i = 0; i < (r - l + 1); i++) {
		arr[l + i] = help[i];
	}

	return res;
}

int process(int arr[], int l, int r)
{
	if (l == r) {
		return 0;
	}

	int m = l + ((r - l) >> 1);

	return process(arr, l, m)
		+ process(arr, m + 1, r)
		+ merge(arr, l, m, r);
}

int reverse_pairs(int arr[], int n)
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

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (arr[i] > (arr[j] << 1)) {
				res++;
			}
		}
	}

	return res;
}

int main(int argc, char *argv[])
{
	int max_size = 10;
	int max_value = 15;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, sizeof(arr1));

		int rp = reverse_pairs(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("reverse pairs: %d\n", rp);
		printf("---8<---------------------------------------------------\n\n");

		if (rp != comparator(arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
