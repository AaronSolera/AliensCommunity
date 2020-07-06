#include "bridge.h"

void initBridge(struct Bridge *bridge, char *filename)
{
	FILE *fp;
	char buffer[1024];

	struct json_object *parsed_json;

	struct json_object *length;
	struct json_object *capacity;
	struct json_object *transit_control_algorithm;
	struct json_object *scheduling_algorithm;
	struct json_object *ordered_aliens_per_side;

	fp = fopen(filename, "r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "length", &length);
	json_object_object_get_ex(parsed_json, "capacity", &capacity);
	json_object_object_get_ex(parsed_json, "transit_control_algorithm", &transit_control_algorithm);
	json_object_object_get_ex(parsed_json, "scheduling_algorithm", &scheduling_algorithm);
	json_object_object_get_ex(parsed_json, "ordered_aliens_per_side", &ordered_aliens_per_side);

	bridge->length = json_object_get_int(length);
	bridge->capacity = json_object_get_int(capacity);
	bridge->transit_control_algorithm = json_object_get_int(transit_control_algorithm);
	bridge->scheduling_algorithm = json_object_get_int(scheduling_algorithm);
	bridge->ordered_aliens_per_side = json_object_get_int(ordered_aliens_per_side);

	//initQueue(bridge->north_aliens_stack, sizeof(struct Alien));
	//initQueue(bridge->south_aliens_stack, sizeof(struct Alien));
}
