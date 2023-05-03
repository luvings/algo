
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val   = (var);                           \
    __ptr->left  = NULL;                            \
    __ptr->right = NULL;                            \
    __ptr;                                          \
})

struct tree_desc {
	int val;

	struct tree_desc *left;
	struct tree_desc *right;
};

void f(struct tree_desc *head)
{
	if (head == NULL) {
		return;
	}
	/* 1 */
	f(head->left);
	/* 2 */
	f(head->right);
	/* 3 */
}

/* root -> left -> right */
void prev_order_traversal(struct tree_desc *head)
{
	if (head == NULL) {
		return;
	}

	printf("%d, ", head->val);
	prev_order_traversal(head->left);
	prev_order_traversal(head->right);
}

void in_order_traversal(struct tree_desc *head)
{
	if (head == NULL) {
		return;
	}

	in_order_traversal(head->left);
	printf("%d, ", head->val);
	in_order_traversal(head->right);
}

void post_order_traversal(struct tree_desc *head)
{
	if (head == NULL) {
		return;
	}

	post_order_traversal(head->left);
	post_order_traversal(head->right);
	printf("%d, ", head->val);
}

void delete_tree_table(struct tree_desc *head)
{
	if (!head) {
		return;
	}

	delete_tree_table(head->left);
	delete_tree_table(head->right);
	free(head);
}

struct tree_desc *create_tree_table(void)
{
	struct tree_desc *head = NULL;

	head = NEW_NODE(struct tree_desc, 1);
	head->left = NEW_NODE(struct tree_desc, 2);
	head->right = NEW_NODE(struct tree_desc, 3);
	head->left->left = NEW_NODE(struct tree_desc, 4);
	head->left->right = NEW_NODE(struct tree_desc, 5);
	head->right->left = NEW_NODE(struct tree_desc, 6);
	head->right->right = NEW_NODE(struct tree_desc, 7);

	return head;
}

int main(int argc, char **argv)
{
	struct tree_desc *head = create_tree_table();

	printf("pre-order:\t");
	prev_order_traversal(head);
	printf("\n\n");

	printf("in-order:\t");
	in_order_traversal(head);
	printf("\n\n");

	printf("post-order:\t");
	post_order_traversal(head);
	printf("\n\n");

	delete_tree_table(head);

	return 0;
}
