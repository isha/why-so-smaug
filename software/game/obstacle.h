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
	POTION
} ObstacleType;

typedef struct{
	ObstacleType type;
	int coordinates_x;
	int coordinates_y;
} Obstacle;

void set_coordinates(Obstacle obstacle, int x, int y){
	obstacle.coordinates_x = x;
	obstacle.coordinates_y = y;
}

#endif /* OBSTACLE_H_ */
