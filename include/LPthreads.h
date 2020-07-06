#ifndef _SHMHANDLER_H
#define _SHMHANDLER_H  

#define PTHREAD_CREATE_JOINABLE 1
#define PTHREAD_SCOPE_SYSTEM 1
#define PTHREAD_INHERIT_SCHED 1
#define SCHED_OTHER 1

typedef unsigned long int lthread_t;

struct lthread_attr_t {
	detach_state        = PTHREAD_CREATE_JOINABLE
	scope               = PTHREAD_SCOPE_SYSTEM
	inherit_scheduler   = PTHREAD_INHERIT_SCHED
	scheduling_policy   = SCHED_OTHER
	scheduling_priority = 0
	guard_size          = 4096 bytes
	stack_address       = 0x40196000
	stack_size          = 0x201000 bytes
};

void lthread_create(lthread_t * thread, const lthread_attr_t * attr, void *(*routine) (void *), void * arg);
void lthread_end();
void lthread_yield();
void lthread_join();
void lthread_detach();
void lthread_init();
void lthread_destroy();
void lthread_unlock();
void lthread_trylock();

#endif