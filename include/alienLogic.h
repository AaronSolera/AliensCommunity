#ifndef ALIENLOGIC_H
#define ALIENLOGIC_H

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include <json-c/json.h>
#include <math.h>
#include <stdbool.h>
//#include<pthread.h>


#define ALIEN_CONFIG_PAHT  "./files/Alientxt.json"
#define MAXSTAGESIZE  10
typedef struct Alien{
    float speed; 
    int type;
    float pos_x;
    float pos_y;
    int dir;
    int priority;
    int weight;
    double max_exec_time;
    int route;
    int cond;
    int stage;
    int route_x[MAXSTAGESIZE];  
	int route_y[MAXSTAGESIZE];
	double find_x;
    double find_y;

} Alien;



bool getNewPos(Alien *alien);
void AlienWhile(Alien *alien);
void* initAlien(void *arg);
void   moveAlien(Alien *alien);
void * newAlien (void *arg);
void readAlienConfig(int *alien_speed,double *alien_max_excec_time );


#endif