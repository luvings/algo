#ifndef  __FS_H__
#define __FS_H__

#include<Windows.h>
#include<pthread.h>
#include"io.h"
#include"events.h"

enum _FsConstants {
	F_OK = 0,	// 文件可见
	W_OK = 2,	// 文件可读
	R_OK = 4,	// 文件可写
	X_OK = 6	// 文件可执行 
};

typedef enum _FsConstants FsConstants;

typedef struct _WIN32_FILE_ATTRIBUTE_DATA *FileAttributes;
#define newFileAttributes() NEW(struct _WIN32_FILE_ATTRIBUTE_DATA)

typedef struct watch * Watch;

typedef void(*WatchFileCallback)(Watch watch);
typedef void(*WatchFile)(string path, WatchFileCallback callback);

typedef void(*AccessFileCallback)(string error, Boolean result);
typedef void(*AccessFile)(string path, AccessFileCallback callback);

typedef struct watch {
	string id;
	string path;
	FileAttributes fileAttributes;
	WatchFileCallback onChange;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
}*Watch;

#define newWatch() NEW(struct watch)

typedef struct fs{
	WatchFile watchFile;
}*Fs;
#define newFs() NEW(struct fs)

static void watchFileTask(let args);
static void onFileChange(Event event, let args);

// 初始化IO服务进程数据
IoService initIoService(Array undealQueue, Array finishQueue);

// 进程退出时释放内存
void freeIoService();

// 处理IO完成结果
void dealFinishIOEvent(IoEvent event);

// 处理已完成的access事件
static void accessFileEvent(Event event, let args);

// 监听文件的变化
void watchFile(string path, WatchFileCallback callback, int interval);

// 验证文件是否存在
void accessFile(string path, FsConstants mode, AccessFileCallback callback);

#endif // ! __FS_H__
