
#include "util.h"

int max_bits(int arr[], int n)
{
	int max = INT_MIN, res = 0;

	for (int i = 0; i < n; i++) {
		max = MAX(max, arr[i]);
	}

	while (max != 0) {
		max /= 10;
		res++;
	}

	return res;
}

int get_specific_digit(int x, int d)
{
	return ( ( x / ((int) pow(10, d)) ) % 10);
}

void radix_sort_un(int arr[], int l, int r, int digit)
{
	int radix = 10;
	int i = 0, j = 0;

	/* 有多少个数准备多少个辅助空间 */
	int help[r - l + 1];
	memset(help, 0, (r - l + 1) * sizeof(help[0]));

	for (int d = 0; d < digit; d++) {	/* 有多少位就进出几次 */
		/*
		 * 10 个空间
		 * count[0] 当前位 (d 位) 是  0        的数字有多少个
		 * count[1] 当前位 (d 位) 是 (0, 1)    的数字有多少个
		 * count[2] 当前位 (d 位) 是 (0, 1, 2) 的数字有多少个
		 * count[i] 当前位 (d 位) 是 (0 ~ i)   的数字有多少个
		 */
		int count[radix];
		memset(count, 0, (radix) * sizeof(count[0]));

		for (i = l; i <= r; i++) {
			/*
			 * 103  1   3
			 * 205  1   5
			 */
			j = get_specific_digit(arr[i], d);
			count[j]++;
		}

		/*
		 * such as, [ 103, 205, 342, 123 ]
		 *
		 * count[2] = 1, count[3] = 3, count[5] = 4
		 *
		 * 个位数 <= 3 的数字有 3 个, 则从右往左遍历,
		 *
		 * 首次出现末位数字为 3 放置在 help[2] 下标
		 */
		for (i = 1; i < radix; i++) {
			count[i] = count[i] + count[i - 1];
		}

		for (i = r; i >= l; i--) {
			j = get_specific_digit(arr[i], d);
			help[count[j] - 1] = arr[i];
			count[j]--;
		}

		for (i = l, j = 0; i <= r; i++) {
			arr[i] = help[j++];
		}
	}
}

/* only for no-negative value */
void radix_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	radix_sort_un(arr, 0, n - 1, max_bits(arr, n));
}

int create_random_array_s(int max_size, int max_value, int *array, int *actual_size)
{
	int size = (int)(RANDOM() * (max_size + 1));

	*actual_size = size;

	if (size == 0) {
		return 0;
	}

	for (int i = 0; i < size; i++) {
		array[i] = (int)(max_value + 1) * RANDOM();
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 20;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int actual_size = 0;
		int arr1[max_size];
		int arr2[max_size];

		create_random_array_s(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, sizeof(arr1));

		radix_sort(arr1, actual_size);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("sort:\t");
		print_array_data(arr1, actual_size);
		printf("---8<---------------------------------------------------\n\n");

		qsort(arr2, actual_size, sizeof(arr2[0]), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
