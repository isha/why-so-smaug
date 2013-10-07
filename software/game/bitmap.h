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
	short int *data;
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
	short int color_data, red, blue, green;
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
		for(x = 0; x < bitmap->width; x++){
			color_data = sdcard_read(fp);
			red = ((color_data & 0xE0) >> 5) * 32 / 8;
			green = ((color_data & 0x1C) >> 2) * 64 / 8;
			blue = ((color_data & 0x03)) * 32 / 4;
			bitmap->data[(int)index+x] = (red << 11 ) + (green << 5 ) + (blue);
		}

	sdcard_fclose(fp);
	return bitmap;
}

Bitmap * bitmap_for(int type) {
	Bitmap * bitmap;
	switch(type) {
	case 0:
		if (bitmap_to_obstacle_type[0] == NULL) {
			bitmap = load_bitmap("chest.bmp");
			bitmap_to_obstacle_type[0] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[0];
		break;
	case 1:
		if (bitmap_to_obstacle_type[1] == NULL) {
			bitmap = load_bitmap("chest.bmp");
			bitmap_to_obstacle_type[1] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[1];
		break;
	case 2:
		if (bitmap_to_obstacle_type[2] == NULL) {
			bitmap = load_bitmap("star.bmp");
			bitmap_to_obstacle_type[2] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[2];
		break;
	case 3:
		if (bitmap_to_obstacle_type[3] == NULL) {
			bitmap = load_bitmap("star.bmp");
			bitmap_to_obstacle_type[3] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[3];
		break;
	case 4:
		if (bitmap_to_obstacle_type[4] == NULL) {
			bitmap = load_bitmap("pylon.bmp");
			bitmap_to_obstacle_type[4] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[4];
		break;
	case 5:
		if (bitmap_to_obstacle_type[5] == NULL) {
			bitmap = load_bitmap("pylon.bmp");
			bitmap_to_obstacle_type[5] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[5];
		break;
	default:
		if (bitmap_to_obstacle_type[0] == NULL) {
			bitmap = load_bitmap("star.bmp");
			bitmap_to_obstacle_type[0] = bitmap;
		}
		else
			bitmap = bitmap_to_obstacle_type[0];
		break;
	}
	return bitmap;
}

#endif /* BITMAP_H_ */
