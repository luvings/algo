
#include <stdio.h>

#include <string>

void func(int n, std::string from, std::string to, std::string other)
{
	if (n == 1) {
		printf("move 1 from %s to %s\n", from.c_str(), to.c_str());
	} else {
		func(n - 1, from, other, to);
		printf("move %d from %s to %s\n", n, from.c_str(), to.c_str());
		func(n - 1, other, to, from);
	}
}

void hanoi_1(int n)
{
	if (n > 0) {
		func(n, "left", "right", "mid");
	}
}

extern void left_2_right(int n);
extern void left_2_mid(int n);
extern void mid_2_left(int n);
extern void mid_2_right(int n);
extern void right_2_mid(int n);
extern void right_2_left(int n);

void left_2_right(int n)
{
	if (n == 1) {
		printf("move 1 from left to right\n");
		return;
	}

	left_2_mid(n - 1);
	printf("move %d from left to right\n", n);
	mid_2_right(n - 1);
}

void right_2_mid(int n)
{
	if (n == 1) {
		printf("move 1 from right to mid\n");
		return;
	}

	right_2_left(n - 1);
	printf("move %d from right to mid\n", n);
	left_2_mid(n - 1);
}

/* 请把 1~N 层圆盘 从左 -> 中 */
void left_2_mid(int n)
{
	if (n == 1) {
		printf("move 1 from left to mid\n");
		return;
	}

	left_2_right(n - 1);
	printf("move %d from left to mid\n", n);
	right_2_mid(n - 1);
}

void right_2_left(int n)
{
	if (n == 1) {
		printf("move 1 from right to left\n");
		return;
	}

	right_2_mid(n - 1);
	printf("move %d from right to left\n", n);
	mid_2_left(n - 1);
}

void mid_2_right(int n)
{
	if (n == 1) {
		printf("move 1 from mid to right\n");
		return;
	}

	mid_2_left(n - 1);
	printf("move %d from mid to right\n", n);
	left_2_right(n - 1);
}

void mid_2_left(int n)
{
	if (n == 1) {
		printf("move 1 from mid to left\n");
		return;
	}

	mid_2_right(n - 1);
	printf("move %d from mid to left\n", n);
	right_2_left(n - 1);
}

/* 请把 1~N 层圆盘 从左 -> 右 */
void hanoi_2(int n)
{
	if (n == 1) {
		printf("move 1 from left to right\n");
		return;
	}

	left_2_mid(n - 1);
	printf("move %d from left to right\n", n);
	mid_2_right(n - 1);
}

int main(int argc, char **argv)
{
	int n = 3;

	hanoi_1(n);
	printf("\n---8<--------------------------------------------------\n\n");

	hanoi_2(n);
	printf("\n---8<--------------------------------------------------\n\n");

	return 0;
}
