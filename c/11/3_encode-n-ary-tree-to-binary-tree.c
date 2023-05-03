/*
 * 要求在多叉树和二叉树之间建立一个双射
 *
 * n-ary tree:
 *
 *          1
 *       /  |  \
 *      3   2   4
 *     / \
 *    5   6
 *
 * binary tree:
 *
 *       1
 *      /
 *     3
 *    / \
 *    5  2
 *     \  \
 *      6  4
 *
 * N叉树根结点1的第一个子结点3被挂到了二叉树的左子结点上,
 * 同一层的结点2挂到了结点3的右子结点上, 同一层的结点4被挂到了结点2的右子结点上.
 * 而结点3本身的子结点也按照这个规律, 第一个子结点5挂到了结点3的左子结点上,
 * 而同一排的结点6挂到了结点5的右子结点上.
 *
 * 对于解码,
 * 也是同样的规律, 先根据根结点值新建一个空的N叉树结点,
 * 由于我们的编码规律, 根结点是一定没有右子结点的, 所以取出左子结点 cur,
 * 并且开始循环, 如果 cur 结点存在, 那么我们对 cur 递归调用解码函数,
 * 将返回的结点加入当前N叉树结点的 children 数组中,
 * 然后 cur 再赋值为其右子结点, 继续递归调用解码函数,
 * 再加入 children 数组, 如此便可将二叉树还原为之前的N叉树
 */

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <string>
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

#define NEW_ROOT(type, var)   ({                    \
    type *__ptr = (type *) malloc(sizeof(type));    \
    assert(__ptr);                                  \
    memset((void *)__ptr, 0, sizeof(type));         \
    std::vector < struct ntree * > __child;         \
    __ptr->val = (var);                             \
    __ptr->children = __child;                      \
    __ptr;                                          \
})

struct ntree {
	int val;

	std::vector < struct ntree * > children;
};

struct tree_desc {
	int val;

	struct tree_desc *left;
	struct tree_desc *right;
};

/*
 * encodes an n-ary tree to a binary tree.
 *
 * n-ary tree:
 *
 *          1
 *       /  |  \
 *      3   2   4
 *     / \
 *    5   6
 */
struct tree_desc *encode(struct ntree *root)
{
	if (!root) {
		return NULL;
	}

	struct tree_desc *res = NEW_NODE(struct tree_desc, root->val);
	printf("%d, ", root->val);

	if (!root->children.empty()) {
		res->left = encode(root->children[0]);
		printf("%d, ", res->left->val);
	}

	struct tree_desc *cur = res->left;

	for (uint32_t i = 1; i < root->children.size(); i++) {
		cur->right = encode(root->children[i]);
		printf("%d, ", cur->right->val);
		cur = cur->right;
	}

	return res;
}

/*
 * decodes your binary tree to an n-ary tree.
 */
struct ntree *decode(struct tree_desc *head)
{
	if (!head) {
		return NULL;
	}

	struct ntree *res = NEW_ROOT(struct ntree, head->val);
	printf("%d, ", head->val);

	struct tree_desc *cur = head->left;

	while (cur != NULL) {
		res->children.push_back(decode(cur));
		printf("%d, ", cur->val);
		cur = cur->right;
	}

	return res;
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

void delete_ntree_table(struct ntree *root)
{
	if (!root) {
		return;
	}

    for (auto child : root->children) {
		delete_ntree_table(child);
	}
	free(root);
}

struct ntree *create_tree_table(void)
{
	struct ntree *n1 = NEW_ROOT(struct ntree, 1);
	struct ntree *n3 = NEW_ROOT(struct ntree, 3);
	struct ntree *n2 = NEW_ROOT(struct ntree, 2);
	struct ntree *n4 = NEW_ROOT(struct ntree, 4);
	struct ntree *n5 = NEW_ROOT(struct ntree, 5);
	struct ntree *n6 = NEW_ROOT(struct ntree, 6);

	n3->children.push_back(n5);
	n3->children.push_back(n6);

	n1->children.push_back(n3);
	n1->children.push_back(n2);
	n1->children.push_back(n4);

	return n1;
}

int main(int argc, char **argv)
{
	struct ntree *root = create_tree_table();

	struct tree_desc *h = encode(root);
	printf("\n");

	struct ntree *rt = decode(h);
	printf("\n");

	delete_tree_table(h);

	delete_ntree_table(root);

	delete_ntree_table(rt);

	return 0;
}
