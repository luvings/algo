
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>
#include <map>

/* [0, 1) */
#define RANDOM() (random() / (RAND_MAX + 1.0))

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val   = (var);                           \
    __ptr->next   = NULL;                           \
    __ptr->random = NULL;                           \
    __ptr;                                          \
})

struct list_node {
	int val;

	struct list_node *next;
	struct list_node *random;
};

struct list_node *copy_list_with_random_node(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	struct list_node *cur = head;
	struct list_node *next = NULL;

	/*
	 * 1 -> 2
	 * 1 -> 1' -> 2
	 */
	while (cur != NULL) {
		next = cur->next;

		struct list_node *t = NEW_NODE(struct list_node, cur->val);
		cur->next = t;
		t->next = next;

		cur = next;
	}

	cur = head;
	struct list_node *cp = NULL;

	/*
	 * set copy node rand
	 * 1 -> 1' -> 2 -> 2'
	 */
	while (cur != NULL) {
		next = cur->next->next;

		cp = cur->next;
		cp->random = cur->random != NULL ? cur->random->next : NULL;

		cur = next;
	}

	struct list_node *res = head->next;
	cur = head;

	/* split */
	while (cur != NULL) {
		next = cur->next->next;

		cp = cur->next;
		cp->next = next != NULL ? next->next : NULL;

		cur->next = next;

		cur = next;
	}

	return res;
}

struct list_node *copy_list_with_random_node_1_map(struct list_node *head)
{
	if (head == NULL) {
		return NULL;
	}

	std::map < struct list_node *, struct list_node * > map;
	struct list_node *cur = head;

	while (cur != NULL) {
		struct list_node *t = NEW_NODE(struct list_node, cur->val);
		map.insert(std::make_pair(cur, t));

		cur = cur->next;
	}

	/* set list tail to NULL */
	map.insert(std::make_pair((struct list_node *)NULL, (struct list_node *)NULL));

	cur = head;
	while (cur != NULL) {
		/*
		 * map.find(cur)->second -> new node
		 * new node.next ->  cur.next 克隆节点找到
		 */
		map[cur]->next = map[cur->next];
		map[cur]->random = map[cur->random];

		cur = cur->next;
	}

	return map.find(head)->second;
}

void print_chain_table(struct list_node *head)
{
	struct list_node *p = head;

	printf("<%p>: \t", p);
	while (p) {
		printf("%2d -> ", p->val);
		p = p->next;
	}
	printf("null\n");

	p = head;

	printf("\t\t\t");
	while (p) {
		printf("%2d -| ", p->random != NULL ? p->random->val : 0);
		p = p->next;
	}
	printf("\n");
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

struct list_node *create_chain_table_from_s(char s[])
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

	struct list_node *cur = head;

	for (uint32_t i = 0; i < strlen(s); i++) {
		struct list_node *__rand = head;

		int n = (int)(RANDOM() * (strlen(s) + 1));
		while (n > 0) {
			__rand = __rand->next;
			n--;
		}
		cur->random = __rand;

		cur = cur->next;
	}

	return head;
}

int main(int argc, char *argv[])
{
	srandom((unsigned) time(0));

	char s[] = "1234567";
	printf("strlen('%s') = %ld\n", s, strlen(s));

	struct list_node *head = create_chain_table_from_s(s);
	print_chain_table(head);

	printf("\ncopy list:\n\n");

	struct list_node *l1 = copy_list_with_random_node_1_map(head);
	print_chain_table(l1);
	free_chain_table(l1);

	printf("\n---8<---------------------------------------------------\n\n");

	struct list_node *l2 = copy_list_with_random_node(head);
	print_chain_table(l2);
	free_chain_table(l2);

	printf("\n");
	free_chain_table(head);

	return 0;
}
