
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <stack>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

struct list_node {
	int val;

	struct list_node *next;
};

struct dlist_node {
	int val;

	struct dlist_node *next, *prev;
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
struct dlist_node *double_list_reverse(struct dlist_node *head)
{
	struct dlist_node *prev = NULL;
	struct dlist_node *next = NULL;

	while (head != NULL) {
		next = head->next;

		head->next = prev;
		head->prev = next;

		prev = head;
		head = next;
	}

	return prev;
}

/*
 * TODO:
 */
struct list_node *create_random_linked_list_v2(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));

	struct list_node *head = NULL;
	struct list_node **tail = &(head);

	for (int i = 0; i < size; i++) {
		*tail = (struct list_node *) malloc(sizeof(struct list_node));
		if (*tail == NULL) {
			printf("can't malloc list_node\n");
			exit(-ENOMEM);
		}

		(*tail)->val = (int)(RANDOM() * (max_value + 1));
		(*tail)->next = NULL;

		tail = &(*tail)->next;
	}

	return head;
}

void free_chain_table_v2(struct list_node **phead)
{
	int n = 0;

	printf("free chain table va %p, nd = ", *phead);
	while (*phead != NULL) {
		struct list_node *entry = *phead;

		*phead = entry->next;
		n++;

		free(entry);
	}
	printf("%d\n", n);

	//*phead = NULL;
}

void print_chain_table(struct list_node *head)
{
	struct list_node *p = head;

	printf("<%p>: ", p);
	while (p) {
		printf("%2d -> ", p->val);
		p = p->next;
	}
	printf("null\n");
}

void print_double_chain_table(struct dlist_node *head)
{
	struct dlist_node *p = head;

	printf("<%p>: ", p);
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

struct list_node *create_random_linked_list(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	if (size == 0) {
		return NULL;
	}

	struct list_node *head = (struct list_node *) malloc(sizeof(*head));
	head->val = (int)(RANDOM() * (max_value + 1));
	head->next = NULL;
	size--;

	struct list_node *prev = head;

	while (size != 0) {
		struct list_node *p = (struct list_node *) malloc(sizeof(*p));
		p->val = (int)(RANDOM() * (max_value + 1));
		p->next = NULL;

		prev->next = p;
		prev = p;
		size--;
	}

	return head;
}

void free_chain_table(struct list_node *head)
{
	struct list_node *next = NULL;
	int n = 0;

	printf("free chain table va %p, nd = ", head);
	while (head) {
		next = head->next;

		head->next = NULL;
		free(head);

		head = next;
		n++;
	}
	printf("%d\n", n);
}

struct dlist_node *create_random_linked_dlist(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));
	if (size == 0) {
		return NULL;
	}

	struct dlist_node *head = (struct dlist_node *) malloc(sizeof(*head));
	head->val = (int)(RANDOM() * (max_value + 1));
	head->prev = NULL;
	head->next = NULL;
	size--;

	struct dlist_node *prev = head;

	while (size != 0) {
		struct dlist_node *p = (struct dlist_node *) malloc(sizeof(*p));
		p->val = (int)(RANDOM() * (max_value + 1));
		p->prev = NULL;
		p->next = NULL;

		prev->next = p;
		p->prev = prev;

		prev = prev->next;
		size--;
	}

	return head;
}

void free_double_chain_table(struct dlist_node *head)
{
	struct dlist_node *next = NULL;
	int n = 0;

	printf("free double chain table va %p, nd = ", head);
	while (head) {
		next = head->next;

		head->prev = NULL;
		head->next = NULL;
		free(head);

		head = next;
		n++;
	}
	printf("%d\n", n);
}

std::vector < struct list_node * > list_reverse_to_vec(struct list_node *head)
{
	std::vector < struct list_node * > v;
	std::stack < struct list_node * > stk;
	struct list_node *p = head;

	while (p != NULL) {
		stk.push(p);
		p = p->next;
	}

	while (!stk.empty()) {
		v.push_back(stk.top());
		stk.pop();
	}

	return v;
}

bool check_list_reverse_from_vec(struct list_node *head, std::vector < struct list_node * > v)
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
		if (v[i] != p)
			return false;

		p = p->next;
	}

	return true;
}

std::vector < struct dlist_node * > double_list_reverse_to_vec(struct dlist_node *head)
{
	std::vector < struct dlist_node * > v;
	std::stack < struct dlist_node * > stk;
	struct dlist_node *p = head;

	while (p != NULL) {
		stk.push(p);
		p = p->next;
	}

	while (!stk.empty()) {
		v.push_back(stk.top());
		stk.pop();
	}

	return v;
}

bool check_double_list_reverse_from_vec(struct dlist_node *head, std::vector < struct dlist_node * > vv)
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
		if (vv[i] != p)
			return false;

		p = p->next;
	}

	return true;
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 30;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct list_node *list = create_random_linked_list_v2(max_size, max_value);
		//struct list_node *list = create_random_linked_list(max_size, max_value);
		print_chain_table(list);

		std::vector < struct list_node * > v = list_reverse_to_vec(list);

		list = list_reverse(list);
		print_chain_table(list);

		if (!check_list_reverse_from_vec(list, v)) {
			printf("oops..1\n");
			succeed = false;
			break;
		}

		free_chain_table_v2(&list);
		//free_chain_table(list);

		printf("\n");

		struct dlist_node *dlist = create_random_linked_dlist(max_size, max_value);
		print_double_chain_table(dlist);

		std::vector < struct dlist_node * > vv = double_list_reverse_to_vec(dlist);

		dlist = double_list_reverse(dlist);
		print_double_chain_table(dlist);

		if (!check_double_list_reverse_from_vec(dlist, vv)) {
			printf("oops..2\n");
			succeed = false;
			break;
		}

		free_double_chain_table(dlist);

		printf("---8<--------------------------------------------------\n\n");
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
