/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5				//number of philosophers and forks

lid32 forks[N];			//array of locks
lid32 lock;	
struct 	lockentry *lptr;	/* ptr for lock table entry */	

/**
 * Delay for a random amount of time
 * @param alpha delay factor
 */
void	holdup(int32 alpha)
{
	long delay = rand() * alpha;
	while (delay-- > 0)
		;	//no op
}

/**
 * Eat for a random amount of time
 */
void	eat()
{
	holdup(10000);
}

/**
 * Think for a random amount of time
 */
void	think()
{
	holdup(1000);
}



/**
 * Philosopher's code
 * @param phil_id philosopher's id
 */
void	philosopher(uint32 phil_id)
{	
	//seed rand() from time of boot
	srand(phil_id);

	//TODO - right fork	
	uint32 left = phil_id;
	
	//TODO - left fork
	uint32 right;

	double odds;
	
	//for 1st philosopher
	if(phil_id == 0)
	{
		right = N-1;
	}
	
	//every other philosopher
	else	
	{
		right = phil_id - 1;
	}
	
	while (TRUE)
	{
		//TODO
		//think 70% of the time
		//eat 30% of the time
		int i;

		//gets a random number between [0,1]
		for(i = 0; i < 10; i++)
		{
			odds = (double)rand() / (double)((unsigned)RAND_MAX);
		}

		//thinks
		if(.3 < odds && odds <= 1)
		{
			//locks to print and then thinks
			acquire(lock);
			kprintf("Philosopher %d thinking: zzzzzZZZz\n", phil_id);
			release(lock);
			think();
		}
		
		//eats
		else
		{
			//grab the right fork
			acquire(forks[right]);
	
			//get a pointer to the lock in the lock table @index lock	
			lptr = &locktab[forks[left]];

			//check to see if the left fork is available
			if(lptr->lock == FALSE)
			{
				//lock the left fork and mutex to print then unlock them
				acquire(forks[left]);
				acquire(lock);
				kprintf("Philosopher %d eating: nom nom nom\n", phil_id);
				release(lock);
				eat();	
				release(forks[left]);
				release(forks[right]);
			}
			
			//drop fork
			else
			{
				release(forks[right]);
			}			
		} 		
	}
}

int	main(uint32 argc, uint32 *argv)
{		
	//initialize locks
	lock = lock_create();
	
	int i;
	for(i = 0; i < N; i++)
	{
		forks[i] = lock_create();
	}
	
	//do not change
	ready(create((void*) philosopher, INITSTK, 15, "Ph1", 1, 0), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph2", 1, 1), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph3", 1, 2), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph4", 1, 3), FALSE);
	ready(create((void*) philosopher, INITSTK, 15, "Ph5", 1, 4), FALSE);

	return 0;
}
