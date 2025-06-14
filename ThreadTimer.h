#ifndef THREAD_TIMER_H
#define THREAD_TIMER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "../glueThread/glthread.h"

typedef struct slotlist_{
    glthread_t slots;
    pthread_mutex_t slot_mutex;
}slotlist_t;

typedef struct thread_timer_{
    uint32_t current_clock_tic;  /* equivalent of itimerspec.it_value */
    uint32_t clock_tic_interval; /* equivalent of itimerspec.it_interval */
    uint32_t timer_size;         /* size of the timer's index */
    uint32_t current_cycle_no;   /* index fof the current index */
    pthread_t timer_thread;      /* Periodic Thread Id to be created for timer and callback/event handling */
    slotlist_t reschd_list;
    uint32_t no_of_timer_elements;
    slotlist_t slotlist[0];
}thread_timer_t;


#endif