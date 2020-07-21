#include "../include/lpthread.h"

// Main queue for living threads
struct Queue thread_queue;
// Base indexes for structure ids
int thread_base_id = 0;
int mutex_base_id = 0;
// Boolean wich restrics just one call to scheduler
bool scheduling = true;
// Time interval setting structures
struct itimerval it;
struct sigaction act;
// Thread structs that stores relevant information
lthread * top_thread;
lthread new_thread, current_thread, previous_thread;
// This ucontext struct points to a final subrutine that kills a thread
ucontext_t finish;
//lthread_mutex_t new_lthread_mutex_t;

// Subroutine for killing threads when their job has finalized
void finishTask(){
	current_thread.isActive = false;
	dequeue(&thread_queue, (void *) &current_thread);
	previous_thread = current_thread;
	setcontext(&(current_thread.context));
}

// This program schedules context switching of enqueued threads
void scheduler(int signal){
	dequeue(&thread_queue, (void *) &current_thread);
	current_thread.status = RUNNING;
	previous_thread.status = READY;
	/*
	if (!current_thread.isActive)
	{
		dequeue(&thread_queue, (void *) &current_thread);
	}*/
	enqueue(&thread_queue, (void *) &previous_thread);
	previous_thread = current_thread;
	swapcontext(&(((lthread *)(thread_queue.rear->data))->context), &(current_thread.context));
}

// This routime starts interval timer for calling scheduler routine
void startScheduler(){
	act.sa_handler = &scheduler;
	sigaction(SIGALRM, &act, NULL); 
	it.it_interval.tv_usec = QUANTUM;
	it.it_value.tv_usec = QUANTUM;
	setitimer(ITIMER_REAL, &it, NULL);
}

// This routime stops interval timer and scheduler routine is not called every quantum
void stopScheduler(){
	act.sa_handler = &scheduler;
	sigaction(SIGALRM, &act, NULL); 
	it.it_interval.tv_usec = 0;
	it.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &it, NULL);
}

// This function verifies if and specific thread is enqueued
bool isThreadInQueue(lthread_t thread){
	stopScheduler();
	struct qNode * aux = thread_queue.front;
	if(current_thread.id == thread){
		startScheduler();
		return true;
	}
	while(aux != NULL && !isEmpty(&thread_queue)){
		if(((lthread *)(aux->data))->id == thread){
			startScheduler();
			return true;
		}
		aux = aux->next;
	}
	startScheduler();
	return false;
}

/*  
*   Sigaction and setitimer in C
*   lthread_create function has a code modification obtained at the website https://stackoverflow.com/questions/59011345/sigaction-and-setitimer-in-c
*   Creator: Aidan Goldfarb. Nov 23th, 2019.
*/
void lthread_create(lthread_t * thread, const lthread_attr_t * attr, void *(*routine) (void*), void * arg){
	// If this is the first call to lthread_create
	if(thread_base_id == 0){
		// Initilizing thread queue
		initQueue(&thread_queue, sizeof(lthread));
		// Setting main thread as previous thread
		previous_thread.id = thread_base_id++;
		previous_thread.isActive = true;
		previous_thread.status = READY;
		previous_thread.context.uc_link = 0;
	}
	// Initializing killing subrouting context
	getcontext(&finish);
	finish.uc_link = 0;
	finish.uc_stack.ss_sp = malloc(STACK_SIZE);
	finish.uc_stack.ss_size = STACK_SIZE;
	finish.uc_stack.ss_flags = 0;
	makecontext(&finish, &finishTask, 0);
	// Creating a new context with given routine parameter
	getcontext(&new_thread.context);
	new_thread.context.uc_link = &finish;
	new_thread.context.uc_stack.ss_sp = malloc(STACK_SIZE);
	new_thread.context.uc_stack.ss_size = STACK_SIZE;
	new_thread.context.uc_stack.ss_flags = 0;
	makecontext(&(new_thread.context), (void (*))(routine), 0);
	// Setting default attributes for enqueueing a new thread
	new_thread.id = thread_base_id++;
	new_thread.attr = (lthread_attr_t *) attr;
	new_thread.isActive = true;
	new_thread.status = READY;
	*thread = new_thread.id;
	enqueue(&thread_queue, (void *) &new_thread);
	// Starting scheduling just once
	if(scheduling){
		scheduling = false;
		startScheduler();
	}
}

void lthread_end(){

}

// This function changes current context to put a new one to run
void lthread_yield(void){
	dequeue(&thread_queue, (void *) &current_thread);
	current_thread.status = RUNNING;
	previous_thread.status = READY;
	enqueue(&thread_queue, (void *) &previous_thread);
	previous_thread = current_thread;
	swapcontext(&(((lthread *)(thread_queue.rear->data))->context), &(current_thread.context));
}

// This function waits to an specified thread until it finishes
void lthread_join(lthread_t thread, void **value_ptr){
	while(isThreadInQueue(thread)){
		dequeue(&thread_queue, (void *) &current_thread);
		current_thread.status = RUNNING;
		previous_thread.status = READY;
		enqueue(&thread_queue, (void *) &previous_thread);
		previous_thread = current_thread;
		swapcontext(&(((lthread *)(thread_queue.rear->data))->context), &(current_thread.context));
	}
}

void lthread_detach(lthread tid){

}

// This function sets initial values for lthread_mutex_t 
void lthread_mutex_init(lthread_mutex_t *restrict mutex, const lthread_mutexattr_t *restrict attr){
	/*if(lthread_mutex_list == NULL){
		lthread_mutex_list = createList(sizeof(lthread_mutex_t));
	}*/
	mutex->lock = false;
	mutex->id = mutex_base_id++;
	//addFirst(lthread_mutex_list, &new_lthread_mutex_t);
}

// This functions sets free given lthread_mutex_t memory
void lthread_mutex_destroy(lthread_mutex_t *mutex){
	/*struct lNode * current = lthread_mutex_list->head;
	int index = 0;
	while(current != NULL){
		if(((lthread_mutex_t *)(current->data))->id == mutex->id){
			removeAt(lthread_mutex_list, index);
		}
		index++;
		current = current->next;
	}*/
	free(mutex);
}

// This function unlock a lthread_mutex_t setting its lock value to false. It only works if the thread which locks it is unlocking it.
void lthread_mutex_unlock(lthread_mutex_t *mutex){
	if(mutex->blocking_thread == current_thread.id){
		mutex->lock = false;
	}
}
 
// This function tries to lock a lthread_mutex_t setting its lock value to true. If it is already lock, the calling thread is put at the queue end  
void lthread_mutex_trylock(lthread_mutex_t *mutex){
	while(mutex->lock){
		stopScheduler();
		dequeue(&thread_queue, (void *) &current_thread);
		current_thread.status = SUSPENDED;
		previous_thread.status = READY;
		enqueue(&thread_queue, (void *) &previous_thread);
		previous_thread = current_thread;
		startScheduler();
		swapcontext(&(((lthread *)(thread_queue.rear->data))->context), &(current_thread.context));
	}
	mutex->lock = true;
	mutex->blocking_thread = current_thread.id;
}