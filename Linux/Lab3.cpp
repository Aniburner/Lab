#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include<utime.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>
#include<fcntl.h>
#include<string>
#include<iostream>
#include<fstream>

#define MAXBYTE 4096

using namespace std;

int FileToFile(const char *source, const char *dest, const struct stat *pstat);
int DirToDir(const char *source, const char *dest);

int main(int argc, char *argv[])
{
	struct stat sourcestat;
	struct stat deststat;
	char destbuf[500];
	int ret=0;
	if(argc!=3)
	{
		cerr << "ERROR: Invalid arguments" << endl;
		return -1;
	}
	else 
	{
		cout << "source file name is " << argv[1] << endl;
		cout << "destination file name is " << argv[2] << endl;
		if(lstat(argv[1],&sourcestat)==-1)
		{
			cerr << "ERROR: get argv[1] stat error" << endl;
			return -1;
		}
		if(S_ISDIR(sourcestat.st_mode))
		{
			if(access(argv[2],F_OK)!=0)
			{
				ret = mkdir(argv[2],sourcestat.st_mode);
				cout << "mkdir the ret is " << ret << endl;
			}
			else
			{
				if(lstat(argv[2],&deststat)==-1)
				{
					cerr << "ERROR: get agv[1] stat error\n";
					return -1;
				}
				if(!S_ISDIR(deststat.st_mode))
				{
					cout << "ERROR; the dest is exist file\n";
					return -1;
				}
				else
				{
					int len;
					char *pSrc;
					len=strlen(argv[1]);
					pSrc=argv[1]+(len-1);
					while(pSrc>=argv[1]&&*pSrc!='/')
					{
						--pSrc;
					}
					++pSrc;
					cout << "the dest file is " << pSrc << endl;
					memset(destbuf,0,500);
					sprintf(destbuf,"%s/%s",argv[2],pSrc);
					cout << "the dest path is " << destbuf << endl;
					if(access(destbuf,F_OK)!=0)
					{
						ret=mkdir(destbuf,0755);
						cout << "mkdir the ret is " << ret << endl;
					}
					ret=DirToDir(argv[1],destbuf);
					return ret;
				}
			}
			ret=DirToDir(argv[1],argv[2]);
			return ret;
		}
		else 
		{
			if(access(argv[2],F_OK)==0)
			{
				cout << "dets file exit\n";
				if(lstat(argv[2],&deststat)==-1)
				{
					cerr << "ERROR: get argv[1] stat error\n";
					return -1;
				}
				if(S_ISDIR(deststat.st_mode))
				{
					int len;
					char *pSrc;
					len=strlen(argv[1]);
					pSrc=argv[1]+(len-1);
					while(pSrc>=argv[1]&&*pSrc!='/')
					{
						--pSrc;
					}
					++pSrc;
					cout << "the dest file is " << pSrc << endl;
					memset(destbuf,0,500);
					sprintf(destbuf,"%s/%s",argv[2],pSrc);
					cout << "the dest path is " << destbuf << endl;
					ret=FileToFile(argv[1],destbuf,&sourcestat);
					return ret;
				}
				else
				{
					ret=FileToFile(argv[1],argv[2],&sourcestat);
					return ret;
				}
			}
			else
			{
				int len;
				char *pSrc;
				ret=mkdir(argv[2],0755);
				cout << "mkdir the ret is " << ret << endl;
				len = strlen(argv[1]);
				pSrc=argv[1]+(len-1);
				while(pSrc>=argv[1]&&*pSrc!='/')
				{
					--pSrc;
				}
				++pSrc;
				cout << "the dest file is " << pSrc << endl;
				memset(destbuf,0,500);
				sprintf(destbuf,"%s/%s",argv[2],pSrc);
				cout << "the dest path is " << destbuf;
				ret=FileToFile(argv[1],destbuf,&sourcestat);
				return ret;
			}
		}
	}
}

int FileToFile(const char *source,const char *dest,const struct stat *pstat)
{
	char buf[MAXBYTE];
	memset(buf,0,MAXBYTE);
	int datacount=(int)pstat->st_size;
	ifstream source_file(source);
	ofstream dest_file(dest);
	if(!source_file)
	{
		cerr << "Open source file failed!\n";
		return -1;
	}
	if(!dest_file)
	{
		cerr << "Create destination file failed!\n";
		return -1;
	}
	int rest=datacount;
	for(;;)
	{
		memset(buf,0,MAXBYTE);
		source_file.read(buf,rest>=MAXBYTE?MAXBYTE:rest);
		dest_file.write(buf,rest>=MAXBYTE?MAXBYTE:rest);
		rest-=MAXBYTE;
		if(rest<=0)
			break;
	}
	chmod(dest,pstat->st_mode);
	source_file.close();
	dest_file.close();
	return 0;
}

int DirToDir(const char *source,const char *dest)
{
	DIR *currentdir=nullptr;
	struct dirent *currentdp=nullptr;
	struct stat currentstat;
	char sourcebuf[500];
	char destbuf[500];
	int ret;
	 //if(lstat(source,&currentstat)==-1)
	 //{
		 //cerr << "get status wrong!\n";
		 //return -1;
	 //}
	 //if(S_ISDIR(currentstat.st_mode))
	 //{
		currentdir=opendir(source);
		while((currentdp=readdir(currentdir))!=nullptr)
		{
			if(strcmp(currentdp->d_name,".")==0||strcmp(currentdp->d_name,"..")==0)
				continue;
			memset(sourcebuf,0,500);
			memset(destbuf,0,500);
			sprintf(sourcebuf,"%s/%s",source,currentdp->d_name);
			sprintf(destbuf,"%s/%s",dest,currentdp->d_name);
			memset(&currentstat,0,sizeof(currentstat));
			if(lstat(sourcebuf,&currentstat)==-1)
			{
				cerr << "get status wrong!\n";
				return -1;
			}
			if(currentdp->d_type==DT_DIR)
			{
				if(access(destbuf,F_OK)!=0)
					mkdir(destbuf,currentstat.st_mode);
				ret=DirToDir(sourcebuf,destbuf);
			}
			else
			{
				ret=FileToFile(sourcebuf,destbuf,&currentstat);
			}
		}
	 //}
	 //else
		 //FileToFile(source,dest,&currentstat);
	return ret;
}
