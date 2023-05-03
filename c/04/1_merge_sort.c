
#include "util.h"

void merge(int arr[], int l, int m, int r)
{
	int p1 = l;
	int p2 = m + 1;
	int idx = 0;

	int help[r - l + 1];

	while (p1 <= m && p2 <= r) {
		help[idx++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
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
}

/*
 * arr[l..r] 排有序
 *
 * T(N) = 2 * T(N / 2) + O(N¹)
 *
 *      =>  a = 2, b = 2, d = 1
 *      =>  logb(a) == d
 *
 *      =>  O(N¹ * logN)
 */
void process(int arr[], int l, int r)
{
	if (l == r) {		/* base case */
		return;
	}

	int m = l + ((r - l) >> 1);

	process(arr, l, m);
	process(arr, m + 1, r);

	merge(arr, l, m, r);
}

/* 递归方法实现 */
void merge_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	process(arr, 0, n - 1);
}

/* 非递归方法实现 */
void merge_sort_un(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	int N = n;
	int mergeSize = 1;	/* 步长 */

	while (mergeSize < N) {	/* logN */
		/* 当前左组的, 第一个位置 */
		int l = 0;

		while (l < N) {
			if (mergeSize >= N - l) {
				break;
			}
			int m = l + mergeSize - 1;
			int r = m + MIN(mergeSize, N - m - 1);
			merge(arr, l, m, r);
			l = r + 1;
		}

		/* 防止溢出 */
		if (mergeSize > N / 2) {
			break;
		}
		mergeSize <<= 1;
	}
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

		merge_sort(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");

		merge_sort_un(arr2, actual_size);
		//qsort(arr2, actual_size, sizeof(arr2[0]), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
