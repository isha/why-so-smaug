#ifndef PLAYER_H_
#define PLAYER_H_
#include "vga.h"
#include "bitmap.h"

#define MAX_HEALTH 10
#define START_TIME 0
#define START_SCORE 0
#define START_COORDINATE_X 160
#define START_COORDINATE_Y 120

#define switches (volatile char *) SWITCHES_BASE
#define leds (char *) LEDS_BASE
#define keys (volatile char *) BUTTONS_BASE

extern bool buttons[4] = {false, false, false, false};
extern void * bitmap_for_player_type[4];

static long int new_timestamp;
static long int old_timestamp;

typedef struct{
	char* screen_name;
	int score;
	int time;
	int health;
	int old_coordinates_x;
	int old_coordinates_y;
	int coordinates_x;
	int coordinates_y;
	int type;
} Player;

typedef enum {
	PLAYER1,
	PLAYER2
} PlayerType;

Player * construct_player(char* screen_name, PlayerType type) {
	Player * player = malloc(sizeof(Player));
	player->screen_name = screen_name;
	player->score = START_SCORE;
	player->time = START_TIME;
	player->health = MAX_HEALTH;
	player->old_coordinates_x = START_COORDINATE_X;
	player->old_coordinates_y = START_COORDINATE_Y;
	player->coordinates_x = START_COORDINATE_X;
	player->coordinates_y = START_COORDINATE_Y;
	player->type = type;
	return player;
}

void next_player (Player * player) {
	new_timestamp = alt_timestamp();
	if(buttons[0]) {
		if (abs(player->coordinates_x - player->old_coordinates_x)<20) {
			player->old_coordinates_x = player->coordinates_x;
			player->coordinates_x+=10;
		} else {
			player->coordinates_x = player->old_coordinates_x;
		}
	}
	if(buttons[1]) {
		if (abs(player->coordinates_y - player->old_coordinates_y)<20) {
			player->old_coordinates_y = player->coordinates_y;
			player->coordinates_y+=10;
		} else {
			player->coordinates_y = player->old_coordinates_y;
		}
	}
	if(buttons[2]) {
		if (abs(player->coordinates_y - player->old_coordinates_y)<20) {
			player->old_coordinates_y = player->coordinates_y;
			player->coordinates_y-=10;
		} else {
			player->coordinates_y = player->old_coordinates_y;
		}
	}
	if(buttons[3]) {
		if (abs(player->coordinates_x - player->old_coordinates_x)<20) {
			player->old_coordinates_x = player->coordinates_x;
			player->coordinates_x-=10;
		} else {
			player->coordinates_x = player->old_coordinates_x;
		}
	}

	constrain_player_movement(player);
	draw_player(player);
}



Bitmap * get_player_bitmap(PlayerType type) {
	Bitmap * bitmap;

	switch(type) {
	case PLAYER1:
		if (bitmap_for_player_type[PLAYER1] == NULL) {
			bitmap = load_bitmap("p1.bmp");
			bitmap_for_player_type[PLAYER1] = bitmap;
		}
		else
			bitmap = bitmap_for_player_type[PLAYER1];
		break;
	case PLAYER2:
		if (bitmap_for_player_type[PLAYER2] == NULL) {
			bitmap = load_bitmap("p2.bmp");
			bitmap_for_player_type[PLAYER2] = bitmap;
		}
		else
			bitmap = bitmap_for_player_type[PLAYER2];
		break;
		printf("\nPlayer2");
	default:
		if (bitmap_for_player_type[PLAYER1] == NULL) {
			bitmap = load_bitmap("p1.bmp");
			bitmap_for_player_type[PLAYER1] = bitmap;
		}
		else {
			bitmap = bitmap_for_player_type[PLAYER1];
		}
		break;
		printf("\nDefault");
	}

	return bitmap;
}

void draw_player(Player* player) {
	int i = 0;
	int j = 0;
	Bitmap * player_bitmap = get_player_bitmap(player->type);
	for (i=0; i < player_bitmap->height; i++) {
		for (j=0; j < player_bitmap->width; j++) {
			if (!(player->coordinates_x+j > RESOLUTION_X || player->coordinates_x+j < 0 ||
					player->coordinates_y+i > RESOLUTION_Y || player->coordinates_y+i < 0)){
				if (player_bitmap->data[i*player_bitmap->width + j])
					pixel_colors[player->coordinates_x+j][player->coordinates_y+i] = player_bitmap->data[i*player_bitmap->width + j];
			}
		}
	}
}

void erase_player(Player* player) {
	int i = 0;
	int j = 0;

	Bitmap * player_bitmap = get_player_bitmap(player->type);

	for (i=0; i < player_bitmap->height; i++) {
		for (j=0; j < player_bitmap->width; j++) {
			if (!(player->coordinates_x+j > RESOLUTION_X || player->coordinates_x+j < 0 ||
					player->coordinates_y+i > RESOLUTION_Y || player->coordinates_y+i < 0)){
				pixel_colors[player->coordinates_x+j][player->coordinates_y+i] = 0;
			}
		}
	}
}

void constrain_player_movement(Player* player) {
	if(player->coordinates_x < 10) player->coordinates_x = 10;
	if(player->coordinates_y < 30) player->coordinates_y = 30;
	if(player->coordinates_x >= (RESOLUTION_X - 10)) player->coordinates_x = RESOLUTION_X - 10;
	if(player->coordinates_y >= (RESOLUTION_Y - 70)) player->coordinates_y = RESOLUTION_Y - 70;
}

void read_buttons() {
	int new_button_values = IORD_8DIRECT(keys, 0);
	buttons[0] = ((new_button_values & 0x01) == 0x01);
	buttons[1] = ((new_button_values & 0x02) == 0x02);
	buttons[2] = ((new_button_values & 0x04) == 0x04);
	buttons[3] = ((new_button_values & 0x08) == 0x08);
}

#endif /* PLAYER_H_ */
