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
#include <stdbool.h>
#include "linked_list.h"
//#include <json-c/json.h>

#define WIDTH 1080
#define HEIGHT 720
#define imgAlienNormal "imgAlien.png"
#define imgAlienBeta   "imgAlien1.png"
#define imgAlienAlfa   "imgAlien2.png"
#define imgCastle      "imgCastle.png"

#define CASTLE0_X 30
#define CASTLE0_Y 30
#define CASTLE1_X 950
#define CASTLE1_Y 590
#define BTN0_X 30
#define BTN0_Y 150
#define BTN1_X 60
#define BTN1_Y 150
#define BTN2_X 90
#define BTN2_Y 150
#define BTN3_X 950
#define BTN3_Y 550
#define BTN4_X 980
#define BTN4_Y 550
#define BTN5_X 1010
#define BTN5_Y 550
const float FPS = 60;


pthread_mutex_t lock; 

struct List *listaAliens;

void* thread_alien(void *arg){
	Alien *alien = (Alien *) arg;
    initAlien(alien,&lock,listaAliens);
}


Alien* create_alien(){
	Alien *alien = (Alien *) malloc(sizeof(Alien));
	pthread_t hilo;
	pthread_create(&hilo,NULL,&thread_alien,(void *) alien);
	sleep (0.01);
	addLast(listaAliens,&alien);
	Alien *alito;
	printf("%d\n", listaAliens->length);
	getAt(listaAliens,0,(void *) &alito);
	printf("%f\n",alito->pos_x);
	return alien;
}

bool detectColition(float x1,float y1, int ancho1, int alto1, float x2, float y2,int ancho2,int alto2){
	return ((y1 <= y2 + alto2)&&(y2<= y1 +alto1)&&(x1 <= x2+ancho2)&&(x2<= x1 +ancho1));
}


int main(int argc, char *argv[])
{

	//Create alien structure
	//pthread_t hilo;
    //char * alien_type = "n";
    //
	int movimiento      = 0;
	int contador        = 0;
	int mouse_timer     = 0;
	int cant_alien      = 0;
	

	//create new alien
	listaAliens = createList(sizeof(Alien *));
	Alien * alien = create_alien();
	Alien * alien_to_show;
	
	//Alien *alien = (Alien *) malloc(sizeof(Alien));
	//pthread_t hilo;
	//pthread_create(&hilo,NULL,&initAlien,(void *) alien);

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_MOUSE_STATE state;
	ALLEGRO_BITMAP *Image = NULL;
	ALLEGRO_BITMAP *Image2 = NULL;
	ALLEGRO_BITMAP *Image3 = NULL;
	ALLEGRO_BITMAP *ImageCastle = NULL;
	bool running = true;
	bool redraw = true;

	// Initialize allegro
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro.\n");
		return 1;
	}
	//init the mouse driver
	al_install_mouse();

	//Init image reader
	al_init_image_addon();

	//Read the bitmap from the image .png
	Image = al_load_bitmap(imgAlienNormal);
	Image2 = al_load_bitmap(imgAlienBeta);
	Image3 = al_load_bitmap(imgAlienAlfa);
	ImageCastle = al_load_bitmap(imgCastle);
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
		if(mouse_timer == 0){
			al_get_mouse_state(&state);
			if (state.buttons & 1) {
			    /* Primary (e.g. left) mouse button is held. */
			    printf("Mouse position: (%d, %d)\n", state.x, state.y);
			    mouse_timer = 30;
			    create_alien();
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
			al_clear_to_color(al_map_rgb(50, 50, 50));
			//al_draw_bitmap(Image, 0, 0, 0);
			//al_draw_bitmap(Image, 10, 10, 0);

			//hilo principal que controla el movimiento
			
			
			//free(alito);
			cant_alien = 0;
			while(cant_alien < listaAliens->length){
				 
				getAt(listaAliens,cant_alien,(void *) &alien_to_show);
				al_draw_bitmap_region(Image,movimiento*20,0,20,20,alien_to_show->pos_x,alien_to_show->pos_y,0);

				//if(alien_to_show->stage != 3){
					pthread_mutex_lock(&lock);
					alien_to_show->cond = 1;
					pthread_mutex_unlock(&lock);
				//}else{
				//	alien_to_show->pos_x = 100;
				//	alien_to_show->pos_y = 100;
					//nachosFunc();
				//}
				cant_alien++;

			}
			//Draw buttoms
			al_draw_bitmap_region(Image,60,40,20,20,BTN0_X, BTN0_Y, 0);
			al_draw_bitmap_region(Image2,60,40,20,20,BTN1_X, BTN1_Y, 0);
			al_draw_bitmap_region(Image3,60,40,20,20,BTN2_X, BTN2_Y, 0);

			al_draw_bitmap_region(Image,60,40,20,20,BTN3_X, BTN3_Y, 0);
			al_draw_bitmap_region(Image2,60,40,20,20,BTN4_X, BTN4_Y, 0);
			al_draw_bitmap_region(Image3,60,40,20,20,BTN5_X, BTN5_Y, 0);

			//draw castle
			al_draw_bitmap(ImageCastle, CASTLE0_X, CASTLE0_Y, 0);
			al_draw_bitmap(ImageCastle, CASTLE1_X, CASTLE1_Y, ALLEGRO_FLIP_HORIZONTAL);


			al_draw_bitmap_region(Image,0,0,20,20,930,650,0);
			//al_draw_bitmap_region(Image,movimiento*20,0,20,20,alien->pos_x,alien->pos_y,0);
			//indique que se mueva
			

			
			if(mouse_timer != 0){
				mouse_timer --;
			}

			al_flip_display();
			redraw = false;
		}
	} 

	// Clean up
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(Image);
	al_destroy_bitmap(Image2);
	al_destroy_bitmap(Image3);
	al_destroy_bitmap(ImageCastle);
	free(alien);
	//al_destroy_path(path);

	return 0;
}