
#include "util.h"

int max_bits(int arr[], int n)
{
	int max = INT_MIN;

	for (int i = 0; i < n; i++) {
		max = MAX(max, arr[i]);
	}

	int res = 0;
	while (max != 0) {
		max /= 10;
		res++;
	}

	return res;
}

int get_digit(int x, int d)
{
	return ((x / ((int)pow(10, d - 1))) % 10);
}

void radix_sort_un(int arr[], int l, int r, int digit)
{
	int radix = 10;
	int i = 0, j = 0;

	/* 有多少个数准备多少个辅助空间 */
	int help[r - l + 1];
	memset(help, 0, (r - l + 1) * sizeof(int));

	for (int d = 1; d <= digit; d++) {	/* 有多少位就进出几次 */
		/*
		 * 10个空间
		 * count[0] 当前位(d位)是0的数字有多少个
		 * count[1] 当前位(d位)是(0和1)的数字有多少个
		 * count[2] 当前位(d位)是(0、1和2)的数字有多少个
		 * count[i] 当前位(d位)是(0~i)的数字有多少个
		 */
		int count[radix];
		memset(count, 0, (radix) * sizeof(int));

		for (i = l; i <= r; i++) {
			/*
			 * 103  1   3
			 * 209  1   9
			 */
			j = get_digit(arr[i], d);
			count[j]++;
		}

		/*
		 * such as, count[3] = 5, count[4..9] = 5,
		 * 个位数 <= 3..9 的数字有5 个, 则从右往左遍历,
		 * 首次出现末位数字为 3..9 放置在4 下标
		 */
		for (i = 1; i < radix; i++) {
			count[i] = count[i] + count[i - 1];
		}

		for (i = r; i >= l; i--) {
			j = get_digit(arr[i], d);
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
	int max_size = 100;
	int max_value = 100;
	int times = 5;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int actual_size = 0;
		int arr1[max_size];
		int arr2[max_size];

		create_random_array_s(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		radix_sort(arr1, actual_size);
		qsort(arr2, actual_size, sizeof(int), cmp);

		if (!is_equal(arr1, arr2, actual_size)) {
			succeed = false;
			printf("---8>--------------------------------------------------\n");
			printf("arr1:\t");
			print_array_data(arr1, actual_size);
			printf("arr2:\t");
			print_array_data(arr2, actual_size);
			printf("---8<---------------------------------------------------\n\n");
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
