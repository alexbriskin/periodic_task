/*Created on: " 30/03/2017"
Author: Alex
Reviewed by:
Rejects:
*/
#ifndef __PERIODIC_H__
#define __PERIODIC_H__
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
struct periodic_task;
struct periodic_task_attr {
  uint64_t usec_interval;
  void (*handler)(void*);
  void *data;
};

struct periodic_task {
  struct periodic_task_attr attr;
  timer_t timer_id;
  int signal_num;
  sigset_t alarm_sig;
  pthread_t thread;
};

int periodic_task_init(void);
int periodic_task_create(struct periodic_task *,
				const struct periodic_task_attr *);
int periodic_task_change(struct periodic_task *,
				const struct periodic_task_attr *);
int periodic_task_delete(struct periodic_task *);
#endif /* __PERIODIC_H__ */
