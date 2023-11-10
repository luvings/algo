#ifndef  __EVENTS_H__
#define __EVENTS_H__

#include"hashMap.h"
#include"array.h"

enum _EventType {
	MoreEvent, OnceEvent
};

static char pub[300] = "123456789";

typedef enum _EventType EventType;

typedef struct event *Event;
typedef struct emitter *Emitter;

typedef void(*Listener)(Event event, let args);
typedef void(*On)(Emitter emitter, string eventName, Listener listener, let args);
typedef void(*Emit)(Emitter emitter, string eventName);
typedef void(*Once)(Emitter emitter, string eventName, Listener listener, let args);
typedef void(*PrependListener)(Emitter emitter, string eventName, Listener listener, let args);
typedef void(*RemoveListener)(Emitter emitter, string eventName);
typedef void(*PrependOnceListener)(Emitter emitter, string eventName, Listener listener, let args);
typedef Array(*GetEventListener)(Emitter emitter, string eventName);

typedef struct emitter {
	HashMap hashMap;	// <string, Event>
	On on;				// ����¼�(�¼�����β��)
	Emit emit;			// �����¼�
	Once once;			// ��ӵ����¼�(�¼�����β��)
	PrependListener prependListener;			// ����¼�(�¼�����ͷ��)
	RemoveListener removeListener;				// ����¼���
	PrependOnceListener prependOnceListener;	// ��ӵ����¼�(�¼�����ͷ��)
    GetEventListener getEventListener;			// ��ȡ�¼��󶨵Ķ���
}*Emitter;

typedef struct event {
	Listener listener;
	EventType type;
	string eventName;
	let args;
}*Event;

#define newEvent() NEW(struct event)

#define newEmitter() NEW(struct emitter)

// �����¼�
Emitter createEmitter();

void freeEmitter(Emitter * emitter);

// ����¼�����(�¼�����β��)
static void on(Emitter emitter, string eventName, Listener listener, let args);

// ��ӵ����¼�����(�¼�����β��)
static void once(Emitter emitter, string eventName, Listener listener, let args);

// �����¼�
static void emit(Emitter emitter, string eventName);

//  ����¼�����(�¼�����ͷ��)
static void prependListener(Emitter emitter, string eventName, Listener listener, let args);

// ��ӵ����¼�����(�¼�����ͷ��)
static void prependOnceListener(Emitter emitter, string eventName, Listener listener, let args);

// �Ƴ��¼�
static void removeListener(Emitter emitter, string eventName);

// ��ȡ�¼��󶨵Ķ���
Array getEventListener(Emitter emitter, string eventName);

// ���״̬
void dispalyEmitter(Emitter emitter);

#endif // ! __EVENTS_H__
