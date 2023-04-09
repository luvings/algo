#ifndef __NODE_H__
#define __NODE_H__

#include"task.h"
#include"events.h"

// 添加普通任务
void NextTick(Task task, let args);

// 添加延时执行的任务
Runtime SetTimeout(Task task, let args, int millisec);

// 添加延时循环执行的任务
Runtime SetInterval(Task task, let args, int millisec);

// 开始执行任务队列
void BeginDealTaskQueue();

// 添加单次事件监听(事件队列头部)
void OnEvent(char * name, Listener listener, let args);

// 添加单次事件监听(事件队列尾部)
void OnEventOnce(char * name, Listener listener, let args);

//  添加事件监听(事件队列头部)
void OnEventPrependListener(char * name, Listener listener, let args);

// 添加单次事件监听(事件队列头部)
void OnEventPrependOnceListener(char * name, Listener listener, let args);

// 触发事件
void EmitEvent(char * name);

// 移除事件
void RemoveListenerEvent(char * name);

// 初始化运行时
void initRunTime();

// 获得任务循环结构
EventLoop getEventLoop();

#endif // !__NODE_H__
