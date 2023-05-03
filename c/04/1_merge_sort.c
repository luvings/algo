
#include "util.h"

void merge(int arr[], int L, int M, int R)
{
	int i = 0;
	int p1 = L;
	int p2 = M + 1;

	int help[R - L + 1];

	while (p1 <= M && p2 <= R) {
		help[i++] = arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++];
	}

	while (p1 <= M) {
		help[i++] = arr[p1++];
	}

	while (p2 <= R) {
		help[i++] = arr[p2++];
	}

	for (i = 0; i < (R - L + 1); i++) {
		arr[L + i] = help[i];
	}
}

/*
 * 请把 arr[L..R] 排有序
 * l...r N
 * T(N) = 2 * T(N / 2) + O(N)
 * O(N * logN)
 */
void process(int arr[], int L, int R)
{
	if (L == R) {		/* base case */
		return;
	}

	int mid = L + ((R - L) >> 1);

	process(arr, L, mid);
	process(arr, mid + 1, R);

	merge(arr, L, mid, R);
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
		int L = 0;

		while (L < N) {
			if (mergeSize >= N - L) {
				break;
			}
			int M = L + mergeSize - 1;
			int R = M + MIN(mergeSize, N - M - 1);
			merge(arr, L, M, R);
			L = R + 1;
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
	int times = 10;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		merge_sort(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");

		merge_sort_un(arr2, actual_size);
		//qsort(arr2, actual_size, sizeof(int), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
