#include <string.h>
#include "ifile_sdfs.h"
#include "ifile_system.h"

int main(int argc, const char * argv[])
{
	char filepath[1024] = {0};
	char filename[128] = {0};
	char text[1024] = "123\n1231231312222\n123\n1231231312222\n123\n";
	char buff[1024];
	char backupFile[1024] = {0};
	//strcpy(backupFile,"/ocsapp/work/wangzhia/ibf/ilog_all/demo/runlog/demo_sdfs_STAT_20170524154520.log");
	strcpy(backupFile,"/sdfslog/CLUSTER_Z00/NRC/tmp/demo_sdfs_STAT_20170524154520.log");
	//strcpy(filepath,"/ocsapp/work/wangzhia/ibf/ilog_all/demo/temp");
	//strcpy(filename,"demo_sdfs_STAT_20170524.log");
	strcpy(filepath,"/sdfslog/CLUSTER_Z00/NRC/tmp");
	strcpy(filename,"demo.log");
	//GDF_FILE *file = new SYSTEM_GDF_FILE(filepath,filename);
	GDF_FILE *file = new SDFS_GDF_FILE(filepath,filename);
	file->Open("a+");
	
	//file->Write(text,sizeof(text),1);
	//file->Fflush();
	
	/*file->Seek(0,SEEK_SET);
	file->Gets(buff, sizeof(buff));
	while(!file->IsFileEnd())
	{
		printf("\t%s",buff);
		file->Gets(buff, sizeof(buff));
	}
	file->Seek(0,SEEK_SET);
	file->Read(buff, 1, sizeof(buff));
	printf("%s\n",buff);*/
	//file->Copy("/ocsapp/work/wangzhia/ibf/ifile_all/demo/a.txt","/ocsapp/work/wangzhia/ibf/ifile_all/demo/b.txt");
	//file->Rename("/ocsapp/work/wangzhia/ibf/ifile_all/demo/bca");
	
	//file->getFileList("/ocsapp/work/wangzhia/ibf/ifile_all/demo", 3);
	/*file->getFileList("/sdfslog/CLUSTER_Z00/OFRF/temp", 3);
	for(vector<stFileInfo*>::iterator it = file->mFileList.begin();it != file->mFileList.end();it++)
	{
		stFileInfo *fileinfo = *it;
		printf("filename:%s\n",fileinfo->sFileName);
	}*/
	
	int iRet = 0;
	iRet = file->Rename (backupFile);
	printf("iRet = %d\n",iRet);
	
	file->Close();
	return 0;
}
