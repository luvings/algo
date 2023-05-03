
#include <stdio.h>
#include <string.h>

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/*
 * index 0~N
 * rest 负~bag
 */
int process(int w[], int v[], int n, int idx, int rest)
{
	if (rest < 0) {
		return -1;
	}

	if (idx == n) {
		return 0;
	}

	int p1 = process(w, v, n, idx + 1, rest);
	int p2 = 0;
	int next = process(w, v, n, idx + 1, rest - w[idx]);
	if (next != -1) {
		p2 = v[idx] + next;
	}

	return MAX(p1, p2);
}

/*
 * 所有的货, 重量和价值, 都在 w 和 v 数组里
 * 为了方便, 其中没有负数
 * bag 背包容量, 不能超过这个载重
 * 返回: 不超重的情况下, 能够得到的最大价值
 */
int max_value(int w[], int v[], int n, int bag)
{
	if (w == NULL || v == NULL || n == 0) {
		return 0;
	}

	return process(w, v, n, 0, bag);
}

int ways_dp(int w[], int v[], int n, int bag)
{
	if (w == NULL || v == NULL || n == 0) {
		return 0;
	}

	int dp[n + 1][bag + 1];
	memset(dp, 0, sizeof(dp));

	/* dp[n][...] = 0; */

	for (int idx = n - 1; idx >= 0; idx--) {
		for (int rest = 0; rest <= bag; rest++) {
			int p1 = dp[idx + 1][rest];
			int p2 = 0;
			int next = rest - w[idx] < 0 ? -1 : dp[idx + 1][rest - w[idx]];
			if (next != -1) {
				p2 = v[idx] + next;
			}

			dp[idx][rest] = MAX(p1, p2);
		}
	}

	return dp[0][bag];
}

int main(int argc, char **argv)
{
	int weights[] = { 3, 2, 4, };
	int values[] = { 5, 6, 7, };
	int bag = 7;

	printf("max value : %d\n", max_value(weights, values, ARRAY_SIZE(weights), bag));
	printf("max value : %d\n", ways_dp(weights, values, ARRAY_SIZE(weights), bag));

	return 0;
}
