/**
 * describe:文件接口函数
 * create date:2017-01-11
 * author wangzhia
 */

#include "ifile_system.h"

SYSTEM_GDF_FILE::SYSTEM_GDF_FILE ()
{
	memset (m_FileName, 0, sizeof(m_FileName));
	
	memset (m_FilePath, 0, sizeof(m_FilePath));
	
	memset (m_FullFileName, 0, sizeof(m_FullFileName));
	
	m_Fp = NULL;
}

SYSTEM_GDF_FILE::SYSTEM_GDF_FILE (const char *fullFileName)
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
	printf("%s,%s,%s\n",m_FileName,m_FilePath,m_FullFileName);
	
	m_StartTime = time(NULL);
}

SYSTEM_GDF_FILE::SYSTEM_GDF_FILE (const char *szFilePath, const char *szFileName)
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
}

SYSTEM_GDF_FILE::~SYSTEM_GDF_FILE ()
{
	if (NULL != m_Fp)
	{
		this->Close ();
	}
}

bool SYSTEM_GDF_FILE::IsFileExist ()
{
	struct stat statMode;
	
	if ((lstat(m_FullFileName, &statMode) == 0))
	{
		return false;
	}
	
	if (access(m_FullFileName, R_OK|W_OK) < 0)
	{
		return false;
	}
	
	return true;
}

bool SYSTEM_GDF_FILE::IsFilePointerNull ()
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

bool SYSTEM_GDF_FILE::IsFileEnd()
{
	return feof(this->m_Fp);
}

int SYSTEM_GDF_FILE::Open (const char *szMode)
{
#ifdef _DEBUG_INFO_
	printf ("m_FullFileName = %s\n", m_FullFileName);
#endif
	m_Fp = fopen (m_FullFileName, szMode);
	
	if (NULL == m_Fp)
	{
		printf ("FILE[%s]LINE[%d]ERR_MSG[File Pointer Is Null.]\n", __FILE__, __LINE__);
		return -1;
	}
	
	return 0;
}

size_t SYSTEM_GDF_FILE::Write (const char *buffer, size_t size, size_t count)
{
	if (this->m_Fp)
		return fwrite (buffer, size, count, this->m_Fp);
	else
		return 0;
}

int SYSTEM_GDF_FILE::Fflush ()
{
	if (this->m_Fp)
	{
		return fflush (this->m_Fp);
	}
	else
	{
		return 0;
	}
}

int SYSTEM_GDF_FILE::Close ()
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
	if (0 != fclose (m_Fp))
	{
		return -1;
	}
	
	m_Fp = NULL;
	
	return 0;
}

int SYSTEM_GDF_FILE::Rename (const char *destFile)
{
	struct stat buf;
	stat (m_FullFileName, &buf);
	if (buf.st_size > 0)
	{
		if (0 != rename (m_FullFileName, destFile))
		{
			return -1;
		}
	}
	
	return 0;
}

int SYSTEM_GDF_FILE::getLines()
{
	int linect = 0;
	char p[255] = {0};
	
	fgets (p, sizeof(p), this->m_Fp);
	while(!feof(this->m_Fp))
	{
		linect++;
		fgets (p, sizeof(p), this->m_Fp);
	}
	
	return linect;
}

void SYSTEM_GDF_FILE::MakeFullName (char *szFullName)
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

int SYSTEM_GDF_FILE::getFileList(char *indir,int num)
{
	DIR* dir;
	dirent* ptr;
	int i = 0;
	char fullFileName[1024] = {0};
	stFileInfo *pNode = NULL;
	struct stat stStatBuf;
	dir = opendir(indir);
	while ((ptr = readdir(dir)) != NULL)
	{
		memset (fullFileName, 0, sizeof(fullFileName));
		//指定入口文件目录
		sprintf(fullFileName, "%s/%s", indir, ptr->d_name);
		if (stat(fullFileName, &stStatBuf) == -1)
		{
			continue;
		}
		if(is_dir(stStatBuf.st_mode))
		{
			if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			{
				continue;
			}
		}
		if(is_reg(stStatBuf.st_mode))
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
	closedir(dir);
	
	return 0;
}

int SYSTEM_GDF_FILE::getFileList(char *indir)
{
	DIR* dir;
	dirent* ptr;
	char fullFileName[1024] = {0};
	stFileInfo *pNode = NULL;
	struct stat stStatBuf;
	
	dir = opendir(indir);
	while ((ptr = readdir(dir)) != NULL)
	{
		memset (fullFileName, 0, sizeof(fullFileName));
		//指定入口文件目录
		sprintf(fullFileName, "%s/%s", indir, ptr->d_name);
		if (stat(fullFileName, &stStatBuf) == -1)
		{
			continue;
		}
		if(is_dir(stStatBuf.st_mode))
		{
			if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
			{
				continue;
			}
		}
		if(is_reg(stStatBuf.st_mode))
		{
			pNode = new stFileInfo;
			if (NULL == pNode)
			{
				printf("[%s][%d]new stFileInfo is failed.\n",__FILE__,__LINE__, ptr->d_name);
				continue;
			}

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
	closedir(dir);
	
	return 0;
}

int SYSTEM_GDF_FILE::Remove ()
{
	if (0 != remove (m_FullFileName))
	{
		return -1;
	}
	
	return 0;
}

int SYSTEM_GDF_FILE::Copy(const char *src_file, const char *des_file)
{
	FILE* des_f = NULL;
	FILE* src_f = NULL;
	char buf[BEST_BUF + 4];
	
	if(!strcmp(src_file,des_file))
		return 0;
	
	if((src_f=fopen(src_file,"rb"))==NULL)
	{
		printf("Can not open src_file -- %s.\n",src_file);
		return 1;
	}
	if((des_f=fopen(des_file,"wb"))==NULL)
	{
		printf("Can not open des_file -- %s.\n",des_file);
		fclose(src_f);
		return 1;
	}
	
	while(!feof(src_f))
	{
		memset(buf,0,sizeof(buf));
		fread(&buf,1,sizeof(buf)+1,src_f);
		if(strlen(buf) < 1)
			continue;
		if(fwrite(&buf,strlen(buf),1,des_f)!=1)
		{
			printf("Copy file |%s| to |%s| error!\n",src_file,des_file);
			fclose(des_f);
			fclose(src_f);
			return 1;
		}
	}
	
	fclose(des_f);
	fclose(src_f);
	return 0;
}

int SYSTEM_GDF_FILE::Move(const char *src_file, const char *des_file)
{
	if(strcmp(src_file,des_file)==0)
		return 0;
	
	if( rename(src_file,des_file) != 0 )
	{
		if(Copy(src_file,des_file))
		{
			return 1;
		}
		remove(src_file);
	}
	
	return 0;
}

int SYSTEM_GDF_FILE::Seek (long offset,int where)
{
	return fseek(this->m_Fp,offset,where);
}

char* SYSTEM_GDF_FILE::Gets (char *p, int size)
{
	return fgets (p, size, this->m_Fp);
}

bool SYSTEM_GDF_FILE::is_dir(mode_t t)
{
	return S_ISDIR(t);
}

bool SYSTEM_GDF_FILE::is_reg(mode_t t)
{
	return S_ISREG(t);
}

size_t SYSTEM_GDF_FILE::Read (void *p, size_t size, size_t count)
{
	return fread (p, size, count, this->m_Fp);
}

int SYSTEM_GDF_FILE::Truncate(char *filename,int num)
{
	return truncate(filename,num);
}

