#include <stdio.h>
#include "vga.h"
#include "lcd.h"

#define switches (volatile char *) 0x0004000
#define leds (char *) 0x0004010

alt_up_character_lcd_dev * char_lcd_dev;
alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev *char_buffer;


void init() {
	initialize_lcd(&char_lcd_dev);
	initialize_vga(&pixel_buffer, &char_buffer);
}

int main(void)
{
	printf("Woohoo!\n");

	init();

	alt_up_character_lcd_string(char_lcd_dev, "Welcome to");
	char second_row[] = "the DE2 board\0";
	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, 0, 1);
	alt_up_character_lcd_string(char_lcd_dev, second_row);

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 240, 320, 0, 0xFFFF, 0);

	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "EECE 381", 60, 50);

	while(1) *leds = *switches;
	return 0;
}

