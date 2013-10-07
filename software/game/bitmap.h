/*
 * Adapted from http://www.brackeen.com/vga/source/djgpp20
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "obstacle.h"
#include "sdcard.h"

typedef struct {
	int width;
	int height;
	char *data;
} Bitmap;

Bitmap * bitmap_to_obstacle_type[6];

void skip(FILE *fp, int chars) {
   int i;
   for (i=0; i<chars; i++)
      sdcard_read(fp);
}

int sdcard_read_int (FILE *fp) {
	unsigned char arr[4];
	int i;
	for (i=0; i<4; i++) {
		arr[i] = sdcard_read(fp);
	}
	int num = (arr[3] << 24) + (arr[2] << 16) + (arr[1] << 8) + arr[0];
	return num;
}

Bitmap * load_bitmap(char *file) {
	Bitmap * bitmap = malloc(sizeof(Bitmap));
	FILE *fp;
	fp = sdcard_fopen(file, 0);
	if (fp == -1) {
		printf("\nError opening bmp file %s", file);
	} else if (fp == -2) {
		printf("\bmp file %s already open", file);
	}
	if (sdcard_read(fp)!='B' || sdcard_read(fp)!='M')
	{
		sdcard_fclose(fp);
		printf("%s is not a bitmap file.\n",file);
	}

	long index;
	int num_colors;
	int x;

	skip(fp,16);
	bitmap->width = sdcard_read_int(fp);
	bitmap->height = sdcard_read_int(fp);
	skip(fp,20);
	num_colors = sdcard_read_int(fp);
	skip(fp,4);

	/* assume we are working with an 8-bit file */
	if (num_colors == 0) num_colors = 256;

	/* try to allocate memory */
	if (( bitmap->data = (char *) malloc((int)(bitmap->width*bitmap->height))) == NULL) {
		sdcard_fclose(fp);
		printf("\nError allocating memory for file %s", file);
	}

	/* Ignore the palette information for now */
	skip(fp, num_colors*4);

	/* read the bitmap */
	for(index = (bitmap->height-1)*bitmap->width; index >= 0; index -= bitmap->width)
		for(x = 0; x < bitmap->width; x++)
		  bitmap->data[(int)index+x]=(char)sdcard_read(fp);

	sdcard_fclose(fp);
	return bitmap;
}

Bitmap * bitmap_for(ObstacleType type) {
	Bitmap * bitmap;
	switch(type) {
	case POTION:
		if (bitmap_to_obstacle_type[POTION] == NULL) {
			bitmap = load_bitmap("chest.bmp");
			bitmap_to_obstacle_type[POTION] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[POTION];
		break;
	case CHEST:
		if (bitmap_to_obstacle_type[CHEST] == NULL) {
			bitmap = load_bitmap("chest.bmp");
			bitmap_to_obstacle_type[CHEST] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[CHEST];
		break;
	case COIN:
		if (bitmap_to_obstacle_type[COIN] == NULL) {
			bitmap = load_bitmap("star.bmp");
			bitmap_to_obstacle_type[COIN] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[COIN];
		break;
	case WENCH:
		if (bitmap_to_obstacle_type[WENCH] == NULL) {
			bitmap = load_bitmap("star.bmp");
			bitmap_to_obstacle_type[WENCH] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[WENCH];
		break;
	case POISON:
		if (bitmap_to_obstacle_type[POISON] == NULL) {
			bitmap = load_bitmap("pylon.bmp");
			bitmap_to_obstacle_type[POISON] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[POISON];
		break;
	default:
		if (bitmap_to_obstacle_type[POTION] == NULL) {
			bitmap = load_bitmap("star.bmp");
			bitmap_to_obstacle_type[POTION] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[POTION];
		break;
	}
	return bitmap;
}

#endif /* BITMAP_H_ */
