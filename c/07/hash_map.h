#ifndef __HASH_MAP_H__
#define __HASH_MAP_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// 哈希结构
typedef struct hashMap *HashMap;
typedef void *let;

typedef struct entry {
	let key;		// 键
	let value;		// 值
	struct entry *next;	// 冲突链表
} *Entry;

#define NEW(type) (type *) malloc(sizeof(type))

#define newEntry() NEW(struct entry)

#define newEntryList(length) (Entry) malloc(length * sizeof(struct entry))

#define newHashMap() NEW(struct hashMap)

// 哈希函数类型
typedef int (*HashCode)(HashMap, let key);

// 判等函数类型
typedef bool (*Equal)(let key1, let key2);

// 添加键函数类型
typedef void (*Put)(HashMap hashMap, let key, let value);

// 获取键对应值的函数类型
typedef let(*Get) (HashMap hashMap, let key);

// 删除键的函数类型
typedef let(*Remove) (HashMap hashMap, let key);

// 清空Map的函数类型
typedef void (*Clear)(HashMap hashMap);

// 判断键值是否存在的函数类型
typedef bool (*Exists)(HashMap hashMap, let key);

typedef struct hashMap {
	int size;		// 当前大小
	int listSize;		// 有效空间大小
	HashCode hashCode;	// 哈希函数
	Equal equal;		// 判等函数
	Entry list;		// 存储区域
	Put put;		// 添加键的函数
	Get get;		// 获取键对应值的函数
	Remove remove;		// 删除键
	Clear clear;		// 清空Map
	Exists exists;		// 判断键是否存在
	bool autoAssign;	// 设定是否根据当前数据量动态调整内存大小, 默认开启
} *HashMap;

// 迭代器结构
typedef struct hashMapIterator {
	Entry entry;		// 迭代器当前指向
	int count;		// 迭代次数
	int hashCode;		// 键值对的哈希值
	HashMap hashMap;
} *HashMapIterator;

#define newHashMapIterator() NEW(struct hashMapIterator)

// 默认哈希函数
static int defaultHashCode(HashMap hashMap, let key);

// 默认判断键值是否相等
static bool defaultEqual(let key1, let key2);

// 默认添加键值对
static void defaultPut(HashMap hashMap, let key, let value);

// 默认获取键对应值
static let defaultGet(HashMap hashMap, let key);

// 默认删除键
static let defaultRemove(HashMap hashMap, let key);

// 默认判断键是否存在
static bool defaultExists(HashMap hashMap, let key);

// 默认清空Map
static void defaultClear(HashMap hashMap);

// 重新构建
static void resetHashMap(HashMap hashMap, int listSize);

// 创建一个哈希结构
HashMap createHashMap(HashCode hashCode, Equal equal);

// 创建哈希结构迭代器
HashMapIterator createHashMapIterator(HashMap hashMap);

// 迭代器是否有下一个
bool hasNextHashMapIterator(HashMapIterator iterator);

// 迭代到下一次
HashMapIterator nextHashMapIterator(HashMapIterator iterator);

// 释放迭代器内存
void freeHashMapIterator(HashMapIterator * iterator);

#endif /* __HASH_MAP_H__ */
