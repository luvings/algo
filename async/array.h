#ifndef __ARRAY_H__
#define __ARRAY_H__

#include"dataStruct.h"

// ˫������
typedef struct list {
	let item;
	struct list * next;
	struct list * last;
}*ListNode, *List;

#define newList() NEW(struct list)
#define newListNode() NEW(struct list)

typedef struct array *Array;

typedef Array(*Concat)(Array a, Array b);

typedef let (*Pop)(Array array);

typedef int(*Push)(Array array, let item);

typedef void(*Reverse)(Array array);

typedef let (*Shift)(Array array);

typedef int (*Unshift)(Array array, let item);

typedef Array(*Slice)(Array array, int start, int end);

typedef int(*SortBy)(let item1, let item2);

typedef void(*Sort)(Array array, SortBy sort);

typedef Boolean(*FilterBy)(let item);

typedef Array(*Filter)(Array array, FilterBy filter);

typedef let (*IndexOf)(Array array, int index);

typedef struct array {
	Pop pop;
	List head;
	List tail;
	Sort sort;
	Push push;
	int length;
	Shift shift;
	Slice slice;
	Filter filter;
	Concat concat;
	Reverse reverse;
	Unshift unshift;
	IndexOf indexOf;
}*Array;

#define newArray() NEW(struct array)

// �������ṹ
typedef struct arrayIterator {
	ListNode node;	// ��������ǰָ��
	int count;		// ��������
	Array array;
}*ArrayIterator;

#define newArrayIterator() NEW(struct arrayIterator)

// �����������飬����һ���µ�����
static Array concat(Array a, Array b);

// �����������һ��Ԫ��
static let pop(Array a);

// ������������һ��Ԫ�أ������³���
static int push(Array array, let item);

// ��ת�������飬���޸�ԭ����
static void reverse(Array array);

// ɾ������������ĵ�һ��Ԫ��
static let shift(Array array);

// ɾ������������ĵ�һ��Ԫ��
static int unshift(Array array, let item);

// ��������ѡȡһ���֣�����һ�������鷵��
static Array slice(Array array, int start, int end);

// �������Ԫ�ؽ�������
static void sort(Array array, SortBy sort);

// ���˳�һ����Ԫ�أ�filter����Trueʱ���������������鷵��
static Array filter(Array array, FilterBy filter);

// ����ָ������λ�õ�ֵ
static let indexOf(Array array, int index);

// ��������
Array createArray(int size);

// �ͷ�����
void freeArray(Array * array);

// �������������
ArrayIterator createArrayIterator(Array array);

// �������Ƿ�����һ��
Boolean hasNextArrayIterator(ArrayIterator iterator);

// ��������һ��
ArrayIterator nextArrayIterator(ArrayIterator iterator);

// �ͷŵ������ڴ�
void freeArrayIterator(ArrayIterator * iterator);

#endif // !__ARRAY_H__
