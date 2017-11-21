/**
 * describe:SDFS文件接口类定义
 * create date:2016/5/19 11:52:10
 * author wangzhia
 */

#ifndef _SDFS_GDF_FILE_H_
#define _SDFS_GDF_FILE_H_

#include "sdfs.h"
#include "ifile.h"

using namespace std;

class SDFS_GDF_FILE : public GDF_FILE
{
public:
	SDFS_GDF_FILE (void);
	
	SDFS_GDF_FILE (const char *szFilePath, const char *szFileName);
	
	SDFS_GDF_FILE (const char *fullFileName);
	
	virtual ~SDFS_GDF_FILE (void);
	
	virtual int Open (const char *szMode);
	
	virtual int Close (void);
	
	virtual int Rename (const char *destFile);
	
	virtual int Remove (void);
	
	virtual size_t Write (const char *buffer, size_t size, size_t count);
	
	virtual size_t Read (void *p, size_t size, size_t count);
	
	int ReadLn (void *p, size_t size);
	
	virtual int Fflush (void);
	
	virtual void MakeFullName (char *szFullName);
	
	virtual bool IsFileExist (void);
	
	virtual bool IsFilePointerNull (void);
	
	virtual int getFileList(char *indir);
	
	virtual bool is_dir(mode_t t);
	
	virtual bool is_reg(mode_t t);
	
	virtual int getLines();
	
	virtual char* Gets (char *p, int size);
	
	virtual int Seek (long offset,int where);
	
	virtual bool IsFileEnd(void);
	
	virtual int getFileList(char *indir,int num);
	
	virtual int Copy(const char *src_file, const char *des_file);
	
	virtual int Move(const char *src_file, const char *des_file);
	
	virtual int Truncate (char *filename,int num);
	
private:
	SFILE* m_Fp;
	
	SDFS sp;
	
};

#endif
