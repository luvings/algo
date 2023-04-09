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
	On on;				// 添加事件(事件队列尾部)
	Emit emit;			// 触发事件
	Once once;			// 添加单次事件(事件队列尾部)
	PrependListener prependListener;			// 添加事件(事件队列头部)
	RemoveListener removeListener;				// 解除事件绑定
	PrependOnceListener prependOnceListener;	// 添加单次事件(事件队列头部)
    GetEventListener getEventListener;			// 获取事件绑定的队列
}*Emitter;

typedef struct event {
	Listener listener;
	EventType type;
	string eventName;
	let args;
}*Event;

#define newEvent() NEW(struct event)

#define newEmitter() NEW(struct emitter)

// 创建事件
Emitter createEmitter();

void freeEmitter(Emitter * emitter);

// 添加事件监听(事件队列尾部)
static void on(Emitter emitter, string eventName, Listener listener, let args);

// 添加单次事件监听(事件队列尾部)
static void once(Emitter emitter, string eventName, Listener listener, let args);

// 触发事件
static void emit(Emitter emitter, string eventName);

//  添加事件监听(事件队列头部)
static void prependListener(Emitter emitter, string eventName, Listener listener, let args);

// 添加单次事件监听(事件队列头部)
static void prependOnceListener(Emitter emitter, string eventName, Listener listener, let args);

// 移除事件
static void removeListener(Emitter emitter, string eventName);

// 获取事件绑定的队列
Array getEventListener(Emitter emitter, string eventName);

// 检测状态
void dispalyEmitter(Emitter emitter);

#endif // ! __EVENTS_H__
