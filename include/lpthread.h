#ifndef _LPTHREAD_H
#define _LPTHREAD_H  

#include<queue.h>
#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include<unistd.h>
#include<stdbool.h> 
#include<signal.h>
#include<sys/types.h>
#include<sys/time.h>
#include<linked_list.h>

#define STACK_SIZE 1024 * 1024
#define QUANTUM 2000

#define RUNNING 0
#define READY 1
#define SUSPENDED 2

/*
*	https://code.woboq.org/userspace/glibc/nptl/pthread_create.c.html
*
*	https://www2.cs.duke.edu/courses/spring09/cps110/projects/1/project1.text
*	http://nitish712.blogspot.com/2012/10/thread-library-using-context-switching.html
*	https://courses.engr.illinois.edu/cs423/sp2011/mps/mp3/mp3.pdf
*	http://web.eecs.umich.edu/~farnam/482/Winter99/pa2-v3.html
*/
typedef unsigned long int lthread_t;

typedef struct {
    int                 __flags;
    size_t              __stacksize;
    void                *__stackaddr;
    void                (*__exitfunc)(void *status);
    int                 __policy;
    //struct sched_param  __param;
    unsigned            __guardsize;
} lthread_attr_t;

typedef struct {
	bool lock;
	unsigned int id;
	lthread_t blocking_thread;
} lthread_mutex_t;

typedef struct {
	
} lthread_mutexattr_t;

typedef struct {
	ucontext_t context;
	int status;
	lthread_t id;
	lthread_attr_t * attr;
	bool isActive;
} lthread;

void lthread_create(lthread_t * thread, const lthread_attr_t * attr, void *(*routine) (void*), void * arg);
void lthread_end();
void lthread_yield(void);
void lthread_join(lthread_t thread, void **value_ptr);
void lthread_detach(lthread tid);
void lthread_mutex_init(lthread_mutex_t *restrict mutex, const lthread_mutexattr_t *restrict attr);
void lthread_mutex_destroy(lthread_mutex_t *mutex);
void lthread_mutex_unlock(lthread_mutex_t *mutex);
void lthread_mutex_trylock(lthread_mutex_t *mutex);

#endif