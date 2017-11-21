#ifndef _IBF_IFILE_DEFINE_H_
#define _IBF_IFILE_DEFINE_H_

#include <pthread.h>

#define BEST_BUF			8192

typedef struct
{
	char sFileName[128];
	char sModifyTime[14+1];
} stFileInfo;

#endif
