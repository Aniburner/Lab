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
#include<string>

using namespace std;

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
	;
}
int main(int argc, char** argv)
{
	if(argc!=2)
		perror("Not enough argument or wrong argument!\n"); 
	char buf[500];
	DIR *currentdir=NULL;
	struct dirent *currentdp = NULL;
	struct stat currentstat;
	struct passwd *p_passwd;
	struct group *p_group;
	char *p_time;
	int i;
	memset(buf,0,500);
	sprintf(buf,"%s",argv[1]);
	sprintf(buf,"%s/%s",buf,currentdp->d_name);
	printf("the file is %s\n",buf);
	lstat(buf,&currentstat);
	p_time=ctime(&currentstat.st_mtime);
	p_passwd=getpwuid(currentstat.st_uid);
	p_group=getgrgid(currentstat.st_gid);
	currentdir=opendir(buf);
	currentdp=readdir(currentdir);
}
