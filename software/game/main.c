#include <stdio.h>
#include <stdbool.h>
#include "sdcard.h"
#include "vga.h"
#include "timer.h"
#include "map.h"
#include "obstacle.h"
#include "player.h"
#include "player_controls.h"
#include "bitmap.h"
#include "audio.h"
#include "screen_name.h"
#include "io.h"

#define HEALTH_BAR_P1_START_X 33
#define HEALTH_BAR_P2_START_X 218
#define HEALTH_BAR_START_Y 12
#define HEALTH_BAR_UNIT_SIZE 2
#define HEALTH_BAR_HEIGHT 2

void check_collision(Player* player, Map* map);
void on_collide(Player* player, Obstacle* obstacle, Obstacle* prev, Map* map );
void damage_health(Player* player, int damage);
void add_health(Player* player, int add_by);
void add_score(Player* player, int score);
void update_health_bar(Player* player1, Player* player2);
void draw_initial_health_bar(void);

void init() {
	initialize_vga();
	initialize_sdcard();
	alt_timestamp_start();
	initialize_audio();
	start_audio();
}

int main(void)
{
	char * player_name;
	char * message;
	int time = 0;
	init();

	// Selection
	player_name = get_screen_name();
	Player *player1 = construct_player(player_name, PLAYER1);
	player_name = get_screen_name();
	Player *player2 = construct_player(player_name, PLAYER2);

	bool game_on = true;
	Map *map = construct_map();

	initial_screen(map);

	// Main game play
	while (game_on && !(player1->health <= 0 || player2->health <= 0)) {
		// User input
		read_buttons();
		read_switches();

		update_health_bar(player1,player2);

		// Update positions
		next_map(map);

		erase_player(player1);
		erase_player(player2);

		// Update screen
		update_screen(map);
		next_player(player1);
		next_player(player2);

		// Draw EVERYTHING
		draw_to_screen();

		check_collision(player1, map);
		check_collision(player2, map);

		message = "Game on!";



		// All text on screen
		text(time, player1, player2, message);

		time++;
	}

	game_over();

	while(1);

	return 0;
}

void check_collision(Player* player, Map* map) {
  // point at the first obstacle
  Obstacle * current_obstacle = map->obstacles;
  Bitmap * player_bitmap = get_player_bitmap(player->type);
  Obstacle * prev = NULL;

  // check if coordinates of player are same as obstacles on map for all obstacles
  while(current_obstacle != NULL) {
    Bitmap * obstacle_bitmap = get_bitmap(current_obstacle->type);
    // Check if current_obstacle corner 1 in player space [ () ] {where square = player and round = obstacle}
    if ( (current_obstacle->coordinates_x >= player->coordinates_x && current_obstacle->coordinates_x <= (player->coordinates_x + player_bitmap->width)) &&
    	 (current_obstacle->coordinates_y >= player->coordinates_y && current_obstacle->coordinates_y <= (player->coordinates_y + player_bitmap->height)) )
    	on_collide(player, current_obstacle, prev, map);
    else if ( ((current_obstacle->coordinates_x + obstacle_bitmap->width) >= player->coordinates_x && (current_obstacle->coordinates_x + obstacle_bitmap->width) <= (player->coordinates_x + player_bitmap->width)) &&
    	 (current_obstacle->coordinates_y >= player->coordinates_y && current_obstacle->coordinates_y <= (player->coordinates_y + player_bitmap->height)) )
    	on_collide(player, current_obstacle, prev, map);
    else if ( (current_obstacle->coordinates_x >= player->coordinates_x && current_obstacle->coordinates_x <= (player->coordinates_x + player_bitmap->width)) &&
    	 ((current_obstacle->coordinates_y + obstacle_bitmap->height) >= player->coordinates_y && (current_obstacle->coordinates_y + obstacle_bitmap->height) <= (player->coordinates_y + player_bitmap->height)) )
    	on_collide(player, current_obstacle, prev, map);
    else if ( ((current_obstacle->coordinates_x + obstacle_bitmap->width) >= player->coordinates_x && (current_obstacle->coordinates_x + obstacle_bitmap->width) <= (player->coordinates_x + player_bitmap->width)) &&
    	 ((current_obstacle->coordinates_y + obstacle_bitmap->height) >= player->coordinates_y && (current_obstacle->coordinates_y + obstacle_bitmap->height) <= (player->coordinates_y + player_bitmap->height)) )
    	on_collide(player, current_obstacle, prev, map);
    prev = current_obstacle;
    current_obstacle = current_obstacle->next; // start looking at the next obstacle
  }
  return;  // no collisions were detected
}

void remove_obstacle(Map* map, Obstacle * prev, Obstacle * current_obstacle){
	int i, j;
	Bitmap * bitmap = get_bitmap(current_obstacle->type);
	// Erase old
	for (i=0; i<bitmap->height; i++) {
		for (j= -1*MAP_VELOCITY; j<bitmap->width; j++) {
			if (!(current_obstacle->old_coordinates_x+j > RESOLUTION_X || current_obstacle->old_coordinates_x+j < 0 ||
					current_obstacle->old_coordinates_y+i > RESOLUTION_Y || current_obstacle->old_coordinates_y+i < 0)){
				pixel_colors[current_obstacle->old_coordinates_x+j][current_obstacle->old_coordinates_y+i] = 0;
			}
		}
	}

	if (prev == NULL){
		map->obstacles = current_obstacle->next;
		free(current_obstacle);
	}
	else{
		prev->next = current_obstacle->next;
		free(current_obstacle);
	}
}

void on_collide(Player* player, Obstacle* obstacle, Obstacle* prev, Map* map ) {
	switch(obstacle->type) {
	case PYLON:
		damage_health(player, 1);
		break;
	case PLANE:
		damage_health(player, 1);
		break;
	case CHEST:
		add_score(player, 100);
		remove_obstacle(map, prev, obstacle);
		break;
	case STAR:
		add_score(player, 50);
		remove_obstacle(map, prev, obstacle);
		break;
	case BURGER:
		add_health(player, 1);
		remove_obstacle(map, prev, obstacle);
		break;
	default:
		// do nothing
		break;
	}
}

void draw_initial_health_bar(){
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P1_START_X, HEALTH_BAR_START_Y,
			HEALTH_BAR_P1_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
			0x740, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P2_START_X, HEALTH_BAR_START_Y,
			HEALTH_BAR_P2_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
			0x740, 0);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P1_START_X, HEALTH_BAR_START_Y,
			HEALTH_BAR_P1_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
			0x740, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P2_START_X, HEALTH_BAR_START_Y,
			HEALTH_BAR_P2_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
			0x740, 0);
}

void update_health_bar(Player * player1, Player * player2) {
	if (player1->old_health != player1->health) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P1_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P1_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x000, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P1_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P1_START_X + HEALTH_BAR_UNIT_SIZE*(player1->health), HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x740, 0);
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P1_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P1_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x000, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P1_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P1_START_X + HEALTH_BAR_UNIT_SIZE*(player1->health), HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x740, 0);
		player1->old_health = player1->health;
	}
	if (player2->old_health != player2->health) {
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P2_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P2_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x000, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P2_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P2_START_X + HEALTH_BAR_UNIT_SIZE*(player2->health), HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x740, 0);
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P2_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P2_START_X + HEALTH_BAR_UNIT_SIZE*MAX_HEALTH, HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x000, 0);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer, HEALTH_BAR_P2_START_X, HEALTH_BAR_START_Y,
				HEALTH_BAR_P2_START_X + HEALTH_BAR_UNIT_SIZE*(player2->health), HEALTH_BAR_START_Y + HEALTH_BAR_HEIGHT,
				0x740, 0);
		player2->old_health = player2->health;
	}
}
