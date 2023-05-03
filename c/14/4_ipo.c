
#include <stdio.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

struct program {
	int capital;
	int profit;
};

/*
 * 最多 K 个项目
 * W 是初始资金
 * Profits[] Capital[] 一定等长
 * 返回最终最大的资金
 */
struct cmp {
	bool operator() (struct program a, struct program b) {
		return a.capital > b.capital;
	}
};

struct cmp_l {
	bool operator() (struct program a, struct program b) {
		return a.profit < b.profit;
	}
};

int find_maximum_captital(int k, int w, std::vector < struct program > progs)
{
	std::priority_queue < struct program, std::vector < struct program >, cmp > min_cost_q;
	std::priority_queue < struct program, std::vector < struct program >, cmp_l > max_profit_q;

    for (auto & e : progs) {
		min_cost_q.push(e);
	}

	for (int i = 0; i < k; i++) {
		while (!min_cost_q.empty() && min_cost_q.top().capital <= w) {
			max_profit_q.push(min_cost_q.top());
			min_cost_q.pop();
		}

		if (max_profit_q.empty()) {
			return w;
		}

		w += max_profit_q.top().profit;
		max_profit_q.pop();
	}

	return w;
}

std::vector < struct program > create_random_array(int max_size, int max_capital, int max_profit)
{
	int size = (int)(RANDOM() * (max_size + 1));
	std::vector < struct program > vec;

	for (int i = 0; i < size; i++) {
		struct program prog = { 0, };
		prog.capital = (int)(RANDOM() * (max_capital + 1)) + 1;
		prog.profit = (int)(RANDOM() * (max_profit + 1));

		vec.push_back(prog);
	}

	return vec;
}

void fun(struct program prog)
{
	std::cout.width(4);
	std::cout << "( " << prog.capital << ", " << prog.profit << " )  ";
}

int main(int argc, char **argv)
{
	int max_size = 10;
	int max_capital = 10;
	int max_profit = 10;
	int times = 5;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		std::vector < struct program > progs = create_random_array(max_size, max_capital, max_profit);
		for_each(progs.begin(), progs.end(), fun);
		std::cout << std::endl;

		printf("find maximum capital: %d\n", find_maximum_captital(2, 5, progs));

		printf("---8<---------------------------------------------------\n\n");
	}

	return 0;
}
