#include<stdio.h>
#include<stdlib.h>
#include "msgs.h"

Port_t Ports[100];
int portnumber = 0;
int portS;
int portC;

void producer()
{
	P(mutex);
	portC = portnumber;
	portnumber++;
	V(mutex);
	while(1) 
	{ 
		Send(portC, 25, &Ports[portS]); // Args -  Source Port Number, Data, Destination Port
	}
}

void consumer()
{
	P(mutex);
	portS = portnumber;
	portnumber++;
	printf("Consumer Listening at Port Number %d\n", portnumber);
	V(mutex);
	while(1) 
	{ 
		Receive(&Ports[portS]);
	}
}

int main()
{
	InitQ(&RunQ);
        mutex = CreateSem(1);
	int i,j;
	for(i=0; i<100; i++)
	{
		for(j=0; j<10; j++)
		{
			Ports[i].mesg[j].data = -1;		
			Ports[i].mesg[j].srcport = -1;
		}
		Ports[i].prod = CreateSem(10);
		Ports[i].cons = CreateSem(0);
		Ports[i].mutex1 = CreateSem(1);
		Ports[i].in   = 0;
		Ports[i].out  = 0;
	}
	start_thread(producer);
	//start_thread(producer);
        start_thread(consumer);
	run();
}
