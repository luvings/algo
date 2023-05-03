
#include "util.h"

int merge(int arr[], int l, int m, int r)
{
	int i = (r - l + 1) - 1;	/* => n - 1 */
	int p1 = m;
	int p2 = r;
	int res = 0;

	int help[r - l + 1];

	while (p1 >= l && p2 > m) {
		res += arr[p1] > arr[p2] ? (p2 - m) : 0;
		help[i--] = arr[p1] > arr[p2] ? arr[p1--] : arr[p2--];
	}

	while (p1 >= l) {
		help[i--] = arr[p1--];
	}

	while (p2 > m) {
		help[i--] = arr[p2--];
	}

	for (i = 0; i < (r - l + 1); i++) {
		arr[l + i] = help[i];
	}

	return res;
}

/*
 * arr[L..R] 既要排好序, 也要求逆序对数量返回
 * 所有 merge 时, 产生的逆序对数量, 累加, 返回
 * 左 排序 merge 并产生逆序对数量
 * 右 排序 merge 并产生逆序对数量
 */
int process(int arr[], int l, int r)
{
	if (l == r) {
		return 0;
	}

	int mid = l + ((r - l) >> 1);
	return process(arr, l, mid) + process(arr, mid + 1, r)
	    + merge(arr, l, mid, r);
}

int number_of_reverse_pair(int arr[], int n)
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
			if (arr[i] > arr[j]) {
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
	int times = 10;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		int rp = number_of_reverse_pair(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("number of reverse pair: %d\n", rp);
		printf("---8<---------------------------------------------------\n\n");

		if (rp != comparator(arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
