
#include <stdio.h>
#include <string.h>

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/* arr[L..R], 先手获得的最好分数返回 */
int f(int arr[], int l, int r)
{
	extern int g(int[], int, int);

	if (l == r) {
		return arr[l];
	}

	int p1 = arr[l] + g(arr, l + 1, r);
	int p2 = arr[r] + g(arr, l, r - 1);

	return MAX(p1, p2);
}

/* arr[L..R], 后手获得的最好分数返回 */
int g(int arr[], int l, int r)
{
	if (l == r) {
		return 0;
	}

	int p1 = f(arr, l + 1, r);	/* 对手拿走了L位置的数 */
	int p2 = f(arr, l, r - 1);	/* 对手拿走了R位置的数 */

	return MIN(p1, p2);
}

/* 根据规则, 返回获胜者的分数 */
int win(int arr[], int n)
{
	if (arr == NULL || n == 0) {
		return 0;
	}

	int first = f(arr, 0, n - 1);
	int second = g(arr, 0, n - 1);

	return MAX(first, second);
}

int f_c(int arr[], int l, int r, int *fmap, int *gmap, int line)
{
	extern int g_c(int[], int, int, int *, int *, int);

	/* fmap[l][r] != -1; */
	if (*(fmap + l * line + r) != -1) {
		return *(fmap + l * line + r);
	}

	int res = 0;

	if (l == r) {
		res = arr[l];
	} else {
		int p1 = arr[l] + g_c(arr, l + 1, r, fmap, gmap, line);
		int p2 = arr[r] + g_c(arr, l, r - 1, fmap, gmap, line);
		res = MAX(p1, p2);
	}

	/* fmap[l][r] = res; */
	*(fmap + l * line + r) = res;

	return res;
}

int g_c(int arr[], int l, int r, int *fmap, int *gmap, int line)
{

	/* gmap[l][r] != -1; */
	if (*(gmap + l * line + r) != -1) {
		return *(gmap + l * line + r);
	}

	int res = 0;

	if (l == r) {
		res = 0;
	} else {
		int p1 = f_c(arr, l + 1, r, fmap, gmap, line);	/* 对手拿走了L位置的数 */
		int p2 = f_c(arr, l, r - 1, fmap, gmap, line);	/* 对手拿走了R位置的数 */
		res = MIN(p1, p2);
	}

	/* gmap[l][r] = res; */
	*(gmap + l * line + r) = res;

	return res;
}

int win_cache(int arr[], int n)
{
	if (arr == NULL || n == 0) {
		return 0;
	}

	int fmap[n][n];
	int gmap[n][n];
	memset(fmap, -1, sizeof(fmap));
	memset(gmap, -1, sizeof(gmap));

	int first = f_c(arr, 0, n - 1, &fmap[0][0], &gmap[0][0], n);
	int second = g_c(arr, 0, n - 1, &fmap[0][0], &gmap[0][0], n);

	return MAX(first, second);
}

/*
 * int arr[] = { 5, 7, 4 };
 *
 *      0   1   2 -> r              0   1   2 -> r
 *  0 | 5 | 7 | 9 |             0 | 0 | 5 | 7 |
 *  1 | x | 7 | 7 |             1 | x | 0 | 4 |
 *  2 | x | x | 4 |             2 | x | x | 0 |
 *  ^                           ^
 *  l                           l
 *
 */
int win_dp(int arr[], int n)
{
	if (arr == NULL || n == 0) {
		return 0;
	}

	int fmap[n][n];
	int gmap[n][n];
	memset(fmap, 0, sizeof(fmap));
	memset(gmap, 0, sizeof(gmap));

	for (int i = 0; i < n; i++) {
		fmap[i][i] = arr[i];
	}

	for (int cols = 1; cols < n; cols++) {
		int l = 0;
		int r = cols;

		while (r < n) {
			fmap[l][r] = MAX(arr[l] + gmap[l + 1][r], arr[r] + gmap[l][r - 1]);
			gmap[l][r] = MIN(fmap[l + 1][r], fmap[l][r - 1]);

			l++;
			r++;
		}
	}

	return MAX(fmap[0][n - 1], gmap[0][n - 1]);
}

int main(int argc, char **argv)
{
	//int arr[] = { 5, 7, 4, 5, 8, 1, 6, 0, 3, 4, 6, 1, 7 };
	int arr[] = { 5, 7, 4 };

	printf("%d\n", win(arr, ARRAY_SIZE(arr)));
	printf("%d\n", win_cache(arr, ARRAY_SIZE(arr)));
	printf("%d\n", win_dp(arr, ARRAY_SIZE(arr)));

	return 0;
}
