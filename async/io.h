#ifndef __IO_H__
#define __IO_H__

#include<signal.h>
#include<pthread.h>
#pragma comment(lib, "pthreadVC2.lib")

#include"array.h"

enum _IoType {
	IO, SocketIO
};

typedef enum _IoType IoType;

enum _eventType {
    AccessFileEvent
};

typedef enum _eventType EventType;

typedef struct ioEvent {
	string id;
	string path;
	let result;
	string error;
	let otherParams;
	let callback;
	IoType ioType;
    EventType eventType;
}*IoEvent;
#define newIoEvent() NEW(struct ioEvent)

typedef struct serviceThreadInfo {
	pthread_t pid;
	pthread_cond_t notifyCond;
	pthread_cond_t waitTimeoutCond;
	pthread_mutex_t waitTimeout;
	pthread_mutex_t notify;
	pthread_mutex_t undeal;
	pthread_mutex_t finish;
	pthread_mutex_t event;
}*ServiceThreadInfo;
#define newServiceThreadInfo() NEW(struct serviceThreadInfo)

typedef struct ioService {
	Array undealQueue;
	Array finishQueue;
	ServiceThreadInfo serviceThreadInfo;
}*IoService;
#define newIoService() NEW(struct ioService)

// 处理io事件队列
void runIO(let args);

void creatIoServicePthread(IoService service);

#endif // !__IO_H__
