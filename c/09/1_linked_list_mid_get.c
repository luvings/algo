
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>

/* [0, 1) */
#define RANDOM() (rand() / (RAND_MAX + 1.0))

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val   = (var);                           \
    __ptr->next  = NULL;                            \
    __ptr;                                          \
})

struct list_node {
	int val;

	struct list_node *next;
};

struct list_node *mid_or_up_mid_node(struct list_node *head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL) {
		return head;
	}

	/*
	 * list has >= 3 node..
	 *
	 * 1 -> 2 -> 3 -> 4 -> 5 -> null
	 *      s    f
	 *
	 */
	struct list_node *slow = head->next;
	struct list_node *fast = head->next->next;

	while (fast->next != NULL && fast->next->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

struct list_node *mid_or_down_mid_node(struct list_node *head)
{
	if (head == NULL || head->next == NULL) {
		return head;
	}

	/* list hast >= 2 node. */
	struct list_node *slow = head->next;
	struct list_node *fast = head->next;

	while (fast->next != NULL && fast->next->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

struct list_node *mid_or_up_mid_prev_node(struct list_node *head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL) {
		return NULL;
	}

	/* list hast >= 3 node. */
	struct list_node *slow = head;
	struct list_node *fast = head->next->next;

	while (fast->next != NULL && fast->next->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

struct list_node *mid_or_down_mid_prev_node(struct list_node *head)
{
	if (head == NULL || head->next == NULL) {
		return NULL;
	}

	/* list hast == 2 node. */
	if (head->next->next == NULL) {
		return head;
	}

	/* list hast >= 3 node. */
	struct list_node *slow = head;
	struct list_node *fast = head->next;

	while (fast->next != NULL && fast->next->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

struct list_node *create_random_linked_list(int max_size, int max_value)
{
	struct list_node *head = NULL;
	struct list_node **cur = &(head);

	for (int i = 0; i < (int)(RANDOM() * (max_size + 1)); i++) {
		/* 1. fill current list_node. */
		*cur = NEW_NODE(struct list_node, (int)(RANDOM() * (max_value + 1)));

		/* 2. get the pointer to next addr. */
		cur = &(*cur)->next;
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

struct list_node *vio_up_mid(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	struct list_node *cur = head;
	std::vector < struct list_node *>v;

	while (cur != NULL) {
		v.push_back(cur);
		cur = cur->next;
	}

	return v.at((v.size() - 1) / 2);
}

struct list_node *vio_down_mid(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	struct list_node *cur = head;
	std::vector < struct list_node *>v;

	while (cur != NULL) {
		v.push_back(cur);
		cur = cur->next;
	}

	return v.at(v.size() / 2);
}

struct list_node *vio_up_mid_prev(struct list_node *head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL) {
		return NULL;
	}

	struct list_node *cur = head;
	std::vector < struct list_node *>v;

	while (cur != NULL) {
		v.push_back(cur);
		cur = cur->next;
	}

	return v.at((v.size() - 3) / 2);
}

struct list_node *vio_down_mid_prev(struct list_node *head)
{
	if (head == NULL || head->next == NULL) {
		return NULL;
	}

	struct list_node *cur = head;
	std::vector < struct list_node *>v;

	while (cur != NULL) {
		v.push_back(cur);
		cur = cur->next;
	}

	return v.at((v.size() - 2) / 2);
}

int main(int argc, char *argv[])
{
	int max_size = 20;
	int max_value = 100;
	int times = 10000;
	bool succeed = true;

	srand((int)time(0));

	for (int i = 0; i < times; i++) {
		struct list_node *l1 = NULL;
		struct list_node *l2 = NULL;

		struct list_node *head = create_random_linked_list(max_size, max_value);
		print_chain_table(head);

		l1 = mid_or_up_mid_node(head);
		l2 = vio_up_mid(head);
		if (l1 != l2) {
			printf("oops..1\n");
			printf("l1->val = %d, l2->val = %d.\n", l1 ? l1->val : -1111, l2 ? l2->val : -1111);
			succeed = false;
			break;
		}

		l1 = mid_or_down_mid_node(head);
		l2 = vio_down_mid(head);
		if (l1 != l2) {
			printf("oops..2\n");
			printf("l1->val = %d, l2->val = %d.\n", l1 ? l1->val : -1111, l2 ? l2->val : -1111);
			succeed = false;
			break;
		}

		l1 = mid_or_up_mid_prev_node(head);
		l2 = vio_up_mid_prev(head);
		if (l1 != l2) {
			printf("oops..3\n");
			printf("l1->val = %d, l2->val = %d.\n", l1 ? l1->val : -1111, l2 ? l2->val : -1111);
			succeed = false;
			break;
		}

		l1 = mid_or_down_mid_prev_node(head);
		l2 = vio_down_mid_prev(head);
		if (l1 != l2) {
			printf("oops..4\n");
			printf("l1->val = %d, l2->val = %d.\n", l1 ? l1->val : -1111, l2 ? l2->val : -1111);
			succeed = false;
			break;
		}

		free_chain_table(head);

		printf("\n---8<--------------------------------------------------\n\n");
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
