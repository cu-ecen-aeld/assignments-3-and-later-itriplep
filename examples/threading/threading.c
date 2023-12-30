#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data *threadData = (struct thread_data *) thread_param;

    sleep_ms(threadData->ms_wait_obtain );

    int n;
    n = pthread_mutex_lock(threadData->mutex);

    if(n == 0){
        sleep_ms(threadData->ms_wait_release );
    }else{
        ERROR_LOG("mutex didn't lock!");
    }

    n = pthread_mutex_unlock(threadData->mutex);

    threadData->thread_complete_success = (n==0);
    
    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */

    struct thread_data *threadData =  malloc(sizeof(struct thread_data));

    if(threadData == NULL){
        return false;
    }
    threadData->mutex = mutex;
    threadData->ms_wait_obtain = wait_to_obtain_ms;
    threadData->ms_wait_release = wait_to_release_ms;
    threadData->thread_complete_success = false;

    if ( pthread_create(thread,NULL,threadfunc,threadData) != 0){
        free(threadData);
        return false;
    }

    return true;
}

