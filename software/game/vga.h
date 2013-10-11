/*
 * vga.h
 *
 *  Created on: 2013-09-18
 *      Author: n1v7
 */

#ifndef VGA_H_
#define VGA_H_

#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "io.h"

#define RESOLUTION_X 320
#define RESOLUTION_Y 240

extern int pixel_colors[RESOLUTION_X][RESOLUTION_Y];
int old_pixel_colors[RESOLUTION_X][RESOLUTION_Y];

alt_up_pixel_buffer_dma_dev* pixel_buffer;
extern alt_up_char_buffer_dev *char_buffer;

unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (320 * 240 * 2);

void initialize_vga() {
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	// Set the 2nd buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
	pixel_buffer_addr2);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(char_buffer);
}

void draw_to_screen() {
	int i, j;
	for (i=0; i<RESOLUTION_X; i++){
		for (j=0; j<RESOLUTION_Y; j++){
			if (pixel_colors[i][j] != old_pixel_colors[i][j]) {
				alt_up_pixel_buffer_dma_draw(pixel_buffer, pixel_colors[i][j], i, j);
				old_pixel_colors[i][j] = pixel_colors[i][j];
				alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
				alt_up_pixel_buffer_dma_draw(pixel_buffer, pixel_colors[i][j], i, j);
			}

		}
	}

}



#endif /* VGA_H_ */
