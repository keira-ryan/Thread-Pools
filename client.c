/*
Keira Ryan
2431681
keiryan@chapman.edu
CPSC 380
Assignment 4 - Thread Pools

client uses three functions from the threadpool API to call on and 
carry out functionality using the thread pool. pool_init initializes
the thread pool, pool_submit sends work the the work queue to be 
completed, and pool_shutdown closes the thread pool. In this case, 
pool_submit is currently being called in a for loop to test the 
multithreading capability of carrying out multiple add calls at
once. The add function is just one example of a function that can 
be carried out by the thread pool.
*/

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data {
    int a;
    int b;
};

//example function to be passed in for testing threadpool functionality
void add(void *param) {
    struct data *temp;
    temp = (struct data*)param;
    printf("I add two values %d and %d result = %d\n", temp->a, temp->b, temp->a + temp->b);
}

int main(void){
    struct data work;
    work.a = 5;
    work.b = 10;

    //initialize threadpool
    pool_init();

    //test multithreading handling of pool
    for (int i = 0; i < 15; i++) {
        int submitStatus = pool_submit(&add, &work);
        if (submitStatus == 1) {
            printf("Work queue is full and submit could not be processed.");
            return 0;
        }
    }

    //close out pool
    pool_shutdown();

    return 0;
}