/*
 * obstacle.h
 *
 *  Created on: 2013-10-01
 *      Author: Kevin
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

typedef enum {
	WALL,
	POTION,
	WENCH,
	CHEST,
	COIN,
	POISION
} ObstacleType;

typedef struct{
	ObstacleType type;
	int coordinates_x;
	int coordinates_y;
	struct Obstacle * next;
} Obstacle;

Obstacle * construct_obstacle(ObstacleType type, int xpos, int ypos) {
	Obstacle * obstacle;
	obstacle->type = type;
	obstacle->coordinates_x = xpos;
	obstacle->coordinates_y = ypos;
	obstacle->next = NULL;
	return obstacle;
}

#endif /* OBSTACLE_H_ */
