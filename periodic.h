/*Created on: " 30/03/2017"
Author: Alex
Reviewed by:
Rejects:
*/
#ifndef __PERIODIC_H__
#define __PERIODIC_H__
struct periodic_task;
struct periodic_task_attr;

int periodic_task_init(void);
int periodic_task_create(struct periodic_task *,
				const struct periodic_task_attr *);
int periodic_task_change(struct periodic_task *,
				const struct periodic_task_attr *);
int periodic_task_delete(struct periodic_task *);
#endif /* __PERIODIC_H__ */
