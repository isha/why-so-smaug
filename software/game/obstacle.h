/*
 * obstacle.h
 *
 *  Created on: 2013-10-01
 *      Author: Kevin
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "bitmap.h"

typedef enum {
	PYLON,
	POTION,
	WENCH,
	CHEST,
	STAR,
	POISON
} ObstacleType;

typedef struct{
	ObstacleType type;
	int coordinates_x;
	int coordinates_y;
	struct Obstacle * next;
} Obstacle;

extern void * bitmap_for_obstacle_type[6];

Bitmap * get_bitmap(ObstacleType type) {
	Bitmap * bitmap;

	switch(type) {
	case PYLON:
		if (bitmap_for_obstacle_type[PYLON] == NULL) {
			bitmap = load_bitmap("pylon.bmp");
			bitmap_for_obstacle_type[PYLON] = bitmap;
		}
		else
			bitmap = bitmap_for_obstacle_type[PYLON];
		break;
	case CHEST:
		if (bitmap_for_obstacle_type[CHEST] == NULL) {
			bitmap = load_bitmap("chest.bmp");
			bitmap_for_obstacle_type[CHEST] = bitmap;
		}
		else
			bitmap = bitmap_for_obstacle_type[CHEST];
		break;
	default:
		if (bitmap_for_obstacle_type[STAR] == NULL) {
			bitmap = load_bitmap("star.bmp");
			bitmap_for_obstacle_type[STAR] = bitmap;
		}
		else {
			bitmap = bitmap_for_obstacle_type[STAR];
		}
		break;
	}
	return bitmap;
}

Obstacle * construct_obstacle(ObstacleType type, int xpos, int ypos) {
	Obstacle * obstacle = malloc(sizeof(Obstacle));
	obstacle->type = type;
	obstacle->coordinates_x = xpos;
	obstacle->coordinates_y = ypos;
	obstacle->next = NULL;
	return obstacle;
}

#endif /* OBSTACLE_H_ */
