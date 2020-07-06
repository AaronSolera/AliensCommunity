#ifndef _SHMHANDLER_H
#define _SHMHANDLER_H  

#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

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
    struct sched_param  __param;
    unsigned            __guardsize;
} lthread_attr_t;

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