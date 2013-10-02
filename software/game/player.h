#ifndef PLAYER_H_
#define PLAYER_H_

#define MAX_HEALTH 3

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

void construct_player(Player* player, char* screen_name, int score, int time, int health, int velocity_x, int velocity_y, int coordinates_x, int coordinates_y) {
	player->screen_name = screen_name;
	player->score = score;
	player->time = time;
	player->health = health;
	player->velocity_x = velocity_x;
	player->velocity_y = velocity_y;
	player->coordinates_x = coordinates_x;
	player->coordinates_y = coordinates_y;
}

void set_coordinates(Player* player) {
	player->coordinates_x += player->velocity_x;
	player->coordinates_y -= player->velocity_y;
	//TODO: constrain coordinates to screen
}

void change_speed(Player* player, int velocity_x, int velocity_y ) {
	player->velocity_x = velocity_x;
	player->velocity_y = velocity_y;
}

Player player1;

#endif /* PLAYER_H_ */