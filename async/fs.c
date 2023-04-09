#include "fs.h"
static IoService service = NULL;
static Emitter emitter = NULL;

#define updateAttributesTime(watch) do{ \
	watch->ftCreationTime = watch->fileAttributes->ftCreationTime; \
	watch->ftLastAccessTime = watch->fileAttributes->ftLastAccessTime; \
	watch->ftLastWriteTime = watch->fileAttributes->ftLastWriteTime; \
}while(0)

IoService initIoService(Array undealQueue, Array finishQueue) {
	emitter = createEmitter();
	service = newIoService();
	if (service == NULL) {
		return NULL;
	}
	service->finishQueue = finishQueue;
	service->undealQueue = undealQueue;
	creatIoServicePthread(service);
	return service;
}

void freeIoService() {
	// 发出终止io线程请求
	pthread_cancel(service->serviceThreadInfo->pid);
    // 唤醒io线程，触发取消点
    pthread_cond_signal(&(service->serviceThreadInfo->notifyCond));
    // 等待结束
	printf("io线程exit code: %d\n", pthread_join(service->serviceThreadInfo->pid, NULL));

	free(service->serviceThreadInfo);
	free(service);

	freeEmitter(&emitter);
}

void dealFinishIOEvent(IoEvent event) {
	emitter->emit(emitter, event->id);
}

void pusuIoEvent(IoEvent event) {
	// 通知服务线程有新任务处理
	pthread_mutex_lock(&(service->serviceThreadInfo->undeal));
	service->undealQueue->push(service->undealQueue, event);
	pthread_mutex_unlock(&(service->serviceThreadInfo->undeal));

	pthread_mutex_lock(&(service->serviceThreadInfo->notify));
	pthread_cond_signal(&(service->serviceThreadInfo->notifyCond));
	pthread_mutex_unlock(&(service->serviceThreadInfo->notify));
}

void watchFileTask(let args) {
	Watch watch = (Watch)args;
	GetFileAttributesExA(watch->path, GetFileExInfoStandard, watch->fileAttributes);
	if (watch->fileAttributes->ftLastWriteTime.dwHighDateTime != watch->ftLastWriteTime.dwHighDateTime
		|| watch->fileAttributes->ftLastWriteTime.dwLowDateTime != watch->ftLastWriteTime.dwLowDateTime) {
		EmitEvent(watch->id);
	}
	updateAttributesTime(watch);
}

void onFileChange(Event event, let args) {
	Watch watch = (Watch)args;
	watch->onChange(watch);
}

void watchFile(string path, WatchFileCallback callback, int interval) {
	Watch watch = newWatch();
	if (watch == NULL) {
		return watch;
	}
	watch->path = path;
	watch->onChange = callback;
	watch->fileAttributes = newFileAttributes();

	if (watch->fileAttributes == NULL) {
		return;
	}

	GetFileAttributesExA(path, GetFileExInfoStandard, watch->fileAttributes);

	updateAttributesTime(watch);

	watch->id = createUUID();
	OnEvent(watch->id, onFileChange, watch);
	SetInterval(watchFileTask, (let)watch, interval);
}

//void watchFile(string path, WatchFileCallback callback, int interval) {
//	IoEvent event = newIoEvent();
//	event->id = createUUID();
//	event->path = path;
//	event->callback = callback;
//	event->type = IO;
//	event->error = NULL;
//	event->result = NULL;
//	event->otherParams = (let)interval;
//
//	pthread_mutex_lock(&(service->serviceThreadInfo->event));
//	emitter->once(emitter, event->id, accessFileEvent, event);
//	pthread_mutex_unlock(&(service->serviceThreadInfo->event));
//
//	pusuIoEvent(event);
//}

void accessFile(string path, FsConstants mode, AccessFileCallback callback) {

	IoEvent event = newIoEvent();
	if (event == NULL) {
		return;
	}
	event->id = createUUID();
	event->path = path;
	event->callback = callback;
	event->ioType = IO;
    event->eventType = AccessFileEvent;
	event->error = NULL;
	event->result = NULL;
	event->otherParams = (let)mode;

	emitter->once(emitter, event->id, accessFileEvent, event);

	pusuIoEvent(event);
}

void accessFileEvent(Event event, let args) {
	IoEvent ioEvent = (IoEvent)args;
	AccessFileCallback callback = (AccessFileCallback)ioEvent->callback;
	callback(ioEvent->error, (Boolean)ioEvent->result);
	free(ioEvent->id);
	free(ioEvent);
}

void watchFileEvent(Event event, let args) {

}