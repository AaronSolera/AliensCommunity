/*
 * This program uses the Allegro game library to display a blank window.
 *
 * It initializes the display and starts up the main game loop. The
 * game loop only checks for two events: timer (determined by the FPS)
 * and display close (when the user tries to close the window).
 * 
 * Original non-available link http://www.damienradtke.org/building-a-mario-clone-with-allegro
 * Referenced by dradtke at https://gist.github.com/dradtke/2494024
 */

#include <lpthread.h>

/*
#include <stdio.h>
#include <allegro5/allegro.h>

const float FPS = 60;

int main(int argc, char *argv[])
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool running = true;
	bool redraw = true;

	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}

	// Create the display
	display = al_create_display(640, 480);
	if (!display) {
		fprintf(stderr, "Failed to create display.\n");
		return 1;
	}

	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "Failed to create event queue.");
		return 1;
	}

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Start the timer
	al_start_timer(timer);

	// Game loop
	while (running) {
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);

		// Fetch the event (if one exists)
		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		if (get_event) {
			switch (event.type) {
				case ALLEGRO_EVENT_TIMER:
					redraw = true;
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					break;
				default:
					fprintf(stderr, "Unsupported event received: %d\n", event.type);
					break;
			}
		}

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) {
			// Redraw
			al_clear_to_color(al_map_rgb(0, 0, 0));
			al_flip_display();
			redraw = false;
		}
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
*/

lthread_t thread_0, thread_1, thread_2;
lthread_mutex_t mutex;
int x = 0;

void * fn0(void *arg){
	for (int i = 0; i < 10; ++i)
	{
		printf("fun0 iteration -> %d\n",i);
	}
}

void * fn1(void *arg){
	for (int i = 0; i < 20; ++i)
	{
		printf("fun1 iteration -> %d\n",i);
	}
}

void * fn2(void *arg){
	for (int i = 0; i < 30; ++i)
	{
		printf("fun2 iteration -> %d\n",i);
	}
}

int fibonacci(int n)
{
  /* La función normal, recursiva */
  if (n>2)
    return fibonacci(n-1) + fibonacci(n-2);
  /* Este es el caso más común,  */
  else if (n==2)        /* Ponemos esto para agilizar un poco el proceso */
    return 1;
  else if (n==1)       
    return 1;
  else if (n==0)
    return 0;
  else
    return -1;          /* Error */
}

int factorial(int Num){
   if(Num==0)
     return 1;
   else
     return (factorial(Num-1)*Num);
}

void * fn3(void *arg){
	printf("%i\n", fibonacci(40));
}

void * fn4(void *arg){
	printf("%i\n", factorial(1));
}

void * fn5(void *arg){
	for (int i = 0; i < 100000; ++i){
		lthread_mutex_trylock(&mutex);
		x++;
		lthread_mutex_unlock(&mutex);
		printf("fun5 iteration -> %d, x = %i\n", i, x);
	}
}

void * fn6(void *arg){
	for (int i = 0; i < 100000; ++i){
		lthread_mutex_trylock(&mutex);
		x++;
		lthread_mutex_unlock(&mutex);
		printf("fun6 iteration -> %d, x = %i\n", i, x);
	}
}

int main(int argc, char const *argv[])
{	
	lthread_mutex_init(&mutex, NULL);
	lthread_create(&thread_0, NULL, fn5, NULL);
	lthread_create(&thread_1, NULL, fn6, NULL);
	lthread_join(thread_1, NULL);
	printf("Fin del programa\n");
	return 0;
}