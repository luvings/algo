#include "task.h"

static IoService ioService = NULL;

int startTimeEqual(Runtime runtime1, Runtime runtime2) {
	return EQUAL(runtime1->startTime, runtime2->startTime);
}

EventLoop initEventLoop() {
	EventLoop eventLoop = newEventLoop();
	if (eventLoop == NULL) {
		return NULL;
	}
	eventLoop->undealIoQueue = createArray(0);
	eventLoop->finishIoQueue = createArray(0);
	eventLoop->heap = createHeap(startTimeEqual, MinHeap);
	eventLoop->wait = 0;
	eventLoop->timeout = 0;
	ioService = initIoService(eventLoop->undealIoQueue, eventLoop->finishIoQueue);
	return eventLoop;
}

void nextTick(EventLoop eventLoop, Task task, let args) {
	Runtime runtime = newRunTime();
	if (runtime == NULL) {
		return;
	}
	runtime->status = QUEUE;
	runtime->type = ONCE;
	runtime->task = task;
	runtime->start = once;
	runtime->startTime = clock();
	runtime->millisec = 0;
	runtime->args = args;
	eventLoop->heap->insert(eventLoop->heap, runtime);
}

void beginDealTaskQueue(EventLoop eventLoop) {
	while (!isEmpty(eventLoop)) {
		eventLoop->timeout = clock();

		// 执行定时任务
		runTimerTask(eventLoop);

		// 执行io任务
		runIoTask(eventLoop);
	}

	freeIoService();

	// 没有任务后释放内存，退出程序
	freeArray(&eventLoop->undealIoQueue);
	freeArray(&eventLoop->finishIoQueue);
	freeHeap(&eventLoop->heap);
	free(eventLoop);
}

void runTimerTask(EventLoop eventLoop) {
	Array undealQueue = createArray(0);
	while (eventLoop->heap->size != 0) {
		Runtime runtime = (Runtime)eventLoop->heap->list[0].data;
		if (runtime->startTime > eventLoop->timeout) break;

		// 收集已超时的任务
		runtime = eventLoop->heap->remove(eventLoop->heap);
		undealQueue->push(undealQueue, runtime);
	}
	while (undealQueue->length != 0) {
		Runtime runtime = undealQueue->shift(undealQueue);
		runTask(runtime, eventLoop);
	}

	if (eventLoop->heap->size != 0) {
		Runtime runtime = (Runtime)eventLoop->heap->list[0].data;
		if (runtime->startTime > eventLoop->timeout) {
			eventLoop->wait = runtime->startTime - eventLoop->timeout;
		}
		else {
			eventLoop->wait = 0;
		}
	}

	freeArray(&undealQueue);
}

void runIoTask(EventLoop eventLoop) {
	if (eventLoop->undealIoQueue->length != 0) {
		struct timeval now;
		struct timespec outtime;

		gettimeofday(&now, NULL);
		outtime.tv_sec = now.tv_sec + eventLoop->wait;
		outtime.tv_nsec = now.tv_usec * 1000;

        // 一直等到返回io结果或者等待超时
		pthread_mutex_lock(&(ioService->serviceThreadInfo->waitTimeout));
		pthread_cond_timedwait(&(ioService->serviceThreadInfo->waitTimeoutCond), &(ioService->serviceThreadInfo->waitTimeout), &outtime);
		pthread_mutex_unlock(&(ioService->serviceThreadInfo->waitTimeout));
	}
	else {
		// 没有待执行的任务直接休眠
		_sleep(eventLoop->wait);
	}

	// 处理所有已完成的IO任务
	while (eventLoop->finishIoQueue->length != 0) {
		pthread_mutex_lock(&(ioService->serviceThreadInfo->finish));
		IoEvent event = eventLoop->finishIoQueue->shift(eventLoop->finishIoQueue);
		pthread_mutex_unlock(&(ioService->serviceThreadInfo->finish));

		switch (event->ioType) {
		case IO: dealFinishIOEvent(event); break;
		case SocketIO: break;
		default: break;
		}
	}
}

Runtime setTimeout(EventLoop eventLoop, Task task, let args, int millisec) {
	Runtime runtime = newRunTime();
	if (runtime == NULL) {
		return NULL;
	}
	runtime->start = loop;
	runtime->status = QUEUE;
	runtime->type = ONCE;
	runtime->task = task;
	runtime->startTime = clock() + millisec;
	runtime->millisec = millisec;
	runtime->args = args;
	eventLoop->heap->insert(eventLoop->heap, runtime);
	return runtime;
}

Runtime setInterval(EventLoop eventLoop, Task task, let args, int millisec) {
	Runtime runtime = newRunTime();
	if (runtime == NULL) {
		return NULL;
	}
	runtime->start = loop;
	runtime->status = QUEUE;
	runtime->type = LOOP;
	runtime->task = task;
	runtime->millisec = RANGE(4, millisec, millisec);
	runtime->startTime = clock() + runtime->millisec;
	runtime->args = args;
	eventLoop->heap->insert(eventLoop->heap, runtime);
	return runtime;
}

void clearTimer(Runtime runtime)
{
	if (runtime != NULL) {
		runtime->status = DIE;
	}
}

void loop(EventLoop eventLoop, Runtime runtime) {
	if (runtime->status != DIE) {
		runtime->status = RUN;
		runtime->task(runtime->args);
	}
	if (runtime->type == LOOP && runtime->status != DIE) {
		// 如果是循环任务则需要再次入队列
		runtime->startTime += runtime->millisec;
		eventLoop->heap->insert(eventLoop->heap, runtime);
	}
	else {
		free(runtime);
	}
}

void once(EventLoop eventLoop, Runtime runtime) {
	runtime->status = RUN;
	runtime->task(runtime->args);
	free(runtime);
}