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
#include<pthread.h>
#include<string>
#include<iostream>
#include<fstream>

extern int list(const char *path, const char *storePath);

using namespace std;

int main(int argc, char *argv[])
{
	if(argc!=3)
	{
		cerr << "The first argument should be source path, the second one should be destination path!\n";
		return -1;
	}
	int ret=list(argv[1], argv[2]);
	return ret;
}
