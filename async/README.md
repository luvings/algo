# async

#### 1. 介绍
C语言单线程异步操作库，极大的提高异步执行效率
``
https://gitee.com/lyxfj/async.git
```


#### 2. 安装教程

1. 开发使用版本为vs2017
3. 运行main.c

#### 3. 测试实例

```c
#include"task.h"

#define log(str) printf(str);printf("\n")

void task1(Queue taskQueue);
void task2(Queue taskQueue);
void task3(Queue taskQueue);
void task4(Queue taskQueue);
void task5(Queue taskQueue);

int main() {
	Queue taskQueue = createTaskQueue();
	nextTick(taskQueue, task1);
	nextTick(taskQueue, task2);
	nextTick(taskQueue, task3);
	nextTick(taskQueue, task4);
	nextTick(taskQueue, task5);
	beginDealTaskQueue(taskQueue);
	return 0;
}

void task1(Queue taskQueue) {
	log("is Task 1....");
}

void task2(Queue taskQueue) {
	log("is Task 2....");
	nextTick(taskQueue, task3);
}

void task3(Queue taskQueue) {
	log("is Task 3....");
	nextTick(taskQueue, task1);
}

void task4(Queue taskQueue) {
	log("is Task 4....");
	nextTick(taskQueue, task2);
}

void task5(Queue taskQueue) {
	log("is Task 5....");
}
```

##### 执行结果

```
is Task 1....
===========================
is Task 2....
===========================
is Task 3....
===========================
is Task 4....
===========================
is Task 5....
===========================
is Task 3....
===========================
is Task 1....
===========================
is Task 2....
===========================
is Task 1....
===========================
is Task 3....
===========================
is Task 1....
===========================
task queue end!
```
