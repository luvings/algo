#ifndef  __FS_H__
#define __FS_H__

#include<Windows.h>
#include<pthread.h>
#include"io.h"
#include"events.h"

enum _FsConstants {
	F_OK = 0,	// �ļ��ɼ�
	W_OK = 2,	// �ļ��ɶ�
	R_OK = 4,	// �ļ���д
	X_OK = 6	// �ļ���ִ�� 
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

// ��ʼ��IO�����������
IoService initIoService(Array undealQueue, Array finishQueue);

// �����˳�ʱ�ͷ��ڴ�
void freeIoService();

// ����IO��ɽ��
void dealFinishIOEvent(IoEvent event);

// ��������ɵ�access�¼�
static void accessFileEvent(Event event, let args);

// �����ļ��ı仯
void watchFile(string path, WatchFileCallback callback, int interval);

// ��֤�ļ��Ƿ����
void accessFile(string path, FsConstants mode, AccessFileCallback callback);

#endif // ! __FS_H__
