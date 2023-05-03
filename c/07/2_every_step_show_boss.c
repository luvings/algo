/*
 * 给定一个整型数组, int arr[] 和一个布尔类型数组, boolean[] op
 * 两个数组一定等长, 假设长度为N, arr[i]表示客户编号, op[i]表示客户操作
 * arr= [3,3,1,2,1,2,5…
 * op = [T,T,T,T,F,T,F…
 * 依次表示:
 * 3用户购买了一件商品
 * 3用户购买了一件商品
 * 1用户购买了一件商品
 * 2用户购买了一件商品
 * 1用户退货了一件商品
 * 2用户购买了一件商品
 * 5用户退货了一件商品…
 * 一对arr[i]和op[i]就代表一个事件:
 * 用户号为arr[i], op[i] == T就代表这个用户购买了一件商品
 * op[i] == F就代表这个用户退货了一件商品
 * 现在你作为电商平台负责人, 你想在每一个事件到来的时候,
 * 都给购买次数最多的前K名用户颁奖.
 * 所以每个事件发生后, 你都需要一个得奖名单（得奖区）
 * 得奖系统的规则:
 *  1. 如果某个用户购买商品数为0, 但是又发生了退货事件,
 *      则认为该事件无效, 得奖名单和上一个事件发生后一致, 例子中的5用户
 *  2. 某用户发生购买商品事件, 购买商品数+1, 发生退货事件, 购买商品数-1
 *  3. 每次都是最多K个用户得奖, K也为传入的参数
 *      如果根据全部规则, 得奖人数确实不够K个, 那就以不够的情况输出结果
 *  4. 得奖系统分为得奖区和候选区, 任何用户只要购买数>0,
 *      一定在这两个区域中的一个
 *  5. 购买数最大的前K名用户进入得奖区,
 *      在最初时如果得奖区没有到达K个用户, 那么新来的用户直接进入得奖区
 *  6. 如果购买数不足以进入得奖区的用户, 进入候选区
 *  7. 如果候选区购买数最多的用户, 已经足以进入得奖区,
 *      该用户就会替换得奖区中购买数最少的用户（大于才能替换）,
 *      如果得奖区中购买数最少的用户有多个, 就替换最早进入得奖区的用户
 *      如果候选区中购买数最多的用户有多个, 机会会给最早进入候选区的用户
 *  8. 候选区和得奖区是两套时间,
 *     因用户只会在其中一个区域, 所以只会有一个区域的时间, 另一个没有
 *     从得奖区出来进入候选区的用户, 得奖区时间删除,
 *     进入候选区的时间就是当前事件的时间（可以理解为arr[i]和op[i]中的i）
 *     从候选区出来进入得奖区的用户, 候选区时间删除,
 *     进入得奖区的时间就是当前事件的时间（可以理解为arr[i]和op[i]中的i）
 *  9. 如果某用户购买数==0, 不管在哪个区域都离开, 区域时间删除,
 *     离开是指彻底离开, 哪个区域也不会找到该用户
 *     如果下次该用户又发生购买行为, 产生>0的购买数,
 *     会再次根据之前规则回到某个区域中, 进入区域的时间重记
 * 请遍历arr数组和op数组, 遍历每一步输出一个得奖名单
 * public List<List<Integer>>  topK (int[] arr, boolean[] op, int k)
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

struct customer {
	int id;
	int buy;
	int enter_time;
};

/*---8>-----------------------------------------------------------------------*/

typedef struct customer T;

struct heap_greater {
	int h_size;
	int (*comp) (T a, T b);

	std::vector < T > v;
	std::map < T, int > i_map;	/* (val, idx) */
};

inline bool operator <(const T & a, const T & b)
{
	return a.id < b.id;
}

int comp(T a, T b)
{
	return a.buy != b.buy ? (a.buy - b.buy) : (a.enter_time - b.enter_time);
}

void swap(struct heap_greater *h, int i, int j)
{
	T o1 = h->v[i];
	T o2 = h->v[j];

	h->v[i] = o2;
	h->v[j] = o1;

	h->i_map[o1] = j;
	h->i_map[o2] = i;

}

void heap_insert(struct heap_greater *h, int idx)
{
	while (comp(h->v[idx], h->v[(idx - 1) / 2]) < 0) {
		swap(h, idx, (idx - 1) / 2);
		idx = (idx - 1) / 2;
	}
}

void heapify(struct heap_greater *h, int idx, int h_size)
{
	int left = idx * 2 + 1;

	while (left < h->h_size) {
		int least = (left + 1) < h->h_size && comp(h->v[left + 1], h->v[left]) < 0 ? left + 1 : left;

		least = comp(h->v[idx], h->v[least]) < 0 ? idx : least;
		if (idx == least) {
			break;
		}

		swap(h, idx, least);
		idx = least;
		left = idx * 2 + 1;
	}
}

void push(struct heap_greater *h, T x)
{
	h->v.push_back(x);
	h->i_map.insert(std::make_pair(x, h->h_size));

	heap_insert(h, h->h_size++);
}

T pop(struct heap_greater *h)
{
	if (h->h_size == 0) {
		printf("heap has been empty\n");
		T one = { -1, };
		return one;
	}

	T res = h->v.at(0);

	swap(h, 0, h->h_size - 1);

	h->i_map.erase(h->i_map.find(res));
	h->v.pop_back();
	h->h_size--;

	heapify(h, 0, h->h_size);

	return res;
}

void resign(struct heap_greater *h, T x)
{
	heap_insert(h, h->i_map.find(x)->second);
	heapify(h, h->i_map.find(x)->second, h->h_size);

	/*
	   heap_insert(h, idx);
	   heapify(h, idx, h->h_size);
	 */
}

void remove(struct heap_greater *h, T x)
{
	if (h->i_map.find(x) == h->i_map.end()) {
		printf("x doesn't exist\n");
		return;
	}

	T replace = h->v[h->h_size - 1];
	h->v.pop_back();
	h->h_size--;

	int idx = h->i_map[x];
	h->i_map.erase(h->i_map.find(x));

	printf("remove.. [%d] = %d\n", idx, x.id);

	if (x.id != replace.id) {
		h->v[idx] = replace;
		h->i_map[replace] = idx;
		/*
		   h->i_map.erase(h->i_map.find(replace));
		   h->i_map.insert(std::make_pair(replace, idx));
		 */

		resign(h, replace);
	}
}

T peek(struct heap_greater *h)
{
	return h->v.front();
}

bool contains(struct heap_greater * h, T x)
{
	return (h->i_map.find(x) != h->i_map.end());
}

bool empty(struct heap_greater * h)
{
	if (h->v.size() != h->i_map.size()) {
		printf("oops: v.size(%ld) != i_map.size(%ld)\n", h->v.size(), h->i_map.size());
	}

	return h->h_size == 0;
}

bool full(struct heap_greater * h)
{
	//return h->h_size == HEAP_LIMIT;
	return false;
}

/*---8<-----------------------------------------------------------------------*/

struct data {
	std::vector < int > arr;
	std::vector < bool > op;
};

struct whos_daddy {
	int limits;

	std::map < int, struct customer > customers;

	struct heap_greater candidate;
	struct heap_greater winner;
};

void move(struct whos_daddy *whos, int time)
{
	if (empty(&whos->candidate)) {
		return;
	}

	if (whos->winner.h_size < whos->limits) {
		T x = pop(&whos->candidate);
		x.enter_time = time;

		push(&whos->winner, x);
	} else {
		if (peek(&whos->candidate).buy > peek(&whos->winner).buy) {
			T r_old = pop(&whos->winner);
			T r_new = pop(&whos->candidate);

			r_old.enter_time = time;
			r_new.enter_time = time;

			push(&whos->winner, r_new);
			push(&whos->candidate, r_old);
		}
	}
}

/* 当前处理 i 号事件, time -> i, arr[i] -> id, op[i] -> buy or refund ? */
void operate(struct whos_daddy *whos, int time, int id, bool buy_refund)
{
	if (!buy_refund && !whos->customers.count(id)) {
		return;
	}

	if (!whos->customers.count(id)) {
		struct customer one = { id, 0, 0, };
		whos->customers.insert(std::make_pair(id, one));
	}

	struct customer c = whos->customers[id];
	c.buy = buy_refund ? c.buy + 1 : c.buy - 1;

	if (c.buy <= 0) {
		whos->customers.erase(whos->customers.find(id));
	}

	if (!contains(&whos->candidate, c) && !contains(&whos->winner, c)) {
		if (whos->winner.h_size < whos->limits) {
			c.enter_time = time;
			push(&whos->winner, c);
		} else {
			c.enter_time = time;
			push(&whos->candidate, c);
		}
	} else if (contains(&whos->candidate, c)) {
		if (c.buy <= 0) {
			remove(&whos->candidate, c);
		} else {
			resign(&whos->candidate, c);
		}
	} else {
		if (c.buy <= 0) {
			remove(&whos->winner, c);
		} else {
			resign(&whos->winner, c);
		}
	}

	if (c.buy > 0) {
		whos->customers[id] = c;
	}

	move(whos, time);
}

std::vector < int > daddies(struct whos_daddy *whos)
{
	std::vector < int > res;

	for (int i = 0; i < whos->winner.h_size; i++) {
		res.push_back(whos->winner.v[i].id);
	}

	return res;
}

std::vector < std::vector < int > > topk(std::vector < int > arr, std::vector < bool > op, int k)
{
	struct whos_daddy whos = { 0, };
	memset((void *)&whos, 0, sizeof(whos));

	whos.limits = k;

	std::map < int, struct customer > __c;
	std::vector < T > __v;
	std::map < T, int > __map;
	whos.customers = __c;
	whos.candidate.v = __v;
	whos.candidate.i_map = __map;
	whos.winner.v = __v;
	whos.winner.i_map = __map;

	std::vector < std::vector < int > > res;

	for (uint32_t i = 0; i < arr.size(); i++) {
		operate(&whos, i, arr[i], op[i]);

		std::vector < int > once = daddies(&whos);
		res.push_back(once);
	}

	printf("\n| id |\t| by |\t| et |\n");
    for (auto it : whos.customers) {
		//int id = it.first;
		struct customer c = it.second;

		printf("  %2d\t  %2d\t  %2d\n", c.id, c.buy, c.enter_time);
	}
	printf("\n");

	return res;
}

struct data create_random_data(int max_size, int max_value)
{
	int size = (int)(RANDOM() * max_size) + 1;

	std::vector < int > arr_v;
	std::vector < bool > op_v;

	for (int i = 0; i < size; i++) {
		/* [0, max_value] */
		arr_v.push_back((int)(RANDOM() * max_value) + 1);
		op_v.push_back(RANDOM() < 0.5 ? true : false);
	}

	struct data dat;
	dat.arr = arr_v;
	dat.op = op_v;

	return dat;
}

int main(int argc, char *argv[])
{
	int max_size = 20;
	int max_value = 10;
	int max_k = 6;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct data dat = create_random_data(max_size, max_value);
		int k = (int)(RANDOM() * max_k) + 1;

		printf("k = %d\n", k);
		for (uint32_t i = 0; i < dat.arr.size(); i++) {
			printf("[ %d, %c ], ", dat.arr[i], dat.op[i] ? 'T' : 'F');
		}
		printf("\n");

		std::vector < std::vector < int > > rv = topk(dat.arr, dat.op, k);

		for (uint32_t i = 0; i < rv.size(); i++) {
			printf("[%2d] => ", i);
			for (uint32_t j = 0; j < rv[i].size(); j++) {
				printf(" %2d, ", rv[i][j]);
			}
			printf("\n");
		}

		printf("---8<--------------------------------------------------\n\n");

		if (0) {
			succeed = false;
			break;
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
