/*
 * player_controls.h
 *
 *  Created on: 2013-10-13
 *      Author: n1v7
 */

#ifndef PLAYER_CONTROLS_H_
#define PLAYER_CONTROLS_H_

#include "player_def.h"

extern bool player2_switches[4] = {false, false, false, false};
extern bool player1_switches[4] = {false, false, false, false};

void read_switches(bool player1) {
	if(player1){
		int new_switch_values = IORD_8DIRECT(SWITCHES_BASE, 0);
		player1_switches[0] = ((new_switch_values & 0x01) == 0x01);
		player1_switches[1] = ((new_switch_values & 0x02) == 0x02);
		player1_switches[2] = ((new_switch_values & 0x04) == 0x04);
		player1_switches[3] = ((new_switch_values & 0x08) == 0x08);
	}
	else{
		int new_switch_values = IORD_32DIRECT(SWITCHES_BASE, 0);
		player2_switches[0] = ((new_switch_values & 0x4000) == 0x4000);
		player2_switches[1] = ((new_switch_values & 0x8000) == 0x8000);
		player2_switches[2] = ((new_switch_values & 0x10000) == 0x10000);
		player2_switches[3] = ((new_switch_values & 0x20000) == 0x20000);
	}
}

bool move_right (Player * player) {
	return player->type == PLAYER1 ? player2_switches[0] : player1_switches[0];
}
bool move_down (Player * player) {
	return player->type == PLAYER1 ? player2_switches[1] : player1_switches[1];
}
bool move_up (Player * player) {
	return player->type == PLAYER1 ? player2_switches[2] : player1_switches[2];
}
bool move_left (Player * player) {
	return player->type == PLAYER1 ? player2_switches[3] : player1_switches[3];
}

#endif /* PLAYER_CONTROLS_H_ */
