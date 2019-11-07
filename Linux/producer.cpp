#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "share.h"
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cerr << "Needs resource file path argument\n";
		return 0;
	}
	void *shm = nullptr;
	shared_use_st *pshared=nullptr;
	char file_buffer[TEXT_SIZE];
	int shmid;
	shmid = shmget((key_t)2345, sizeof(shared_use_st), 0666|IPC_CREAT);
	if(shmid == -1)
	{
		cerr << "get shared memory failed\n";
		exit(EXIT_FAILURE);
	}
	shm = shmat(shmid, (void *)0, 0);
	if(shm == (void *)-1)
	{
		exit(EXIT_FAILURE);
	}
	cout << "Memory attached at " << shm << endl;
	pshared = (shared_use_st *)shm;
	ifstream file;
	file.open(argv[1]);
	int semid;
	semid=semget(MYKEY, 1, IPC_CREAT);
	cout << "semid = " << semid << endl;
	init_sem(semid, 1);
	bool noMoreResource = false;
	while(!noMoreResource)
	{
		memset(file_buffer, 0, TEXT_SIZE);
		file.read(file_buffer, TEXT_SIZE);
		semaphore_p(semid, 0);
		for(int i=0; i<5 ;i++)
		{
			if(pshared->written[i])
			{
				if(i == 4)
				{
					semaphore_v(semid, 0);
					sleep(1);
					cout << "Waiting...\n";
					semaphore_p(semid, 0);
					i = 0;
				}
				else
					continue;
			}
			else
			{
				strncpy(pshared->text[i], file_buffer, TEXT_SIZE);
				cout << "You read: " << file_buffer << endl;
				pshared->written[i] = true;
				semaphore_v(semid, 0);
				sleep(1);
				break;
			}
		}
		if(file.eof())
		{
			noMoreResource = true;
		}
	}
	file.close();
	if(shmdt(shm)==-1)
	{
		exit(EXIT_FAILURE);
	}
	if(shmctl(shmid, IPC_RMID, 0) == -1)
	{
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
