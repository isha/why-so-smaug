#ifndef MAP_H_
#define MAP_H_

#include <stdio.h>
#include "obstacle.h"
#include "vga.h"
#include "player.h"

int pixel_colors[RESOLUTION_X][RESOLUTION_Y];
alt_up_char_buffer_dev *char_buffer;
void * bitmap_for_obstacle_type[6];

/* Following info will usually come from a map structure in the sdcard */
#define PHRASES_COUNT 5
#define MAP_VELOCITY 10
#define DIFFICULTY 6
char* phrases[PHRASES_COUNT] = {"Pow", "Nice Job", "You Suck", "", ""};
/**/

typedef struct{
	Bitmap * bitmap;
	int velocity;
	char *phrases[PHRASES_COUNT];
	Obstacle * obstacles;
} Map;

Map * construct_map() {
	Map * map = malloc(sizeof(Map));
//	map->bitmap = load_bitmap("bwin.bmp");
	map->velocity = MAP_VELOCITY;
	map->obstacles = construct_obstacle(alt_timestamp()%6, alt_timestamp()%100 + 100, alt_timestamp()%40 + 100);
	return map;
}

void add_obstacle (Map * map) {
	Obstacle * current = map->obstacles;
	while (current->next !=  NULL) {
		current = current->next;
	}
	current->next = construct_obstacle(alt_timestamp()%6, alt_timestamp()%100 + 100, alt_timestamp()%40 + 100);
}

void next_map(Map * map){
	int number_of_obstacles = 0;

	// Update existing Obstacle positions
	Obstacle * prev = NULL;
	Obstacle * current = map->obstacles;

	while(current != NULL) {
		Bitmap * bitmap = get_bitmap(current->type);
		if ((current->coordinates_x > (320 + bitmap->width)  || current->coordinates_x < (0 - bitmap->width)) || (current->coordinates_y > 240 || current->coordinates_y < 0)) {
			if (current == map->obstacles) {
				map->obstacles = current->next;
				prev = NULL;
				free(current);
				current = map->obstacles;
			}
			else {
				prev->next = current->next;
				free(current);
				current = prev->next;
			}
		} else {
			current->coordinates_x -= map->velocity;
			prev = current;
			current = current->next;
		}
		number_of_obstacles++;
	}
	printf("\nNumber of obstacles = %d", number_of_obstacles);

	// Generate more obstacles if needed
	if (alt_timestamp() % 10 < DIFFICULTY && number_of_obstacles < 6) {
		printf("\nAdding obstacle");
		add_obstacle(map);
	}
}

void update_screen(Map * map) {
	int i, j, k;

	// Main map
	for (i=0, k=0; i<map->bitmap->height; i++)
		for(j=0; j<map->bitmap->width; j++, k++)
			pixel_colors[i][j] = map->bitmap->data[k];

	// Obstacles
	Obstacle * current = map->obstacles;
	while (current != NULL) {
		Bitmap * bitmap = get_bitmap(current->type);
		for (i=0; i<bitmap->height; i++) {
			for (j=0; j<bitmap->width; j++) {
				if (bitmap->data[i*bitmap->width + j]) pixel_colors[current->coordinates_x+i][current->coordinates_y+j] = bitmap->data[i*bitmap->width + j];
			}
		}
		current = current->next;
	}
}

void text (Map * map, Player * player) {
	alt_up_char_buffer_clear(char_buffer);

	char str[30];
	sprintf(str, "Health: %d", player->health);

	alt_up_char_buffer_string(char_buffer, str, 2, 15);
}


void game_over() {
	int i, j;
	for (i=0; i<RESOLUTION_X; i++)
		for(j=0; j<RESOLUTION_Y; j++) {
			pixel_colors[i][j] = 0;
		}
	draw_to_screen();
	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "G A M E   O V E R !!!", 30, 30);
}
#endif /* LCD_H_ */
