
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/*
 *  (a, b)
 *        (c, d)
 *
 *  b == d || abs(a - c) == abs(b - d)
 */
bool valid(int record[], int i, int j)
{
	for (int k = 0; k < i; k++) {
		if (record[k] == j || abs(i - k) == abs(record[k] - j)) {
			return false;
		}
	}

	return true;
}

/*
 * 当前来到i行, 一共是 0~N-1 行
 * 在 i 行上放皇后, 所有列都尝试
 * 必须要保证跟之前所有的皇后不打架
 * record[x] = y 之前的第x行的皇后, 放在了y列上
 * 返回: 不关心i以上发生了什么, i.... 后续有多少合法的方法数
 */
int process(int idx, int record[], int n)
{
	if (idx == n) {
		return 1;
	}

	int res = 0;

	/* i 行的皇后, 放哪一列呢? j 列 */
	for (int j = 0; j < n; j++) {
		if (valid(record, idx, j)) {
			record[idx] = j;
			res += process(idx + 1, record, n);
		}
	}

	return res;
}

int n_queens(int n)
{
	if (n < 1) {
		return 0;
	}

	int record[n];
	memset(record, 0, sizeof(record[0]) * n);

	return process(0, record, n);
}

/*
 * 优化常数项时间
 *
 * 7皇后问题
 * limit : 0....0 1 1 1 1 1 1 1
 * 之前皇后的列影响 : cols_limit
 * 之前皇后的左下对角线影响 : left_diagonal_lmt
 * 之前皇后的右下对角线影响 : right_diagonal_lmt
 */
int process_opt(int limit, int cols_limit, int left_diagonal_lmt, int right_diagonal_lmt)
{
	if (cols_limit == limit) {
		return 1;
	}

	int res = 0;

	int pos = limit & (~(cols_limit | left_diagonal_lmt | right_diagonal_lmt));
	int most_right_1 = 0;

	while (pos != 0) {
		most_right_1 = pos & (~pos + 1);
		/* ^ -> 无进位相加: pos = pos - most_right_1 */
		pos = pos ^ most_right_1;

		res += process_opt(limit,
				   cols_limit | most_right_1,
				   (left_diagonal_lmt | most_right_1) << 1, (right_diagonal_lmt | most_right_1) >> 1);
	}

	return res;
}

int n_queens_optimize(int n)
{
	if (n < 1 || n > 32) {
		return 0;
	}

	int limit = n == 32 ? -1 : (1 << n) - 1;

	return process_opt(limit, 0, 0, 0);
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
	int n = 9;
	long long t0 = __timestamp();

	//usleep(1 * 1000);
	printf("%d queens:     %d,\t", n, n_queens(n));
	printf("elapse: %lld ms\n", __timestamp() - t0);

	t0 = __timestamp();
	printf("%d queens opt: %d,\t", n, n_queens_optimize(n));
	printf("elapse: %lld ms\n", __timestamp() - t0);

	return 0;
}
