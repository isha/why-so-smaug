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
#define debounce_interval 250000

#define COLLISION_DAMAGE 10

bool buttons[4] = {false, false, false, false};
// 0 is not pressed

void init() {
	initialize_vga();
	initialize_sdcard();
	alt_timestamp_start();
}

int main(void)
{
	int old_timestamp = 0;
	int new_timestamp = 0;
	int i,j;

	alt_timestamp_start();

	init();
	bool game_on = true;

	char * name = "Elf friend";
	Player *player1 = construct_player(name);
	printf("\nPlayer created with name %s", player1->screen_name);

	Map *map = construct_map();
	printf("\nConstructed Map with velocity %d", map->velocity);

	test_collision(player1, map);

	printf("\nRock n' Roll\n");
/*
	while(player1->health > 0) {
		read_buttons();
		new_timestamp = alt_timestamp();
//		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//		alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x00, player1->coordinates_x, player1->coordinates_y);
//		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
//		if(new_timestamp >= old_timestamp + debounce_interval) {
//			move_player(player1);
//			old_timestamp = new_timestamp;
//		}
//		alt_up_pixel_buffer_dma_draw(pixel_buffer, 0x740, player1->coordinates_x, player1->coordinates_y);
//		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);

		check_collision(player1, map);

		// Calculations
		next_map(map);

		// Modify screen buffer matrix
		update_screen(map);

		// Character buffer
		text(map, player1);

		// Draw to screen
		draw_to_screen();

	}
*/

//	while (player1->health != 0 && game_on) {
//		// Calculations
//		next_map(map);
//
//		// Modify screen buffer matrix
//		update_screen(map);
//
//		// Character buffer
//		text(map, player1);
//
//		// Draw to screen
//		draw_to_screen();
//	}

	/* Game over screen displayed.
	 * Press any key to continue
	 * then move to scoreboard
	 */
	game_over();

	return 0;
}

void check_collision(Player* player, Map* map) {

	// point at the first obstacle
	Obstacle * current_obstacle = map->obstacles;

	// check if coordinates of player are same as obstacles on map for all obstacles
	while(current_obstacle != NULL) {
		printf("\nChecking linked list...");
		if (player->coordinates_x == current_obstacle->coordinates_x && player->coordinates_y == current_obstacle->coordinates_y) {
			on_collide(player, current_obstacle->type);
			printf("\nCollision has occurred!");
			return; // if the coordinates of player are same as obstacle, collision occurred
		}
		current_obstacle = current_obstacle->next; // start looking at the next obstacle
	}

	return;	// no collisions were detected
}

void on_collide(Player* player, ObstacleType type ) {
	switch(type) {
	case WALL:
		damage_health(player, 10);
		break;
	case POTION:
		set_health(player, MAX_HEALTH);
		break;
	case WENCH:
		damage_health(player, 50);
		break;
	case CHEST:
		add_score(player, 100);
		break;
	case COIN:
		add_score(player, 10);
		break;
	case POISON:
		damage_health(player, 50);
		break;
	default:
		// do nothing
		break;
	}
}

void damage_health(Player* player, int damage) {
	player->health = player->health - damage;
	printf ("\nPlayer has been damaged %d health", damage);
}

void set_health(Player* player, int new_health) {
	player-> health = new_health;
	printf ("\nYour health has returned to %d", new_health);
}

void add_score(Player* player, int score) {
	player->score = player->score + score;
	printf ("\nYou gained %d points", score);
}

void test_collision(Player* player, Map* map) {
	player->coordinates_x = 100;
	player->coordinates_y = 240;

	check_collision(player, map);

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
