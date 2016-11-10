#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 5

//create main philosopher function to drive everything else
void *philosopher( void *num);

//create function pickupSticks() that lets the philosopher pick the sticks up if free(both or nothing)
void pickupSticks(int x);

//create function dropSticks() that allow for both sticks to be dropped
void dropSticks(int x);

void printAll();
//create semaphore array of size 5 for chopsticks
sem_t semArr[5];

//create mutex lock mainly for mutators
sem_t mutex;

//create integer array of size 5 for the philosophers and their current condition
	//	0	-	Thinking
	//	1	-	Hungry
	//	2	-	Eating
int philState[5];

//create array with 0...4
int testArr[5];

int main()
{
	int i;	//loop driver variable
	time_t t;
	srand((unsigned) time(&t));
	
	
	sem_init(&mutex,0,1);
	for(i=0;i<5;i++)
	{
		philState[i] = 0;
		sem_init(&semArr[i],0,1);
		testArr[i]=i;
	}
	//Create 5 threads
	pthread_t userThread[5];
	
	//Start them off in a method, say philosopher()
	for(i=0;i<5;i++)
	{
		pthread_create(&userThread[i],NULL,philosopher, &testArr[i]);
	}
	
	//Join 5 threads
	for(i=0;i<5;i++)
	{
		pthread_join(userThread[i],NULL	);
	}
	
	//Destroy semaphore and exit
	sem_destroy(&mutex);
	return 0;
}

//Main philosopher method contains
	//an infinite loop inside which
	//random sleep times
	//pick up chopsticks with function, pickupSticks()
	//random sleep times
	//drop chopsticks with function, dropSticks()
void *philosopher(void* inputNum)
{
	int num = *((int*)inputNum);
	while(1)
	{
		printf("Philosopher %d is thinking\n",num);
		sleep(rand()%4);
		printf("Philosopher %d is hungry\n",num);
		pickupSticks(num);
		printf("Philosopher %d is eating\n",num);
		sleep(rand()%4);
		dropSticks(num);
		//printAll();
	}
}

//pickupSticks()
	//acquire mutex lock
	//check whether left and right chopsticks are free
	//pick chopsticks up/acquire their semaphores/wait
	//release mutex lock
void pickupSticks( int x)
{
	int status = 0;
	philState[x] = 1;
	while(!(philState[(x+1)%5] != 2 && philState[(x+4)%5] !=2));
	sem_wait(&mutex);
	philState[x] = 2;
	sem_wait(&semArr[x]);
	sem_post(&mutex);
}


//dropSticks()
	//acquire mutex lock
	//release chopsticks/signal/post
	//release mutex lock
void dropSticks(int x)
{
	sem_wait(&mutex);
	philState[x] = 0;
	sem_post(&semArr[x]);
	sem_post(&mutex);
}

void printAll()
{
	int arrOne[5];
	int i;
	for(i=0;i<5;i++)
	{
		arrOne[i]=9;
	}
	for(i=0;i<5;i++)
	{
		if(philState[i] == 2)
		{
			arrOne[i] = i;
			arrOne[(i+1)%5] = i;
		}
	}
	for(i=0;i<5;i++)
	{
		printf("%d",arrOne[i]);
	}
	printf("\n");
}
