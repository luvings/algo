
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <stack>
#include <queue>

/* [0, 1) */
#define RANDOM() (random() / (RAND_MAX + 1.0))

struct list_node {
	int val;

	struct list_node *next, *prev;
};

struct q_list {
	int count;

	struct list_node *head, *tail;
};

struct stack {
	struct q_list q;

	void (*push)(struct q_list * q, int v);
	int (*pop)(struct q_list * q);
	bool (*empty)(struct q_list * q);
};

struct queue {
	struct q_list q;

	void (*push)(struct q_list * q, int v);
	int (*pop)(struct q_list * q);
	bool (*empty)(struct q_list * q);
};

void push_from_head(struct q_list *q, int val)
{
	struct list_node *cur = (struct list_node *) malloc(sizeof(*cur));
	if (cur == NULL) {
		printf("couldn't malloc list_node\n");
		return;
	}
	cur->val = val;
	cur->next = cur->prev = NULL;

	if (q->head == NULL) {
		q->head = cur;
		q->tail = cur;
	} else {
		cur->next = q->head;
		q->head->prev = cur;

		q->head = cur;
	}
}

void push_from_bottom(struct q_list *q, int val)
{
	struct list_node *cur = (struct list_node *) malloc(sizeof(*cur));
	if (cur == NULL) {
		printf("couldn't malloc list_node\n");
		return;
	}
	cur->val = val;
	cur->next = cur->prev = NULL;

	if (q->head == NULL) {
		q->head = cur;
		q->tail = cur;
	} else {
		cur->prev = q->tail;
		q->tail->next = cur;

		q->tail = cur;
	}
}

int pop_from_head(struct q_list *q)
{
	int rval = INT_MAX;

	if (q->head == NULL) {
		printf("pop errors\n");
		return rval;
	}

	rval = q->head->val;

	if (q->head == q->tail) {
		free(q->head);
		q->head = q->tail = NULL;
	} else {
		struct list_node *cur = q->head;

		q->head = q->head->next;
		q->head->prev = NULL;

		cur->next = cur->prev = NULL;
		free(cur);
	}

	return rval;
}

int pop_from_bottom(struct q_list *q)
{
	int rval = INT_MAX;

	if (q->tail == NULL) {
		printf("pop errors\n");
		return rval;
	}

	rval = q->tail->val;

	if (q->head == q->tail) {
		free(q->head);
		q->head = q->tail = NULL;
	} else {
		struct list_node *cur = q->tail;

		q->tail = q->tail->prev;
		q->tail->next = NULL;

		cur->next = cur->prev = NULL;
		free(cur);
	}

	return rval;
}

bool empty(struct q_list *q)
{
	return q->head == NULL;
}

int main(int argc, char *argv[])
{
	int max_value = 50;
	int times = 100;
	int once = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {

		struct stack stk = {
			.q = { 0, NULL, NULL },
			.push = push_from_bottom,
			.pop = pop_from_bottom,
			.empty = empty,
		};
		std::stack < int > sys_stk;

		struct queue q = {
			.q = { 0, NULL, NULL },
			.push = push_from_bottom,
			.pop = pop_from_head,
			.empty = empty,
		};
		std::queue < int > sys_que;

		for (int j = 0; j < once; j++) {

			int nums = (int)(RANDOM() * (max_value + 1));
			if (stk.empty(&stk.q)) {
				stk.push(&stk.q, nums);
				sys_stk.push(nums);
			} else {
				if (RANDOM() < 0.4) {
					stk.push(&stk.q, nums);
					sys_stk.push(nums);
				} else {
					/*
					 * top(): 如果堆栈容器为空, 则会导致未定义的行为
					 */
					if (stk.pop(&stk.q) != sys_stk.top()) {
						succeed = false;
						printf("oops..1\n");
						break;
					}
					sys_stk.pop();
				}
			}

			int numq = (int)(RANDOM() * (max_value + 1));
			if (q.empty(&q.q)) {
				q.push(&q.q, numq);
				sys_que.push(numq);
			} else {
				if (RANDOM() < 0.5) {
					q.push(&q.q, numq);
					sys_que.push(numq);
				} else {
					/*
					 * front(), back(): 如果队列容器为空, 则会导致未定义的行为
					 */
					if (q.pop(&q.q) != sys_que.front()) {
						succeed = false;
						printf("oops..2\n");
						break;
					}
					sys_que.pop();
				}
			}
		}

		while (!stk.empty(&stk.q)) {
			stk.pop(&stk.q);
		}

		while (!q.empty(&q.q)) {
			q.pop(&q.q);
		}
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
