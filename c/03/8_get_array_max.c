
#include <stdio.h>

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/*
 *          [1, 2, 3, 4]
 *          /         \
 *       [1, 2]     [3, 4]
 *       /    \     /    \
 *      [1]  [2]   [3]  [4]
 */

/* arr[L..R]范围上求最大值  L ... R   N */
int process(int arr[], int l, int r)
{
	/* arr[L..R] 范围上只有一个数, 直接返回, base case */
	if (l == r) {
		return arr[l];
	}

	/* mid = (l + r) / 2 */
	int mid = l + ((r - l) >> 1);

	int l_max = process(arr, l, mid);
	int r_max = process(arr, mid + 1, r);

	return MAX(l_max, r_max);
}

int get_array_max(int arr[], int n)
{
	return process(arr, 0, n - 1);
}

int main(int argc, char *argv[])
{
	int arr[4] = { 1, 2, 3, 4 };

	printf("array max value: %d\n", get_array_max(arr, ARRAY_SIZE(arr)));

	return 0;
}
