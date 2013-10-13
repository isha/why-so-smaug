/*
 * player_controls.h
 *
 *  Created on: 2013-10-13
 *      Author: n1v7
 */

#ifndef PLAYER_CONTROLS_H_
#define PLAYER_CONTROLS_H_

#include "player_def.h"

extern bool buttons[4] = {false, false, false, false};
extern bool switches[4] = {false, false, false, false};

void read_buttons() {
	int new_button_values = IORD_8DIRECT(BUTTONS_BASE, 0);
	buttons[0] = ((new_button_values & 0x01) == 0x01);
	buttons[1] = ((new_button_values & 0x02) == 0x02);
	buttons[2] = ((new_button_values & 0x04) == 0x04);
	buttons[3] = ((new_button_values & 0x08) == 0x08);
}

void read_switches() {
	int new_switch_values = IORD_8DIRECT(SWITCHES_BASE, 0);
	switches[0] = ((new_switch_values & 0x01) == 0x01);
	switches[1] = ((new_switch_values & 0x02) == 0x02);
	switches[2] = ((new_switch_values & 0x04) == 0x04);
	switches[3] = ((new_switch_values & 0x08) == 0x08);
}

bool move_right (Player * player) {
	return player->type == PLAYER1 ? buttons[0] : switches[0];
}
bool move_down (Player * player) {
	return player->type == PLAYER1 ? buttons[1] : switches[1];
}
bool move_up (Player * player) {
	return player->type == PLAYER1 ? buttons[2] : switches[2];
}
bool move_left (Player * player) {
	return player->type == PLAYER1 ? buttons[3] : switches[3];
}

#endif /* PLAYER_CONTROLS_H_ */
