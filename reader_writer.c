#include<stdio.h>
#include<stdlib.h>
#include "sem.h"
//#include<pthread.h>
//#include<semaphore.h>
#include<stdint.h>

//sem_t r_sem, w_sem, mutex;
struct Semaphore_t* r_sem;
struct Semaphore_t* w_sem;
struct Semaphore_t* mutex;
int rwait=0, ractive=0, wwait=0, wactive=0, global_ID=0;

// Expectation: There can be multiple readers at the same but only 1 writer at a time. Avoid starvation by favoring writers when a reader exits// and favoring readers when a writer exits. 

void reader_entry(int ID)
{
	// If no active/waiting writer, then enter. Else block on read queue. 	
	P(mutex);
	if(wactive > 0 || wwait > 0) 
	{
		printf("Reader %d blocked for a writer\n", ID);
		rwait++;
		V(mutex);	
		V(r_sem); 
		rwait--;
	}
	ractive++;
	if(rwait > 0) 
	{
		V(r_sem);
	}
	else
	{
		V(mutex);
	}
}

void reader_exit(int ID)
{
	// If no active reader is left (this was the last active reader) and there's atleast one waiting writer, wake up one writer from the	       // write queue
	P(mutex);
	ractive--;
	if(ractive == 0 && wwait > 0)
	{
		V(w_sem);
	}
	else
	{
		V(mutex);
	}
}

void writer_entry(int ID)
{
	// If no active reader/writer, enter. Else, block on write queue. 
	P(mutex);
	if(ractive > 0 || wactive > 0)
	{
		printf("Writer %d blocked for others\n", ID);
		wwait++;	
		V(mutex);
		P(w_sem);
		wwait--;
	}
	wactive++;
	V(mutex);
}

void writer_exit(int ID)
{
	// If there are waiting reader(s), wake all of them. Else, if there are waiting writers, wake one of them. 
	P(mutex);
	wactive--;
	if(rwait > 0)
	{
		V(r_sem);
	}
	else if(wwait > 0)
	{
		V(w_sem);
	}
	else
	{
		V(mutex);
	}
}

void reader()
{
	int ID;
	P(mutex);
		ID = global_ID++;
	V(mutex);
	while(1)
	{
		reader_entry(ID);
		printf("Reader %d Reading\n", ID);
		sleep(1);
		reader_exit(ID);
	}
}

void writer()
{
	int ID;
	P(mutex);
		ID = global_ID++;
	V(mutex);
	while(1)
	{
		writer_entry(ID);
		printf("Writer %d Writing\n", ID);
		sleep(1);
		writer_exit(ID);		
	}
}

int main()
{
	InitQ(&RunQ);
	mutex = CreateSem(1);
	r_sem = CreateSem(0);
	w_sem = CreateSem(0);
	//sem_init(&mutex, 0, 1);
	//sem_init(&r_sem, 0, 0);
	//sem_init(&w_sem, 0, 0);
	start_thread(reader);
	start_thread(reader);
	start_thread(reader);
	start_thread(writer);
	start_thread(writer);
	run();
	
	/*
	pthread_t thread1, thread2, thread3, thread4, thread5;

	pthread_create(&thread1, NULL, &reader, NULL);
	pthread_create(&thread2, NULL, &reader, NULL);
	pthread_create(&thread3, NULL, &reader, NULL);
	pthread_create(&thread4, NULL, &writer, NULL);
	pthread_create(&thread5, NULL, &writer, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	pthread_join(thread3, NULL);
	pthread_join(thread4, NULL);
	pthread_join(thread4, NULL);
	*/
	
	return 1;
}
