#ifndef MAP_H_
#define MAP_H_

#include "obstacle.h"

/* Following info will usually come from a map structure in the sdcard */
#define PHRASES_COUNT 5
#define MAP_VELOCITY 10
char* phrases[PHRASES_COUNT] = {"Pow", "Nice Job", "You Suck", "", ""};
/**/

typedef struct{
	int graphics[640][240];
	int velocity;
	char *phrases[PHRASES_COUNT];
	Obstacle * obstacles;
} Map;

Map * construct_map() {
	Map * map;
	map->velocity = MAP_VELOCITY;
	memcpy(map->phrases, phrases, sizeof(phrases));
	map->obstacles = construct_obstacle(WALL, 100, 100);
}

#endif /* LCD_H_ */
