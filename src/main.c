/*
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../include/alienLogic.h"
#include <pthread.h>
#include <stdbool.h>
#include "linked_list.h"
#include "../include/bridge.h"
//#include <json-c/json.h>

#define WIDTH 1080
#define HEIGHT 720
#define imgAlienNormal "imgAlien.png"
#define imgAlienBeta   "imgAlien1.png"
#define imgAlienAlfa   "imgAlien2.png"
#define imgCastle      "imgCastle.png"
#define imgMode        "imgmode.png"

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
#define BTNMODE_X 520
#define BTNMODE_Y 10
const float FPS = 60;


pthread_mutex_t lock; 

struct List *listaAliens;



void* thread_alien(void *arg){
	Alien *alien = (Alien *) arg;
    initAlien(alien,&lock,listaAliens);
}


Alien* create_alien(int type){
	Alien *alien = (Alien *) malloc(sizeof(Alien));
	alien->type          = type % 3;
	int numero = rand() % 3; 
	printf("%d",numero);
	if(type < 3){
		alien->route         = numero;
	}else if(type < 6){
		alien->route         = numero + 3;
	}
	pthread_t hilo;
	pthread_create(&hilo,NULL,&thread_alien,(void *) alien);
	//sleep (0.01);
	addLast(listaAliens,&alien);
	//Alien *alito;
	//printf("%d\n", listaAliens->length);
	//getAt(listaAliens,0,(void *) &alito);
	//printf("%f\n",alito->pos_x);
	return alien;
}

bool detectColition(float x1,float y1, int ancho1, int alto1, float x2, float y2,int ancho2,int alto2){
	return ((y1 <= y2 + alto2)&&(y2<= y1 +alto1)&&(x1 <= x2+ancho2)&&(x2<= x1 +ancho1));
}

void detectButtonPresed(int mouse_x,int mouse_y, int *game_mode){
	if(!(*game_mode)){
		if(detectColition(BTN0_X,BTN0_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(0);
		if(detectColition(BTN1_X,BTN1_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(1);
		if(detectColition(BTN2_X,BTN2_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(2);
		if(detectColition(BTN3_X,BTN3_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(3);
		if(detectColition(BTN4_X,BTN4_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(4);
		if(detectColition(BTN5_X,BTN5_Y,20,20,mouse_x,mouse_y,0,0))   create_alien(5);
	}
	if(detectColition(BTNMODE_X,BTNMODE_Y,40,40,mouse_x,mouse_y,0,0))
		(*game_mode) = ((* game_mode) + 1) % 2;
}
void eliminarAlien(int mouse_x,int mouse_y){
	int cantidad = 0;
	Alien *alien_to_delete;
	while(cantidad < listaAliens->length){
		getAt(listaAliens,cantidad,(void *) &alien_to_delete);
		if(detectColition(alien_to_delete->pos_x,alien_to_delete->pos_y, 20, 20, mouse_x, mouse_y,0,0)){
			alien_to_delete->stage = 9;
			removeAt(listaAliens,cantidad);
		}
		cantidad ++;
	}
	//removeAt
}

int main(int argc, char *argv[])
{

	struct Bridge east_bridge ;
	struct Bridge center_bridge ;
	struct Bridge west_bridge ;
	//Create alien structure
	//pthread_t hilo;
    //char * alien_type = "n";
    //
	int movimiento      = 0;
	int contador        = 0;
	int mouse_timer     = 0;
	int cant_alien      = 0;
	int game_mode       = 0;
	
	initBridge(&east_bridge,EAST_BRIDGE_CONFIG_FILENAME);
	initBridge(&center_bridge,CENTRAL_BRIDGE_CONFIG_FILENAME);
	initBridge(&west_bridge,WEST_BRIDGE_CONFIG_FILENAME);

	//create new alien
	listaAliens = createList(sizeof(Alien *));
	Alien * alien_to_show;
	

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_MOUSE_STATE state;
	ALLEGRO_BITMAP *Image = NULL;
	ALLEGRO_BITMAP *Image2 = NULL;
	ALLEGRO_BITMAP *Image3 = NULL;
	ALLEGRO_BITMAP *ImageCastle = NULL;
	ALLEGRO_BITMAP *ImageMode = NULL;
	
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
	ImageMode   = al_load_bitmap(imgMode);
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
			    printf("Mouse position: (%d, %d)\n", state.x, state.y);
			    detectButtonPresed(state.x,state.y,&game_mode);
			    mouse_timer = 30;
			    pthread_mutex_lock(&lock);
			    eliminarAlien(state.x,state.y);
			    pthread_mutex_unlock(&lock);
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

			if(game_mode == 0)
				al_draw_bitmap_region(ImageMode,0,40,40,40,BTNMODE_X,BTNMODE_Y,0);
			else
				al_draw_bitmap_region(ImageMode,0,0,40,40,BTNMODE_X,BTNMODE_Y,0);
			

			cant_alien = 0;
			pthread_mutex_lock(&lock);
			while(cant_alien < listaAliens->length){
				getAt(listaAliens,cant_alien,(void *) &alien_to_show);
				if(alien_to_show->type == 0){
					al_draw_bitmap_region(Image,movimiento*20,0,20,20,alien_to_show->pos_x,alien_to_show->pos_y,0);
				}
				if(alien_to_show->type == 1){
					al_draw_bitmap_region(Image2,movimiento*20,0,20,20,alien_to_show->pos_x,alien_to_show->pos_y,0);
				}
				if(alien_to_show->type == 2){
					al_draw_bitmap_region(Image3,movimiento*20,0,20,20,alien_to_show->pos_x,alien_to_show->pos_y,0);
				}

				if(alien_to_show->stage != 5){
					alien_to_show->cond = 1;
				}else{
					///add to the queue
					if(!alien_to_show->queue){
						alien_to_show->queue = 1;
						if(alien_to_show->route == 0){
							insertAlienInNorth(&east_bridge,alien_to_show);
						}
						if(alien_to_show->route == 1){
							insertAlienInNorth(&center_bridge,alien_to_show);
						}
						if(alien_to_show->route == 2){
							insertAlienInNorth(&west_bridge,alien_to_show);
						}
						if(alien_to_show->route == 3){
							insertAlienInSouth(&east_bridge,alien_to_show);

						}
						if(alien_to_show->route == 4){
							insertAlienInSouth(&center_bridge,alien_to_show);
						}
						if(alien_to_show->route == 5){
							insertAlienInSouth(&west_bridge,alien_to_show);
						}
					}
					if(alien_to_show->isSelected){
						alien_to_show->cond = 1;
					}

					alien_to_show->pos_x = 200;
					alien_to_show->pos_y = 200;
					//nachosFunc();
				}
				cant_alien++;
			}
			pthread_mutex_unlock(&lock);
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
	al_destroy_bitmap(ImageMode);

	return 0;
}