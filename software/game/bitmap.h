/*
 * Adapted from http://www.brackeen.com/vga/source/djgpp20
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "obstacle.h"
#include "sdcard.h"
#include <stdlib.h>

typedef struct {
	int width;
	int height;
	short int *data;
} Bitmap;

void skip(short int fp, int chars) {
   int i;
   for (i=0; i<chars; i++)
      sdcard_read(fp);
}

int sdcard_read_int (short int fp) {
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
	short int fp;
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
	printf("\nWidth = %d, height = %d, num_colors = %d", bitmap->width, bitmap->height, num_colors);

	/* assume we are working with an 8-bit file */
	if (num_colors == 0) num_colors = 256;

	/* try to allocate memory */
	if (( bitmap->data = (short int *) malloc((int)(bitmap->width*bitmap->height))) == NULL) {
		sdcard_fclose(fp);
		printf("\nError allocating memory for file %s", file);
	}

	/* Ignore the palette information for now */
	skip(fp, num_colors*4);

	/* read the bitmap */
	for (index = bitmap->height-1; index >=0; index--) {
		for (x = 0; x < bitmap->width; x++){
			color_data = sdcard_read(fp);
			red = ((color_data & 0xE0) >> 5) * 31 / 7;
			green = ((color_data & 0x1C) >> 2) * 63 / 7;
			blue = ((color_data & 0x03)) * 31 / 3;
			bitmap->data[((bitmap->height-1-index) * bitmap->width) + x] = (red << 11 ) + (green << 5 ) + (blue);
		}
		if (bitmap->width % 4) skip(fp, (4 - bitmap->width % 4));
	}
	sdcard_fclose(fp);
	return bitmap;
}


#endif /* BITMAP_H_ */
