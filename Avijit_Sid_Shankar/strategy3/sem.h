#include<stdlib.h>
#include "threads.h"


struct Semaphore_t
{
	int count;
	struct node* SemQ;
};

struct Semaphore_t* CreateSem(int InputValue)
{
	struct Semaphore_t* sem = (struct Semaphore_t*)malloc(sizeof(struct Semaphore_t));
	sem->count = InputValue;
	InitQ(&(sem->SemQ));
	return sem; 
}

void P(struct Semaphore_t *sem) 
{
	sem->count--;
	if(sem->count < 0) 
	{
		TCB_t* temp;
		temp = DelQ(&RunQ);
		AddQ(&(sem->SemQ), temp);
		swapcontext(&(temp->context), &(RunQ->tcb).context);
	}
}	

void V(struct Semaphore_t *sem)
{
	sem->count++;
	//struct node* temp;
	if(sem->count <= 0) 
	{
		TCB_t* temp;
		temp = DelQ(&(sem->SemQ));
		AddQ(&RunQ, temp);
	}
	yield();
}
