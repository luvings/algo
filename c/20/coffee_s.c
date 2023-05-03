
#include <limits.h>
#include <stdio.h>
#include <string.h>

#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#define MIN(a, b)       ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof((x)[0]))

/*
 * process(drinks, 3, 10, 0,0)
 *
 * a 洗一杯的时间 固定变量
 * b 自己挥发干净的时间 固定变量
 * drinks 每一个员工喝完的时间 固定变量
 * drinks[0..index-1]都已经干净了, 不用你操心了
 * drinks[index...]都想变干净, 这是我操心的, washLine表示洗的机器何时可用
 * drinks[index...]变干净, 最少的时间点返回
 */
int process_vio(int drinks[], int n, int a, int b, int idx, int wash_line)
{
	if (idx == n - 1) {
		return MIN(MAX(wash_line, drinks[idx]) + a, drinks[idx] + b);
	}

	/*
	 * 剩不止一杯咖啡
	 * wash 是我当前的咖啡杯, 洗完的时间
	 */
	int wash = MAX(wash_line, drinks[idx]) + a;	/* 洗, index一杯, 结束的时间点 */
	/* index+1...变干净的最早时间 */
	int next1 = process_vio(drinks, n, a, b, idx + 1, wash);
	int p1 = MAX(wash, next1);

	int dry = drinks[idx] + b;	/* 挥发, index一杯, 结束的时间点 */
	int next2 = process_vio(drinks, n, a, b, idx + 1, wash_line);
	int p2 = MAX(dry, next2);

	return MIN(p1, p2);
}

int min_time_vio(int drinks[], int n, int a, int b)
{
	if (drinks == NULL || n < 1) {
		return -1;
	}

	return process_vio(drinks, n, a, b, 0, 0);
}

int min_time_dp(int drinks[], int n, int a, int b)
{
	if (drinks == NULL || n < 1) {
		return -1;
	}

	if (a >= b) {
		return drinks[n - 1] + b;
	}
	/* 全洗杯子为最长时间 */
	int limit = 0;		/* 咖啡机什么时候可用 */
	for (int i = 0; i < n; i++) {
		limit = MAX(limit, drinks[i]) + a;
	}

	int dp[n][limit + 1];
	memset(dp, 0, sizeof(dp));

	/* N-1行, 所有的值 */
	for (int wash_line = 0; wash_line <= limit; wash_line++) {
		dp[n - 1][wash_line] = MIN(MAX(wash_line, drinks[n - 1]) + a, drinks[n - 1] + b);
	}

	for (int index = n - 2; index >= 0; index--) {
		for (int wash_line = 0; wash_line <= limit; wash_line++) {
			int p1 = INT_MAX;

			int wash = MAX(wash_line, drinks[index]) + a;
			if (wash <= limit) {
				p1 = MAX(wash, dp[index + 1][wash]);
			}

			int p2 = MAX(drinks[index] + b, dp[index + 1][wash_line]);

			dp[index][wash_line] = MIN(p1, p2);
		}
	}

	return dp[0][0];
}

int main(int argc, char **argv)
{
	int test[] = { 1, 1, 5, 5, 7, 10, 12, 12, 12, 12, 12, 12, 15 };
	int a = 3;
	int b = 10;

	printf("%d\n", min_time_vio(test, ARRAY_SIZE(test), a, b));
	printf("%d\n", min_time_dp(test, ARRAY_SIZE(test), a, b));

	return 0;
}
