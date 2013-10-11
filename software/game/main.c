#include <stdio.h>
#include <stdbool.h>
#include "sdcard.h"
#include "vga.h"
#include "timer.h"
#include "map.h"
#include "obstacle.h"
#include "player.h"
#include "bitmap.h"
#include "audio.h"

void init() {
	initialize_vga();
	initialize_sdcard();
	alt_timestamp_start();
	initialize_audio();
	start_audio();
}

int main(void)
{
	char * player_name;
	init();

	// Selection
	player_name = get_screen_name();
	Player *player1 = construct_player(player_name);

	bool game_on = true;
	Map *map = construct_map();

	initial_screen(map);

	// Main game play
	while (game_on && player1->time < 500) {
		// User input
		read_buttons();

		// Update positions
		next_map(map);

		// Update screen
		update_screen(map);
		next_player(player1);

		// All text on screen
		text(map, player1->time, player1->screen_name);

		// Draw EVERYTHING
		draw_to_screen();

		player1->time++;
	}

	// Game over
	game_over();

	while(1);

	return 0;
}
