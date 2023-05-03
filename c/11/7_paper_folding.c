
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * 当前你来了一个节点, 脑海中想象的
 * 这个节点在第i层, 一共有N层, N固定不变的
 * 这个节点如果是凹的话, down = T
 * 这个节点如果是凸的话, down = F
 * 函数的功能: 中序打印以你想象的节点为头的整棵树
 *
 *  对折 2 次
 *
 *  ______________________________________________
 *  |__________|__________|__________|___________|
 *
 *            2down     1down       2up
 *
 *
 *  对折 3 次
 *
 *  _________________________________________________
 *  |_____|_____|_____|_____|_____|_____|_____|______|
 *
 *       3d     2d    3u    1d    3d    2u    3u
 *
 */
void process(int i, int n, bool down)
{
	if (i > n) {
		return;
	}

	process(i + 1, n, true);
	printf(down ? "%d凹,  " : "%d凸,  ", i);
	process(i + 1, n, false);
}

void print_all_folds(int n)
{
	process(1, n, true);
	printf("\n");
}

int main(int argc, char **argv)
{
	int N = 3;

	print_all_folds(N);

	return 0;
}
