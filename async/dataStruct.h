#ifndef  __DATASTRUCT__H__
#define __DATASTRUCT__H__

#include"util.h"

// ʵ�����ݵĻ�������


// �ַ�������
#define string char *
#define newString(str) strcpy((char *)malloc(strlen(str) + 1), str)

// ��������
enum _Boolean { True = 1, False = 0 };
typedef enum _Boolean  Boolean;

#define let void *

#endif // ! __DATASTRUCT__H__
