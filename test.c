#include<stdio.h>
#include<stdlib.h>
#include "threads.h"
void functionA() 
{
	while(1) 
	{
		printf("xxx\n");
		yield();
	}
}

void functionB() 
{
        while(1)
        {
                printf("yyy\n");
                yield();
        }
}

void functionC() 
{
        while(1)
        {
                printf("zzz\n");
                yield();
        }
}

int main()
{
	InitQ(&RunQ);
	start_thread(functionA);
	start_thread(functionB);
	start_thread(functionC);
	run();
	return 1;
}


