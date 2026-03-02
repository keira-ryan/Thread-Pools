/*
Keira Ryan
2431681
keiryan@chapman.edu
CPSC 380
Assignment 4 - Thread Pools

threadpool implements all functions that power the execution and handling
of the threadpool. pool_init() creates all the threads depending on the 
predefined number of threads, initializes the mutex lock, and initializes
the named semaphore. pool_submit() takes the function and data requested
to be carried out and encapsulates said info into a task which gets added
to the task queue through enqueue() and is then carried out by a thread
by calling dequeue() and execute(). each thread runs in a while loop waiting
for a task to become available. finally, pool_shutdown() closes out the
threads after they finish functionality through cancel and join, as well as
closes out the mutex lock and semaphore. 
*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

//define task struct
typedef struct {
    void (*function)(void *p);
    void *data;
}
task;

//initialize work queue
task workQueue[QUEUE_SIZE];
int frontIndex = 0;
int rearIndex = 0;
int queueCount = 0;

//worker threads
pthread_t thr[NUMBER_OF_THREADS];

//mutex lock and semaphore
pthread_mutex_t mutex;
sem_t *sem;

int enqueue(task t) {
    //lock for enqueue because it accesses and modifies common work queue variable
    pthread_mutex_lock(&mutex);

    //return 0 if successful, 1 if not successful or queue is full
    if (queueCount == QUEUE_SIZE) {
        return 1;
    } else { //update queue values if queue is not full
        workQueue[rearIndex] = t;
        rearIndex = (rearIndex + 1) % QUEUE_SIZE;
        queueCount++;
    }
    printf("queue count: %d \n", queueCount);

    //now can unlock because common variables are no longer being accessed
    pthread_mutex_unlock(&mutex);

    //if enqueue is successful, then semaphore should post that work is available
    sem_post(sem);
    printf("task posted \n");

    return 0;
}

task dequeue() {
    //mutex lock the dequeue because it is modifying the common access work queue
    pthread_mutex_lock(&mutex);

    //remove a task from the queue and update queue
    printf("dequeuing task\n ");
    task taskToDequeue = workQueue[frontIndex];
    frontIndex = (frontIndex + 1) % QUEUE_SIZE;
    queueCount--;

    //unlock now that common variables are done being accessed
    pthread_mutex_unlock(&mutex);

    //return dequeued task
    return taskToDequeue;
}

void *worker(void *param) {
    //loop to ensure that threads are continously looking for new tasks
    while(TRUE) {
        //wait until work has been posted
        //this is also super awesome cancellation point for when we want to shutdown()
        sem_wait(sem);

        //dequeue the posted task
        task taskToExecute = dequeue();

        //worker thread executes task
        execute(taskToExecute.function, taskToExecute.data);
    } 
    pthread_exit(0);
}

void execute(void (*somefunction)(void *p), void *p) {
    //execute task provided to thread pool
    (*somefunction)(p);
}

int pool_submit(void (*somefunction)(void *p), void *p) {
    //creates task with parameter function and data
    task worktodo;
    worktodo.function = somefunction;
    worktodo.data = p;

    //insert said task into work queue
    int enqueueStatus = enqueue(worktodo);

    if (enqueueStatus == 1) {//notify if enqueue failure
        printf("enqueue failed");
        return 1;
    }

    return 0;
}

void pool_init(void) {
    //initialize mutex lock and named semaphore
    pthread_mutex_init(&mutex, NULL);
    //i ran into issues running the program back to back without unlinking SEM from previous intializations
    sem_unlink("SEM");
    //initializing to 0 because the work queue starts empty until an enqueue
    sem = sem_open("SEM", O_CREAT, 0666, 0);
    
    //initialize thread pool with predefined number of threads
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_create(&thr[i], NULL, worker, NULL);
    }
}

void pool_shutdown(void) {
    //intiate thread cancellation for each thread
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_cancel(thr[i]);
    }
    
    //shutdown thread pool
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        pthread_join(thr[i], NULL);
    }

    //clean up mutex and semaphore
    pthread_mutex_destroy(&mutex);
    sem_close(sem);
}