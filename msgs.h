#include "sem.h"
#define MAX 1000

typedef struct Port
{
	int mesg[10];
	int in;
	int out;
	struct Semaphore_t* prod;
	struct Semaphore_t* cons;
	struct Semaphore_t* mutex1;
} Port_t, *Port_p;

struct Semaphore_t* mutex;

//Port_t Ports[100];

void Send(int data, Port_p p)
{
	int i;
	for(i = 0 ; i < 15; i++)
	{ 
		P(p->prod);
		P(p->mutex1);
		// Add data to port
		p->mesg[p->in] = data; 
		printf("Sending data %d in Message slot %d\n", data, p->in);
		p->in = (p->in + 1) % 10;
		V(p->mutex1);
		V(p->cons);
	}
}

void Receive(Port_p p)
{
	int i; 
	for(i = 0 ; i < 15; i++)
	{ 
		P(p->cons);
		P(p->mutex1);
		int data = p->mesg[p->out];
		printf("Received data %d from Message slot %d\n", data, p->out);
		p->out = (p->out + 1) % 10;
		V(p->mutex1); 
		V(p->prod);
	}
}
