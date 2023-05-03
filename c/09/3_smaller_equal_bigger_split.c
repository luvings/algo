
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct list_node {
	int val;

	struct list_node *next;
};

struct list_node *chain_table_split(struct list_node *head, int pivot)
{
	/*
	 * pointer
	 *
	 * small head, small tail
	 * equal head, equal tail
	 * big head,   big tail
	 */
	struct list_node *sh = NULL, *st = NULL;
	struct list_node *eh = NULL, *et = NULL;
	struct list_node *bh = NULL, *bt = NULL;

	struct list_node *next = NULL;
	while (head != NULL) {
		next = head->next;
		head->next = NULL;

		if (head->val < pivot) {
			if (sh == NULL) {
				sh = st = head;
			} else {
				st->next = head;
				st = head;
			}
		} else if (head->val == pivot) {
			if (eh == NULL) {
				eh = et = head;
			} else {
				et->next = head;
				et = head;
			}
		} else {	/* head->val > pivot */
			if (bh == NULL) {
				bh = bt = head;
			} else {
				bt->next = head;
				bt = head;
			}
		}

		head = next;
	}

	/* 小于区域的尾巴, 连等于区域的头, 等于区域的尾巴, 连大于区域的头 */

	if (st != NULL) {
		st->next = eh;
		/* 下一步, 谁去连大于区域的头, 谁就变成 et */
		et = et == NULL ? st : et;
	}

	/*
	 * 下一步, 一定是需要用 et 去接 大于区域的头
	 * 有等于区域, et -> 等于区域的尾结点
	 * 无等于区域, et -> 小于区域的尾结点
	 */
	if (et != NULL) {
		et->next = bh;
	}

	return sh != NULL ? sh : (eh != NULL ? eh : bh);
}

void swap(struct list_node **data, int i, int j)
{
	struct list_node *node = data[i];
	data[i] = data[j];
	data[j] = node;
}

void partition(struct list_node *array[], int n, int pivot)
{
	int small = -1, big = n;
	int index = 0;

	while (index != big) {
		if (array[index]->val < pivot) {
			swap(array, index++, ++small);
		} else if (array[index]->val == pivot) {
			index++;
		} else {	/* array[index]->val > pivot */
			swap(array, index, --big);
		}
	}
}

struct list_node *chain_table_partition_cmp(struct list_node *head, int pivot)
{
	if (head == NULL) {
		return NULL;
	}

	struct list_node *cur = head;
	int n = 0, i = 0;

	while (cur != NULL) {
		n++;
		cur = cur->next;
	}

	struct list_node *array[n];
	memset(array, 0, sizeof(array[0]) * n);

	cur = head;
	for (i = 0; i < n; i++) {
		array[i] = cur;
		cur = cur->next;
	}

	partition(array, n, pivot);

	for (i = 1; i < n; i++) {
		array[i - 1]->next = array[i];
	}
	array[i - 1]->next = NULL;

	return array[0];
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

struct list_node *create_chain_table_from_strs(char s[])
{
	struct list_node *head = NULL;
	struct list_node **tail = &(head);

	for (uint32_t i = 0; i < strlen(s); i++) {
		*tail = (struct list_node *) malloc(sizeof(struct list_node));
		if (*tail == NULL) {
			printf("can't malloc list_node\n");
			exit(-12);
		}

		(*tail)->val = s[i] - '0';
		(*tail)->next = NULL;

		tail = &(*tail)->next;
	}

	return head;
}

int main(int argc, char *argv[])
{
	char s[] = "7918525";
	printf("strlen('%s') = %ld\n", s, strlen(s));

	struct list_node *head = create_chain_table_from_strs(s);
	print_chain_table(head);

	int pivot = 5;
	printf("\nsplit by: '%d'\n\n", pivot);

	//head = chain_table_partition_cmp(head, pivot);
	head = chain_table_split(head, pivot);
	print_chain_table(head);

	free_chain_table_v2(&head);

	return 0;
}

