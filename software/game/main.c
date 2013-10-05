#include <stdio.h>
#include <stdbool.h>
#include "sdcard.h"
#include "vga.h"
#include "lcd.h"
#include "timer.h"
#include "map.h"
#include "obstacle.h"
#include "player.h"
#include "bitmap.h"

#define switches (volatile char *) SWITCHES_BASE
#define leds (char *) LEDS_BASE
#define keys (volatile char *) BUTTONS_BASE

void init() {
	initialize_vga();
	alt_timestamp_start();
}

int main(void)
{
	init();
	bool game_on = true;

	char * name = "Elf friend";
	Player *player1 = construct_player(name);
	printf("\nPlayer created with name %s", player1->screen_name);

	Map *map = construct_map();
	printf("\nConstructed Map with velocity %d", map->velocity);

	while (player1->health != 0 && game_on) {
		// usleep(1000000);

		// Calculations
		next_map(map);

		// Modify screen buffer matrix
		update_screen(map);

		// Draw to screen
		draw_to_screen();
	}
	return 0;
}

void test() {
	int *buttons;
	bool bouncing = false;

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 240, 320, 0, 0xFFFF, 0);

	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "EECE 381", 60, 50);

	timer_test();
	sdcard_test();

	while(1){
		*leds = *switches;

		buttons = (int *) keys;
		if (buttons[0] == 8 && !bouncing){
			bouncing = true;
			alt_up_character_lcd_string(char_lcd_dev, "3");
		}
		else if (buttons[0] == 4 && !bouncing){
			bouncing = true;
			alt_up_character_lcd_string(char_lcd_dev, "2");
		}
		else if (buttons[0] == 2 && !bouncing){
			bouncing = true;
			alt_up_character_lcd_string(char_lcd_dev, "1");
		}
		else if (buttons[0] == 1 && !bouncing){
			bouncing = true;
			alt_up_character_lcd_string(char_lcd_dev, "0");
		}
		else if (buttons[0] == 0){
			bouncing = false;
		}
	}
}
