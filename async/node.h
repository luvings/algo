#ifndef __NODE_H__
#define __NODE_H__

#include"task.h"
#include"events.h"

// �����ͨ����
void NextTick(Task task, let args);

// �����ʱִ�е�����
Runtime SetTimeout(Task task, let args, int millisec);

// �����ʱѭ��ִ�е�����
Runtime SetInterval(Task task, let args, int millisec);

// ��ʼִ���������
void BeginDealTaskQueue();

// ��ӵ����¼�����(�¼�����ͷ��)
void OnEvent(char * name, Listener listener, let args);

// ��ӵ����¼�����(�¼�����β��)
void OnEventOnce(char * name, Listener listener, let args);

//  ����¼�����(�¼�����ͷ��)
void OnEventPrependListener(char * name, Listener listener, let args);

// ��ӵ����¼�����(�¼�����ͷ��)
void OnEventPrependOnceListener(char * name, Listener listener, let args);

// �����¼�
void EmitEvent(char * name);

// �Ƴ��¼�
void RemoveListenerEvent(char * name);

// ��ʼ������ʱ
void initRunTime();

// �������ѭ���ṹ
EventLoop getEventLoop();

#endif // !__NODE_H__
