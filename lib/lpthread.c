#include "../include/lpthread.h"

struct Queue thread_queue;
int thread_base_id = 0;
bool scheduling = true;
struct itimerval it;
struct sigaction act;
lthread * top_thread;
lthread new_thread, current_thread, previous_thread;

void scheduler(int signal){

	enqueue(&thread_queue, (void *) &previous_thread);
	dequeue(&thread_queue, (void *) &current_thread);

	previous_thread = current_thread;

	top(&thread_queue, top_thread);

	swapcontext(&(((lthread *)(thread_queue.rear->data))->context), &(current_thread.context));
}

void lthread_create(lthread_t * thread, const lthread_attr_t * attr, void (*routine) (void), void * arg){

	if(thread_base_id == 0){
		initQueue(&thread_queue, sizeof(lthread));
		previous_thread.id = thread_base_id++;
	}

	getcontext(&new_thread.context);
	new_thread.context.uc_link = 0;
	new_thread.context.uc_stack.ss_sp = malloc(STACK_SIZE);
	new_thread.context.uc_stack.ss_size = STACK_SIZE;
	new_thread.context.uc_stack.ss_flags = 0;
	makecontext(&(new_thread.context), routine, 0);

	new_thread.id = thread_base_id++;
	new_thread.attr = *attr;
	*thread = new_thread.id;
	enqueue(&thread_queue, (void *) &new_thread);

	if(scheduling){
		
		scheduling = false;

		act.sa_handler = &scheduler;
		sigaction(SIGALRM, &act, NULL); 

		it.it_interval.tv_sec = 1;
		it.it_interval.tv_usec = 0;
		it.it_value.tv_sec = 1;
		it.it_value.tv_usec = 0;
		setitimer(ITIMER_REAL, &it, NULL);
	}
}

void lthread_end(){

}

void lthread_yield(){

}

void lthread_join(){

}

void lthread_detach(){

}

void lthread_init(){
	
}

void lthread_destroy(){

}

void lthread_unlock(){

}

void lthread_trylock(){

}