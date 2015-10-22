#include<stdio.h>
#include<stdlib.h>
#include "sem.h"

struct Semaphore_t *mutexb, *mutex1, *mutex2, *wait;
int count=0, x=0, y=0;

void barrier(int n)
{
	P(mutexb);
	count++;
	printf("Inside Barrier count : %d\n", count);
	if(count < n)
	{
		V(mutexb);
		P(wait);
		P(mutexb);
		count--;
	}
	else
	{
		//count = 0;
		V(wait);
		V(wait);
		V(wait);
	}
	V(mutexb);
}

void functionA()
{
	int i;
	for(i=0; i<5; i++)
	{
		P(mutex1);
		x++;	
		printf("funcA : x = %d\n", x);
		V(mutex1);

		barrier(3);
		sleep(1);

		P(mutex2);
                y++;
                printf("***funcA : y = %d\n", y);
                V(mutex2);
		
		barrier(3);
		sleep(1);
	}
}

void functionB()
{
	int i;
        for(i=0; i<5; i++)
        {
                P(mutex1);
                x++;
                printf("funcB : x = %d\n", x);
                V(mutex1);

                barrier(3);
		sleep(1);

                P(mutex2);
                y++;
                printf("***funcB : y = %d\n", y);
                V(mutex2);

		barrier(3);
		sleep(1);
        }
}



		
void functionC()
{
        int i;
	for(i=0; i<5; i++)
        {
                P(mutex1);
                x++;
                printf("func C : x = %d\n", x);
                V(mutex1);

                barrier(3);
		sleep(1);

                P(mutex2);
                y++;
                printf("***func C : y = %d\n", y);
                V(mutex2);
		
		barrier(3);
		sleep(1);
        }
}
int main()
{
	InitQ(&RunQ);
	mutexb = CreateSem(1);
	mutex1 = CreateSem(1);
	mutex2 = CreateSem(1);
	wait = CreateSem(1);

	start_thread(functionA);
	start_thread(functionB);
	start_thread(functionC);

	run();	
	return 1;
}
