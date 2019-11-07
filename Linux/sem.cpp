#include "share.h"
#include <stdio.h>
#include <sys/sem.h>

union semun 
{
    int     val;            /* value for SETVAL */
    struct  semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    u_short *array;         /* array for GETALL & SETALL */
};

int init_sem(int sem_id, int init_value)
{
	union semun sem_union;
	sem_union.val = init_value;
	if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		perror("Initialize semaphore");
		return -1;
	}
	return 1;
}

int semaphore_p(int sem_id, short sem_no)
{
	struct sembuf sem_b;
	sem_b.sem_num = sem_no;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		perror("Semaphore_p failed;");
		return 0;
	}
	return 1;
}

int semaphore_v(int sem_id, short sem_no)
{
	struct sembuf sem_b;
	sem_b.sem_num = sem_no;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem_b, 1) == -1)
	{
		perror("Semaphore_p failed;");
		return 0;
	}
	return 1;
}

int del_sem(int sem_id)
{
	union semun sem_union;
	if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
	{
		perror("Delete semaphore");
		return -1;
	}
	return 1;
}

