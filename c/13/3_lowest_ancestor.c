
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <map>
#include <string>
#include <set>
#include <vector>

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

struct info {
	bool find_o1;
	bool find_o2;
	struct tree_desc *ancestor;
};

struct info process(struct tree_desc *x, struct tree_desc *o1, struct tree_desc *o2)
{
	struct info rep = { false, false, NULL, };
	if (x == NULL) {
		return rep;
	}

	struct info l_info = process(x->left, o1, o2);
	struct info r_info = process(x->right, o1, o2);

	bool find_o1 = (x == o1) || l_info.find_o1 || r_info.find_o1;
	bool find_o2 = (x == o2) || l_info.find_o2 || r_info.find_o2;

	struct tree_desc *ancestor = NULL;
	if (l_info.ancestor != NULL) {
		ancestor = l_info.ancestor;
	}

	if (r_info.ancestor != NULL) {
		ancestor = r_info.ancestor;
	}

	if (ancestor == NULL) {
		if (find_o1 && find_o2) {
			ancestor = x;
		}
	}

	rep.find_o1 = find_o1;
	rep.find_o2 = find_o2;
	rep.ancestor = ancestor;

	return rep;
}

struct tree_desc *lowest_ancestor(struct tree_desc *head, struct tree_desc *o1, struct tree_desc *o2)
{
	if (head == NULL) {
		return 0;
	}

	return process(head, o1, o2).ancestor;
}

void preprocess_parent_map(struct tree_desc *head, std::map < struct tree_desc *, struct tree_desc * > &m_father)
{
	if (head == NULL) {
		return;
	}

	if (head->left != NULL) {
		m_father.insert(std::make_pair(head->left, head));
	}

	if (head->right != NULL) {
		m_father.insert(std::make_pair(head->right, head));
	}

	preprocess_parent_map(head->left, m_father);
	preprocess_parent_map(head->right, m_father);
}

struct tree_desc *lowest_ancestor_cmp(struct tree_desc *head, struct tree_desc *o1, struct tree_desc *o2)
{
	if (head == NULL) {
		return NULL;
	}

	std::map < struct tree_desc *, struct tree_desc * > m_father;
	m_father.insert(std::make_pair(head, head));

	preprocess_parent_map(head, m_father);

	std::set < struct tree_desc * > set;
	struct tree_desc *cur = o1;

	set.insert(o1);

	while (m_father[cur] != cur) {
		cur = m_father[cur];
		set.insert(cur);
	}

	cur = o2;
	while (!set.count(cur)) {
		cur = m_father[cur];
	}

	return cur;
}

/*
 * 1) o1 是 o2 的最低公共祖先, 或 o2 是 o1 的最低公共祖先
 *
 * 2) o1, o2 互不为最低公共祖先, 继续向上可找到
 */
struct tree_desc *lowest_ancestor_cmpp(struct tree_desc *head, struct tree_desc *o1, struct tree_desc *o2)
{
	if (head == NULL || head == o1 || head == o2) {
		return head;
	}

	struct tree_desc *left = lowest_ancestor_cmpp(head->left, o1, o2);
	struct tree_desc *right = lowest_ancestor_cmpp(head->right, o1, o2);
	if (left != NULL && right != NULL) {
		return head;
	}

	return left != NULL ? left : right;
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

void preprocess_tree_node(struct tree_desc *head, std::vector < struct tree_desc * > &v)
{
	if (head == NULL) {
		return;
	}

	preprocess_tree_node(head->left, v);
	v.push_back(head);
	preprocess_tree_node(head->right, v);
}

struct tree_desc *pick_one_random_node(struct tree_desc *head)
{
	if (head == NULL) {
		return NULL;
	}

	std::vector < struct tree_desc * > v;
	preprocess_tree_node(head, v);

	int rand_idx = (int)(RANDOM() * v.size());

	return v.at(rand_idx);
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

		struct tree_desc *o1 = pick_one_random_node(head);
		struct tree_desc *o2 = pick_one_random_node(head);
		print_tree_table(head);

		struct tree_desc *ancestor = lowest_ancestor(head, o1, o2);
		printf("o1(%d), o2(%d) ancestor: %d\n", o1 ? o1->val : -111, o2 ? o2->val : -111,
		       ancestor ? ancestor->val : -111);

		if (lowest_ancestor(head, o1, o2) != lowest_ancestor_cmp(head, o1, o2)) {
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
