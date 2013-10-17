/*
 * player_def.h
 *
 *  Created on: 2013-10-13
 *      Author: n1v7
 */

#ifndef PLAYER_DEF_H_
#define PLAYER_DEF_H_

typedef struct{
	char* screen_name;
	bool hurt;
	int score;
	int time;
	int health;
	int old_health;
	int coordinates_x;
	int coordinates_y;
	int type;
} Player;

typedef enum {
	PLAYER1,
	PLAYER2
} PlayerType;

#endif /* PLAYER_DEF_H_ */
