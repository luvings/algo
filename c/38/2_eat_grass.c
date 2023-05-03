
#include <stdio.h>

#include <string>

/*
 * 如果n份草, 最终先手赢, 返回"先手"
 * 如果n份草, 最终后手赢, 返回"后手"
 */
std::string who_win(int n)
{
	if (n < 5) {
		return n == 0 || n == 2 ? "后手" : "先手";
	}

	/* 进到这个过程里来, 当前的先手, 先选 */
	int want = 1;
	while (want <= n) {
		if (who_win(n - want) == ("后手")) {
			return "先手";
		}

		/* 防止 base * 4 之后溢出 */
		if (want <= (n / 4)) {
			want *= 4;
		} else {
			break;
		}
	}

	return "后手";
}

std::string winner(int n)
{
	if (n % 5 == 0 || n % 5 == 2) {
		return "后手";
	} else {
		return "先手";
	}
}

int main(int argc, char **argv)
{
	for (int i = 0; i <= 50; i++) {
		printf("%4d : %s\n", i, who_win(i).c_str());
	}

	return 0;
}
