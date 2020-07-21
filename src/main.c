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

#include <stdio.h>
#include <unistd.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "alienLogic.h"
#include <pthread.h>
//#include <json-c/json.h>

#define WIDTH 1080
#define HEIGHT 720
#define imgAlienNormal "imgAlien.png"

const float FPS = 60;
Alien* create_alien(){
	Alien *alien = (Alien *) malloc(sizeof(Alien));
	pthread_t hilo;
	pthread_create(&hilo,NULL,&initAlien,(void *) alien);
	return alien;
}

int main(int argc, char *argv[])
{

	//Create alien structure
	//pthread_t hilo;
    //char * alien_type = "n";
    //
	int movimiento = 0;
	int contador   = 0;
	//create new alien
	Alien * alien = create_alien();
	//Alien *alien = (Alien *) malloc(sizeof(Alien));
	//pthread_t hilo;
	//pthread_create(&hilo,NULL,&initAlien,(void *) alien);

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	//ALLEGRO_BITMAP *bitmap = NULL;
	ALLEGRO_BITMAP *Image = NULL;
	bool running = true;
	bool redraw = true;

	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}

	//Init image reader
	al_init_image_addon();
	//Read the bitmap from the image .png
	Image = al_load_bitmap(imgAlienNormal);
	//if image is null finish the program 
	assert(Image != NULL);

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}
	




	display = al_create_display(WIDTH, HEIGHT);
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
	//configure the window
	al_set_window_title(display,"Alien Community");

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Start the timer
	al_start_timer(timer);

	
	

	float x = 0;
	// Game loop
	while (running) {
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;
		ALLEGRO_BITMAP *bitmap;

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
			if(contador == 60){
				movimiento = (movimiento+1)%4;
				contador = 0;
			}
			contador += 1;
			al_clear_to_color(al_map_rgb(101, 10, 0));
			al_draw_bitmap(Image, 0, 0, 0);
			//al_draw_bitmap(Image, 10, 10, 0);

			//hilo principal que controla el movimiento
			al_draw_bitmap_region(Image,0,0,20,20,500,500,0);
			al_draw_bitmap_region(Image,movimiento*20,0,20,20,alien->pos_x,alien->pos_y,0);
			//indique que se mueva
			

			if(alien->stage != 3){
				alien->cond = 1;
				
			}else{
				alien->pos_x = 100;
				alien->pos_y = 100;
				//nachosFunc();
			}


			al_flip_display();
			redraw = false;
		}
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(Image);
	free(alien);
	//al_destroy_path(path);

	return 0;
}