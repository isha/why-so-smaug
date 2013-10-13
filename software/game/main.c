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
#include "screen_name.h"
#include "nunchuck.h"

void init() {
	initialize_vga();
	initialize_sdcard();
	alt_timestamp_start();
	initialize_audio();
	start_audio();
}

int main(void) {
}

int old_main(void)
{
	init();

	// Selection
	char * name = get_screen_name();
	Player *player1 = construct_player(name);
	printf("\nPlayer created with name %s\n", player1->screen_name);

	bool game_on = true;
	Map *map = construct_map();

	// Main game play
	initial_screen(map);
	while (game_on ) {
		// User input
		//read_buttons();

		// Update positions
		next_map(map);
		//next_player(player1);

		update_screen(map);

		// All text on screen
		text(map, player1);

		// Draw EVERYTHING
		draw_to_screen();

		player1->time++;
	}

	// Game over
	game_over();

	free(map->obstacles);
	free(map);
	free(player1);

	while(1);

	return 0;
}
