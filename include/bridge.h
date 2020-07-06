#ifndef BRIDGE_H
#define BRIDGE_H

#include <stdio.h>
#include <json-c/json.h>
#include "queue.h"

#define WEST_BRIDGE_CONFIG_FILENAME 	"west_bridge_config.json"
#define CENTRAL_BRIDGE_CONFIG_FILENAME  "central_bridge_config.json"
#define EAST_BRIDGE_CONFIG_FILENAME  	"east_bridge_config.json"

struct Bridge
{
	int length;
	int capacity;
	int transit_control_algorithm;
	int scheduling_algorithm;
	int ordered_aliens_per_side;
	struct Queue *north_aliens_queue;
	struct Queue *south_aliens_queue;
};

void initBridge(struct Bridge *bridge, char *filename);

#endif
