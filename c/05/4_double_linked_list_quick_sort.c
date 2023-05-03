
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <list>

/* [0, 1) */
#define RANDOM()    (rand() / (RAND_MAX + 1.0))

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val   = (var);                           \
    __ptr->last  = NULL;                            \
    __ptr->next  = NULL;                            \
    __ptr;                                          \
})

typedef struct list_node {
	int val;

	struct list_node *last;
	struct list_node *next;
} list_node_t;

typedef struct ht {
	bool b_null;
	struct list_node *head;
	struct list_node *tail;
} list_ht_t;

typedef struct info {
	struct list_node *lh;
	struct list_node *lt;
	struct list_node *rh;
	struct list_node *rt;
	struct list_node *eh;
	struct list_node *et;

	int ls;
	int rs;
} rep_info_t;

/*
 * (L....一直到空), 是一个双向链表
 * pivot 是一个不在(L....一直到空)的独立节点, 它作为划分值
 * 根据荷兰国旗问题的划分方式, 把(L....一直到空)划分成:
 * <pivot 、 =pivot 、 >pivot 三个部分, 然后把pivot融进=pivot的部分
 *
 * 比如 4(L)->6->7->1->5->0->9->null pivot=5(这个5和链表中的5, 是不同的节点)
 * 调整完成后:
 * 4->1->0 小于的部分
 * 5->5 等于的部分
 * 6->7->9 大于的部分
 *
 * 三个部分是断开的
 * 然后返回Info:
 * 小于部分的头、尾、节点个数 : lh,lt,ls
 * 大于部分的头、尾、节点个数 : rh,rt,rs
 * 等于部分的头、尾 : eh,et
 */
struct info partition(struct list_node *left, struct list_node *pivot)
{
	struct info rep = { 0, };
	rep.eh = pivot;
	rep.et = pivot;

	struct list_node *next = NULL;

	while (left != NULL) {
		next = left->next;

		left->next = NULL;
		left->last = NULL;

		if (left->val < pivot->val) {
			rep.ls++;

			if (rep.lh == NULL) {
				rep.lh = left;
				rep.lt = left;
			} else {
				rep.lt->next = left;
				left->last = rep.lt;
				rep.lt = left;
			}
		} else if (left->val > pivot->val) {
			rep.rs++;

			if (rep.rh == NULL) {
				rep.rh = left;
				rep.rt = left;
			} else {
				rep.rt->next = left;
				left->last = rep.rt;
				rep.rt = left;
			}
		} else {
			rep.et->next = left;
			left->last = rep.et;
			rep.et = left;

		}

		left = next;
	}

	return rep;
}

struct ht process(struct list_node *left, struct list_node *right, int n)
{
	struct ht ht = { true, NULL, NULL };

	if (left == NULL) {
		return ht;
	}

	if (left == right) {
		ht.b_null = false;
		ht.head = left;
		ht.tail = left;
		return ht;
	}

	/* L..R 上不只一个节点, 随机得到一个随机下标 */
	int rand_idx = (int)(RANDOM() * n);
	struct list_node *rand_node = left;

	while (rand_idx-- != 0) {
		rand_node = rand_node->next;
	}

	/*
	 * 把随机节点从原来的环境里分离出来
	 * 比如 a(L) -> b -> c -> d(R), 如果 random_node = c, 那么调整之后
	 * a(L) -> b -> d(R), c 会被挖出来, random_node = c
	 */
	if (rand_node == left) {
		left = rand_node->next;
		left->last = NULL;
	} else if (rand_node == right) {
		rand_node->last->next = NULL;
	} else {		/* random_node 一定是中间的节点 */
		rand_node->last->next = rand_node->next;
		rand_node->next->last = rand_node->last;
	}

	rand_node->last = NULL;
	rand_node->next = NULL;

	struct info info = partition(left, rand_node);

	/* < random_node 的部分去排序 */
	struct ht l_ht = process(info.lh, info.lt, info.ls);
	/* > random_node 的部分去排序 */
	struct ht r_ht = process(info.rh, info.rt, info.rs);

	/* 左部分排好序、右部分排好序, 把它们串在一起 */
	if (l_ht.b_null != true) {
		l_ht.tail->next = info.eh;
		info.eh->last = l_ht.tail;
	}

	if (r_ht.b_null != true) {
		info.et->next = r_ht.head;
		r_ht.head->last = info.et;
	}

	/* 返回排好序之后总的头和总的尾 */
	struct list_node *h = l_ht.b_null != true ? l_ht.head : info.eh;
	struct list_node *t = r_ht.b_null != true ? r_ht.tail : info.et;

	ht.b_null = false;
	ht.head = h;
	ht.tail = t;

	return ht;
}

struct list_node *qsort_chain_table(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	int n = 0;
	struct list_node *cur = head, *tail = NULL;

	while (cur != NULL) {
		n++;
		tail = cur;
		cur = cur->next;
	}

	return process(head, tail, n).head;
}

bool cmp(struct list_node *l1, struct list_node *l2)
{
	return l1->val < l2->val;
}

struct list_node *n_recursive_sort_chain_table(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	std::list < struct list_node *>list;
	while (head != NULL) {
		list.push_back(head);
		head = head->next;
	}

	list.sort(cmp);

	struct list_node *h = list.front();
	h->last = NULL;
	h->next = NULL;
	list.pop_front();

	struct list_node *p = h;

	for (std::list < struct list_node * >::iterator it = list.begin(); it != list.end(); it++) {
		struct list_node *c = *it;

		p->next = c;
		c->last = p;
		c->next = NULL;

		p = c;
	}

	return h;
}

struct list_node *clone_chain_table(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	struct list_node *h = (struct list_node *)malloc(sizeof(struct list_node));
	h->val = head->val;
	h->last = NULL;
	h->next = NULL;

	struct list_node *p = h;
	head = head->next;

	while (head != NULL) {
		struct list_node *t = (struct list_node *)malloc(sizeof(struct list_node));
		t->val = head->val;
		t->last = NULL;
		t->next = NULL;

		p->next = t;
		t->last = p;
		p = t;

		head = head->next;
	}

	return h;
}

struct list_node *create_random_linked_list(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	if (size == 0) {
		return NULL;
	}

	struct list_node *head = NEW_NODE(struct list_node, (int)(RANDOM() * (max_value + 1)));
	size--;

	struct list_node *prev = head;

	while (size != 0) {
	    struct list_node *t = NEW_NODE(struct list_node, (int)(RANDOM() * (max_value + 1)));

		prev->next = t;
		t->last = prev;

		prev = prev->next;
		size--;
	}

	return head;
}

void print_chain_table(struct list_node *head)
{
	struct list_node *p = head;

	printf("<%p> : \t", p);
	printf("null <- ");
	while (p) {
		if (p->next == NULL)
			printf("%2d -> ", p->val);
		else
			printf("%2d <-> ", p->val);

		p = p->next;
	}
	printf("null\n");
}

void free_chain_table(struct list_node *head)
{
	struct list_node *next = NULL;
	int n = 0;

	printf("<%p> free chain table : ", head);
	while (head) {
		next = head->next;

		head->last = NULL;
		head->next = NULL;
		free(head);

		head = next;
		n++;
	}
	printf("%d\n", n);
}

bool is_equal(struct list_node *l1, struct list_node *l2)
{
	if (l1 == NULL && l2 == NULL) {
		return true;
	}

	if (l1 == NULL || l2 == NULL) {
		return false;
	}

	while (l1 != NULL) {
		if (l1->val != l2->val) {
			return false;
		}

		l1 = l1->next;
		l2 = l2->next;
	}

	if (l1 != NULL || l2 != NULL) {
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 30;
	int times = 10;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		struct list_node *h1 = create_random_linked_list(max_size, max_value);
		struct list_node *h2 = clone_chain_table(h1);
		print_chain_table(h1);

		struct list_node *s1 = qsort_chain_table(h1);
		struct list_node *s2 = n_recursive_sort_chain_table(h2);

		if (!is_equal(s1, s2)) {
			succeed = false;
			break;
		}

		free_chain_table(s1);
		free_chain_table(s2);

		printf("---8<--------------------------------------------------\n\n");
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
