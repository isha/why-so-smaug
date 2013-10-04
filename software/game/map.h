#ifndef MAP_H_
#define MAP_H_

#include "obstacle.h"
#include "vga.h"

int pixel_colors[RESOLUTION_X][RESOLUTION_Y];

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
	add_obstacle(map);

	return map;
}

void add_obstacle (Map * map) {
	Obstacle * current = map->obstacles;
	while (current->next != NULL) current = current->next;
	current->next = construct_obstacle(POTION, 150, 200);
}

void next_map(Map * map){
	printf("\nNext map");

	Obstacle * prev = NULL;
	Obstacle * current = map->obstacles;

	while(current != NULL) {
		if (current->coordinates_x < 320 && current->coordinates_y < 240) {
			current->coordinates_x += map->velocity;
			printf("\nType %d, position %d, %d", current->type, current->coordinates_x, current->coordinates_y);

			prev = current;
		} else {
			if (current == map->obstacles) {
				map->obstacles = current->next;
				prev = NULL;
			}
			else prev->next = current->next;
		}
		current = current->next;
	}
}

void update_screen(Map * map) {
	Obstacle * current = map->obstacles;
	while (current != NULL) {
		pixel_colors[current->coordinates_x][current->coordinates_y] = (current->type == WALL) ? 20 : 30;

		current = current->next;
	}
}

#endif /* LCD_H_ */
