
#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>

/* [0, 1) */
#define RANDOM() (random() / (RAND_MAX + 1.0))

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
	 * list has >= 3 node.
	 *
	 * h: 1 -> 2 -> 3 -> 4 -> 5 -> null
	 *         s    f
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

	/*
	 * list has >= 2 node.
	 *
	 * h: 1 -> 2 -> 3 -> 4 -> null
	 *        s/f
	 */
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

	/*
	 * list has >= 3 node.
	 *
	 * h: 1 -> 2 -> 3 -> 4 -> 5 -> null
	 *    s         f
	 */
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

	/* list has 2 node. */
	if (head->next->next == NULL) {
		return head;
	}

	/*
	 * list has >= 3 node.
	 *
	 * h: 1 -> 2 -> 3 -> 4 -> null
	 *    s    f
	 */
	struct list_node *slow = head;
	struct list_node *fast = head->next;

	while (fast->next != NULL && fast->next->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	return slow;
}

struct list_node *create_random_linked_list_v2(int max_size, int max_value)
{
	int size = (int)(RANDOM() * (max_size + 1));

	struct list_node *head = NULL;
	struct list_node **tail = &(head);

	for (int i = 0; i < size; i++) {
		*tail = (struct list_node *) malloc(sizeof(struct list_node));
		if (*tail == NULL) {
			printf("can't malloc list_node\n");
			exit(-12);
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

struct list_node *vio_up_mid(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	struct list_node *cur = head;
	std::vector < struct list_node * > v;

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
	std::vector < struct list_node * > v;

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
	std::vector < struct list_node * > v;

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
	std::vector < struct list_node * > v;

	while (cur != NULL) {
		v.push_back(cur);
		cur = cur->next;
	}

	return v.at((v.size() - 2) / 2);
}

int main(int argc, char *argv[])
{
	int max_size = 15;
	int max_value = 20;
	int times = 1000;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct list_node *l1 = NULL;
		struct list_node *l2 = NULL;

		struct list_node *head = create_random_linked_list_v2(max_size, max_value);
		print_chain_table(head);

		l1 = mid_or_up_mid_node(head);
		l2 = vio_up_mid(head);
		printf("\nm = %d, ", l1 ? l1->val : -61);
		if (l1 != l2) {
			succeed = false;
			break;
		}

		l1 = mid_or_down_mid_node(head);
		l2 = vio_down_mid(head);
		printf("dm = %d, ", l1 ? l1->val : -61);
		if (l1 != l2) {
			succeed = false;
			break;
		}

		l1 = mid_or_up_mid_prev_node(head);
		l2 = vio_up_mid_prev(head);
		printf("m prev = %d, ", l1 ? l1->val : -61);
		if (l1 != l2) {
			succeed = false;
			break;
		}

		l1 = mid_or_down_mid_prev_node(head);
		l2 = vio_down_mid_prev(head);
		printf("dm prev = %d\n\n", l1 ? l1->val : -61);
		if (l1 != l2) {
			succeed = false;
			break;
		}

		free_chain_table_v2(&head);

		printf("\n---8<--------------------------------------------------\n\n");
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
