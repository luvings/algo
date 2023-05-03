
#include "util.h"

/*
 * arr[L..R]上, 以arr[R]位置的数做划分值
 * 1. arr[i] <= num, 当前数与 <=区 下一个交换, <=区右扩, i++
 * 2. arr[i] > num,  i++;
 */
int partition(int arr[], int l, int r)
{
	if (l > r) {
		return -1;
	}

	if (l == r) {
		return l;
	}

	int less_equal = l - 1;
	int index = l;

	while (index < r) {
		if (arr[index] <= arr[r]) {
			swap(arr, index, ++less_equal);
		}
		index++;
	}

	swap(arr, ++less_equal, r);

	return less_equal;
}

/*
 * arr[L...R] 玩荷兰国旗问题的划分, 以arr[R]做划分值
 * < arr[R] ~~ == arr[R] ~~ > arr[R]
 */
int netherlands_flag(int arr[], int l, int r, int m_equal_area[])
{
	/* L...R, L > R */
	if (l > r) {
		m_equal_area[0] = -1;
		m_equal_area[1] = -1;
		return -1;
	}

	if (l == r) {
		m_equal_area[0] = l;
		m_equal_area[1] = l;
		return l;
	}

	int less = l - 1;	        /* < 区右边界 */
	int more = r;		        /* > 区左边界 */
	int index = l;
	while (index < more) {	    /* 当前位置, 不能和 >区的左边界撞上 */
		if (arr[index] == arr[r]) {
			index++;
		} else if (arr[index] < arr[r]) {
			swap(arr, index++, ++less);
		} else {
			swap(arr, index, --more);
		}
	}

	swap(arr, more, r);	    /* <[R]   =[R]   >[R] */

	m_equal_area[0] = less + 1;
	m_equal_area[1] = more;

	return less + 1;
}

void process_1(int arr[], int l, int r)
{
	if (l >= r) {
		return;
	}

	/* L..R partition arr[R] [ <=arr[R] arr[R] >arr[R] ] */
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

void process_2(int arr[], int l, int r)
{
	if (l >= r) {
		return;
	}

	int m_equal_area[2] = { -1, -1 };

	netherlands_flag(arr, l, r, m_equal_area);

	process_2(arr, l, m_equal_area[0] - 1);
	process_2(arr, m_equal_area[1] + 1, r);
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

	int m_equal_area[2] = { -1, -1 };

	netherlands_flag(arr, l, r, m_equal_area);

	process_3(arr, l, m_equal_area[0] - 1);
	process_3(arr, m_equal_area[1] + 1, r);
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
	int times = 10;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int arr3[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));
		memcpy(arr3, arr1, actual_size * sizeof(int));

		quick_sort_1(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");

		quick_sort_2(arr2, actual_size);
		quick_sort_3(arr3, actual_size);
		//qsort(arr2, actual_size, sizeof(int), cmp);

		if (!is_equal(arr1, arr2, actual_size) || !is_equal(arr1, arr3, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
