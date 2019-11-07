#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/sem.h>

#define MYKEY 0x1a0a
#define TEXT_SIZE 100

using namespace std;

struct shared_use_st
{
	bool written[5];
	char text[5][100];
	bool noMoreResource;
};

int init_sem(int,int);
int semaphore_p(int,short);
int semaphore_v(int,short);
int del_sem(int);
