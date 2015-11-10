#include "sem.h"
#define MAX 1000

typedef struct Mesg
{
	int data;
	int srcport;
} Mesg_t, *Mesg_p;

typedef struct Port
{
	Mesg_t mesg[10];
	int in;
	int out;
	struct Semaphore_t* prod;
	struct Semaphore_t* cons;
	struct Semaphore_t* mutex1;
} Port_t, *Port_p;

struct Semaphore_t* mutex;

//Port_t Ports[100];

void Send(int id, int flag, int srcport, int dstport, int data, Port_p p)
{
	P(p->prod);
	P(p->mutex1);
	// Add data to port
	p->mesg[p->in].data = data; 
	p->mesg[p->in].srcport = srcport;
	if (flag)
		printf("Client %d sending data -> %d from source port : %d to dest port : %d in Message slot %d\n", id, data, srcport, dstport, p->in);
	else
		printf("Server %d sending data -> %d from source port : %d to dest port : %d in Message slot %d\n", id, data, srcport, dstport, p->in);
	p->in = (p->in + 1) % 10;
	V(p->mutex1);
	V(p->cons);
}

Mesg_t Receive(int id, int port, int flag, Port_p p)
{
	P(p->cons);
	P(p->mutex1);
	int data = p->mesg[p->out].data;
	int srcport = p->mesg[p->out].srcport;
	if(flag)
		printf("Server %d received data -> %d on port %d from Message slot %d\n", id, data, port, p->out);
	else
		printf("Client %d received data -> %d on port %d from Message slot %d\n", id, data, port, p->out);
	p->out = (p->out + 1) % 10;
	V(p->mutex1); 
	V(p->prod);
	Mesg_t recv_msg; 
	recv_msg.data = data;
	recv_msg.srcport = srcport;
	return recv_msg;
}
