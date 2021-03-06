#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include "green.h"
#include "queue.h"


#define FALSE 0
#define TRUE 1
#define PERIOD 10
#define STACK_SIZE 4*1024

static ucontext_t main_cntx = {0};

static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};

static green_t * running = &main_green;

static sigset_t block;

green_t * head_ll = &main_green;
queue_t * ready_queue;

void handler();

// this function is called once program loads into memory regurdless of what function is called!
static void init( ) __attribute__((constructor));
void init()
{
	//sigemptyset(&block);
	//sigaddset(&block, SIGALRM);

	// struct sigaction act = {0};
	// struct timeval interval;
	// struct itimerval period;

	// act.sa_handler = handler;
	// assert(sigaction(SIGVTALRM, &act , NULL) == 0 );
	// interval.tv_sec = 0;
	// interval.tv_usec = PERIOD;
	// period.it_interval = interval;
	// period.it_value = interval;
	// setitimer(ITIMER_VIRTUAL, &period , NULL);
	signal(SIGALRM, handler);
	ualarm(10, 10);

	getcontext(&main_cntx);
	ready_queue = createQueue();
}

void handler()
{
	
	//write(STDOUT_FILENO, "Hi\n",strlen("Hi\n"));
	
	green_t * susp = running ;
	enQueue(ready_queue, susp);

	green_t* next = deQueue(ready_queue);
	running = next ;

	swapcontext(susp->context, next->context);	

}


// this functions is mapped to every Context
void green_thread(){
	
	green_t * this=running;
	// call target function and save its result
	void * res = (*this->fun)(this->arg);

	// place waiting (joining) thread in ready queue
	green_t * temp = head_ll;

	while(temp->next != NULL)
	{
		if(temp->join == this)
			enQueue(ready_queue, temp);

		temp = temp->next;
	}

	// save result of execution and zombie status
	this->retval = res;
	this->zombie = 1;

	// find the next thread to run and write its address to next variable	
	green_t * next = deQueue(ready_queue);
	running =next;
	
	setcontext(next->context);

}

// will create a new green thread
int green_create(green_t *new ,void *(*fun)(void *),void *arg) {

	ucontext_t * cntx = (ucontext_t *) malloc(sizeof(ucontext_t));
	getcontext(cntx);

	void * stack = malloc(STACK_SIZE);
	cntx->uc_stack.ss_sp = stack;
	cntx->uc_stack.ss_size = STACK_SIZE;

	makecontext(cntx, green_thread, 0);

	new->context = cntx ;
	new->fun = fun ;
	new->arg = arg ;
	new->next = NULL;
	new->join = NULL;
	new->retval = NULL;
	new->zombie = FALSE;
	
	green_t * temp = head_ll;
	while(temp->next != NULL)
		temp = temp->next;
	temp->next = new;
	
	enQueue(ready_queue, new);
	return 0 ;
}

// will give other green thread opportunity of having CPU!
int green_yield()
{
	
	green_t * susp = running ;
	enQueue(ready_queue, susp);
	// select the next thread for execution
	green_t* next = deQueue(ready_queue);
	running = next ;
	// save current state into susp->context and switch to next->context
	
	swapcontext(susp->context, next->context);
	return 0 ;
}

// waits for specefied thread till it finishes and get result value
int green_join(green_t * thread ,void ** res) 
{
	// check if target thread has finished
	//printQueue(ready_queue, head_ll);
	if (!thread->zombie){
		green_t * susp = running ;

		// add as joining thread
		susp -> join = thread;


		// select the next thread for execution
		green_t * next = deQueue(ready_queue);
		running = next ;
		// save current state into susp->context and switch to next->context
		swapcontext(susp->context, next->context);
	}

	// collect result
	*res = thread -> retval;


	// free context
	free(thread->context);
	
	return 0 ;
}
