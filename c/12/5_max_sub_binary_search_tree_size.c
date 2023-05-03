
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <vector>
#include <string>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

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

#if 1

/*---8>-----------------------------------------------------------------------*/

struct info {
	bool b_null;
	bool b_bst;
	int max_sub_bst_size;
	int max;
	int min;
};

struct info process(struct tree_desc *x)
{
	struct info rep = { true, true, 0, };
	if (x == NULL) {
		return rep;
	}

	struct info l_info = process(x->left);
	struct info r_info = process(x->right);

	int max = x->val, min = x->val;

	if (!l_info.b_null) {
		max = std::max(l_info.max, max);
		min = std::min(l_info.min, min);
	}
	if (!r_info.b_null) {
		max = std::max(r_info.max, max);
		min = std::min(r_info.min, min);
	}

	int max_sub_bst_size = 0;

	if (!l_info.b_null) {
		max_sub_bst_size = l_info.max_sub_bst_size;
	}
	if (!r_info.b_null) {
		max_sub_bst_size = std::max(r_info.max_sub_bst_size, max_sub_bst_size);
	}

	bool b_bst = false;

	if ((l_info.b_null ? true : l_info.b_bst) &&
	    (r_info.b_null ? true : r_info.b_bst) &&
	    (l_info.b_null ? true : l_info.max < x->val) && (r_info.b_null ? true : r_info.min > x->val)
	    ) {
		max_sub_bst_size =
		    (l_info.b_null ? 0 : l_info.max_sub_bst_size) + (r_info.b_null ? 0 : r_info.max_sub_bst_size) + 1;
		b_bst = true;
	}

	rep.b_null = false;
	rep.b_bst = b_bst;
	rep.max_sub_bst_size = max_sub_bst_size;
	rep.max = max;
	rep.min = min;

	return rep;
}

int largest_BST_subtree(struct tree_desc *head)
{
	if (head == NULL) {
		return 0;
	}

	return process(head).max_sub_bst_size;
}

/*---8<-----------------------------------------------------------------------*/

#else

/*---8>-----------------------------------------------------------------------*/

struct info {
	bool b_null;
	int max_sub_bst_size;
	int t_size;
	int max;
	int min;
};

struct info process(struct tree_desc *x)
{
	struct info rep = { true, 0, };
	if (x == NULL) {
		return rep;
	}

	struct info l_info = process(x->left);
	struct info r_info = process(x->right);

	int max = x->val, min = x->val;
	int t_size = 1;

	if (!l_info.b_null) {
		max = std::max(l_info.max, max);
		min = std::min(l_info.min, min);
		t_size += l_info.t_size;
	}

	if (!r_info.b_null) {
		max = std::max(r_info.max, max);
		min = std::min(r_info.min, min);
		t_size += r_info.t_size;
	}

	int l_sub_size = -1;
	if (!l_info.b_null) {
		l_sub_size = l_info.max_sub_bst_size;
	}

	int r_sub_size = -1;
	if (!r_info.b_null) {
		r_sub_size = r_info.max_sub_bst_size;
	}

	int a_sub_size = -1;

	bool lf_bst = l_info.b_null ? true : (l_info.max_sub_bst_size == l_info.t_size);
	bool rf_bst = r_info.b_null ? true : (r_info.max_sub_bst_size == r_info.t_size);

	if (lf_bst && rf_bst) {
		bool l_max_less_x = l_info.b_null ? true : (l_info.max < x->val);
		bool r_min_more_x = r_info.b_null ? true : (r_info.min > x->val);

		if (l_max_less_x && r_min_more_x) {
			int l_s_s = l_info.b_null ? 0 : l_info.t_size;
			int r_s_s = r_info.b_null ? 0 : r_info.t_size;
			a_sub_size = l_s_s + r_s_s + 1;
		}
	}

	rep.b_null = false;
	rep.max_sub_bst_size = std::max(std::max(l_sub_size, r_sub_size), a_sub_size);
	rep.t_size = t_size;
	rep.max = max;
	rep.min = min;

	return rep;
}

int largest_BST_subtree(struct tree_desc *head)
{
	if (head == NULL) {
		return 0;
	}

	return process(head).max_sub_bst_size;
}

/*---8<-----------------------------------------------------------------------*/

#endif

void in(struct tree_desc *x, std::vector < int > &vv)
{
	if (x == NULL) {
		return;
	}

	in(x->left, vv);
	vv.push_back(x->val);
	in(x->right, vv);
}

int get_bst_size(struct tree_desc *head)
{
	if (head == NULL) {
		return 0;
	}

	std::vector < int > vv;

	in(head, vv);

	for (uint32_t i = 1; i < vv.size(); i++) {
		if (vv.at(i) <= vv.at(i - 1)) {
			return 0;
		}
	}

	return vv.size();
}

int max_sub_bst_size_cmp(struct tree_desc *head)
{
	if (head == NULL) {
		return 0;
	}

	int h = get_bst_size(head);
	if (h != 0) {
		return h;
	}

	return std::max(max_sub_bst_size_cmp(head->left), max_sub_bst_size_cmp(head->right));
}

struct tree_desc *alloc_tree_node(int level, int max_level, int max_value)
{
	if (level > max_level || RANDOM() < 0.5) {
		return NULL;
	}

	struct tree_desc *head = NEW_NODE(struct tree_desc, (int)(RANDOM() * (max_value + 1)));
	head->left = alloc_tree_node(level + 1, max_level, max_value);
	head->right = alloc_tree_node(level + 1, max_level, max_value);

	return head;
}

struct tree_desc *create_random_tree_table(int max_level, int max_value)
{
	return alloc_tree_node(1, max_level, max_value);
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

std::string space(int n)
{
	std::string s;

	s.resize(n, ' ');

	return s;
}

void in_order_p(struct tree_desc *head, int height, std::string to, int width)
{
	if (head == NULL) {
		return;
	}

	in_order_p(head->right, height + 1, "v", width);

	std::string val = to + std::to_string(head->val) + to;
	int len_m = val.length();
	int len_l = (width - len_m) >> 1;
	int len_r = width - len_m - len_l;
	val = space(len_l) + val + space(len_r);
	printf("%s%s\n", space(height * width).c_str(), val.c_str());

	in_order_p(head->left, height + 1, "^", width);
}

void print_tree_table(struct tree_desc *head)
{
	printf("\n\n");

	in_order_p(head, 0, "H", 12);

	printf("\n\n\n\n");
}

int main(int argc, char **argv)
{
	int max_level = 6;
	int max_value = 100;
	int times = 5;
	bool succeed = true;

	srandom((unsigned) time(0));

	for (int i = 0; i < times; i++) {
		struct tree_desc *head = create_random_tree_table(max_level, max_value);

		print_tree_table(head);
		int max_sub_bst_size = largest_BST_subtree(head);
		printf("max sub BST size: %d\n", max_sub_bst_size);

		if (largest_BST_subtree(head) != max_sub_bst_size_cmp(head)) {
			print_tree_table(head);
			printf("oops\n");
			succeed = false;
			break;
		}

		delete_tree_table(head);
	}

	printf("result: %s\n", succeed ? "nice" : "fucked");

	return 0;
}
