
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue>
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

void level_traversal(struct tree_desc *head)
{
	printf("level traversal:\t");

	if (head == NULL) {
		return;
	}

	std::queue < struct tree_desc * > q;
	q.push(head);

	while (!q.empty()) {
		head = q.front();
		q.pop();
		printf("%d, ", head->val);

		if (head->left != NULL) {
			q.push(head->left);
		}
		if (head->right != NULL) {
			q.push(head->right);
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

	level_traversal(head);

	delete_tree_table(head);

	return 0;
}
