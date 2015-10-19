#include<stdio.h>
#include<stdlib.h>
#include "sem.h"

int x=0;
struct Semaphore_t* sem;

void functionA() 
{
	int i;
	//while(1) 
	for(i=0; i<10; i++)
	{
		P(sem);
		printf("\nFunction A : Iteration : %d ", i+1);
		x = x + 1;
		printf(" Yield from Function A ");
		yield();
		printf(" x : %d ", x);
		V(sem);
	}
}

void functionB() 
{
	int i;
        //while(1)
	for(i=0; i<10; i++)
        {
		P(sem);
		printf("\nFunction B: Iteration : %d ", i+1);
		x = x + 2;
		printf(" x : %d ", x);
		V(sem);
        }
}

void functionC() 
{
	int i;
        //while(1)
	for(i=0; i<10; i++)
        {
		P(sem);
                printf("\nFunction C: Iteration : %d", i+1);
                x = x+5;
		printf(" x : %d ", x);
                V(sem);
        }
}

int main()
{
	InitQ(&RunQ);
	sem = CreateSem(1);
	start_thread(functionA);
	start_thread(functionB);
	start_thread(functionC);
	run();
	return 1;
}


