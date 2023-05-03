
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <list>
#include <vector>
#include <string>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->happy = (var);                           \
    v.push_back(__ptr);                             \
    __ptr;                                          \
})

struct employee {
	int happy;

	std::vector < struct employee * > nexts;
};

struct info {
	int no, yes;
};

static std::vector < struct employee * > v;

struct info process(struct employee *x)
{
	struct info rep = { 0, 0, };
	if (x == NULL) {
		return rep;
	}

	int no = 0;
	int yes = x->happy;

	for (std::vector < struct employee * >::iterator it = x->nexts.begin(); it != x->nexts.end(); it++) {
		struct info n_rep = process(*it);
		no += std::max(n_rep.no, n_rep.yes);
		yes += n_rep.no;
	}

	rep.no = no;
	rep.yes = yes;

	return rep;
}

int max_happy_un(struct employee *boss)
{
	struct info rep = process(boss);

	return std::max(rep.no, rep.yes);
}

/*
 * 当前来到的节点叫cur,
 * up表示cur的上级是否来,
 * 该函数含义:
 * 如果up为true, 表示在cur上级已经确定来, 的情况下, cur整棵树能够提供最大的快乐值是多少?
 * 如果up为false, 表示在cur上级已经确定不来, 的情况下, cur整棵树能够提供最大的快乐值是多少?
 */
int process_cmp(struct employee *cur, bool up)
{
	if (up) {
		int happy = 0;
		for (std::vector < struct employee * >::iterator it = cur->nexts.begin(); it != cur->nexts.end(); it++) {
			happy += process_cmp(*it, false);
		}
		return happy;
	} else {
		int happy_1 = cur->happy;
		int happy_2 = 0;
		for (std::vector < struct employee * >::iterator it = cur->nexts.begin(); it != cur->nexts.end(); it++) {
			happy_1 += process_cmp(*it, true);
			happy_2 += process_cmp(*it, false);
		}
		return std::max(happy_1, happy_2);
	}
}

int max_happy_cmp(struct employee *boss)
{
	if (boss == NULL) {
		return 0;
	}

	return process_cmp(boss, false);
}

void alloc_subordinates_node(struct employee *e, int level, int max_level, int max_happy, int max_nexts)
{
	if (level > max_level) {
		return;
	}

	int nexts = (int)(RANDOM() * (max_nexts + 1));

	for (int i = 0; i < nexts; i++) {
		struct employee *next = NEW_NODE(struct employee, (int)(RANDOM() * (max_happy + 1)));
		e->nexts.push_back(next);

		alloc_subordinates_node(next, level + 1, max_level, max_happy, max_nexts);
	}
}

struct employee *create_group_multi_tree(int max_level, int max_happy, int max_nexts)
{
	if (RANDOM() < 0.02) {
		return NULL;
	}

	struct employee *boss = NEW_NODE(struct employee, (int)(RANDOM() * (max_happy + 1)));

	alloc_subordinates_node(boss, 1, max_level, max_happy, max_nexts);

	return boss;
}

void delete_mtree_node(std::vector < struct employee * > &v)
{
	for (std::vector < struct employee * >::iterator it = v.begin(); it != v.end(); it++) {
		free(*it);
	}
}

int main(int argc, char **argv)
{
	int max_level = 4;
	int max_happy = 100;
	int max_nexts = 7;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct employee *boss = create_group_multi_tree(max_level, max_happy, max_nexts);

		if (max_happy_un(boss) != max_happy_cmp(boss)) {
			printf("oops\n");
			succeed = false;
			break;
		}
	}

	delete_mtree_node(v);

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
