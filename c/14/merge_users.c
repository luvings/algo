
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define ARRAY_MAX   256

struct union_find {
	int data_size;
	int father[ARRAY_MAX];
	int size[ARRAY_MAX];
	int help[ARRAY_MAX];
};

struct user {
	std::string a;
	std::string b;
	std::string c;
};

int find_father(struct union_find *u_find, int x)
{
	int h_idx = 0;
	while (x != u_find->father[x]) {
		u_find->help[h_idx++] = x;
		x = u_find->father[x];
	}

	for (h_idx--; h_idx >= 0; h_idx--) {
		u_find->father[u_find->help[h_idx]] = x;
	}

	return x;
}

bool is_same_set(struct union_find *u_find, int x, int y)
{
	if (x >= u_find->data_size || y >= u_find->data_size) {
		return false;
	}

	return find_father(u_find, x) == find_father(u_find, y);
}

void union_f(struct union_find *u_find, int x, int y)
{
	if (x >= u_find->data_size || y >= u_find->data_size) {
		return;
	}

	int x_f = find_father(u_find, x);
	int y_f = find_father(u_find, y);

	if (x_f != y_f) {
		if (u_find->size[x_f] >= u_find->size[y_f]) {
			u_find->size[x_f] += u_find->size[y_f];
			u_find->father[y_f] = x_f;
			u_find->size[y_f] = -1;
		} else {
			u_find->size[y_f] += u_find->size[x_f];
			u_find->father[x_f] = y_f;
			u_find->size[x_f] = -1;
		}
	}
}

int get_sets_num(struct union_find *u_find)
{
	int num = 0;

	for (int i = 0; i < u_find->data_size; i++) {
		if (u_find->size[i] != -1) {
			num++;
		}
	}

	return num;
}

int union_init(struct union_find *u_find, int n)
{
	u_find->data_size = n;

	for (int i = 0; i < n; i++) {
		u_find->father[i] = i;
		u_find->size[i] = 1;
	}

	return 0;
}

/*
 * 如果两个 user, a 字段一样, 或者 b 字段一样, 或者 c 字段一样, 就认为是同一个人,
 * 请合并 users, 并返回合并后用户数量
 */
int merge_users(std::vector < struct user > users)
{
	struct union_find u_find = {
		.data_size = 0,
		.father = { -1, },
		.size = { -1, },
		.help = { -1, },
	};
	union_init(&u_find, users.size());

	std::map < std::string, int > a_map;
	std::map < std::string, int > b_map;
	std::map < std::string, int > c_map;

	for (uint32_t i = 0; i < users.size(); i++) {
		if (a_map.find(users[i].a) != a_map.end()) {
			union_f(&u_find, i, a_map.find(users[i].a)->second);
		} else {
			a_map.insert(std::make_pair(users[i].a, i));
		}

		if (b_map.find(users[i].b) != b_map.end()) {
			union_f(&u_find, i, b_map.find(users[i].b)->second);
		} else {
			b_map.insert(std::make_pair(users[i].b, i));
		}

		if (c_map.find(users[i].c) != c_map.end()) {
			union_f(&u_find, i, c_map.find(users[i].c)->second);
		} else {
			c_map.insert(std::make_pair(users[i].c, i));
		}
	}

#if 1
	printf("father:\t");
	for (int k = 0; k < u_find.data_size; k++) {
		printf("(%2d, %2d), ", k, u_find.father[k]);
	}
	printf("\nsize:\t");
	for (int k = 0; k < u_find.data_size; k++) {
		printf("(%2d, %2d), ", k, u_find.size[k]);
	}
	printf("\n");
#endif

	/* 并查集询问, 合并后还有几个集合? */
	return get_sets_num(&u_find);
}

int main(int argc, char **argv)
{
	struct user u1 = { "1", "10", "13", };
	struct user u2 = { "2", "10", "37", };
	struct user u3 = { "3", "44", "37", };
	struct user u4 = { "4", "11", "12", };
	struct user u5 = { "4", "55", "77", };

	std::vector < struct user > users;
	users.push_back(u1);
	users.push_back(u2);
	users.push_back(u3);
	users.push_back(u4);
	users.push_back(u5);

	int count = merge_users(users);
	printf("users count: %d\n", count);

	return 0;
}
