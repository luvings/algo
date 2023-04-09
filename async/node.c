#include"node.h"

static Emitter emitter = NULL;
static EventLoop eventLoop = NULL;

void initRunTime() {
	emitter = createEmitter();
	eventLoop = initEventLoop();
}

EventLoop getEventLoop() {
	return eventLoop;
}

void NextTick(Task task, let args) {
	nextTick(eventLoop, task, args);
}

Runtime SetTimeout(Task task, let args, int millisec) {
	return setTimeout(eventLoop, task, args, millisec);
}

Runtime SetInterval(Task task, let args, int millisec) {
	return setInterval(eventLoop, task, args, millisec);
}

void BeginDealTaskQueue() {
	beginDealTaskQueue(eventLoop);
	freeEmitter(&emitter);
}

void OnEvent(char * eventName, Listener listener, let args) {
	emitter->on(emitter, eventName, listener, args);
}

void OnEventOnce(char * eventName, Listener listener, let args) {
	emitter->once(emitter, eventName, listener, args);
}

void EmitEvent(char * eventName) {
	emitter->emit(emitter, eventName);
}

void OnEventPrependListener(char * eventName, Listener listener, let args) {
	emitter->prependListener(emitter, eventName, listener, args);
}

void OnEventPrependOnceListener(char * eventName, Listener listener, let args) {
	emitter->prependOnceListener(emitter, eventName, listener, args);
}

void RemoveListenerEvent(char * eventName) {
	emitter->removeListener(emitter, eventName);
}