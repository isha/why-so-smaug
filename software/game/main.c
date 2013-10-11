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
#include "io.h"

void init() {
	initialize_vga();
	initialize_sdcard();
	alt_timestamp_start();
	initialize_audio();
	start_audio();
}

int main(void)
{
	init();
	Map* map = construct_map();
	Player* player1 = construct_player(get_screen_name());

	initial_screen(map);
	while (game_on) {
		// User input
		read_buttons();

		// Update positions
		next_map(map);
		next_player(player1);

		// Update screen
		update_screen(map);

		// All text on screen
		text(map, player1);

		// Draw EVERYTHING
		draw_to_screen();

		player1->time++;
	}

	return 0;
}
