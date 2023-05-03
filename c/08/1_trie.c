
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

typedef struct trie_node {
	int pass;
	int end;

	/*
	 * 0    a
	 * 1    b
	 * 2    c
	 * ..   ..
	 * 25   z
	 * nexts[i] == null   i 方向的路不存在
	 * nexts[i] != null   i 方向的路存在
	 */
	struct trie_node *nexts[26];
} m_node_t;

typedef struct trie {
	struct trie_node root;
} m_trie_t;

void insert(m_trie_t *trie, std::string word)
{
	if (word.empty()) {
		return;
	}

	const char *chs = word.c_str();
	struct trie_node *node = &trie->root;
	node->pass++;

	int path = 0;
	for (uint32_t i = 0; i < word.length(); i++) {	/* 从左往右遍历字符 */
		path = chs[i] - 'a';	/* 由字符, 对应成走向哪条路 */
		if (node->nexts[path] == NULL) {
			node->nexts[path] = (struct trie_node *) malloc(sizeof(struct trie_node));
			memset(node->nexts[path], 0, sizeof(struct trie_node));
		}

		node = node->nexts[path];
		node->pass++;
	}

	node->end++;
}

int search(m_trie_t *trie, std::string word)
{
	if (word.empty()) {
		return 0;
	}

	const char *chs = word.c_str();
	struct trie_node *node = &trie->root;

	int path = 0;
	for (uint32_t i = 0; i < word.length(); i++) {
		path = chs[i] - 'a';
		if (node->nexts[path] == NULL) {
			return 0;
		}

		node = node->nexts[path];
	}

	return node->end;
}

/* 所有加入的字符串中, 有几个是以 prefix 这个字符串作为前缀的 */
int prefix_number(m_trie_t *trie, std::string prefix)
{
	if (prefix.empty()) {
		return 0;
	}

	const char *chs = prefix.c_str();
	struct trie_node *node = &trie->root;

	int path = 0;
	for (uint32_t i = 0; i < prefix.length(); i++) {
		path = chs[i] - 'a';
		if (node->nexts[path] == NULL) {
			return 0;
		}

		node = node->nexts[path];
	}

	return node->pass;
}

void delete_un(m_trie_t *trie, std::string word)
{
	printf("delete '%s'..\n", word.c_str());

	if (search(trie, word) == 0) {
		return;
	}

	/* word exist */
	const char *chs = word.c_str();
	struct trie_node *prev = &trie->root;
	prev->pass--;

	int path = 0;

	for (uint32_t i = 0; i < word.length(); i++) {
		path = chs[i] - 'a';

		if (--prev->nexts[path]->pass == 0) {
			struct trie_node **tail = &(prev->nexts[path]);

			while (*tail != NULL) {
				struct trie_node *entry = *tail;

				*tail = entry->nexts[chs[++i] - 'a'];

				free(entry);
			}

			//prev->nexts[path] = NULL;

			return;
		}

		prev = prev->nexts[path];
	}

	if (prev != NULL)
		prev->end--;
}

void print_trie_node(struct trie_node *node, int k)
{
	if (node == NULL) {
		return;
	}

	printf("(%d, %d)", node->pass, node->end);

	for (int i = 0; i < 26; i++) {
		if (node->nexts[i] != NULL) {
			printf("->%c", i + 'a');
			print_trie_node(node->nexts[i], k + 1);

			printf("\n");
			for (int j = k; j >= 0; j--)
				printf("\t");
		}
	}
}

typedef struct trie_node1 {
	int pass;
	int end;

	std::map < int, struct trie_node1 * > nexts;
} m_node1_t;

typedef struct trie1 {
	struct trie_node1 root;
} m_trie1_t;

bool contains(std::map < int, struct trie_node1 * > nexts, int x)
{
	return (nexts.find(x) != nexts.end());
}

void insert(m_trie1_t *trie, std::string word)
{
	if (word.empty()) {
		return;
	}

	const char *chs = word.c_str();
	struct trie_node1 *node = &trie->root;
	node->pass++;

	int path = 0;
	for (uint32_t i = 0; i < word.length(); i++) {
		path = (int) chs[i];

		if (!contains(node->nexts, path)) {
			struct trie_node1 *t = (struct trie_node1 *) malloc(sizeof(*t));
			// FIXME: ? ?
			memset((void *)t, 0, sizeof(*t));
			std::map < int, struct trie_node1 * > __map;
			__map.clear();
			t->nexts = __map;

			node->nexts.insert(std::make_pair(path, t));
		}

		node = node->nexts.at(path);
		if (node)
			node->pass++;
	}

	node->end++;
}

int search(m_trie1_t *trie, std::string word)
{
	if (word.empty()) {
		return 0;
	}

	const char *chs = word.c_str();
	struct trie_node1 *node = &trie->root;

	int path = 0;
	for (uint32_t i = 0; i < word.length(); i++) {
		path = (int) chs[i];

		if (!contains(node->nexts, path)) {
			return 0;
		}

		node = node->nexts.at(path);
	}

	return node->end;
}

void delete_un(m_trie1_t *trie, std::string word)
{
	printf("delete '%s'..\n", word.c_str());

	if (search(trie, word) == 0) {
		return;
	}

	/* word exist */
	const char *chs = word.c_str();
	struct trie_node1 *prev = &trie->root;
	prev->pass--;

	for (uint32_t i = 0; i < word.length(); i++) {

		if (--prev->nexts[(int) chs[i]]->pass == 0) {
			struct trie_node1 **tail = &(prev->nexts[(int) chs[i]]);

			while (*tail != NULL) {
				struct trie_node1 *entry = *tail;

				*tail = entry->nexts[(int) chs[++i]];

				entry->nexts.clear();
				free(entry);
			}

			//prev->nexts[(int) chs[i]] = NULL;

			return;
		}

		prev = prev->nexts[(int) chs[i]];
	}

	if (prev != NULL)
		prev->end--;
}

int main(int argc, char *argv[])
{
	m_trie_t trie = {
		.root = { 0, 0, NULL, },
	};

	std::string word("abc");
	insert(&trie, word);

	insert(&trie, "ab");
	insert(&trie, "bf");
	insert(&trie, "cca");
	insert(&trie, "ca");
	insert(&trie, "cb");

	print_trie_node(&trie.root, 0);

	delete_un(&trie, "bf");
	print_trie_node(&trie.root, 0);

	std::string prefix("ab");
	printf("prefix '%s' number: %d\n", prefix.c_str(), prefix_number(&trie, prefix));

	printf("\n---8>--------------------------------------------------\n\n");

	m_trie1_t trie1 = {
		.root = { 0, 0, },
	};

	insert(&trie1, "bf");
	insert(&trie1, "baa");
	insert(&trie1, "aa");

	delete_un(&trie1, "baa");

	return 0;
}
