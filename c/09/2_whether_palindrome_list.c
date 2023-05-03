
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>
#include <stack>

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

/* need n extra space */
bool whether_palindrome_list_1(struct list_node *head)
{
	std::stack < struct list_node *>s;
	struct list_node *cur = head;

	while (cur != NULL) {
		s.push(cur);
		cur = cur->next;
	}

	while (head != NULL) {
		if (head->val != s.top()->val) {
			return false;
		}
		s.pop();
		head = head->next;
	}

	return true;
}

/* need n/2 extra space */
bool whether_palindrome_list_2(struct list_node *head)
{
	if (head == NULL || head->next == NULL) {
		return true;
	}

	/* list has >= 2 node.. */
	struct list_node *cur = head;
	struct list_node *right = head->next;

	while (cur->next != NULL && cur->next->next != NULL) {
		right = right->next;
		cur = cur->next->next;
	}

	std::stack < struct list_node *>s;
	while (right != NULL) {
		s.push(right);
		right = right->next;
	}

	while (!s.empty()) {
		if (head->val != s.top()->val) {
			return false;
		}
		s.pop();
		head = head->next;
	}

	return true;
}

/* need O(1) extra space */
bool whether_palindrome_list_3(struct list_node *head)
{
	if (head == NULL || head->next == NULL) {
		return true;
	}

	/* list has >= 2 node.. */
	struct list_node *n1 = head;
	struct list_node *n2 = head;

	while (n2->next != NULL && n2->next->next != NULL) {
		n1 = n1->next;	/* n1 -> mid */
		n2 = n2->next->next;
	}

	n2 = n1->next;		/* n2 -> right part first node */
	n1->next = NULL;	/* mid.next -> null */

	struct list_node *n3 = NULL;
	/* right part covert */
	while (n2 != NULL) {
		n3 = n2->next;
		n2->next = n1;
		n1 = n2;
		n2 = n3;
	}

	n3 = n1;		/* n3 -> right last node */
	n2 = head;		/* n2 -> left first node */

	bool res = true;
	while (n1 != NULL && n2 != NULL) {	/* check palindrome */
		if (n1->val != n2->val) {
			res = false;
			break;
		}
		n1 = n1->next;
		n2 = n2->next;
	}

	/* such as: O -> O -> null */

	n1 = n3->next;
	n3->next = NULL;
	while (n1 != NULL) {
		n2 = n1->next;
		n1->next = n3;
		n3 = n1;
		n1 = n2;
	}

	return res;
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

struct list_node *create_chain_table_from_strs(char s[])
{
	struct list_node *head = NULL;
	struct list_node **cur = &(head);

	for (uint32_t i = 0; i < strlen(s); i++) {
		/* 1. fill current list_node. */
		*cur = NEW_NODE(struct list_node, s[i] - '0');

		/* 2. get the pointer to next addr. */
		cur = &(*cur)->next;
	}

	return head;
}

int main(int argc, char *argv[])
{
	char s[] = "12321";
	printf("strlen('%s') = %ld\n", s, strlen(s));

	struct list_node *head = create_chain_table_from_strs(s);
	print_chain_table(head);

	printf("whether palindrome list: %d | %d | %d\n", whether_palindrome_list_1(head),
	       whether_palindrome_list_2(head), whether_palindrome_list_3(head));

	free_chain_table(head);

	printf("---8<---------------------------------------------------\n\n");

	return 0;
}
