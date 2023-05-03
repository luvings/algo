
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <stack>

/* [0, 1) */
#define RANDOM()    (rand() / (RAND_MAX + 1.0))

struct list_node {
	int val;

	struct list_node *next;
};

struct dlist_node {
	int val;

	struct dlist_node *last;
	struct dlist_node *next;
};

/*
 *       1 -> 2 -> 3 -> null
 *  rvs: 3 -> 2 -> 1 -> null
 */
struct list_node *list_reverse(struct list_node *head)
{
	struct list_node *prev = NULL;
	struct list_node *next = NULL;

	while (head != NULL) {
		next = head->next;
		head->next = prev;
		prev = head;
		head = next;
	}

	return prev;
}

/*
 *       null <- 1 <-> 2 <-> 3 -> null
 *  rvs: null <- 3 <-> 2 <-> 1 -> null
 */
struct dlist_node *dlist_reverse(struct dlist_node *head)
{
	struct dlist_node *prev = NULL;
	struct dlist_node *next = NULL;

	while (head != NULL) {
		next = head->next;

		head->next = prev;
		head->last = next;

		prev = head;
		head = next;
	}

	return prev;
}

struct list_node *create_random_linked_list(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	if (size == 0) {
		return NULL;
	}

	struct list_node *head = (struct list_node *) malloc(sizeof(struct list_node));
	head->val = (int)(RANDOM() * (max_value + 1));
	head->next = NULL;
	size--;

	struct list_node *prev = head;

	while (size != 0) {
		struct list_node *t = (struct list_node *) malloc(sizeof(struct list_node));
		t->val = (int)(RANDOM() * (max_value + 1));
		t->next = NULL;

		prev->next = t;
		prev = t;
		size--;
	}

	return head;
}

struct dlist_node *create_random_linked_dlist(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	if (size == 0) {
		return NULL;
	}

	struct dlist_node *head = (struct dlist_node *) malloc(sizeof(struct dlist_node));
	head->val = (int)(RANDOM() * (max_value + 1));
	head->last = NULL;
	head->next = NULL;
	size--;

	struct dlist_node *prev = head;

	while (size != 0) {
		struct dlist_node *t = (struct dlist_node *) malloc(sizeof(struct dlist_node));
		t->val = (int)(RANDOM() * (max_value + 1));
		t->last = NULL;
		t->next = NULL;

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
	while (p) {
		printf("%2d -> ", p->val);
		p = p->next;
	}
	printf("null\n");
}

void print_d_chain_table(struct dlist_node *head)
{
	struct dlist_node *p = head;

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

		head->next = NULL;
		free(head);

		head = next;
		n++;
	}
	printf("%d\n", n);
}

void free_d_chain_table(struct dlist_node *head)
{
	struct dlist_node *next = NULL;
	int n = 0;

	printf("<%p> free d-chain table : ", head);
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

std::vector < int > list_origin_order_to_vec(struct list_node *head)
{
	std::vector < int >v;
	std::stack < int >stk;
	struct list_node *p = head;

	while (p != NULL) {
		stk.push(p->val);
		p = p->next;
	}

	while (!stk.empty()) {
		v.push_back(stk.top());
		stk.pop();
	}

	return v;
}

bool check_list_reverse_from_vec(struct list_node *head, std::vector < int >v)
{
	struct list_node *p = head;
	unsigned long n = 0;

	while (p != NULL) {
		p = p->next;
		n++;
	}
	if (v.size() != n) {
		printf("mismatch: v.size() = %ld, n = %ld\n", v.size(), n);
		return false;
	}

	p = head;

	for (unsigned long i = 0; i < v.size(); i++) {
		if (v[i] != p->val)
			return false;

		p = p->next;
	}

	return true;
}

std::vector < int > dlist_origin_order_to_vec(struct dlist_node *head)
{
	std::vector < int >v;
	std::stack < int >stk;
	struct dlist_node *p = head;

	while (p != NULL) {
		stk.push(p->val);
		p = p->next;
	}

	while (!stk.empty()) {
		v.push_back(stk.top());
		stk.pop();
	}

	return v;
}

bool check_dlist_reverse_from_vec(struct dlist_node *head, std::vector < int >vv)
{
	struct dlist_node *p = head;
	unsigned long n = 0;

	while (p != NULL) {
		p = p->next;
		n++;
	}
	if (vv.size() != n) {
		printf("mismatch: vv.size() = %ld, n = %ld\n", vv.size(), n);
		return false;
	}

	p = head;

	for (unsigned long i = 0; i < vv.size(); i++) {
		if (vv[i] != p->val)
			return false;

		p = p->next;
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
		struct list_node *list = create_random_linked_list(max_size, max_value);
		print_chain_table(list);

		std::vector < int > v = list_origin_order_to_vec(list);

		list = list_reverse(list);
		print_chain_table(list);

		if (!check_list_reverse_from_vec(list, v)) {
			printf("oops..1\n");
			succeed = false;
			break;
		}

		free_chain_table(list);

		printf("\n");

		struct dlist_node *dlist = create_random_linked_dlist(max_size, max_value);
		print_d_chain_table(dlist);

		std::vector < int > vv = dlist_origin_order_to_vec(dlist);

		dlist = dlist_reverse(dlist);
		print_d_chain_table(dlist);

		if (!check_dlist_reverse_from_vec(dlist, vv)) {
			printf("oops..2\n");
			succeed = false;
			break;
		}

		free_d_chain_table(dlist);

		printf("---8<--------------------------------------------------\n\n");
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
