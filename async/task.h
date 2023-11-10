#ifndef __TASK_H__
#define __TASK_H__

#include<stdio.h>
#include<time.h>

#include"heap.h"
#include"fs.h"

// ����״̬
enum _Status {
	QUEUE,	// ������
	RUN,	// ������
	WAIT,	// �ȴ�����ʱ��ִ��
	DIE		// �ѱ�ɱ��
};

// ��������
enum _Type{
	LOOP,
	ONCE
};

typedef enum _Type Type;
typedef enum _Status Status;

#define newRunTime() NEW(struct runtime)

// ����������
typedef void(*Task)(let args);

typedef struct eventLoop * EventLoop;

typedef struct runtime * Runtime;

// ��������������
typedef void(*Start)(EventLoop eventLoop, Runtime runtime);

typedef struct runtime{
	// �û�����
	Task task;
	
	// ִ��״̬
	Status status;

	// ��������
	Type type;

	// ������������
	Start start;

	// ���񴴽�ʱ���
	unsigned long startTime;

	// ��ʱʱ��
	int millisec;

	// �ص�����
	let args;

}*Runtime;

typedef struct eventLoop {
	long wait;
	long timeout;
	Array undealIoQueue;
	Array finishIoQueue;
	Heap heap;
}*EventLoop;

#define newEventLoop() NEW(struct eventLoop)

// ִ������
#define runTask(runtime, eventLoop) (runtime->start)(eventLoop, runtime)

// ��������Ƿ�Ϊ��
#define isEmpty(eventLoop) (eventLoop->undealIoQueue->length == 0 \
	&& eventLoop->finishIoQueue->length == 0 && eventLoop->heap->size == 0)

// �����������
EventLoop initEventLoop();

// ѭ������
void loop(EventLoop eventLoop, Runtime runtime);

// ��ͨ����
void once(EventLoop eventLoop, Runtime runtime);

// �����ͨ����
void nextTick(EventLoop eventLoop, Task task, let args);

// ��Ӷ�ʱִ�е�����
Runtime setTimeout(EventLoop eventLoop, Task task, let args, int millisec);

// ��Ӷ�ʱѭ��ִ�е�����
Runtime setInterval(EventLoop eventLoop, Task task, let args, int millisec);

// �����ʱ����
void clearTimer(Runtime runtime);

// ��ʼִ���������
void beginDealTaskQueue(EventLoop runtimeQueue);

// ִ�ж�ʱ����
void runTimerTask(EventLoop eventLoop);

// ִ��io����
void runIoTask(EventLoop eventLoop);

#endif // !__TASK_H__