/*Created on: " 30/03/2017"
Author: Alex
Reviewed by:
Rejects:
*/
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "periodic.h"

void task_1(void * ptr)
{
  printf("[%s][%d]\n", __FUNCTION__, __LINE__);
}

void task_2(void * ptr)
{
  printf("[%s][%d]\n", __FUNCTION__, __LINE__);
}

static struct periodic_task_attr attributes[] = {
  {
	.usec_interval = 1000000,
	.handler = task_1,
	.data = NULL,
  },
  {
	.usec_interval = 3000000,
	.handler = task_2,
	.data = NULL,
  },
};
#define ARR_ELLEMENTS(_arr)    (sizeof(_arr)/sizeof((_arr)[0]))
struct periodic_task 
tasks[ARR_ELLEMENTS(attributes)] = {0};

int main( int argc , char ** argv, char ** env)
{
  int rv, i;

  rv = periodic_task_init();
  assert(!rv && "periodic_task_init failed");

  for(i=0; i < ARR_ELLEMENTS(attributes); ++i) {
	rv = periodic_task_create(&tasks[i], &attributes[i]);
	assert(!rv && "periodic task create failed");
  }

  usleep(5000000);
  attributes[0].handler = task_2;
  attributes[1].handler = task_1;
  for(i=0; i < ARR_ELLEMENTS(attributes); ++i) {
	rv = periodic_task_change(&tasks[i], &attributes[i]);
	assert(!rv && "periodic task create failed");
  }
  while (1)
	usleep(100000000);
  return 0;
}
