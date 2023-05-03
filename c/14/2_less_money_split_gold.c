
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <queue>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

int less_money(std::vector < int > vec)
{
	std::priority_queue < int, std::vector < int >, std::greater < int > > min_heap;

    for (auto & e : vec) {
		min_heap.push(e);
	}

	int sum = 0;
	int cur = 0;
	while (min_heap.size() > 1) {
		cur = min_heap.top();
		min_heap.pop();
		cur += min_heap.top();
		min_heap.pop();

		sum += cur;
		min_heap.push(cur);
	}

	return sum;
}

/*
 * 等待合并的数都在 arr 里, pre 之前的合并行为产生了多少总代价
 * arr 中只剩一个数字的时候, 停止合并, 返回最小的总代价
 */
std::vector < int > copy_and_merge_two(std::vector < int > vec, uint32_t ix, uint32_t jx)
{
	std::vector < int > cm;

	for (uint32_t i = 0; i < vec.size(); i++) {
		if (i != ix && i != jx) {
			cm.push_back(vec[i]);
		}
	}
	cm.push_back(vec[ix] + vec[jx]);

	return cm;
}

int process_cmp(std::vector < int > vec, int prev_expense)
{
	if (vec.size() == 1) {
		return prev_expense;
	}

	int ans = INT_MAX;

	for (uint32_t i = 0; i < vec.size(); i++) {
		for (uint32_t j = i + 1; j < vec.size(); j++) {
			ans = std::min(ans, process_cmp(copy_and_merge_two(vec, i, j), prev_expense + vec[i] + vec[j]));
		}
	}

	return ans;
}

int less_money_cmp(std::vector < int > vec)
{
	if (vec.size() == 0) {
		return 0;
	}

	return process_cmp(vec, 0);
}

std::vector < int > create_random_array(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	std::vector < int > vec;

	for (int i = 0; i < size; i++) {
		vec.push_back((int)(RANDOM() * (max_value + 1)));
	}

	return vec;
}

template < typename T > void fun(T val)
{
	std::cout.width(4);
	std::cout << val << ", ";
}

int main(int argc, char **argv)
{
	int max_size = 6;
	int max_value = 1000;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		std::vector < int > vec = create_random_array(max_size, max_value);

		if (less_money(vec) != less_money_cmp(vec)) {
			for_each(vec.begin(), vec.end(), fun < int >);
			std::cout << std::endl;

			printf("less money: %d, %d\n", less_money(vec), less_money_cmp(vec));
			printf("oops\n");
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
