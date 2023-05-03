
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stack>
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

/* root -> left -> right */
void un_prev_order_traversal(struct tree_desc *head)
{
	printf("pre-order:\t");

	if (head != NULL) {
		std::stack < struct tree_desc * > stack;
		stack.push(head);

		while (!stack.empty()) {
			head = stack.top();
			stack.pop();
			printf("%d, ", head->val);

			if (head->right != NULL) {
				stack.push(head->right);
			}

			if (head->left != NULL) {
				stack.push(head->left);
			}
		}
	}
	printf("\n\n");
}

/*
 * 左 -> 头 -> 右
 *              v
 *           左 -> 头
 *
 * 左边界依次进栈, 弹出时打印:    左 -> 头
 *
 * 对右子树, 也以左边界依次进栈..
 */
void un_in_order_traversal(struct tree_desc *head)
{
	printf("in-order:\t");

	if (head != NULL) {
		struct tree_desc *cur = head;
		std::stack < struct tree_desc * > s;

		while (!s.empty() || cur != NULL) {
			if (cur != NULL) {
				s.push(cur);
				cur = cur->left;
			} else {
				cur = s.top();
				s.pop();
				printf("%d, ", cur->val);
				cur = cur->right;
			}
		}
	}
	printf("\n\n");
}

void un_post_order_traversal(struct tree_desc *head)
{
	printf("post-order:\t");

	if (head != NULL) {
		std::stack < struct tree_desc * > s1;
		std::stack < struct tree_desc * > s2;
		s1.push(head);

		while (!s1.empty()) {
			/* root -> right -> left */
			head = s1.top();
			s1.pop();

			s2.push(head);

			if (head->left != NULL) {
				s1.push(head->left);
			}

			if (head->right != NULL) {
				s1.push(head->right);
			}
		}

		/* left -> right -> root */
		while (!s2.empty()) {
			head = s2.top();
			s2.pop();
			printf("%d, ", head->val);
		}
	}
	printf("\n\n");
}

void un_post_order_traversal_2(struct tree_desc *h)
{
	printf("post-order 2:\t");

	if (h != NULL) {
		struct tree_desc *c = NULL;
		std::stack < struct tree_desc * > s;
		s.push(h);

		while (!s.empty()) {
			/* c -> current node, h -> prev pop node */
			c = s.top();

			if (c->left != NULL && c->left != h && c->right != h) {
				s.push(c->left);
			} else if (c->right != NULL && c->right != h) {
				s.push(c->right);
			} else {
				c = s.top();
				s.pop();
				printf("%d, ", c->val);
				h = c;
			}
		}
	}
	printf("\n\n");
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

	un_prev_order_traversal(head);

	un_in_order_traversal(head);

	un_post_order_traversal(head);

	un_post_order_traversal_2(head);

	delete_tree_table(head);

	return 0;
}
