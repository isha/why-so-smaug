/*
 * Adapted from http://www.brackeen.com/vga/source/djgpp20
 */

#ifndef BITMAP_H_
#define BITMAP_H_

typedef struct {
	int width;
	int height;
	char *data;
} Bitmap;

void skip(FILE *fp, int chars) {
   int i;
   for (i=0; i<chars; i++)
      fgetc(fp);
}

void load_bitmap(char *file, Bitmap *bitmap) {
	FILE *fp;

	if ((fp = fopen(file,"rb")) == NULL) {
		printf("\nError opening bmp file %s", file);
	}
	if (fgetc(fp)!='B' || fgetc(fp)!='M') {
		fclose(fp);
		printf("\n%s is not a bitmap file", file);
	}

	long index;
	int num_colors;
	int x;

	skip(fp,16);
	fread(&(bitmap->width), sizeof(int), 1, fp);
	// skip(fp,2);
	fread(&(bitmap->height), sizeof(int), 1, fp);
	skip(fp,20);
	fread(&num_colors, sizeof(int), 1, fp);
	skip(fp,4);

	/* assume we are working with an 8-bit file */
	if (num_colors == 0) num_colors = 256;


	/* try to allocate memory */
	if (( bitmap->data = (char *) malloc((int)(bitmap->width*bitmap->height))) == NULL) {
		fclose(fp);
		printf("\nError allocating memory for file %s", file);
	}

	/* Ignore the palette information for now */
	skip(fp, num_colors*4);

	/* read the bitmap */
	for(index = (bitmap->height-1)*bitmap->width; index >= 0; index -= bitmap->width)
		for(x = 0; x < bitmap->width; x++)
		  bitmap->data[(int)index+x]=(char)fgetc(fp);

	fclose(fp);
}
#endif /* BITMAP_H_ */
