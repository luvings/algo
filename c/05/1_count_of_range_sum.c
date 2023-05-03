
#include "util.h"

/*
 *  e.g.            [ 0, 1, 2, 3 ],     range [0, 2]
 *
 *  prefix_sum[] =  [ 0, 1, 3, 5 ]
 *
 *
 *              [ 0, 1, 3, 5 ]
 *               /          \
 *           [ 0, 1 ]    [ 3, 5 ]
 *            /    \      /    \
 *          [ 0 ] [ 1 ] [ 3 ] [ 5 ]
 *
 *
 *  max = 3 - 0 = 3, min = 3 - 2 = 1
 *
 *  右区减去 lower, 说明左区域可允许的最大数
 *  右区减去 upper, 说明左区域可允许的最小数
 *
 *  =>  wr = 2, wl = 1
 *
 *      wr -> exclusive
 *      wl -> inclusive
 *
 *  =>  res += wr - wl
 */
int merge(long prefix_sum[], int l, int m, int r, int lower, int upper)
{
	int res = 0;

	int window_l = l;
	int window_r = l;

	for (int i = m + 1; i <= r; i++) {
		long min = prefix_sum[i] - upper;
		long max = prefix_sum[i] - lower;

		while (window_r <= m && prefix_sum[window_r] <= max) {
			window_r++;
		}
		while (window_l <= m && prefix_sum[window_l] < min) {
			window_l++;
		}

		res += window_r - window_l;
	}

	int help[r - l + 1];
	int idx = 0;
	int p1 = l, p2 = m + 1;

	while (p1 <= m && p2 <= r) {
		help[idx++] = prefix_sum[p1] <= prefix_sum[p2] ? prefix_sum[p1++] : prefix_sum[p2++];
	}

	while (p1 <= m) {
		help[idx++] = prefix_sum[p1++];
	}

	while (p2 <= r) {
		help[idx++] = prefix_sum[p2++];
	}

	for (int i = 0; i < (r - l + 1); i++) {
		prefix_sum[l + i] = help[i];
	}

	return res;
}

int process(long prefix_sum[], int l, int r, int lower, int upper)
{
	if (l == r) {
		return prefix_sum[l] <= upper && prefix_sum[l] >= lower ? 1 : 0;
	}

	int m = l + ((r - l) >> 1);

	return process(prefix_sum, l, m, lower, upper)
	    + process(prefix_sum, m + 1, r, lower, upper)
	    + merge(prefix_sum, l, m, r, lower, upper);
}

int count_of_range_sum(int nums[], int n, int lower, int upper)
{
	if (nums == NULL || n == 0) {
		return 0;
	}

	long prefix_sum[n];

	prefix_sum[0] = nums[0];

	for (int i = 1; i < n; i++) {
		prefix_sum[i] = prefix_sum[i - 1] + nums[i];
	}

	return process(prefix_sum, 0, n - 1, lower, upper);
}

/*
 *  e.g.            [ -2, 5, -1 ],  range [-2, 2]
 *
 *  prefix_sum[] = [ 0, -2, 3, 2 ]
 *
 *  S(i, j) = prefix_sum[j + 1] - prefix_sum[i]
 *
 *  => S(1, 2) = 2 - (-2) = 4
 *
 *  => S(0, 2) = 2 - 0 = 2
 *
 */
int count_of_range_sum_vio(int nums[], int n, int lower, int upper)
{
	int count = 0;
	long prefix_sum[n + 1];

	if (nums == NULL || n == 0) {
		return count;
	}

	prefix_sum[0] = 0;

	for (int i = 0; i < n; i++) {
		prefix_sum[i + 1] = prefix_sum[i] + nums[i];
	}

	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n + 1; j++) {
			if (prefix_sum[j] - prefix_sum[i] >= lower && prefix_sum[j] - prefix_sum[i] <= upper) {
				count++;
			}
		}
	}

	return count;
}

int main(int argc, char *argv[])
{
	int max_size = 10;
	int max_value = 9;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		int arr1[max_size];
		int arr2[max_size];
		int actual_size = 0;

		create_random_array(max_size, max_value, arr1, &actual_size);
		memcpy(arr2, arr1, sizeof(arr1));

		int a = (int)((max_value + 1) * RANDOM());
		int b = (int)((max_value + 1) * RANDOM());
		int lower = MIN(a, b);
		int upper = MAX(a, b);
		if (lower == upper) {
			upper += 1;
		}

		int cnt_1 = count_of_range_sum(arr1, actual_size, lower, upper);
		int cnt_2 = count_of_range_sum_vio(arr2, actual_size, lower, upper);

		printf("[%d]:\t", actual_size);
		print_array_data(arr2, actual_size);
		printf("lower = %2d, upper = %2d\n", lower, upper);
		printf("count = %2d\n", cnt_1);
		printf("---8<---------------------------------------------------\n\n");

		if (cnt_1 != cnt_2) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
