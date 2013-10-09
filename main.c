/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:消费者和生产者的问题 
 *
 *        Version:  1.0
 *        Created:  2013年09月25日 21时24分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  eally-chen, eallyclw@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "thread_pool.h"
#include "thread_pool.c"
#define SIZE  10 



sem_t            *empty ;
sem_t            *full ;
pthread_mutex_t   mutex_number = PTHREAD_MUTEX_INITIALIZER ;
int               in = 0 ;
int               out  = 0 ;
char              *buf[SIZE];

void producetask(char *args) {
        
        if (sem_wait (empty))
	{
		fprintf (stderr , "wait for the sem_wait () \n");
		return ;
	}
	pthread_mutex_lock (&mutex_number);
	buf[in] = args ;
	in = (in +1 )%SIZE;
	printf ("produce %d task is running \n", in);
	pthread_mutex_unlock(&mutex_number);
	sem_post (full);
//	printf ("%d  task is running\n ", args);
	
}
void customertask (char *args){
	char   *number ;
	if (sem_wait (full))
	{
	      fprintf (stderr, "wait for the sem_wait ()\n");
	      return ;
	}
	pthread_mutex_lock (&mutex_number);
	number = buf[out];
	out = (out+1)%SIZE ;
	printf ("customer  %d task is running ,and get the number is %s\n", out, number);
	pthread_mutex_unlock (&mutex_number);
	sem_post (empty);
}

int main(int argc, char *argv[])
{
	int  i ;
        empty = (sem_t *)malloc (sizeof (sem_t));
	full = (sem_t*)malloc (sizeof (sem_t));

	sem_init (empty , 0 , SIZE);
	sem_init (full , 0 , 0);

	char *args[] = {
		"1", "2", "3", "4","5", "6", "7","8","9","10"
	};
	thpool_t  *produce ;
	thpool_t  *customer ;
        produce  = thpool_init (4);
	customer = thpool_init (4);
	int     tag ;

	
	for (i = 0 ; i < 10 ; i++)
        {
		
		tag = thpool_add_work (produce , (void*)producetask,  args[i]);
		
		
	}
	for (i =  0 ; i < 10 ;i++)
	{
		tag = thpool_add_work (customer , (void*)customertask, args[i]);
	}
	sleep (2);
	puts ("will kill threadpool");
	sem_destroy (empty);
	sem_destroy (full);
	pthread_mutex_destroy (&mutex_number);
	thpool_destory (produce);
	thpool_destory (customer);





	return EXIT_SUCCESS;
}

