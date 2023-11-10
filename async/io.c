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

	// �����˳��߳�
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	// �첽ȡ�����߳̽ӵ�ȡ���źź������˳�
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	IoService service = (IoService)args;
	puts("io server start ....");
	while (True) {
        // ���ȡ����
        pthread_testcancel();
		if (service->undealQueue->length != 0) {
            // �Ӵ�������ȡ������
			pthread_mutex_lock(&(service->serviceThreadInfo->undeal));
			IoEvent ioEvent = (IoEvent)service->undealQueue->shift(service->undealQueue);
			pthread_mutex_unlock(&(service->serviceThreadInfo->undeal));

			// ����io�¼�
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

            // ����ɵ�����ŵ�����ɶ�����
			pthread_mutex_lock(&(service->serviceThreadInfo->finish));
			service->finishQueue->push(service->finishQueue, ioEvent);
			pthread_mutex_unlock(&(service->serviceThreadInfo->finish));
		}
		else {
			// û�д�����������ȴ����̻߳���
			pthread_mutex_lock(&(service->serviceThreadInfo->notify));
			pthread_cond_wait(&(service->serviceThreadInfo->notifyCond), &(service->serviceThreadInfo->notify));
			pthread_mutex_unlock(&(service->serviceThreadInfo->notify));
		}
	}
}