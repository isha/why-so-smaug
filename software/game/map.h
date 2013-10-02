#ifndef MAP_H_
#define MAP_H_

#include "obstacle.h"

typedef struct{
	int graphics[640][240];
	int velocity;
	char *phrases[5];
	Obstacle obstacles [];
} Map;

#endif /* LCD_H_ */
