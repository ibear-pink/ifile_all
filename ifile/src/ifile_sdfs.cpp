/**
 * describe:SDFS文件接口函数
 * create date:2016/5/19 12:01:26
 * author wangzhia
 */

#include "ifile_sdfs.h"

int ifile_sdfs_init(SDFS *ssp)
{
	char err_msg[256];
	memset(ssp, 0, sizeof(SDFS));
	if (0 != sinit ("h", ssp, err_msg))
	{
		printf("SDFS start failed[%s]...\n",err_msg);
		sdestory(ssp);
		return -1;
	}
	return 0;
}

void ifile_sdfs_destory(SDFS *ssp)
{
	sdestory(ssp);
}


SDFS_GDF_FILE::SDFS_GDF_FILE (SDFS *ssp)
{
	memset (m_FileName, 0, sizeof(m_FileName));
	
	memset (m_FilePath, 0, sizeof(m_FilePath));
	
	memset (m_FullFileName, 0, sizeof(m_FullFileName));
	
	m_Fp = NULL;
	
	memcpy( &sp, ssp, sizeof(SDFS));
}

SDFS_GDF_FILE::SDFS_GDF_FILE (SDFS *ssp,const char *fullFileName)
{
	memset (m_FileName, 0, sizeof(m_FileName));
	
	memset (m_FilePath, 0, sizeof(m_FilePath));
	
	memset (m_FullFileName, 0, sizeof(m_FullFileName));
	
	m_Fp = NULL;

	strcpy (m_FullFileName, fullFileName);
	int DirLen = strlen(fullFileName);
	
	for (int i = DirLen - 1; i >= 0; i--)
	{
		if (fullFileName[i] == '/')
		{
			strcpy (m_FileName, m_FullFileName + i);
			strncpy (m_FilePath, m_FullFileName ,i);
			break;
		}
	}	
	m_StartTime = time(NULL);
	memcpy( &sp, ssp, sizeof(SDFS));
}

SDFS_GDF_FILE::SDFS_GDF_FILE (SDFS *ssp, const char *szFilePath, const char *szFileName)
{
	memset (m_FileName, 0, sizeof(m_FileName));
	
	memset (m_FilePath, 0, sizeof(m_FilePath));
	
	memset (m_FullFileName, 0, sizeof(m_FullFileName));
	
	m_Fp = NULL;
	
	strcpy (m_FileName, szFileName);
	
	strcpy (m_FilePath, szFilePath);
	
	strcpy (m_FullFileName, m_FilePath);
	
	int DirLen = strlen(m_FilePath);
	
	if (m_FilePath[DirLen - 1] == '/')
	{
		strcpy (m_FullFileName + DirLen, m_FileName);
	}
	else
	{
		m_FullFileName[DirLen] = '/';
		strcpy (m_FullFileName + DirLen + 1, m_FileName);
	}
	
	m_StartTime = time(NULL);
	memcpy( &sp, ssp, sizeof(SDFS));
}

SDFS_GDF_FILE::~SDFS_GDF_FILE (void)
{
	if (NULL != m_Fp)
	{
		this->Close ();
	}
}

int SDFS_GDF_FILE::Open (const char *szMode)
{
#ifdef _DEBUG_INFO_
	printf ("m_FullFileName = %s\n", m_FullFileName);
#endif
	m_Fp = sopen (m_FullFileName, szMode, &sp);
	if (NULL == m_Fp)
	{
		printf ("FILE[%s]LINE[%d]ERR_MSG[File(%s) Pointer(%s) Is Null.]\n", __FILE__, __LINE__,m_FullFileName,szMode);
		printf ("cp:szInfile sopen failed... errstr[%s] errno[%d]\n",serrstr2(&sp),serrno2(&sp));
		return -1;
	}
	
	return 0;
}

size_t SDFS_GDF_FILE::Read (void *p, size_t size, size_t count)
{
	if (this->m_Fp)
		return sread (p, size, count, this->m_Fp);
	else
		return 0;
}

int SDFS_GDF_FILE::ReadLn (void *p, size_t size)
{
	if (this->m_Fp)
		return sreadln (p, size, this->m_Fp);
	else
		return 0;
}

size_t SDFS_GDF_FILE::Write (const char *buffer, size_t size, size_t count)
{
	if (this->m_Fp)
		return swrite (buffer, size, count, this->m_Fp);
	else
		return 0;
}

int SDFS_GDF_FILE::getFileList(char *indir)
{
	SDIR* dir;
	sdirent* ptr;
	char fullFileName[1024] = {0};
	stFileInfo *pNode = NULL;
	struct sstat stStatBuf;
	
	dir = sopendir(indir,&sp);
	while ((ptr = sreaddir(dir,&sp)) != NULL)
	{
		memset (fullFileName, 0, sizeof(fullFileName));
		//指定入口文件目录
		sprintf(fullFileName, "%s/%s", indir, ptr->d_name);
		if (sstat(fullFileName, &stStatBuf, &sp) == -1)
		{
			continue;
		}
		
		if(SS_ISDIR(stStatBuf.st_mode))	
		{
			if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			{
				continue;
			}
		}
		
		if(SS_ISREG(stStatBuf.st_mode))	
		{
			pNode = new stFileInfo;
			if (NULL == pNode)
			{
				printf("[%s][%d]new stFileInfo is failed.\n",__FILE__,__LINE__, ptr->d_name);
				continue;
			}
			
			memset (pNode, 0, sizeof(stFileInfo));
			
			strcpy (pNode->sFileName, ptr->d_name);
			
			struct tm tDateTime;
			
			time_t lSeconds;
			time( &lSeconds );
			localtime_r( &lSeconds, &tDateTime );
			
			sprintf (pNode->sModifyTime, "%04d%02d%02d%02d%02d%02d", tDateTime.tm_year + 1900, tDateTime.tm_mon + 1, tDateTime.tm_mday,tDateTime.tm_hour, tDateTime.tm_min, tDateTime.tm_sec);
			
#ifdef _DEBUG_INFO_
	printf ("file_name|%s|modify_time|%s\n", pNode->sFileName, pNode->sModifyTime);
#endif
			mFileList.push_back (pNode);
		}
	}
	sclosedir(dir,&sp);
	
	return 0;
}

int SDFS_GDF_FILE::Close (void)
{
	if ((this->mFileList).size()>0)
	{
		for(vector<stFileInfo *>::iterator it = (this->mFileList).begin();it!=(this->mFileList).end();it++)
		{
			stFileInfo *file = *it;
			delete file;
			file = NULL;
		}
		(this->mFileList).clear();
	}
	if (0 != sclose (m_Fp))
	{
		printf ("cp:szInfile sopen failed... errstr[%s] errno[%d]\n",serrstr2(&sp),serrno2(&sp));
		return -1;
	}
	
	m_Fp = NULL;
	return 0;
}

int SDFS_GDF_FILE::Rename (const char *destFile)
{	
	struct sstat buf;
	sstat (m_FullFileName, &buf, &sp);
	if (buf.st_size > 0)
	{
		if (0 != srename (m_FullFileName, destFile, &sp))
		{
			return -1;
		}
	}
	
	return 0;
}

void SDFS_GDF_FILE::MakeFullName (char *szFullName)
{
	strcpy (szFullName, m_FilePath);

	int DirLen = strlen(m_FilePath);
	
	if (m_FilePath[DirLen - 1] == '/')
	{
		strcpy (szFullName + DirLen, m_FileName);
	}
	else
	{
		szFullName[DirLen] = '/';
		strcpy (szFullName + DirLen + 1, m_FileName);
	}
}

int SDFS_GDF_FILE::Remove (void)
{	
	if (0 != sremove (m_FullFileName, &sp))
	{
		return -1;
	}
	
	return 0;
}

int SDFS_GDF_FILE::Fflush (void)
{
	if (this->m_Fp)
	{
		return sflush (this->m_Fp);
	}
	else
	{
		return 0;
	}
}

bool SDFS_GDF_FILE::IsFileExist ()
{	
	struct sstat statMode;

	if ((sstat(m_FullFileName, &statMode, &sp) == 0))
	{
		return false;
	}

	if (saccess(m_FullFileName, R_OK|W_OK, &sp) < 0)
	{
		return false;
	}

	return true;
}

bool SDFS_GDF_FILE::IsFilePointerNull (void)
{
	if (NULL == m_Fp)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int SDFS_GDF_FILE::Copy(const char *src_file, const char *des_file)
{
	SFILE* des_f = NULL;
	SFILE* src_f = NULL;
	int len=0;
	char buf[BEST_BUF + 4];

	if(!strcmp(src_file,des_file))
		return 0;

	if((src_f=sopen(src_file,"rb",&sp))==NULL)
	{
		printf("Can not open file -- %s.\n",src_file);
		return 1;
	}
	if((des_f=sopen(des_file,"wb",&sp))==NULL)
	{
		printf("Can not open file -- %s.\n",src_file);
		sclose(src_f);
		return 1;
	}

	while(src_f != NULL)
	{
		memset(buf,0,sizeof(buf));
		sread(&buf, BEST_BUF,1,src_f);
		len = strlen(buf);
		if(len<1)
			continue;
		if(swrite(&buf,len,1,des_f)!=1)
		{
			printf("Copy file |%s| to |%s| error!\n",src_file,des_file);
			sclose(des_f);
			sclose(src_f);
			return 1;
		}
	} 

	sclose(des_f);
	sclose(src_f);
	return 0;
}

int SDFS_GDF_FILE::Move(const char *src_file, const char *des_file)
{
	if(strcmp(src_file,des_file)==0)
		return 0;

	if( srename(src_file,des_file,&sp) != 0 )
	{
		if(this->Copy(src_file,des_file))
		{
			return 1;
		}
		sremove(src_file,&sp);
	}
	
	return 0;
}

int SDFS_GDF_FILE::getLines()
{
	int linect = 0;
	char p[255] = {0};
	
	sgets (p, sizeof(p), this->m_Fp);
	while(!seof(this->m_Fp))
	{
		linect++;
		sgets (p, sizeof(p), this->m_Fp);
	}
	
	return linect;
}
bool SDFS_GDF_FILE::IsFileEnd()
{
	return seof(this->m_Fp);
}

int SDFS_GDF_FILE::getFileList(char *indir,int num)
{
	SDIR* dir;
	sdirent* ptr;
	int i = 0;
	char fullFileName[1024] = {0};
	stFileInfo *pNode = NULL;
	struct sstat stStatBuf;
	dir = sopendir(indir, &sp);
	while ((ptr = sreaddir(dir, &sp)) != NULL)
	{
		memset (fullFileName, 0, sizeof(fullFileName));
		//指定入口文件目录
		sprintf(fullFileName, "%s/%s", indir, ptr->d_name);
		if (sstat(fullFileName, &stStatBuf, &sp) == -1)
		{
			continue;
		}
		if(SS_ISDIR(stStatBuf.st_mode))
		{
			if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			{
				continue;
			}
		}
		
		if(SS_ISREG(stStatBuf.st_mode))
		{
			pNode = new stFileInfo;
			if (NULL == pNode)
			{
				printf("[%s][%d]new stFileInfo is failed.\n",__FILE__,__LINE__, ptr->d_name);
				continue;
			}
			
			strcpy (pNode->sFileName, ptr->d_name);
			mFileList.push_back (pNode);
			i++;
		}
		if (i >= num)
		{
			break;
		}
	}
	sclosedir(dir, &sp);
	
	return 0;
}
int SDFS_GDF_FILE::Seek (long offset,int where)
{
	return sseek(this->m_Fp,offset,where);
}
char* SDFS_GDF_FILE::Gets (char *p, int size)
{
	return sgets (p, size, this->m_Fp);
}
bool SDFS_GDF_FILE::is_dir(mode_t t)
{
	return SS_ISDIR(t);
}
bool SDFS_GDF_FILE::is_reg(mode_t t)
{
	return SS_ISREG(t);
}

int SDFS_GDF_FILE::Truncate(char *filename,int num)
{
	//待实现
	return 0;
}



