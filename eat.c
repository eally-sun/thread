/*
 * =====================================================================================
 *
 *       Filename:  eat.c
 *
 *    Description:  哲学家吃饭问题
 *
 *        Version:  1.0
 *        Created:  2013年10月07日 21时28分58秒
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

#define    SIZE  4 


sem_t      *people ;
//pthread_mutex_t   mutex_number = PTHREAD_MUTEX_INITIALIZER ;
sem_t      buf[5] ;

int eat (char *args) 
{
	int number ;
	if (sem_wait (people))
	{
		fprintf (stderr , "wait for the sem_wait()\n");
		return -1 ;
	}
	
	sem_wait (&buf[atoi(args)%5]) ;
	sem_wait (&buf[(atoi(args)+1)%5]);

	printf ("%s i am eating \n",args );

	sem_post (&buf[(atoi(args)+1) %5]);
	sem_post (&buf[atoi(args)%5]);
	sem_post (people);
	return 0 ;
	
}





int main(int argc, char *argv[])
{
	int         i = 0 ;
	thpool_t    *philosopher; 
	int         tag ;
        char       *args[] = {"0", "1", "2","3","4"}; 
	

        people = (sem_t *)malloc (sizeof (sem_t));
	sem_init (people , 0 ,SIZE);
	for ( i = 0 ; i< 5 ;i++)
	{
		sem_init (&buf[i], 0 , 1);
	}
	philosopher = thpool_init (5);

	for (i = 0 ; i < 5 ;i++)
	{
		tag = thpool_add_work (philosopher, (void*)eat , args[i]);

	}
	sleep (2);
	printf ("it will kill\n");
	sem_destroy (people);
	for (i = 0 ;i < 5 ;i++)
	{
		sem_destroy (&buf[i]);
	}
	thpool_destory (philosopher);




	return EXIT_SUCCESS;
}

