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

void initialize_audio() {
	alt_up_av_config_dev * av_config = alt_up_av_config_open_dev("/dev/audio_config");
	while (!alt_up_av_config_read_ready(av_config)) {
	}
}

void testAudio(){
	/* used for audio record/playback */
	unsigned int buf [2];
	buf[0] = 0x00007FFF;
	buf[1] = 0x00008000;

	// open the Audio port;
	alt_up_audio_dev * audio_dev = alt_up_audio_open_dev("/dev/audio");
	if ( audio_dev == NULL)
		alt_printf ("Error: could not open audio device \n");
	else
		alt_printf ("Opened audio device \n");

	/* Write audio data */
	while(1){
		if (alt_up_audio_write_fifo_space(audio_dev, ALT_UP_AUDIO_LEFT) > 2){
			usleep(100);
			// write audio buffer
			alt_up_audio_write_fifo (audio_dev, buf, 2, ALT_UP_AUDIO_LEFT);
			alt_up_audio_write_fifo (audio_dev, buf, 2, ALT_UP_AUDIO_RIGHT);
		}else{
			continue;
		}
	}
}

#endif /* AUDIO_H_ */
