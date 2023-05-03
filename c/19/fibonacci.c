
#include <stdio.h>

/*
 *      f(7) = f(5) + f(6)
 *      f(6) = f(5) + f(4)
 *              ^
 *              |
 *      暴力递归, 重复计算操作过多, 如 f(5)
 *
 *      optimize: f(5), f(4) .. 计算过程结果保存进数组, 后续使用直接拿取
 */
int fibonacci(int n)
{
	if (n == 1 || n == 2) {
		return 1;
	}

	return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char **argv)
{
	int n = 6;

	printf("fibonacci(%2d) = %2d\n", n, fibonacci(n));

	return 0;
}
