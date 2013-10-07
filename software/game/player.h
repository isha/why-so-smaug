#ifndef PLAYER_H_
#define PLAYER_H_

#include "screen_name.h"

#define MAX_HEALTH 10
#define START_TIME 0
#define START_SCORE 0
#define START_VELOCITY_X 0
#define START_VELOCITY_Y 0
#define START_COORDINATE_X 120
#define START_COORDINATE_Y 0

typedef struct{
	char* screen_name;
	int score;
	int time;
	int health;
	
	int velocity_x;
	int velocity_y;
	int coordinates_x;
	int coordinates_y;
} Player;

Player * construct_player(char* screen_name) {
	Player * player = malloc(sizeof(Player));
	strcpy(player->screen_name, screen_name);
	player->score = START_SCORE;
	player->time = START_TIME;
	player->health = MAX_HEALTH;
	player->velocity_x = START_VELOCITY_X;
	player->velocity_y = START_VELOCITY_Y;
	player->coordinates_x = START_COORDINATE_X;
	player->coordinates_y = START_COORDINATE_Y;
	return player;
}

void set_player_coordinates(Player* player) {
	// Y-component is subtracted to abastract array convention (y=0 at top, y=240 at bottom)
	player->coordinates_x += player->velocity_x;
	player->coordinates_y -= player->velocity_y;
	//TODO: constrain coordinates to screen
}

void change_velocity(Player* player, int velocity_x, int velocity_y ) {
	player->velocity_x = velocity_x;
	player->velocity_y = velocity_y;
}

#endif /* PLAYER_H_ */
