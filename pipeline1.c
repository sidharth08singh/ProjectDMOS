#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int countA=0, countB=0;

void *funcA()
{
	if(countA < 5)
	{
		countA++; 
		printf("for %d, xxx\n", countA);
	}
}

void *funcB()
{
	if(countB < 5)
	{
		countB++;
		printf("for %d, yyy\n", countB);
	}
}

int main()
{
	pthread_t th1, th2;
	int i;
	for(i=0; i<5; i++)
	{
		pthread_create(&th1, NULL, funcA, NULL);
		pthread_join(th1, NULL);
	}

	for(i=0; i<5; i++)
	{
		pthread_create(&th2, NULL, funcB, NULL);
		pthread_join(th2, NULL);
	}
	return 1;
}
