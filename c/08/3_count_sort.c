
#include "util.h"

/* only for 0~200 value */
void count_sort(int arr[], int n)
{
	if (arr == NULL || n < 2) {
		return;
	}

	int max = INT_MIN;
	for (int i = 0; i < n; i++) {
		max = MAX(max, arr[i]);
	}

	int bucket[max + 1];
	memset(bucket, 0, sizeof(int) * (max + 1));

	for (int i = 0; i < n; i++) {
		bucket[arr[i]]++;
	}

	for (int i = 0, j = 0; j < max + 1; j++) {
		while (bucket[j]-- > 0) {
			arr[i++] = j;
		}
	}
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
	int max_size = 200;
	int max_value = 150;
	int times = 5;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		int actual_size = 0;
		int arr1[max_size];
		int arr2[max_size];
		memset(arr1, 0, sizeof(int) * max_size);
		memset(arr2, 0, sizeof(int) * max_size);

		create_random_array_s(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, actual_size * sizeof(int));

		count_sort(arr1, actual_size);
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
