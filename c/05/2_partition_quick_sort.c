
#include "util.h"

/*
 * arr[l..r], 以 arr[r] 位置的数做划分值
 *
 * 1) arr[i] <= num, 当前数与 <=区 下一个交换, <=区右扩, i++
 * 2) arr[i] > num, i++
 *
 * eg.
 *      [ 7, 7', 4, 3, 6, 5 ]
 *  0:  ) 7
 *  1:  ) 7  7'
 *  2:    4) 7'  7
 *  3:    4  3)  7  7'
 *  4:    4  3)  7  7' 6
 *
 *  5:    4  3)  5  7' 6  7
 */
int partition(int arr[], int l, int r)
{
	if (l > r) {
		return -1;
	}

	if (l == r) {
		return l;
	}

	int less_eq = l - 1;
	int idx = l;

	while (idx < r) {
		if (arr[idx] <= arr[r]) {
			swap(arr, ++less_eq, idx);
		}
		idx++;
	}

	swap(arr, ++less_eq, r);

	return less_eq;
}

void process_1(int arr[], int l, int r)
{
	if (l >= r) {
		return;
	}

	/* l..r partition: <=[r],  [r],  >[r] */
	int m = partition(arr, l, r);

	process_1(arr, l, m - 1);
	process_1(arr, m + 1, r);
}

void quick_sort_1(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	process_1(arr, 0, n - 1);
}

/*
 * arr[l...r] 玩荷兰国旗问题的划分, 以 arr[r] 做划分值
 *
 * <[r],  =[r],  >[r]
 *
 * eg.
 *      3:  0, 1, 2,) 3, (4, 5, 6, 3'
 *      4:
 */
void netherlands_flag(int arr[], int l, int r, int eq_area[])
{
	if (l > r) {
		eq_area[0] = eq_area[1] = -1;
		return;
	}

	if (l == r) {
		eq_area[0] = eq_area[1] = l;
		return;
	}

	int less = l - 1;	/* < 区右边界 */
	int more = r;		/* > 区左边界 */
	int idx = l;
	while (idx < more) {	/* 当前位置, 不能和 >区 的左边界撞上 */
		if (arr[idx] == arr[r]) {
			idx++;
		} else if (arr[idx] < arr[r]) {
			swap(arr, ++less, idx++);
		} else {	    /* arr[idx] > arr[r] */
			swap(arr, --more, idx);
		}
	}

	swap(arr, more, r);

	eq_area[0] = less + 1;
	eq_area[1] = more;
}

void process_2(int arr[], int l, int r)
{
	if (l >= r) {
		return;
	}

	int eq_area[2] = { -1, -1 };

	netherlands_flag(arr, l, r, eq_area);

	process_2(arr, l, eq_area[0] - 1);
	process_2(arr, eq_area[1] + 1, r);
}

void quick_sort_2(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	process_2(arr, 0, n - 1);
}

void process_3(int arr[], int l, int r)
{
	if (l >= r) {
		return;
	}

	swap(arr, l + (int)(RANDOM() * (r - l + 1)), r);

	int eq_area[2] = { -1, -1 };

	netherlands_flag(arr, l, r, eq_area);

	process_3(arr, l, eq_area[0] - 1);
	process_3(arr, eq_area[1] + 1, r);
}

void quick_sort_3(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	process_3(arr, 0, n - 1);
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 20;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int arr3[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, sizeof(arr1));
		memcpy(arr3, arr1, sizeof(arr1));

		quick_sort_1(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");

		quick_sort_2(arr2, actual_size);
		quick_sort_3(arr3, actual_size);
		//qsort(arr2, actual_size, sizeof(arr2[0]), cmp);

		if (!is_equal(arr1, arr2, actual_size) || !is_equal(arr1, arr3, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
