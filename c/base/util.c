
#include "util.h"

void swap(int *data, int i, int j)
{
	if (i != j) {
		data[i] = data[i] ^ data[j];
		data[j] = data[i] ^ data[j];
		data[i] = data[i] ^ data[j];

		return;
	}

	int tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}

void print_array_data(int *data, int n)
{
	if (data == NULL) {
		return;
	}

	for (int i = 0; i < n; i++) {
		printf("%4d\t", data[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}

	printf("\n");
}

int create_random_array(int max_size, int max_value, int *array, int *actual_length)
{
	int size = (int)(RANDOM() * (max_size + 1));

	*actual_length = size;

	if (size == 0) {
		return 0;
	}

	for (int i = 0; i < size; i++) {
		/* [-?, +?] */
		// array[i] = (int)(max_value * RANDOM()) - (int)((max_value + 1) * RANDOM());
		array[i] = (int)((max_value + 1) * RANDOM());
	}

	return 0;
}

int cmp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

bool is_equal(int *a, int *b, int n)
{
	if ((a == NULL && b != NULL) || (a != NULL && b == NULL)) {
		return false;
	}

	if (a == NULL && b == NULL) {
		return true;
	}

	for (int i = 0; i < n; i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}

	return true;
}

/*---8<-----------------------------------------------------------------------*/

/*
 * TODO:
 *
 *
 */
