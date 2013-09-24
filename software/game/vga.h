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

unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (320 * 240 * 2);

void initialize_vga(alt_up_pixel_buffer_dma_dev **pixel_buffer, alt_up_char_buffer_dev **char_buffer) {
	*pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	alt_up_pixel_buffer_dma_change_back_buffer_address(*pixel_buffer, pixel_buffer_addr1);
	alt_up_pixel_buffer_dma_swap_buffers(*pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(*pixel_buffer));

	// Set the 2nd buffer address
	alt_up_pixel_buffer_dma_change_back_buffer_address(*pixel_buffer,
	pixel_buffer_addr2);
	// Clear both buffers (this makes all pixels black)
	alt_up_pixel_buffer_dma_clear_screen(*pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(*pixel_buffer, 1);

	*char_buffer = alt_up_char_buffer_open_dev("/dev/char_drawer");
	alt_up_char_buffer_init(*char_buffer);
}
#endif /* VGA_H_ */
