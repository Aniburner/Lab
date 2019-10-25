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
#include<pthread.h>
#include<string>
#include<iostream>

struct pathStructure
{
	char sourcePath[500];
	char destPath[500];
};

extern int copy(const char *sourcePath, const char *destPath);

void *copyExe(void *arg)
{
	pathStructure *paths=(pathStructure *)arg;	
	copy(paths->sourcePath,paths->destPath);
	return nullptr;
}

using namespace std;

enum file_type {regular, directory, character, block, fifo, symblink, socket};
int print_perm(mode_t st_mode)
{
	int i;
	unsigned int mask = 0x7;
	static const char * perm[]={"---","--w","-w-","-wx","r--","r-x","rw-","rwx"};
	for(i=3;i>0;--i)
	{
		printf("%3s",perm[st_mode>>(i-1)*3&mask]);	
	}
	printf(" ");
	return 0;
}

int print_type(mode_t st_mode)
{
	file_type t;
	if(S_ISREG(st_mode))
	{
		t=regular;
		cout << "-";
	}
	if(S_ISDIR(st_mode))
	{
		t=directory;
		cout << "d";
	}
	if(S_ISCHR(st_mode))
	{
		t=character;
		cout << "c";
	}
	if(S_ISBLK(st_mode))
	{
		t=block;
		cout << "b";
	}
	if(S_ISFIFO(st_mode))
	{
		t=fifo;
		cout << "p";
	}
	if(S_ISLNK(st_mode))
	{
		t=symblink;
		cout << "l";
	}
	if(S_ISSOCK(st_mode))
	{
		t=socket;
		cout << "s";
	}
	return t;
}

int list(const char *path, const char *storePath)
{
	char buf[500];
	DIR *currentdir=NULL;
	struct dirent *currentdp = NULL;
	struct stat currentstat;
	struct passwd *p_passwd;
	struct group *p_group;
	char *p_time;
	int i;
	memset(buf,0,500);
	sprintf(buf,"%s",path);
	if((currentdir=opendir(buf))==NULL)
	{
		cout << "Open directory fail" << endl;
		return 0;
	}
	while((currentdp=readdir(currentdir))!=NULL)
	{
		if(currentdp->d_name[0]!='.')
		{
			sprintf(buf,"%s/%s",path,currentdp->d_name);
			if(lstat(buf,&currentstat)==-1)
			{
				cout << "get stat error" <<endl;
				continue;
			}
			else
			{
				p_time=ctime(&currentstat.st_mtime);
				p_passwd=getpwuid(currentstat.st_uid);
				p_group=getgrgid(currentstat.st_gid);
				print_type(currentstat.st_mode);
				print_perm(currentstat.st_mode);
				printf("%d ",(int)currentstat.st_nlink);
				if(p_passwd!=NULL)
					printf("%-10s ",p_passwd->pw_name);
				else
					cout << currentstat.st_uid;
				if(p_group!=NULL)
					printf("%-10s ",p_group->gr_name);
				else
					cout << currentstat.st_gid;
				printf(" %d ",(int)currentstat.st_size);
				for(i=0;p_time[i]!=0&&p_time[i]!='\n';i++)
					putchar(p_time[i]);
				printf(" ");
				cout << currentdp->d_name << endl;
			}
			pthread_t *pthreadID=new pthread_t;
			pathStructure *paths=new pathStructure;
			strcpy(paths->sourcePath,buf);
			strcpy(paths->destPath,storePath);
			pthread_create(pthreadID,nullptr,copyExe,paths);
			if(pthreadID==nullptr)
			{
				cerr << "Create thead failed!\n";
				return -1;
			}
		}
	}
	closedir(currentdir);
	return 0;
}
