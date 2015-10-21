//Producer Consumer
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<stdint.h>

#define N 4

sem_t empty, full, mutex;
int buffer[N];
int in=0, out=0, item_num=0;

void* producer(void *arg)
{
	int id;
	id = (intptr_t)arg;
	while(1)
	{
		printf("Producer %d is ready to produce\n", id);
		sem_wait(&empty); //down semaphore
			sem_wait(&mutex);
				printf("Producer %d inserting item %d into slot %d\n", id, item_num, in);
				buffer[in] = item_num;
				item_num++;
				in = (in+1) % N;
			sem_post(&mutex); //up semaphore
		sem_post(&full);
		//sleep(1);
	}
}

void* consumer(void *arg)
{
	int id;
	id = (intptr_t)arg;
	while(1)
	{
		printf("Consumer %d is ready to consume\n", id);
		sem_wait(&full); //down semaphore
			sem_wait(&mutex); 
				printf("Consumer %d deleting item %d from slot %d\n", id, buffer[out], out);
				out = (out+1) % N;
			sem_post(&mutex); //up semaphore
		sem_post(&empty);
		//sleep(2);
	}
}

int main()
{
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);
	
	int threads = 5, i = 0, ret = -1;
	pthread_t *thread = (pthread_t*)malloc(sizeof(pthread_t)*threads);
	for(i=1; i<=threads; i++)
	{
		//int *arg = (int*)malloc(sizeof(int));
		//*arg = i; 
		
		if(i % 2 == 1)
		{
			ret = pthread_create(&thread[i], NULL, &producer, (void *)(intptr_t)i);
		}
		
		else
		{
			ret = pthread_create(&thread[i], NULL, &consumer, (void *)(intptr_t)i);
		}

		if(ret != 0)
		{
			printf("Create pthread error!\n");
			exit(1);
		}
	}
	
	for(i=1; i<=threads; i++)
	{
		pthread_join(thread[i], NULL);
	}

	return 1;
}
