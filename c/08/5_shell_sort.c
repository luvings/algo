
#include "util.h"

/*
 * 希尔排序
 * 这个排序就是调整步长的插入排序, 也可以认为是插入排序的小改进版本
 * 改变不了时间复杂度, 只是优化了常数时间
 */
void shell_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	/*
	 * 步长依次为5、2、1
	 * 在插入排序中, 一步一步往前交换, 直到左边的数<=当前的数, 停止
	 * 在希尔排序中, 如果步长依次为5、2、1
	 * 那么来到每个数, 每次跳5步往前交换, 直到往前5步的数<=当前的数, 停止
	 * 然后再来到每个数, 每次跳3步往前交换, 直到往前3步的数<=当前的数, 停止
	 * 然后再来到每个数, 每次跳1步往前交换, 直到往前1步的数<=当前的数, 停止
	 */
	int step[] = { 5, 2, 1 };

	for (uint32_t s = 0; s < ARRAY_SIZE(step); s++) {
		for (int i = 0; i < n; i++) {
			for (int j = i - step[s]; j >= 0 && arr[j] > arr[j + step[s]]; j -= step[s]) {
				swap(arr, j, j + step[s]);
			}
		}
	}
}

void insertion_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	/*
	 * 0 ~ 0
	 * 0 ~ 1
	 * 0 ~ 2
	 * ...
	 * 0 ~ n-1
	 */
	for (int i = 1; i < n; i++) {
		for (int j = i - 1; j >= 0 && arr[j] > arr[j + 1]; j--) {
			swap(arr, j, j + 1);
		}
	}
}

int main(int argc, char *argv[])
{
	int times = 5;
	int max_size = 100;
	int max_value = 100;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		shell_sort(arr1, actual_size);
		//insertion_sort(arr2, actual_size);
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
