#ifndef MAP_H_
#define MAP_H_

#include "obstacle.h"
#define PHRASES_COUNT 5

typedef struct{
	int graphics[640][240];
	int velocity;
	char *phrases[PHRASES_COUNT];
	Obstacle obstacles [];
} Map;

void construct_map(Map* map, char* phrases[PHRASES_COUNT], int velocity) {
	Obstacle obstacle1, obstacle2;

	construct_obstacle(&obstacle1, WALL, 0, 0);
	construct_obstacle(&obstacle2, POTION, 10, 10);

	map->velocity = velocity;
	memcpy(map->phrases, phrases, sizeof(phrases));
	map->obstacles[0] = obstacle1;
	map->obstacles[1] = obstacle1;
}

#endif /* LCD_H_ */
