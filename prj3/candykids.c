#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "bbuff.h"
#include "stats.h"

typedef struct  {
    int factory_number;
    double time_stamp_in_ms;
} candy_t;

struct number{
	int index;
}*N;

_Bool stop_thread;
sem_t *empty, *full, *mutex;

double current_time_in_ms(void)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

void *Candyfactory(void* arg)
{
	int delay, number;
	struct number *Nin;
	Nin = (struct number*) arg;
	srand((unsigned)time(0));

	number = Nin->index;

	while(!stop_thread){
		delay = rand() % 3;
		printf("\tFactory %d ships candy & waits %ds\n",number,delay);
		
		sem_wait(empty);
		sem_wait(mutex);
		// CS
		candy_t *new_candy;
		new_candy = (candy_t*)malloc(sizeof(candy_t));
		new_candy->factory_number = number;
		new_candy->time_stamp_in_ms = current_time_in_ms();
		bbuff_blocking_insert(new_candy); 
		stats_record_produced(number);
		// CS
		sem_post(mutex);
		sem_post(full);
		
		sleep(delay);
	}
	printf("\tFactory Done!\n");
	return NULL;
}

void *Kidthread(void* arg)
{
	int delay;
	double temp;
	
	srand((unsigned)time(0));
	
	while(!stop_thread){
		delay = rand() % 2;
		sem_wait(full);
		sem_wait(mutex);
		//CS
		candy_t *eaten_candy;
		eaten_candy = bbuff_blocking_extract();
		temp = current_time_in_ms();
		temp -= eaten_candy->time_stamp_in_ms;
		stats_record_consumed(eaten_candy->factory_number,temp);
		free(eaten_candy);
		//CS
		sem_post(mutex);
		sem_post(empty);
		sleep(delay);
	}
	printf("\tKids Done!\n");
	return NULL;
}

int main(int argc,char* argv[])
{
	int factory, kids, seconds;
	int i;
	pthread_t *fid, *kid;
	candy_t *temp;
	
	factory =  atoi(argv[1]);
	kids = atoi(argv[2]);
	seconds = atoi(argv[3]);
	
	fid = (pthread_t*)malloc(sizeof(pthread_t)*factory);
	kid = (pthread_t*)malloc(sizeof(pthread_t)*kids);

	N = (struct number*)malloc(sizeof(struct number)*factory);

	full = (sem_t*)malloc(sizeof(sem_t));
	empty = (sem_t*)malloc(sizeof(sem_t));
	mutex = (sem_t*)malloc(sizeof(sem_t));

	
	sem_init(mutex, 0, 1);
	sem_init(empty, 0, BUFFER_SIZE);
	sem_init(full, 0, 0);

	stats_init(factory);
	
	for(i=0;i<factory;i++)
	{
		N[i].index = i;
		pthread_create(&fid[i],NULL,Candyfactory,&N[i]);
	}
	
	for(i=0;i<kids;i++)
	{
		N[i].index = i;
		pthread_create(&kid[i],NULL,Kidthread,&N[i]);
	}
	
	sleep(seconds); 
	
	stop_thread = true;
	for(i=0;i<factory;i++)
	{
		pthread_join(fid[i],NULL);
	}
	
	for(i=0;i<kids;i++)
	{
		pthread_cancel(kid[i]);
	}

	stats_display();

	free(fid);
	free(kid);
	free(N);
	free(empty);
	free(full);
	free(mutex);

	while(!bbuff_is_empty())  //free heap if there are candies tat are not eaten
	{
		temp = bbuff_blocking_extract();
		free(temp);
	}
	stats_cleanup();
	return 0;
} 
