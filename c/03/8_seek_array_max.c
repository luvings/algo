
#include <stdio.h>

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/*
 * arr[l..r], seek maximum value.
 *
 *          [1, 2, 3, 4]
 *          /         \
 *       [1, 2]     [3, 4]
 *       /    \     /    \
 *      [1]  [2]   [3]  [4]
 */
int process(int arr[], int l, int r)
{
	if (l == r) {	// base case
		return arr[l];
	}

	/* int m = (l + r) / 2 */
	int m = l + ((r - l) >> 1);

	int l_max = process(arr, l, m);
	int r_max = process(arr, m + 1, r);

	return MAX(l_max, r_max);
}

int seek_array_max(int arr[], int n)
{
	return process(arr, 0, n - 1);
}

int main(int argc, char *argv[])
{
	int arr[4] = { 1, 2, 3, 4 };

	printf("array[] max value: %d\n", seek_array_max(arr, ARRAY_SIZE(arr)));

	return 0;
}
