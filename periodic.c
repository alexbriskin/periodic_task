/*Created on: " 30/03/2017"
Author: Alex
Reviewed by:
Rejects:
*/
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
/* void *thread_1 (void *arg) */
/* { */
/*     struct periodic_info info; */

/*     make_periodic (10000, &info); */
/*     while (1) */
/*     { */
/*         /1* Do useful work *1/ */
/*         wait_period (&info); */
/*     } */
/*     return NULL; */
/* } */
struct periodic_task {
  uint64_t usec_interval;

};

int periodic_task_init(void)
{
    sigset_t alarm_sig;
    int i;

    /* Block all real time signals so they can be used for the timers.
Note: this has to be done in main() before any threads are created
so they all inherit the same mask. Doing it later is subject to
race conditions */
    sigemptyset (&alarm_sig);
    for (i = SIGRTMIN; i <= SIGRTMAX; i++)
        sigaddset (&alarm_sig, i);
    sigprocmask (SIG_BLOCK, &alarm_sig, NULL);
    return 0;
}
