//
//  ifile.hpp
//  ibf
//
//  Created by wangzhia on 2017/5/17.
//  Copyright 2017 wangzhia. All rights reserved.
//
#ifndef ifile_hpp
#define ifile_hpp

#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include "ifile_defile.h"

using namespace std;

class GDF_FILE
{
public:
	GDF_FILE ();
	
	virtual ~GDF_FILE (void) = 0;
	
	char *GetFileName () {return m_FileName;}
	
	char *GetFullFileName () {return m_FullFileName;}
	
	void SetFileStartTime () {m_StartTime = time(NULL);}
	
	time_t GetFileStartTime () {return m_StartTime;}
	
public:
	virtual int Open (const char *szMode) = 0;
	
	virtual int Close (void) = 0;
	
	virtual size_t Write (const char *buffer, size_t size, size_t count) = 0;
	
	virtual size_t Read (void *p, size_t size, size_t count) = 0;
	
	//块读块写(文本文件)
	virtual char* Gets (char *p, int size) = 0;
	
	//SEEK_SET： 文件开头	SEEK_CUR： 当前位置	　　SEEK_END： 文件结尾
	virtual int Seek (long offset,int where) = 0;
	
	virtual int Fflush (void) = 0;
	
	virtual void MakeFullName (char *szFullName) = 0;
	
	virtual bool IsFileExist (void) = 0;
	
	virtual bool IsFilePointerNull (void) = 0;
	
	//对文件按时间排序
	virtual int getFileList(char *indir) = 0;
	
	//不排序,可选加载前几个文件
	virtual int getFileList(char *indir,int num) = 0;
	
	virtual bool is_dir(mode_t t) = 0;
	
	virtual bool is_reg(mode_t t) = 0;
	
	virtual int getLines() = 0;
	
	virtual bool IsFileEnd(void) = 0;
	
	virtual int Copy(const char *src_file, const char *des_file) = 0;
	
	virtual int Move(const char *src_file, const char *des_file) = 0;
	
	virtual int Rename (const char *destFile) = 0;
	
	virtual int Remove (void) = 0;
	
	virtual int Truncate (char *filename,int num) = 0;
	
public:
	vector<stFileInfo *> mFileList;
	
protected:
	
	char m_FileName[512];
	
	char m_FilePath[512];
	
	char m_FullFileName[1024];
	
	time_t m_StartTime;
	
};

#endif /* ifile_hpp */
