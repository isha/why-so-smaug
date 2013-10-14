#ifndef MAP_H_
#define MAP_H_

#include <stdio.h>
#include "obstacle.h"
#include "vga.h"
#include "player.h"

int pixel_colors[RESOLUTION_X][RESOLUTION_Y];
int initial_pixel_colors[RESOLUTION_X][RESOLUTION_Y];
alt_up_char_buffer_dev *char_buffer;
void * bitmap_for_obstacle_type[6];
void * bitmap_for_player_type[4];

/* Following info will usually come from a map structure in the sdcard */
#define MAP_VELOCITY 10
#define DIFFICULTY 6
/**/


typedef struct{
	Bitmap * bitmap;
	int velocity;
	Obstacle * obstacles;
} Map;

Map * construct_map() {
	Map * map = malloc(sizeof(Map));
	map->bitmap = load_bitmap("bwin.bmp");
	map->velocity = MAP_VELOCITY;

	int d1 = alt_timestamp()%6;
	int d2 = 320;
	int d3 = alt_timestamp()%110 + 30;

	map->obstacles = construct_obstacle(d1, d2, d3);
	return map;
}

void add_obstacle (Map * map) {
	Obstacle * current = map->obstacles;
	while (current->next !=  NULL) {
		current = current->next;
	}
	int time = alt_timestamp();
	if (time <= 0){
		alt_timestamp_start();
		time = alt_timestamp();
	}
	int d1 = time%6;
	int d2 = 320;
	int d3 = time%110 + 30;

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
				if (prev) {
					prev->next = current->next;
					free(current);
					current = prev->next;
				} else {
					current = current->next;
				}
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
	if (alt_timestamp() % 10 < DIFFICULTY && number_of_obstacles < 10) {
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

	Bitmap * bitmap = load_bitmap("moon.bmp");
	for (i=0; i<bitmap->width; i++)
		for(j=0; j<bitmap->height; j++)
			pixel_colors[i+250][j+10] = bitmap->data[i*(bitmap->width)+j];

	for (i=0; i<RESOLUTION_X; i++) {
		for(j=0; j<RESOLUTION_Y; j++) {
			initial_pixel_colors[i][j] = pixel_colors[i][j];
		}
	}
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
				if (!(current->old_coordinates_x+j > RESOLUTION_X || current->old_coordinates_x+j < 0 ||
						current->old_coordinates_y+i > RESOLUTION_Y || current->old_coordinates_y+i < 0)){
					pixel_colors[current->old_coordinates_x+j][current->old_coordinates_y+i] = 0;
				}

				if (!(current->coordinates_x+j > RESOLUTION_X || current->coordinates_x+j < 0 ||
						current->coordinates_y+i > RESOLUTION_Y || current->coordinates_y+i < 0)){
					if (bitmap->data[i*bitmap->width + j])
						pixel_colors[current->coordinates_x+j][current->coordinates_y+i] = bitmap->data[i*bitmap->width + j];
				}
			}
		}
		current = current->next;
	}
}

void text (int time, Player * player1, Player * player2, char * message) {
	alt_up_char_buffer_clear(char_buffer);

	char str1[50], str2[30], str3[30], str4[30];

	sprintf(str1, "%s", player1->screen_name);
	sprintf(str2, "Score: %d", player1->score);
	sprintf(str3, "Health: %d", player1->health);
	sprintf(str4, "Time: %d", time);

	alt_up_char_buffer_string(char_buffer, str1, 1, 1);
	alt_up_char_buffer_string(char_buffer, str2, 1, 2);
	alt_up_char_buffer_string(char_buffer, str3, 1, 3);
	alt_up_char_buffer_string(char_buffer, str4, 1, 5);

	alt_up_char_buffer_string(char_buffer, " VS ", 45, 1);

	sprintf(str1, "%s", player2->screen_name);
	sprintf(str2, "Score: %d", player2->score);
	sprintf(str3, "Health: %d", player2->health);

	alt_up_char_buffer_string(char_buffer, str1, 50, 1);
	alt_up_char_buffer_string(char_buffer, str2, 50, 2);
	alt_up_char_buffer_string(char_buffer, str3, 50, 3);

	if (message) {
		alt_up_char_buffer_string(char_buffer, message, 30, 60);
	}
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
