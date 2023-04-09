#ifndef __TASK_H__
#define __TASK_H__

#include<stdio.h>
#include<time.h>

#include"heap.h"
#include"fs.h"

// 任务状态
enum _Status {
	QUEUE,	// 队列中
	RUN,	// 运行中
	WAIT,	// 等待（延时）执行
	DIE		// 已被杀死
};

// 任务类型
enum _Type{
	LOOP,
	ONCE
};

typedef enum _Type Type;
typedef enum _Status Status;

#define newRunTime() NEW(struct runtime)

// 任务函数类型
typedef void(*Task)(let args);

typedef struct eventLoop * EventLoop;

typedef struct runtime * Runtime;

// 启动任务函数类型
typedef void(*Start)(EventLoop eventLoop, Runtime runtime);

typedef struct runtime{
	// 用户任务
	Task task;
	
	// 执行状态
	Status status;

	// 任务类型
	Type type;

	// 任务启动函数
	Start start;

	// 任务创建时间戳
	unsigned long startTime;

	// 延时时间
	int millisec;

	// 回调传参
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

// 执行任务
#define runTask(runtime, eventLoop) (runtime->start)(eventLoop, runtime)

// 任务队列是否为空
#define isEmpty(eventLoop) (eventLoop->undealIoQueue->length == 0 \
	&& eventLoop->finishIoQueue->length == 0 && eventLoop->heap->size == 0)

// 创建任务队列
EventLoop initEventLoop();

// 循环任务
void loop(EventLoop eventLoop, Runtime runtime);

// 普通任务
void once(EventLoop eventLoop, Runtime runtime);

// 添加普通任务
void nextTick(EventLoop eventLoop, Task task, let args);

// 添加定时执行的任务
Runtime setTimeout(EventLoop eventLoop, Task task, let args, int millisec);

// 添加定时循环执行的任务
Runtime setInterval(EventLoop eventLoop, Task task, let args, int millisec);

// 清除定时任务
void clearTimer(Runtime runtime);

// 开始执行任务队列
void beginDealTaskQueue(EventLoop runtimeQueue);

// 执行定时任务
void runTimerTask(EventLoop eventLoop);

// 执行io任务
void runIoTask(EventLoop eventLoop);

#endif // !__TASK_H__