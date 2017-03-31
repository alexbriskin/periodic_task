/*Created on: " 30/03/2017"
Author: Alex
Reviewed by:
Rejects:
*/
#include <stdio.h>
#include <signal.h>
#include <stdint.h>

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
