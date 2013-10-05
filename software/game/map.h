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
	Map * map = malloc(sizeof(Map));
	map->velocity = MAP_VELOCITY;
	map->obstacles = construct_obstacle(WALL, 240, 100);
	// add_obstacle(map);
	return map;
}

void add_obstacle (Map * map) {
	Obstacle * current = map->obstacles;
	while (current->next !=  NULL) current = current->next;
	current->next = construct_obstacle(POTION, 200, 200);
}

void next_map(Map * map){
	Obstacle * prev = NULL;
	Obstacle * current = map->obstacles;

	while(current != NULL) {
		if ((current->coordinates_x > 320  || current->coordinates_x < 0) || (current->coordinates_y > 240 || current->coordinates_y < 0)) {
			if (current == map->obstacles) {
				map->obstacles = current->next;
				prev = NULL;
			}
			else prev->next = current->next;
		} else {
			current->coordinates_x -= map->velocity;
			printf("\nType %d, position %d, %d", current->type, current->coordinates_x, current->coordinates_y);

			prev = current;

		}
		current = current->next;
	}
}

void update_screen(Map * map) {
	int i, j;

	// Main map
	for (i=0; i<RESOLUTION_X; i++)
		for(j=0; j<RESOLUTION_Y; j++)
			pixel_colors[i][j] = 0;

	// Obstacles
	Obstacle * current = map->obstacles;
	while (current != NULL) {
		for (i=0; i<current->bitmap->width; i++) {
			for (j=0; j<current->bitmap->height; j++) {
				pixel_colors[current->coordinates_x+i][current->coordinates_y+j] = current->bitmap->data[i*current->bitmap->height + j];
			}
		}
		current = current->next;
	}
}

#endif /* LCD_H_ */
