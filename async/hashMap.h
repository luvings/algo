#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include"dataStruct.h"
#include"array.h"

typedef struct entry {
	let key;				// ��
	let value;				// ֵ
	struct entry * next;	// ��ͻ����
}*Entry;

#define newEntry() NEW(struct entry)
#define newEntryList(length) (Entry)malloc(length * sizeof(struct entry))

// ��ϣ�ṹ
typedef struct hashMap *HashMap;

#define newHashMap() NEW(struct hashMap)

// ��ϣ��������
typedef int(*HashCode)(HashMap, let key);

// �еȺ�������
typedef Boolean(*Equal)(let key1, let key2);

// ��Ӽ���������
typedef void(*Put)(HashMap hashMap, let key, let value);

// ��ȡ����Ӧֵ�ĺ�������
typedef let(*Get)(HashMap hashMap, let key);

// ɾ�����ĺ�������
typedef let(*Remove)(HashMap hashMap, let key);

// ���Map�ĺ�������
typedef void(*Clear)(HashMap hashMap);

// �жϼ�ֵ�Ƿ���ڵĺ�������
typedef Boolean(*Exists)(HashMap hashMap, let key);

typedef struct hashMap {
	int size;			// ��ǰ��С
	int listSize;		// ��Ч�ռ��С
	HashCode hashCode;	// ��ϣ����
	Equal equal;		// �еȺ���
	Entry list;			// �洢����
	Put put;			// ��Ӽ��ĺ���
	Get get;			// ��ȡ����Ӧֵ�ĺ���
	Remove remove;		// ɾ����
	Clear clear;		// ���Map
	Exists exists;		// �жϼ��Ƿ����
	Boolean autoAssign;	// �趨�Ƿ���ݵ�ǰ��������̬�����ڴ��С��Ĭ�Ͽ���
}*HashMap;

// �������ṹ
typedef struct hashMapIterator {
	Entry entry;	// ��������ǰָ��
	int count;		// ��������
	int hashCode;	// ��ֵ�ԵĹ�ϣֵ
	HashMap hashMap;
}*HashMapIterator;

#define newHashMapIterator() NEW(struct hashMapIterator)

// Ĭ�Ϲ�ϣ����
static int defaultHashCode(HashMap hashMap, let key);

// Ĭ���жϼ�ֵ�Ƿ����
static Boolean defaultEqual(let key1, let key2);

// Ĭ����Ӽ�ֵ��
static void defaultPut(HashMap hashMap, let key, let value);

// Ĭ�ϻ�ȡ����Ӧֵ
static let defaultGet(HashMap hashMap, let key);

// Ĭ��ɾ����
static let defaultRemove(HashMap hashMap, let key);

// Ĭ���жϼ��Ƿ����
static Boolean defaultExists(HashMap hashMap, let key);

// Ĭ�����Map
static void defaultClear(HashMap hashMap);

// ���¹���
static void resetHashMap(HashMap hashMap, int listSize);

// ����һ����ϣ�ṹ
HashMap createHashMap(HashCode hashCode, Equal equal);

// ������ϣ�ṹ������
HashMapIterator createHashMapIterator(HashMap hashMap);

// �������Ƿ�����һ��
Boolean hasNextHashMapIterator(HashMapIterator iterator);

// ��������һ��
HashMapIterator nextHashMapIterator(HashMapIterator iterator);

// �ͷŵ������ڴ�
void freeHashMapIterator(HashMapIterator * iterator);

#endif // !__HASHMAP_H__