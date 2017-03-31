/*Created on: " 30/03/2017"
Author: Alex
Reviewed by:
Rejects:
*/
#include <stdio.h>
#include "periodic.h"

/* void *thread_1 (void *arg) */
/* { */
/*   struct periodic_info info; */

/*   make_periodic (10000, &info); */
/*   while (1) */
/*   { */
/* 	/1* Do useful work *1/ */
/* 	wait_period (&info); */
/*   } */
/*   return NULL; */
/* } */

int main( int argc , char ** argv, char ** env)
{
  int rv;

  rv = periodic_task_init();
  return 0;
}
