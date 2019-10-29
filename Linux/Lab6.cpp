#include <pthread.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define MAXLINE 30
int currentline=0;

static pthread_cond_t   pWait = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t  pLock = PTHREAD_MUTEX_INITIALIZER;
int N = 0;
int nsecs = 1;
int *pFork; // 01 array presents the forks; 1 -> available
int isAvailable(int i) {
    if (pFork[i] && pFork[(i+1) % N])
        return 1;
    else
        return 0;
}

static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char    buf[MAXLINE];
   vsnprintf(buf, MAXLINE, fmt, ap);
   if (errnoflag)
       snprintf(buf+strlen(buf), MAXLINE-strlen(buf), ": %s",
         strerror(error));
   strcat(buf, "\n");
   fflush(stdout);     /* in case stdout and stderr are the same */
   fputs(buf, stderr);
   fflush(NULL);       /* flushes all stdio output streams */
}


void err_quit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}

void takeFork(int i) {
	//cout << "breakpoint here\n";
	pthread_mutex_lock(&pLock);
	while(!isAvailable(i))
	{
		pthread_cond_wait(&pWait,&pLock);   
	}
	pFork[i]=0;
	pFork[(i+1)%N]=0;
	//cout << "philosopher " << i <<" takes forks successfully!\n";
	pthread_cond_broadcast(&pWait);
	pthread_mutex_unlock(&pLock);
}

void putFork(int i) {
	pFork[i]=1;
	pFork[(i+1)%N]=1;
}
void thinking(int i, int nsecs) {
	sleep(nsecs);    
	cout << "Philosopher: " << i << " is thinking.\n";
}
void eating(int i, int nsecs) {
	cout << "Philosopher: " << i << " is eating.\n";
	sleep(nsecs);
}

void* philosopher(void * arg) {
	int num=(int)(*((int*)arg));
	for(;currentline<=MAXLINE;currentline++)
	{
		thinking(num,nsecs);
		takeFork(num);
		eating(num,nsecs);
		putFork(num);
		thinking(num,nsecs);
	}
	if(currentline==MAXLINE+1)
		cout << "\n*********Output lines should reach MAXLINE preset by program around here*********\n";
	return nullptr;
}

int main(int argc, char *argv[]) {
    int i, err;
    pthread_t tid[20];
    pthread_attr_t attr;
    if (argc < 2)
        err_quit("useage: philosopher_th N <-t nsecs>");
    if (argc == 4 && (strcmp(argv[2], "-t")) == 0) {
        nsecs = atoi(argv[3]);
    }
        N = atoi(argv[1]);
        if (N < 2)
                err_quit("N is the number of philosopher\n");
        if (N > 20)
                err_quit("too many philosopher");
    pFork = (int *)malloc(N*sizeof(int));
    for (i = 0; i < N; i++) {
        pFork[i] = 1;
    }
    err = pthread_attr_init(&attr); // detach
    if (err != 0)
	{
        err_quit("pthread arr init error");
	}
   	err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if (err != 0)
	{
        err_quit("set detachstate error");
	}
	int num[N];
	//pthread_mutex_unlock(&pLock);
	//pthread_cond_broadcast(&pWait);
    for (i = 0; i < N; i++) {
		num[i]=i;
        err = pthread_create(&tid[i],&attr,philosopher,&num[i]);
        if (err != 0)
            err_quit("can't create thread: %s\n", strerror(err));
    }
	pause();
    return 0;
}
