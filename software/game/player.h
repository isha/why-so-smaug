#ifndef PLAYER_H_
#define PLAYER_H_

#include "vga.h"
#include "bitmap.h"
#include "player_controls.h"
#include "player_def.h"

#define MAX_HEALTH 10
#define START_TIME 0
#define START_SCORE 0
#define START_COORDINATE_X 160
#define START_COORDINATE_Y 100

extern void * bitmap_for_player_type[4];

Player * construct_player(char* screen_name, PlayerType type) {
	Player * player = malloc(sizeof(Player));
	player->screen_name = screen_name;
	player->score = START_SCORE;
	player->time = START_TIME;
	player->health = MAX_HEALTH;
	player->coordinates_x = START_COORDINATE_X;
	player->coordinates_y = type == PLAYER1 ? START_COORDINATE_Y : START_COORDINATE_Y + 50;
	player->type = type;
	return player;
}

void next_player (Player * player) {
	if(move_right(player)) {
			player->coordinates_x+=10;
	}
	if(move_down(player)) {
			player->coordinates_y+=10;
	}
	if(move_up(player)) {
			player->coordinates_y-=10;
	}
	if(move_left(player)) {
			player->coordinates_x-=10;
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
	default:
		if (bitmap_for_player_type[PLAYER1] == NULL) {
			bitmap = load_bitmap("p1.bmp");
			bitmap_for_player_type[PLAYER1] = bitmap;
		}
		else {
			bitmap = bitmap_for_player_type[PLAYER1];
		}
		break;
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
				pixel_colors[player->coordinates_x+j][player->coordinates_y+i] = initial_pixel_colors[player->coordinates_x+j][player->coordinates_y+i];
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


void damage_health(Player* player, int damage) {
  player->health = player->health - damage;
}

void add_health(Player* player, int add_by) {
	if (player->health <= MAX_HEALTH - 1){
		  player->health = player->health + add_by;
	}
	else{
		  add_score(player, 25);
	}
}

void add_score(Player* player, int score) {
  player->score = player->score + score;
}
#endif /* PLAYER_H_ */
