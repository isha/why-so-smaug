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
#include "audio.h"

#define switches (volatile char *) SWITCHES_BASE
#define leds (char *) LEDS_BASE
#define keys (volatile char *) BUTTONS_BASE
#define debounce_interval 250000

bool buttons[4] = {false, false, false, false};
// 0 is not pressed

void init() {
	initialize_vga();
	initialize_sdcard();
	alt_timestamp_start();
	//initialize_audio();
	//start_audio();
}

int main(void)
{
	int new_timestamp, old_timestamp;
	int previous_x_position, previous_y_position;
	init();
	bool game_on = true;
	Map *map = construct_map();

	// Selection
	char * name = "Elf";
	Player *player1 = construct_player(name);
	printf("Player created with name %s\n", player1->screen_name);

//	printf("\nRock n' Roll\n");
//	while(1) {
//		read_buttons();
//		new_timestamp = alt_timestamp();
//		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//		alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x00, player1->coordinates_x, player1->coordinates_y);
//		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//		if(new_timestamp >= old_timestamp + debounce_interval) {
//			move_player(player1);
//			old_timestamp = new_timestamp;
//		}
//		alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x740, player1->coordinates_x, player1->coordinates_y);
//		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//	}

	// Main game play
	initial_screen(map);
	initial_screen(map);
	while (player1->health > 0 && game_on) {
		previous_x_position = player1->coordinates_x;
		previous_y_position = player1->coordinates_y;

		read_buttons();
		new_timestamp = alt_timestamp();
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//		next_map(map);
//		update_screen(player1, map);
//		text(map, player1);
//		draw_to_screen();
		erase_previous_player_position(player1);

		constrain_player_movement(player1);

		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
		if(new_timestamp >= old_timestamp + debounce_interval) {
			move_player(player1);
			old_timestamp = new_timestamp;
		}
		draw_player(player1);


		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//		player1->health--;
	}

	// Game over
	game_over();

	return 0;
}

void read_buttons() {
	int *new_button_values = (int *) keys;
	buttons[0] = ((new_button_values[0] & 0x01) == 0x01);
	buttons[1] = ((new_button_values[0] & 0x02) == 0x02);
	buttons[2] = ((new_button_values[0] & 0x04) == 0x04);
	buttons[3] = ((new_button_values[0] & 0x08) == 0x08);
	*leds = (buttons[3] << 3) + (buttons[2] << 2) + (buttons[1] << 1) + (buttons[0]);
}

void move_player(Player* player) {
	if(buttons[0]) player->coordinates_x++;
	if(buttons[1]) player->coordinates_y++;
	if(buttons[2]) player->coordinates_y--;
	if(buttons[3]) player->coordinates_x--;
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
