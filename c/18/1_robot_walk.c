
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/*
 * N    : 位置为 1 ~ N, 固定参数
 * cur  : 当前在 cur 位置, 可变参数
 * rest : 还剩 rest 步没有走, 可变参数
 * aim  : 最终目标位置是 aim, 固定参数
 * 含义 : 只能在 1~N 这些位置上移动, 当前在 cur 位置, 走完 rest 步之后, 停在 aim 位置的方法数作为返回值
 */
int process_vio(int cur, int rest, int aim, int n)
{
	/*
	 * 如果没有剩余步数了, 当前的 cur 位置就是最后的位置
	 * 如果最后的位置停在 aim 上, 那么之前做的移动是有效的
	 * 如果最后的位置没在 aim 上, 那么之前做的移动是无效的
	 */
	if (rest == 0) {
		return cur == aim ? 1 : 0;
	}

	/*
	 * 如果还有 rest 步要走, 而当前的 cur 位置在 1 位置上, 那么当前这步只能从 1 走向 2
	 * 后续的过程就是, 来到 2 位置上, 还剩 rest - 1 步要走
	 */
	if (cur == 1) {
		return process_vio(2, rest - 1, aim, n);
	}

	/*
	 * 如果还有 rest 步要走, 而当前的 cur 位置在 N 位置上, 那么当前这步只能从 N 走向 N-1
	 * 后续的过程就是, 来到 N-1 位置上, 还剩 rest-1 步要走
	 */
	if (cur == n) {
		return process_vio(n - 1, rest - 1, aim, n);
	}

	/*
	 * 如果还有 rest 步要走, 而当前的 cur 位置在中间位置上, 那么当前这步可以走向左, 也可以走向右
	 * 走向左之后, 后续的过程就是, 来到 cur-1 位置上, 还剩 rest-1 步要走
	 * 走向右之后, 后续的过程就是, 来到 cur+1 位置上, 还剩 rest-1 步要走
	 * 走向左、走向右是截然不同的方法, 所以总方法数要都算上
	 */
	return process_vio(cur - 1, rest - 1, aim, n)
	    + process_vio(cur + 1, rest - 1, aim, n);
}

/*
 * violent recursion
 */
int ways_vio(int n, int start, int aim, int k)
{
	if (n < 2 || start < 1 || start > n || aim < 1 || aim > n || k < 1) {
		return -1;
	}

	/* 总共 n 个位置, 从 start 点出发, 还剩 k 步, 返回最终能达到 aim 的方法数 */
	return process_vio(start, k, aim, n);
}

/*
 * cur  范围 : 1 ~ N
 * rest 范围 : 0 ~ K
 */
int process_cache(int cur, int rest, int aim, int n, int *dp, int line)
{
	if (*(dp + cur * line + rest) != -1) {
		return *(dp + cur * line + rest);
	}

	/* 之前没算过 */
	int res = 0;

	if (rest == 0) {
		res = (cur == aim) ? 1 : 0;
	} else if (cur == 1) {
		res = process_cache(2, rest - 1, aim, n, dp, line);
	} else if (cur == n) {
		res = process_cache(n - 1, rest - 1, aim, n, dp, line);
	} else {
		res = process_cache(cur - 1, rest - 1, aim, n, dp, line)
		    + process_cache(cur + 1, rest - 1, aim, n, dp, line);
	}

	*(dp + cur * line + rest) = res;

	return res;
}

int ways_dp(int n, int start, int aim, int k)
{
	if (n < 2 || start < 1 || start > n || aim < 1 || aim > n || k < 1) {
		return -1;
	}

	int dp[n + 1][k + 1];
	memset(dp, 0, sizeof(dp));

	dp[aim][0] = 1;

	for (int rest = 1; rest <= k; rest++) {
		dp[1][rest] = dp[2][rest - 1];

		for (int cur = 2; cur < n; cur++) {
			dp[cur][rest] = dp[cur - 1][rest - 1] + dp[cur + 1][rest - 1];
		}

		dp[n][rest] = dp[n - 1][rest - 1];
	}

	return dp[start][k];
}

/*
 * 记忆化搜索
 *
 * std::map < std::string, int > map;
 *
 * such as: start = 2, k = 5 -> "2_5"
 *
 *      n => [1, 7], aim = 3, start = 2, k = 5
 *
 *      cur = 1, rest = ?  -> 与 cur = 2, rest - 1 相关, 即第一行 ? 位置与 (2, 0) 结果相同
 *      cur = 7, rest = ?  -> 与 cur = 6, rest - 1 相关, 即最后一行 ? 位置与 (6, 0) 结果相同
 *
 *      中间数据 = 左下斜角 + 左上斜角
 *
 *      rest == 0 时, 0 列可直接推出结果, (cur == aim) ? 1 : 0
 *
 *           0   1   2   3   4   5   rest
 *      0  | x | x | x | x | x | x |
 *      1  | 0 | ? | 1 | 0 | 3 | 0 |
 *      2  | 0 | 1 | 0 | 3 | 0 | 9 |  <-*
 *      3  | 1 | 0 | 2 | 0 | 6 | 0 |
 *      4  | 0 | 1 | 0 | 3 | 0 | 10|
 *      5  | 0 | 0 | 1 | 0 | 4 | 0 |
 *      6  | 0 | 0 | 0 | 1 | 0 | 5 |
 *      7  | 0 | ? | 0 | 0 | 1 | 0 |
 *     cur
 */
int ways_cache(int n, int start, int aim, int k)
{
	if (n < 2 || start < 1 || start > n || aim < 1 || aim > n || k < 1) {
		return -1;
	}

	/*
	 * dp 就是缓存表
	 * dp[cur][rest] == -1 -> process1(cur, rest) 之前没算过.
	 * dp[cur][rest] != -1 -> process1(cur, rest) 之前算过. 返回值, dp[cur][rest]
	 * N+1 * K+1
	 */
	int line = k + 1;
	int dp[n + 1][k + 1];
	memset(dp, -1, sizeof(dp));

	for (int rows = 0; rows <= n; rows++) {
		for (int cols = 0; cols <= k; cols++) {
			dp[rows][cols] = -1;
		}
	}

	/*
	 * FIXME:
	 *
	 * *a <==> *&a[0], 也即一维数组 a[0]
	 * *a[0] <==> *&a[0][0], 也即 a[0][0]
	 * &a[0][0] 是一维数组 a[0] 的首元素地址, &a[0] 是整个一维数组 a[0] 的地址, 它们的值相同但是含义不同.
	 * 
	 * a 是二维数组名, 它可以代表数组首元素地址
	 * 二维数组都是由一维数组组成的, 所以二维数组 a 的首元素就是一维数组 a[0], a 就是 a[0] 的地址
	 * 假设为p (p是行指针), a+i 相当于 p+i,
	 * 也就是 p 跳过 i 个一维数组后的地址, 也就是 a[i] 的地址, 也即 &a[i]
	 * 
	 * 如果 a 是二维数组, 那么 a+1 就是 &a[1], *(a+1) 就是 a[1]
	 */
	return process_cache(start, k, aim, n, &dp[0][0], line);
}

long long __timestamp(void)
{
	long long t;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	t = tv.tv_sec;
	t = t * 1000;
	t = t + (tv.tv_usec / 1000);

	return t;
}

int main(int argc, char **argv)
{
	long long t0 = __timestamp();

	printf("%d,\t", ways_vio(7, 2, 3, 5));
	printf("elapse: %lld ms\n", __timestamp() - t0);

	t0 = __timestamp();
	printf("%d,\t", ways_cache(7, 2, 3, 5));
	printf("elapse: %lld ms\n", __timestamp() - t0);

	t0 = __timestamp();
	printf("%d,\t", ways_dp(7, 2, 3, 5));
	printf("elapse: %lld ms\n", __timestamp() - t0);

	return 0;
}
