#include"node.h"
#include"fs.h"
//#include<vld.h>

void callback(string error, Boolean result) {
    printf("result: %d\n", result);
}

void task(int x) {
	accessFile("E:/桌面/1.txt", W_OK | R_OK, callback);
}

int main() {
	initRunTime();
    SetInterval(task, "asdfas", 100);
	BeginDealTaskQueue();
    return 0;
}