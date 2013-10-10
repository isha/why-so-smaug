#ifndef PLAYER_H_
#define PLAYER_H_

#include "screen_name.h"
#include "vga.h"
#include "bitmap.h"

#define MAX_HEALTH 10
#define START_TIME 0
#define START_SCORE 0
#define START_VELOCITY_X 0
#define START_VELOCITY_Y 0
#define START_COORDINATE_X 160
#define START_COORDINATE_Y 120
#define debounce_interval 250000

#define switches (volatile char *) SWITCHES_BASE
#define leds (char *) LEDS_BASE
#define keys (volatile char *) BUTTONS_BASE

extern bool buttons[4] = {false, false, false, false};
static long int new_timestamp;
static long int old_timestamp;

static int old_coordinates_x = START_COORDINATE_X;
static int old_coordinates_y = START_COORDINATE_Y;

typedef struct{
	Bitmap * bitmap;
	char* screen_name;
	int score;
	int time;
	int health;
	
	int old_coordinates_x;
	int old_coordinates_y;
	int coordinates_x;
	int coordinates_y;
} Player;

Player * construct_player(char* screen_name) {
	Player * player = malloc(sizeof(Player));
	//strcpy(player->screen_name, screen_name);
	player->score = START_SCORE;
	player->time = START_TIME;
	player->health = MAX_HEALTH;
	old_coordinates_x = START_COORDINATE_X;
	old_coordinates_y = START_COORDINATE_Y;
	player->coordinates_x = START_COORDINATE_X;
	player->coordinates_y = START_COORDINATE_Y;
	player->bitmap = load_bitmap("star.bmp");
	return player;
}

void next_player (Player * player) {
	new_timestamp = alt_timestamp();
	erase_previous_player_position(player);
	if(new_timestamp >= old_timestamp + debounce_interval) {
		if(buttons[0]) {
			if (abs(player->coordinates_x - old_coordinates_x)<20) {
				old_coordinates_x = player->coordinates_x;
				player->coordinates_x+=15;
			} else {
				player->coordinates_x = old_coordinates_x;
			}
		}
		if(buttons[1]) {
			if (abs(player->coordinates_y - old_coordinates_y)<20) {
				old_coordinates_y = player->coordinates_y;
				player->coordinates_y+=15;
			} else {
				player->coordinates_y = old_coordinates_y;
			}
		}
		if(buttons[2]) {
			if (abs(player->coordinates_y - old_coordinates_y)<20) {
				old_coordinates_y = player->coordinates_y;
				player->coordinates_y-=15;
			} else {
				player->coordinates_y = old_coordinates_y;
			}
		}
		if(buttons[3]) {
			if (abs(player->coordinates_x - old_coordinates_x)<20) {
				old_coordinates_x = player->coordinates_x;
				player->coordinates_x-=15;
			} else {
				player->coordinates_x = old_coordinates_x;
			}
		}
		old_timestamp = new_timestamp;
	}
	constrain_player_movement(player);

	draw_player(player);
	old_timestamp = new_timestamp;

}

void erase_previous_player_position(Player* player) {
	pixel_colors[player->coordinates_x+1][player->coordinates_y] = 0x00;
	pixel_colors[player->coordinates_x+1][player->coordinates_y-1] = 0x00;
	pixel_colors[player->coordinates_x+1][player->coordinates_y+1] = 0x00;
	pixel_colors[player->coordinates_x][player->coordinates_y] = 0x00;
	pixel_colors[player->coordinates_x][player->coordinates_y-1] = 0x00;
	pixel_colors[player->coordinates_x][player->coordinates_y+1] = 0x00;
	pixel_colors[player->coordinates_x-1][player->coordinates_y] = 0x00;
	pixel_colors[player->coordinates_x-2][player->coordinates_y] = 0x00;
	pixel_colors[player->coordinates_x-3][player->coordinates_y] = 0x00;
	pixel_colors[player->coordinates_x-4][player->coordinates_y] = 0x00;
	pixel_colors[player->coordinates_x-4][player->coordinates_y-1] = 0x00;
}

void draw_player(Player* player) {
	pixel_colors[player->coordinates_x+1][player->coordinates_y] = 0x740;
	pixel_colors[player->coordinates_x+1][player->coordinates_y-1] = 0x740;
	pixel_colors[player->coordinates_x+1][player->coordinates_y+1] = 0x740;
	pixel_colors[player->coordinates_x][player->coordinates_y] = 0x740;
	pixel_colors[player->coordinates_x][player->coordinates_y-1] = 0x740;
	pixel_colors[player->coordinates_x][player->coordinates_y+1] = 0x740;
	pixel_colors[player->coordinates_x-1][player->coordinates_y] = 0x740;
	pixel_colors[player->coordinates_x-2][player->coordinates_y] = 0x740;
	pixel_colors[player->coordinates_x-3][player->coordinates_y] = 0x740;
	pixel_colors[player->coordinates_x-4][player->coordinates_y] = 0x740;
	pixel_colors[player->coordinates_x-4][player->coordinates_y-1] = 0x740;
}

void constrain_player_movement(Player* player) {
	if(player->coordinates_x < 10) player->coordinates_x = 10;
	if(player->coordinates_y < 30) player->coordinates_y = 30;
	if(player->coordinates_x >= (RESOLUTION_X - 10)) player->coordinates_x = RESOLUTION_X - 10;
	if(player->coordinates_y >= (RESOLUTION_Y - 70)) player->coordinates_y = RESOLUTION_Y - 70;
}

void read_buttons() {
	int *new_button_values = (int *) keys;
	buttons[0] = ((new_button_values[0] & 0x01) == 0x01);
	buttons[1] = ((new_button_values[0] & 0x02) == 0x02);
	buttons[2] = ((new_button_values[0] & 0x04) == 0x04);
	buttons[3] = ((new_button_values[0] & 0x08) == 0x08);
	*leds = (buttons[3] << 3) + (buttons[2] << 2) + (buttons[1] << 1) + (buttons[0]);
}


#endif /* PLAYER_H_ */
