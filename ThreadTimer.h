#ifndef THREAD_TIMER_H
#define THREAD_TIMER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "../glueThread/glthread.h"

typedef void (*app_callback)(void *arg, int sizeof_arg);

typedef struct slotlist_{
    glthread_t slots;
    pthread_mutex_t slot_mutex;
}slotlist_t;

typedef enum{
    TT_CREATE,
    TT_RESCHD,
    TT_DELETE,
    TT_SCHEDULED,
    TT_UNKNOWN
}tt_state_t;

typedef struct thread_timer_elem_{
    tt_state_t timerState;              /* State for timer element */
    uint32_t time_interval;             /* Timer interval for the timer element */
    uint32_t new_time_interval;         /* New time interval for the timer element */
    uint32_t execute_cycle_no;          /* Execute cycle/ occurance number */
    uint32_t slot_no;                   /* slot number in clcock position */
    app_callback appCallback;
    void *arg;
    uint32_t arg_size;
    bool is_recurrence;
    uint32_t N_secheduled;
    slotlist_t *slotlist_head;
    glthread_t glue;
    glthread_t reschedule_glue;
}thread_timer_elem_t;

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

GLTHREAD_TO_STRUCT(glthread_glue_to_tt_elem, thread_timer_elem_t, glue);
GLTHREAD_TO_STRUCT(glthread_glue_to_reschedule_tt_elem, thread_timer_elem_t, reschedule_glue);

#define TT_SLOTLIST(tt_ptr, index)        (&(tt_ptr->slotlist[index]))
#define TT_SLOTLIST_HEAD(tt_ptr, index)   (&(tt_ptr->slotlist[index].slots))
#define TT_SLOTLIST_MUTEX(tt_ptr, index)  (&(tt_ptr->slotlist[index].slot_mutex))
#define GET_TT_ELEM_SLOTLIST(tt_elem_ptr) (tt_elem_ptr->slotlist_head)
#define TT_LOCK_SLOTLIST(slotlist_ptr)    pthread_mutex_lock(&(slotlist_ptr->slot_mutex))
#define TT_UNLOCK_SLOTLIST(slotlist_ptr)  pthread_mutex_unlock(&(slotlist_ptr->slot_mutex))

#define TT_LOCK_TT_ELEM_SLOTLIST(tt_elem_ptr)                   \
{                                                               \
    slotlist_t *_slotlist = GET_TT_ELEM_SLOTLIST(tt_elem_ptr);  \
    if(_slotlist)                                               \
        TT_LOCK_SLOTLIST(_slotlist);                            \
}

#define TT_UNLOCK_TT_ELEM_SLOTLIST(tt_elem_ptr)                 \
{                                                               \
    slotlist_t *_slotlist = GET_TT_ELEM_SLOTLIST(tt_elem_ptr);  \
    if(_slotlist)                                               \
        TT_UNLOCK_SLOTLIST(_slotlist);                          \
}

#define TT_IS_SLOT_EMPTY(slotlist_ptr) IS_GLTHREAD_LIST_EMPTY(&(slotlist_ptr->slots));
#define TT_GET_RESCHD_SLOTLIST(tt_ptr)       (&(tt_ptr->reschd_list))
#define TT_GET_RESCHD_SLOTLIST_HEAD(tt_ptr)  (&(tt_ptr->reschd_list->slots))

#endif