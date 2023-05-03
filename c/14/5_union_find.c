
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <stack>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val = (var);                             \
    __ptr;                                          \
})

struct node {
	int val;
};

struct union_find {
	std::map < void *, void * > father;
	std::map < void *, int >    size;
};

/*
 * 并查集是用 map 实现的
 * 但是平时用的并查集一律用数组实现, union_find_m.c
 */
void *find_father(struct union_find *u_find, void *cur)
{
	std::stack < void * > s_path;

	while (u_find->father.size() && cur != u_find->father.find(cur)->second) {
		s_path.push(cur);
		cur = u_find->father.find(cur)->second;
	}

	while (!s_path.empty()) {
		void *p = s_path.top();
		s_path.pop();

		u_find->father[p] = cur;
	}

	return cur;
}

bool is_same_set(struct union_find *u_find, void *a, void *b)
{
	if (!u_find->father.count(a) || !u_find->father.count(b)) {
		return false;
	}

	return find_father(u_find, a) == find_father(u_find, b);
}

void union_set(struct union_find *u_find, void *a, void *b)
{
	if (!u_find->father.count(a) || !u_find->father.count(b)) {
		return;
	}

	void *a_father = find_father(u_find, a);
	void *b_father = find_father(u_find, b);

	if (a_father != b_father) {
		int a_size = u_find->size.find(a_father)->second;
		int b_size = u_find->size.find(b_father)->second;

		void *big = a_size > b_size ? a_father : b_father;
		void *small = big == a_father ? b_father : a_father;

		u_find->father[small] = big;
		u_find->size[big] = a_size + b_size;
		u_find->size.erase(u_find->size.find(small));
	}
}

int set_init(struct union_find *u_find, std::vector < void * > nodes)
{
	u_find->father.clear();
	u_find->size.clear();

    for (auto & e : nodes) {
		u_find->father.insert(std::make_pair(e, e));
		u_find->size.insert(std::make_pair(e, 1));
	}

	return 0;
}

std::vector < void * > create_random_node(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	std::vector < void * > vec;

	for (int i = 0; i < size; i++) {
		struct node *p = NEW_NODE(struct node, (int)((max_value + 1) * RANDOM()));
		vec.push_back(p);
	}

	return vec;
}

void fun(void *p)
{
	std::cout.width(4);
	//std::cout << "< " << p << " > " << ((struct node *)p)->val << ",  ";
	printf("%d <%p>,  ", p ? ((struct node *)p)->val : -111, p);
}

int main(int argc, char **argv)
{
	int max_size = 10;
	int max_value = 100;
	int times = 5;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		std::vector < void * > nodes = create_random_node(max_size, max_value);
		for_each(nodes.begin(), nodes.end(), fun);
		std::cout << std::endl << std::endl;
		printf("nodes.size() = %ld\n", nodes.size());

		struct union_find u_find;
		set_init(&u_find, nodes);

		struct node *a = nodes.empty()? NULL : (struct node *)nodes.front();
		struct node *b = nodes.empty()? NULL : (struct node *)nodes.back();

		printf("(%d, %d) is same set? .. %d\n", a ? a->val : -111, b ? b->val : -111,
		       is_same_set(&u_find, a, b));

		printf("\nunion_f (%d, %d)\n", a ? a->val : -111, b ? b->val : -111);
		union_set(&u_find, a, b);
		printf("(%d, %d) is same set? .. %d\n", a ? a->val : -111, b ? b->val : -111,
		       is_same_set(&u_find, a, b));

		printf("find_father(.., a) = %p\n", find_father(&u_find, a));
		printf("find_father(.., b) = %p\n", find_father(&u_find, b));

		/* free */
		for (std::vector < void * >::iterator it = nodes.begin(); it != nodes.end(); it++) {
			free(*it);
		}

		printf("---8<---------------------------------------------------\n\n");
	}

	return 0;
}
