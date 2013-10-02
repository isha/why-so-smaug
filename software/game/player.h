#ifndef PLAYER_H_
#define PLAYER_H_

#define MAX_HEALTH 3
#define X_COMPONENT 0
#define Y_COMPONENT 1

typedef struct{
	char* screen_name;
	int score;
	int time;
	int health;
	
	int velocity_x;
	int velocity_y;
	int position_x;
	int position_y;
} Player;

void construct_player(Player* player, char* screen_name, int score, int time, int health, int velocity_x, int velocity_y, int position_x, int position_y) {
	int i = 0;
	player->screen_name = screen_name;
	player->score = score;
	player->time = time;
	player->health = health;
	player->velocity_x = velocity_x;
	player->velocity_y = velocity_y;
	player->position_x = position_x;
	player->position_y = position_y;
}

void set_position(Player* player) {
	player->position_x += player->velocity_x;
	player->position_y -= player->velocity_y;
	//TODO: constrain position to screen
}

void change_speed(Player* player, int velocity_x, int velocity_y ) {
	player->velocity_x = velocity_x;
	player->velocity_y = velocity_y;
}

Player player1;

#endif /* PLAYER_H_ */