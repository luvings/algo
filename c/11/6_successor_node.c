
#include <limits.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

#define NEW_NODE(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    __ptr->val    = (var);                          \
    __ptr->left   = NULL;                           \
    __ptr->right  = NULL;                           \
    __ptr->parent = NULL;                           \
    __ptr;                                          \
})

struct tree_desc {
	int val;

	struct tree_desc *left;
	struct tree_desc *right;
	struct tree_desc *parent;
};

/*
 *           1
 *         /   \
 *        2     3
 *       / \   / \
 *      4   5 6   7
 *
 *  in order: 4, 2, 5, 1, 6, 3, 7
 *
 *  1) x 节点如果有右树, x 后继节点为其右树的最左节点
 *
 *  2) x 节点如果无右树, 向上找节点, 如果是其父节点的左孩子..
 */
struct tree_desc *get_left_most(struct tree_desc *node)
{
	if (node == NULL) {
		return NULL;
	}

	while (node->left != NULL) {
		node = node->left;
	}

	return node;
}

struct tree_desc *successor_node(struct tree_desc *node)
{
	if (node == NULL) {
		return NULL;
	}

	if (node->right != NULL) {
		return get_left_most(node->right);
	} else {		/* 无右子树 */
		struct tree_desc *parent = node->parent;
		while (parent != NULL && parent->right == node) {	/* 当前节点是其父亲节点右孩子 */
			node = parent;
			parent = node->parent;
		}
		return parent;
	}
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

	head = NEW_NODE(struct tree_desc, 6);
	head->parent = NULL;
	head->left = NEW_NODE(struct tree_desc, 3);
	head->left->parent = head;
	head->left->left = NEW_NODE(struct tree_desc, 1);
	head->left->left->parent = head->left;
	head->left->left->right = NEW_NODE(struct tree_desc, 2);
	head->left->left->right->parent = head->left->left;
	head->left->right = NEW_NODE(struct tree_desc, 4);
	head->left->right->parent = head->left;
	head->left->right->right = NEW_NODE(struct tree_desc, 5);
	head->left->right->right->parent = head->left->right;
	head->right = NEW_NODE(struct tree_desc, 9);
	head->right->parent = head;
	head->right->left = NEW_NODE(struct tree_desc, 8);
	head->right->left->parent = head->right;
	head->right->left->left = NEW_NODE(struct tree_desc, 7);
	head->right->left->left->parent = head->right->left;
	head->right->right = NEW_NODE(struct tree_desc, 10);
	head->right->right->parent = head->right;

	return head;
}

int main(int argc, char **argv)
{
	struct tree_desc *head = create_tree_table();

	printf("in-order:\t");
	in_order_traversal(head);
	printf("\n\n");

	struct tree_desc *t = head->left->left;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head->left->left->right;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head->left;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head->left->right;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head->left->right->right;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head->right->left->left;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head->right->left;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);
	t = head->right;
	printf("'%d', next: '%d'\n", t->val, successor_node(t)->val);

	t = head->right->right;	/* 10's next is null */
	char s[128] = "null";
	if (successor_node(t)) {
		/*
		 * snprintf
		 *
		 * 1) 如果格式化后的字符串长度 < n, 则将此字符串全部复制到 str 中, 并给其后添加一个字符串结束符 '\0'
		 * 2) 如果格式化后的字符串长度 >= n, 则只将其中的(n-1)个字符复制到 str 中, 并给其后添加一个字符串结束符 '\0'
		 *
		 * 返回参数 str 字符串长度, 错误原因存于 errno
		 */
		snprintf(s, 128, "%d", successor_node(t)->val);
	}
	printf("'%d', next: '%s'\n", t->val, s);

	delete_tree_table(head);

	return 0;
}
