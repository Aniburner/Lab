#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include "share.h"
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		cerr << "Needs destination file path argumen\n";
		return 0;
	}
	void *shm=nullptr;
	shared_use_st *pshared=nullptr;
	char file_buffer[TEXT_SIZE];
	int shmid;
	shmid = shmget((key_t)2345, sizeof(shared_use_st), 0666|IPC_CREAT);
	if(shmid == -1)
	{
		exit(EXIT_FAILURE);
	}
	shm = shmat(shmid, (void *)0, 0);
	if(shm == (void *)-1)
	{
		exit(EXIT_FAILURE);
	}
	cout << "Memory attached at " << shm << endl;
	pshared=(shared_use_st *)shm;
	ofstream file;
	file.open(argv[1]);
	int semid;
	semid=semget(MYKEY, 1, IPC_CREAT);
	cout << "semid = " << semid << endl;
	init_sem(semid, 1);
	bool repeat = false, running = true;
	while(running)
	{
		semaphore_p(semid, 0);
		for(int i=0; i<5; i++)
		{
			if(pshared->written[i])
			{
				memset(file_buffer, 0, TEXT_SIZE);
				strncpy(file_buffer, pshared->text[i], TEXT_SIZE);
				semaphore_v(semid, 0);
				cout << "You wrote: " << file_buffer << endl;
				file.write(file_buffer, TEXT_SIZE);	
				memset(pshared->text[i], 0, TEXT_SIZE);
				pshared->written[i] = false;
				sleep(1);
				break;
			}
			else
			{
				if(i == 4)
				{
					if(repeat)
					{
						semaphore_v(semid, 0);
						cout << "There may be no more resources. Do you wanna try waiting awhile? (y for yes, other for no)\n";
						string cmd;
						cin >> cmd;
						if(cmd == "y")
							break;
						else
						{
							running = false;
							break;
						}
					}
					repeat = true;
					i = 0;
					sleep(1);
					continue;
				}
				else
					continue;
			}
		}
	}
	file.close();
	//if(shmdt(shm) == -1)
	//{
		//exit(EXIT_FAILURE);
	//}
	//if(shmctl(shmid, IPC_RMID, 0) == -1)
	//{
		//exit(EXIT_FAILURE);
	//}
	exit(EXIT_SUCCESS);
}
