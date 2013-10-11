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
#define MAP_VELOCITY 10
#define DIFFICULTY 4
/**/

typedef struct{
	Bitmap * bitmap;
	int velocity;
	Obstacle * obstacles;
} Map;

Map * construct_map() {
	Map * map = malloc(sizeof(Map));
	map->bitmap = load_bitmap("bsum.bmp");
	map->velocity = MAP_VELOCITY;

//	int d1 = alt_timestamp()%6;
//	int d2 = 320;
//	int d3 = alt_timestamp()%55 + 30;
//
//	map->obstacles = construct_obstacle(d1, d2, d3);
	return map;
}

void add_obstacle (Map * map) {
	Obstacle * current = map->obstacles;
	while (current->next !=  NULL) {
		current = current->next;
	}
	int d1 = alt_timestamp()%6;
	int d2 = 320;
	int d3 = alt_timestamp()%55 + 30;

	current->next = construct_obstacle(d1, d2, d3);
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
			current->old_coordinates_x = current->coordinates_x;
			current->coordinates_x -= map->velocity;
			prev = current;
			current = current->next;
		}
		number_of_obstacles++;
	}

	// Generate more obstacles if needed
	if (alt_timestamp() % 10 < DIFFICULTY && number_of_obstacles < 6 && prev->coordinates_x <= 200) {
		add_obstacle(map);
	}
}

void initial_screen(Map * map) {
	int i, j;
	for (i=0; i<map->bitmap->height; i++)
		for(j=0; j<map->bitmap->width; j++)
			pixel_colors[j][i+150] = map->bitmap->data[i*(map->bitmap->width)+j];

	free(map->bitmap->data);
	free(map->bitmap);

	Bitmap * bitmap = load_bitmap("sun.bmp");
	for (i=0; i<bitmap->width; i++)
		for(j=0; j<bitmap->height; j++)
			pixel_colors[i+250][j+10] = bitmap->data[i*(bitmap->width)+j];

	draw_to_screen();

}

void update_screen(Map * map) {
	int i, j;

	// Obstacles
	Obstacle * current = map->obstacles;
	while (current != NULL) {
		Bitmap * bitmap = get_bitmap(current->type);
		// Erase old
		for (i=0; i<bitmap->height; i++) {
			for (j=0; j<bitmap->width; j++) {
				pixel_colors[current->old_coordinates_x+j][current->old_coordinates_y+i] = 0;
			}
		}
		for (i=0; i<bitmap->height; i++) {
			for (j=0; j<bitmap->width; j++) {
				if (bitmap->data[i*bitmap->width + j]) pixel_colors[current->coordinates_x+j][current->coordinates_y+i] = bitmap->data[i*bitmap->width + j];
			}
		}
		current = current->next;
	}
}

void text (Map * map, Player * player) {
	alt_up_char_buffer_clear(char_buffer);

	char str1[30], str2[30], str3[30];
	sprintf(str1, "Time: %d", player->time);
	sprintf(str2, "Health: %d", player->health);
	sprintf(str3, "%s", player->screen_name);

	alt_up_char_buffer_string(char_buffer, str1, 1, 3);
	alt_up_char_buffer_string(char_buffer, str2, 1, 2);
	alt_up_char_buffer_string(char_buffer, str3, 1, 1);
}


void game_over() {
	Bitmap * bmp = load_bitmap("game.bmp");

	int i, j;
	for (i=0; i<RESOLUTION_X; i++)
		for(j=0; j<RESOLUTION_Y; j++) {
			pixel_colors[j][i] = bmp->data[i*bmp->width + j];
		}
	draw_to_screen();
}
#endif /* LCD_H_ */
