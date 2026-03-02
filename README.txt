Keira Ryan
Assignment 4 - Thread Pools

In this program, the client.c file uses the thread pool API to submit tasks to a work queue 
to be executed by a multithread pool. Available threads remove and complete work from the 
queue. If all threads are in use, the queue holds pending work until a thread is available.
The threadpool.c class implements the thread pool using a POSIX mutex lock, named semaphore,
and Pthreads.

Source Files Included: 
client.c, threadpool.c, threadpool.h

Known Errors:
There are no known errors in my program.

References Used:
- Referenced code in Lecture 4 on Thread and Concurrency to look at how to intialize array of threads
and join array of threads to close thread pool
- Used zyBooks 7.3 to reference code on POSIX mutex locks and POSIX named semaphores
- Referenced to brush up on how to implement circular queues with an array: https://www.programiz.com/dsa/circular-queue
- Used to determine using named instead of unnamed semaphores: https://w3.cs.jmu.edu/kirkpams/OpenCSF/Books/csf/html/IPCSems.html#:~:text=macOS%20provides%20POSIX%20named%20semaphores,will%20not%20block%20the%20process.
- Used to learn about sem_unlink() to debug semaphore problems: https://www.qnx.com/developers/docs/8.0/com.qnx.doc.neutrino.lib_ref/topic/s/sem_unlink.html#:~:text=The%20sem_unlink()%20function%20destroys,sem_close()%20to%20close%20it.

Instructions to Compile:
gcc client.c threadpool.c -o client

Instructions to Run:
./client
