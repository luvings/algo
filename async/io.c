#include "io.h"

void creatIoServicePthread(IoService service) {
	service->serviceThreadInfo = newServiceThreadInfo();
	if (service->serviceThreadInfo == NULL) {
		return;
	}
	pthread_cond_init(&(service->serviceThreadInfo->notifyCond), NULL);
	pthread_cond_init(&(service->serviceThreadInfo->waitTimeoutCond), NULL);
	pthread_mutex_init(&(service->serviceThreadInfo->notify), NULL);
	pthread_mutex_init(&(service->serviceThreadInfo->undeal), NULL);
	pthread_mutex_init(&(service->serviceThreadInfo->finish), NULL);
	pthread_mutex_init(&(service->serviceThreadInfo->waitTimeout), NULL);
	pthread_mutex_init(&(service->serviceThreadInfo->event), NULL);
	pthread_create(&(service->serviceThreadInfo->pid), NULL, runIO, service);
}

void runIO(let args) {

	// 允许退出线程
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	// 异步取消，线程接到取消信号后立即退出
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	IoService service = (IoService)args;
	puts("io server start ....");
	while (True) {
        // 检查取消点
        pthread_testcancel();
		if (service->undealQueue->length != 0) {
            // 从待处理中取出任务
			pthread_mutex_lock(&(service->serviceThreadInfo->undeal));
			IoEvent ioEvent = (IoEvent)service->undealQueue->shift(service->undealQueue);
			pthread_mutex_unlock(&(service->serviceThreadInfo->undeal));

			// 处理io事件
			_sleep(100);
            switch (ioEvent->eventType) {
            case AccessFileEvent:
                ioEvent->result = access(ioEvent->path, ioEvent->otherParams) == 0 ? True : False;
                ioEvent->error = NULL;
                break;
            default: break;
            }

			pthread_mutex_lock(&(service->serviceThreadInfo->waitTimeout));
			pthread_cond_signal(&(service->serviceThreadInfo->waitTimeoutCond), &(service->serviceThreadInfo->waitTimeout));
			pthread_mutex_unlock(&(service->serviceThreadInfo->waitTimeout));

            // 已完成的任务放到已完成队列中
			pthread_mutex_lock(&(service->serviceThreadInfo->finish));
			service->finishQueue->push(service->finishQueue, ioEvent);
			pthread_mutex_unlock(&(service->serviceThreadInfo->finish));
		}
		else {
			// 没有待处理任务，则等待主线程唤醒
			pthread_mutex_lock(&(service->serviceThreadInfo->notify));
			pthread_cond_wait(&(service->serviceThreadInfo->notifyCond), &(service->serviceThreadInfo->notify));
			pthread_mutex_unlock(&(service->serviceThreadInfo->notify));
		}
	}
}