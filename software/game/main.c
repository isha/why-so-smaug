#include <stdio.h>
#include <stdbool.h>
#include "vga.h"
#include "lcd.h"
#include "timer.h"
#include "map.h"
#include "obstacle.h"


#define switches (volatile char *) SWITCHES_BASE
#define leds (char *) LEDS_BASE
#define keys (volatile char *) BUTTONS_BASE


void init() {
	initialize_lcd();
	initialize_vga();
}

int main(void)
{
	init();

	//Player player1;
	Map map;
	Obstacle obstacle1, obstacle2;

	obstacle1.type = WALL;
	set_coordinates(obstacle1, 0, 0);

	obstacle2.type = POTION;
	set_coordinates(obstacle2, 10, 10);

	map.velocity = 10;
	char **phrases = {"Pow", "Nice Job", "You Suck", "", ""};
	memcpy(map.phrases, phrases, sizeof(phrases));

	Obstacle obstacles[2] = {obstacle1, obstacle2};
	memcpy(map.obstacles, obstacles, sizeof(obstacles));
	return 0;
}

void test() {
	int *buttons;
	bool bouncing = false;

	printf("Woohoo!\n");

	init();

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 240, 320, 0, 0xFFFF, 0);

	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "EECE 381", 60, 50);

	timer_test();

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
