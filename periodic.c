/*Created on: " 30/03/2017"
Author: Alex
Reviewed by:
Rejects:
*/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include "periodic.h"



void *thread_func(void *arg);

int periodic_task_init(void)
{
  sigset_t alarm_sig;
  int i;

  /**
    Block all real time signals so they can be used for the timers.
    Note: this has to be done in main() before any threads are created
    so they all inherit the same mask. Doing it later is subject to
    race conditions
  */
  sigemptyset (&alarm_sig);
  for (i = SIGRTMIN; i <= SIGRTMAX; i++)
    sigaddset (&alarm_sig, i);
  sigprocmask (SIG_BLOCK, &alarm_sig, NULL);
  return 0;
}

static void set_itemerspec_from_usec(struct itimerspec *itval, uint64_t usec)
{
  uint64_t sec, ns;
  /* Make the timer periodic */
  sec = usec/1000000;
  ns = (usec - (sec * 1000000)) * 1000;
  itval->it_interval.tv_sec = sec;
  itval->it_interval.tv_nsec = ns;
  itval->it_value.tv_sec = sec;
  itval->it_value.tv_nsec = ns;
}

int periodic_task_create(struct periodic_task * task,
                         const struct periodic_task_attr * attr)
{
  static int next_sig;
  int rv = 0;
  struct sigevent sigev;

  /*
     Initialise next_sig first time through. We can't use static
     initialisation because SIGRTMIN is a function call, not a constant
  */
  if (next_sig == 0)
    next_sig = SIGRTMIN;
  /* Check that we have not run out of signals */
  if (next_sig > SIGRTMAX)
    return -1;
  task->signal_num = next_sig;
  next_sig++;

  /* Create the signal mask that will be used in wait_period */
  sigemptyset (&(task->alarm_sig));
  sigaddset (&(task->alarm_sig), task->signal_num);

  /* Create a timer that will generate the signal we have chosen */
  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = task->signal_num;
  sigev.sigev_value.sival_ptr = (void *) &task;
  rv = timer_create (CLOCK_MONOTONIC, &sigev, &task->timer_id);
  if (rv) {
    perror("Periodic timer not created");
    goto out;
  }

  rv = periodic_task_change(task, attr);

out:
  return rv;
}

int periodic_task_change(struct periodic_task *task,
                   const struct periodic_task_attr *attr)
{
  int rv = 0;
  struct itimerspec itval;

  periodic_task_delete(task);

  rv = pthread_create(&task->thread, NULL, thread_func, task);
  if (rv) {
    perror("Periodic thread not created");
    goto out;
  }

  /* This function assumes that the timer was already created */
  set_itemerspec_from_usec(&itval, attr->usec_interval);
  rv = timer_settime(task->timer_id, 0, &itval, NULL);
  if (rv) {
    perror("Periodic timer not set");
    goto out;
  }
  task->attr = *attr;

out:
  return rv;
}

int periodic_task_delete(struct periodic_task *task)
{
  int rv = 0;
  struct itimerspec itval;

  /* If new_value->it_value specifies a zero value */
  /* (i.e., both subfields are zero), then the timer is disarmed */
  set_itemerspec_from_usec(&itval, 0);
  rv = timer_settime(task->timer_id, 0, &itval, NULL);
  if (rv) {
    perror("Periodic timer not disarmed");
    goto out;
  }

  rv = pthread_cancel(task->thread);
  if (rv != 0 && rv != ESRCH) {
    perror("Periodic thread not canceled");
    goto out;
  }

  memset(&task->thread, 0, sizeof(pthread_t));

out:
  return rv;
}

void *thread_func(void *arg)
{
  struct periodic_task *task = arg;
  struct periodic_task_attr * attr = &task->attr;
  assert(task != NULL);

  while (1)
  {
    int signal_num = 0;

    /* Suspend execution until signal recieved*/
    sigwait(&task->alarm_sig, &signal_num);
    /* Test valid signal number */
    assert(signal_num == task->signal_num);
    /* Execute the commanf */
    attr->handler(attr->data);
  }
  return NULL;
}
