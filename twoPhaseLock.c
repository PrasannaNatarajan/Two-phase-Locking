#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>

//create main philosopher function to drive everything else
void *philosopher( void *num);

//create function pickupSticks() that lets the philosopher pick the sticks up if free(both or nothing)
void pickupSticks(int x);

//create function dropSticks() that allow for both sticks to be dropped
void dropSticks(int x);

void printAll(int philosopherIndex, int currentStatus);

void initialiseStatus();

void setMode();

void runPhilosopherModule();

void setPhilosopherCount(int size);

void initialiseSemaphore();
//create semaphore array of size 5 for chopsticks
sem_t *semArr;

//create mutex lock mainly for mutators
sem_t mutex;

//create integer array of size 5 for the philosophers and their current condition
	//	0	-	Thinking
	//	1	-	Hungry
	//	2	-	Eating
int *philState;
//create array with 0...4

int mode;
//	0	-	No Print
//	1	-	Print variables as they come
//	2	-	Print only current status of all variables


int SIZE;
//	Number of philosophers

int *testArr;

//Array 
int *status;

int main(int argc, char* argv[] )
{
	if(argc > 3)
		return 1;
	if(argc == 3){
		setPhilosopherCount(atoi(argv[1]));
		setMode(atoi(argv[2]));
	}
	else if(argc == 2){
		setPhilosopherCount(atoi(argv[1]));
		setMode(1);
	}
	else{
		setMode(1);
		setPhilosopherCount(5);
	}
	runPhilosopherModule();
	return 0;
}

void runPhilosopherModule()
{
	int i;	//loop driver variable
	

	time_t t;
	srand((unsigned) time(&t));
	
	initialiseStatus();
	initialiseSemaphore();
	// setMode(1);

	sem_init(&mutex,0,1);
	for(i=0;i<SIZE;i++)
	{
		philState[i] = 0;
		sem_init(&semArr[i],0,1);
		testArr[i]=i;
	}
	//Create 5 threads
	pthread_t userThread[5];
	
	//Start them off in a method, say philosopher()
	for(i=0;i<SIZE;i++)
	{
		pthread_create(&userThread[i],NULL,philosopher, &testArr[i]);
	}
	
	//Join 5 threads
	for(i=0;i<SIZE;i++)
	{
		pthread_join(userThread[i],NULL	);
	}
	
	//Destroy semaphore and exit
	sem_destroy(&mutex);
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
		printAll(num,0);
		sleep(rand()%4);
		printAll(num,1);
		pickupSticks(num);
		printAll(num,2);
		sleep(rand()%4);
		dropSticks(num);
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

void printAll(int philosopherIndex, int currentStatus)
{
	fflush(stdout);
	status[philosopherIndex] = currentStatus;

	if(mode == 1)
	{
		if(currentStatus == 0)
			printf("Philosopher %d is thinking\n",philosopherIndex);
		if(currentStatus == 1)
			printf("Philosopher %d is hungry\n",philosopherIndex);
		if(currentStatus == 2)
			printf("Philosopher %d is eating\n",philosopherIndex);
	}
	else if(mode == 2)
	{
		system("clear");
		int i;
		for(i=0;i<SIZE;i++)
		{
			if(status[i] == 0)
				printf("Philosopher %d is thinking\n",i);
			if(status[i] == 1)
				printf("Philosopher %d is hungry\n",i);
			if(status[i] == 2)
				printf("Philosopher %d is eating\n",i);
		}	
	}
	fflush(stdout);
}

void initialiseStatus()
{
	status = (int*)malloc(sizeof(int)*SIZE);
	int i;
	for(i=0;i<SIZE;i++)
	{
		status[i] = 0;
	}
}

void setMode(int inputMode)
{
	mode = inputMode;
}

void setPhilosopherCount(int size){
	SIZE = size;
}

void initialiseSemaphore(){
	semArr = (sem_t*)malloc(sizeof(sem_t)*SIZE);
	philState = (int*)malloc(sizeof(int)*SIZE);
	testArr = (int*)malloc(sizeof(int)*SIZE);
}
