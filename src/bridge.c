#include "bridge.h"

void initBridge(struct Bridge *bridge, const char *filename)
{
	FILE *fp;
	char buffer[1024];

	struct json_object *parsed_json;

	struct json_object *length;
	struct json_object *capacity;
	struct json_object *transit_control_algorithm;
	struct json_object *scheduling_algorithm;
	struct json_object *ordered_aliens_per_side;
	struct json_object *Y;
	struct json_object *M;
	struct json_object *N;

	fp = fopen(filename, "r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "length", &length);
	json_object_object_get_ex(parsed_json, "capacity", &capacity);
	json_object_object_get_ex(parsed_json, "transit_control_algorithm", &transit_control_algorithm);
	json_object_object_get_ex(parsed_json, "scheduling_algorithm", &scheduling_algorithm);
	json_object_object_get_ex(parsed_json, "ordered_aliens_per_side", &ordered_aliens_per_side);
	json_object_object_get_ex(parsed_json, "Y", &Y);
	json_object_object_get_ex(parsed_json, "M", &M);
	json_object_object_get_ex(parsed_json, "N", &N);

	bridge->length = json_object_get_int(length);
	bridge->capacity = json_object_get_int(capacity);
	bridge->transit_control_algorithm = json_object_get_int(transit_control_algorithm);
	bridge->scheduling_algorithm = json_object_get_int(scheduling_algorithm);
	bridge->ordered_aliens_per_side = json_object_get_int(ordered_aliens_per_side);
	bridge->Y = json_object_get_int(Y);
	bridge->M = json_object_get_int(M);
	bridge->N = json_object_get_int(N);

	bridge->aliens_crossing = 0;

	bridge->north_aliens_queue = calloc(1, sizeof(struct Queue));
	initQueue(bridge->north_aliens_queue, sizeof(struct Alien));
	
	bridge->south_aliens_queue = calloc(1, sizeof(struct Queue));
	initQueue(bridge->south_aliens_queue, sizeof(struct Alien));
}


void insertAlienInNorth(struct Bridge *bridge, struct Alien *alien)
{
	enqueue(bridge->north_aliens_queue, alien);

	if ((bridge->scheduling_algorithm == PRIORITY || bridge->scheduling_algorithm == SJF) && 
		 bridge->north_aliens_queue->size <= bridge->ordered_aliens_per_side)
		sortBridgeSide(bridge, &bridge->north_aliens_queue);
}


void insertAlienInSouth(struct Bridge *bridge, struct Alien *alien)
{
	enqueue(bridge->south_aliens_queue, alien);

	if ((bridge->scheduling_algorithm == PRIORITY || bridge->scheduling_algorithm == SJF) && 
		 bridge->south_aliens_queue->size <= bridge->ordered_aliens_per_side)
		sortBridgeSide(bridge, &bridge->south_aliens_queue);
}


void sortBridgeSide(struct Bridge *bridge, struct Queue **queue)
{
	switch (bridge->scheduling_algorithm)
	{
		case PRIORITY:
			bubbleSort2(*queue, bridge->ordered_aliens_per_side, priority);
			break;

		case SJF:
			bubbleSort3(*queue, bridge->ordered_aliens_per_side, shortestJobFirst, (const void *)bridge);
			break;

		default:
			break;
	}
}


int priority(const void *a, const void *b)
{
	int alien1_priority = ((struct Alien *)a)->priority;
	int alien2_priority = ((struct Alien *)b)->priority;

	return alien1_priority > alien2_priority;
}


int shortestJobFirst(const void *a, const void *b, const void *c)
{
	int bridge_length = ((struct Bridge *)c)->length;
	int alien1_speed = ((struct Alien *)a)->speed;
	int alien2_speed = ((struct Alien *)b)->speed;

	return !(bridge_length / alien1_speed > bridge_length / alien2_speed);
}
