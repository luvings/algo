
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val   = (var);                           \
    __ptr->next  = NULL;                            \
    vb.push_back(__ptr);                            \
    __ptr;                                          \
})

struct list_node {
	int val;

	struct list_node *next;
};

/* 找到链表第一个入环节点, 如果无环, 返回 NULL */
struct list_node *get_loop_node(struct list_node *head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL) {
		return NULL;
	}

	struct list_node *slow = head->next;
	struct list_node *fast = head->next->next;

	while (slow != fast) {
		if (fast->next == NULL || fast->next->next == NULL) {
			return NULL;
		}

		slow = slow->next;
		fast = fast->next->next;
	}

	/* slow fast 相遇 */
	/* n2 -> walk again from head */
	fast = head;

	while (slow != fast) {
		slow = slow->next;
		fast = fast->next;
	}

	return slow;
}

/*
 * 如果两个链表都无环, 返回第一个相交节点, 如果不想交, 返回 null
 *    \
 *     \   /
 *      \ /
 *       |
 *       |
 */
struct list_node *no_loop(struct list_node *head1, struct list_node *head2)
{
	if (head1 == NULL || head2 == NULL) {
		return NULL;
	}

	struct list_node *cur1 = head1;
	struct list_node *cur2 = head2;
	int n = 0;

	while (cur1 != NULL) {
		cur1 = cur1->next;
		n++;
	}

	while (cur2 != NULL) {
		cur2 = cur2->next;
		n--;
	}

	if (cur1 != cur2) {
		return NULL;
	}

	/* n : 链表1 长度减去链表2 长度的值 */
	cur1 = n > 0 ? head1 : head2;
	cur2 = cur1 == head1 ? head2 : head1;
	n = abs(n);

	while (n != 0) {
		cur1 = cur1->next;
		n--;
	}

	while (cur1 != cur2) {
		cur1 = cur1->next;
		cur2 = cur2->next;
	}

	return cur1;
}

/* 两个有环链表, 返回第一个相交节点, 如果不想交返回 null */
struct list_node *both_loop(struct list_node *head1, struct list_node *loop1,
			    struct list_node *head2, struct list_node *loop2)
{
	struct list_node *cur1 = NULL;
	struct list_node *cur2 = NULL;

	if (loop1 == loop2) {
		/*
		 *      \ /
		 *       |
		 *       O
		 */
		cur1 = head1;
		cur2 = head2;
		int n = 0;

		while (cur1 != loop1) {
			cur1 = cur1->next;
			n++;
		}

		while (cur2 != loop2) {
			cur2 = cur2->next;
			n--;
		}

		cur1 = n > 0 ? head1 : head2;
		cur2 = cur1 == head1 ? head2 : head1;
		n = abs(n);

		while (n != 0) {
			cur1 = cur1->next;
			n--;
		}

		while (cur1 != cur2) {
			cur1 = cur1->next;
			cur2 = cur2->next;
		}

		return cur1;
	} else {
		cur1 = loop1->next;
		while (cur1 != loop1) {
			if (cur1 == loop2) {
				return loop1;
			}

			cur1 = cur1->next;
		}

		return NULL;
	}
}

struct list_node *get_intersect_node(struct list_node *head1, struct list_node *head2)
{
	if (head1 == NULL || head2 == NULL) {
		return NULL;
	}

	struct list_node *loop1 = get_loop_node(head1);
	struct list_node *loop2 = get_loop_node(head2);

	if (loop1 == NULL && loop2 == NULL) {
		return no_loop(head1, head2);
	}

	if (loop1 != NULL && loop2 != NULL) {
		return both_loop(head1, loop1, head2, loop2);
	}

	/* one has no loop, other has loop, then no intersect. */
	return NULL;
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

void free_chain_table(std::vector < struct list_node * > &v)
{
	for (std::vector < struct list_node * >::iterator it = v.begin(); it != v.end(); it++) {
		free(*it);
	}
}

struct list_node *create_chain_table_from_strs(char s[], std::vector < struct list_node * > &vb)
{
	struct list_node *head = NULL;
	struct list_node *prev = NULL;

	for (uint32_t i = 0; i < strlen(s); i++) {
		struct list_node *p = NEW_NODE(struct list_node, s[i] - '0');

		if (i == 0) {
			head = p;
			prev = head;
		} else {
			prev->next = p;
			prev = p;
		}
	}

	return head;
}

int main(int argc, char *argv[])
{
	std::vector < struct list_node * > vb;

	char s1[] = "1234567";
	char s2[] = "098";
	printf("strlen('%s') = %ld\n\n", s1, strlen(s1));

	struct list_node *head1 = create_chain_table_from_strs(s1, vb);
	//print_chain_table(head1);

	struct list_node *head2 = create_chain_table_from_strs(s2, vb);
	head2->next->next->next = head1->next->next->next->next->next;

	printf("h1: 1->2->3->4->5->6->7->null\n");
	printf("h2: 0->9->8->6->7->null\n");

	struct list_node *res = get_intersect_node(head1, head2);
	printf("intersect node->val: %d\n", res != NULL ? res->val : -111);

	printf("\n---8<---------------------------------------------------\n\n");

	struct list_node *head3 = create_chain_table_from_strs(s1, vb);
	head3->next->next->next->next->next->next = head3->next->next->next;

	struct list_node *head4 = create_chain_table_from_strs(s2, vb);
	head4->next->next->next = head3->next;

	printf("h3: 1->2->3->4->5->6->7->4..\n");
	printf("h4: 0->9->8->2->3..\n");

	struct list_node *res2 = get_intersect_node(head3, head4);
	printf("intersect node->val: %d\n", res2 != NULL ? res2->val : -111);

	printf("\n---8<---------------------------------------------------\n\n");

	struct list_node *head5 = create_chain_table_from_strs(s2, vb);
	head5->next->next->next = head3->next->next->next->next->next;

	printf("h3: 1->2->3->4->5->6->7->4..\n");
	printf("h5: 0->9->8->6->4->5->6..\n");

	struct list_node *res3 = get_intersect_node(head3, head5);
	printf("intersect node->val: %d\n", res3 != NULL ? res3->val : -111);

	printf("\n---8<---------------------------------------------------\n\n");

	free_chain_table(vb);

	return 0;
}
