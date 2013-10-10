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

char * screen_name;

void init() {
	initialize_vga();
	initialize_sdcard();
	alt_timestamp_start();
	initialize_audio();
	start_audio();
}

int main(void)
{
	int time =0;
	init();

	// Selection
	screen_name = get_screen_name();
	Player *player1 = construct_player("LOL");
	printf("\nPlayer created with name %s\n", player1->screen_name);

	bool game_on = true;
	Map *map = construct_map();

	// Main game play
	initial_screen(map);
	//initial_screen(map);
	while (game_on && time < 200) {
		// User input
		read_buttons();

		// Update positions
		next_map(map);


		// Update screen
		update_screen(map);
		next_player(player1);

		// All text on screen
		text(map, time);

		// Draw EVERYTHING
		draw_to_screen();

		time++;
	}

	// Game over
	game_over();

	while(1);

	return 0;
}
