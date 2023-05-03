
#include <stdio.h>
#include <stdlib.h>

struct list_node {
	int val;

	struct list_node *next;
};

struct list_node *create_random_linked_list_v2(int node_size)
{
	struct list_node *head = NULL;
	struct list_node **tail = &(head);

	for (int i = 0; i < node_size; i++) {
		/* fill current list_node. */
		*tail = (struct list_node *) malloc(sizeof(struct list_node));
		if (*tail == NULL) {
			printf("couldn't malloc list_node\n");
			exit(-12);
		}

		(*tail)->val = i;
		(*tail)->next = NULL;

		/* tail points to next addr. */
		tail = &(*tail)->next;
	}

	return head;
}

void remove_if_v2(struct list_node **phead, int num)
{
	printf("remove node->val: '%d'\n", num);

	while (*phead != NULL) {
		struct list_node *entry = *phead;

		if (entry->val == num) {
			*phead = entry->next;

			free(entry);
		} else {
			phead = &(entry)->next;
		}
	}
}

/*
 * TODO: memcheck
 */
struct list_node *remove_entry_given_value(struct list_node *head, int num)
{
	printf("remove node->val: '%d'\n", num);

	while (head != NULL) {
		if (head->val != num)
			break;

		head = head->next;
	}

	struct list_node *prev = head;
	struct list_node *cur = head;

	while (cur != NULL) {
		if (cur->val == num) {
			prev->next = cur->next;
		} else {
			prev = cur;
		}

		cur = cur->next;
	}

	return head;
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

int main(int argc, char *argv[])
{
	struct list_node *head = create_random_linked_list_v2(5);

	print_chain_table(head);

	remove_if_v2(&head, 0);

	print_chain_table(head);

	free_chain_table_v2(&head);
	//free_chain_table(head);

	printf("\n---8<--------------------------------------------------\n\n");

	struct list_node l[] = {
		{ .val = 3, .next = &l[1] },
		{ .val = 1, .next = &l[2] },
		{ .val = 2, .next = &l[3] },
		{ .val = 3, .next = &l[4] },
		{ .val = 8, .next = &l[5] },
		{ .val = 5, .next = &l[6] },
		{ .val = 3, .next = NULL }
	};

	print_chain_table(l);

	struct list_node *p = remove_entry_given_value(l, 3);

	print_chain_table(p);

	return 0;
}
