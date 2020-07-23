#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <json-c/json.h>
#include "queue.h"
#include "alien.h"

#define WEST_BRIDGE_CONFIG_FILENAME 	"west_bridge_config.json"
#define CENTRAL_BRIDGE_CONFIG_FILENAME  "central_bridge_config.json"
#define EAST_BRIDGE_CONFIG_FILENAME  	"east_bridge_config.json"

#define ROUND_ROBIN 						0
#define PRIORITY							1
#define SJF 								2
#define LOTTERY								3
#define FIFO 								4

#define Y 									0
#define SEMAPHORES							1
#define SURVIVAL							2

#define NORTH 								0
#define SOUTH 								1

struct Bridge
{
	int length;
	int capacity;
	int transit_control_algorithm;
	int scheduling_algorithm;
	int ordered_aliens_per_side;
	int Y;
	int M;
	int N;
	int aliens_crossing;
	struct Queue *north_aliens_queue;
	struct Queue *south_aliens_queue;
};

void initBridge(struct Bridge *bridge, const char *filename);

void insertAlienInNorth(struct Bridge *bridge, struct Alien *alien);

void insertAlienInSouth(struct Bridge *bridge, struct Alien *alien);

void sortBridgeSide(struct Bridge *bridge, struct Queue **queue);

int priority(const void *a, const void *b);

int shortestJobFirst(const void *a, const void *b, const void *c);

#endif
