/*
 * audio.h
 *
 *  Created on: 2013-10-03
 *      Author: Kevin
 */

#ifndef AUDIO_H_
#define AUDIO_H_

#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_audio.h"
#include <sys/alt_alarm.h>

double game_of_thrones[57][2] = {{1.2, 98.00}, {1.2, 65.41}, {.2, 77.78}, {.2, 87.31}, {.4, 98.00},
			{.4, 65.41}, {.2, 77.78}, {.2, 87.31}, {4.8, 73.42}, {1.2, 87.31}, {1.2, 58.27}, {.2, 77.78},
			{.2, 73.42}, {.4, 87.31}, {.4, 58.27}, {.2, 77.78}, {.2, 73.42}, {3.6, 65.41}, {0.4, 0},
			{1.2, 98.00}, {1.2, 65.41}, {.2, 77.78}, {.2, 87.31}, {.4, 98.00}, {.4, 65.41}, {.2, 77.78},
			{.2, 87.31}, {4.8, 73.42}, {1.2, 87.31}, {1.2, 58.27}, {.2, 77.78}, {.2, 73.42}, {.4, 87.31},
			{.4, 58.27}, {.2, 77.78}, {.2, 73.42}, {3.6, 65.41}, {0.4, 0}, {1.2, 196.00}, {1.2, 130.81},
			{.2, 155.56}, {.2, 174.61}, {.4, 196.00}, {0.4, 130.81}, {.2, 155.56}, {.2, 174.61},
			{4.8, 146.83}, {5, 0}};

int ticks_per_s;
int note_iterator = 0;
double ticks_start_note;

alt_u32 handle_timer_interrupts(alt_up_audio_dev * device)
{
	unsigned int buf [2] = {0x000007FF, 0x0000800};

	if (game_of_thrones[note_iterator][0] == 5 && game_of_thrones[note_iterator][1] == 0){
		ticks_start_note = alt_nticks();
		note_iterator = 0;
	}else if ((alt_nticks() - ticks_start_note) / ticks_per_s >= game_of_thrones[note_iterator][0]){
		ticks_start_note = alt_nticks();
		note_iterator++;
	}

	int period = 0;
	if (game_of_thrones[note_iterator][1] == 0){
		period = 1;
	}else{
		period = floor(1000/game_of_thrones[note_iterator][1] + 0.5);
	}

	if (alt_up_audio_write_interrupt_pending(device) && period != 1){
		alt_up_audio_write_fifo (device, buf, 1, ALT_UP_AUDIO_LEFT);
		alt_up_audio_write_fifo (device, buf, 1, ALT_UP_AUDIO_RIGHT);
	}

	return period;
}

void initialize_audio() {
	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev("/dev/audio_config");
	if (!alt_up_av_config_read_ready(av_config)) {
		printf("Audio can not be initialised\n");
	}
}

alt_up_audio_dev * start_audio(){
	// open the Audio port;
	alt_up_audio_dev * audio_dev = alt_up_audio_open_dev("/dev/audio");
	if ( audio_dev == NULL)
		alt_printf ("Error: could not open audio device \n");
	else
		alt_printf ("Opened audio device \n");

	ticks_per_s = alt_ticks_per_second();
	ticks_start_note = alt_nticks();
	alt_up_audio_enable_write_interrupt(audio_dev);

	static alt_alarm alarm;
	alt_alarm_start(&alarm, 1, handle_timer_interrupts, audio_dev);

}

#endif /* AUDIO_H_ */
