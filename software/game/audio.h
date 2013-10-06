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
	if (!alt_up_av_config_read_ready(av_config)) {
		printf("Audio can not be initialised\n");
	}
}

//not working
void play_note(double length, int period, alt_up_audio_dev * device){
	int ticks_per_s = alt_ticks_per_second();
	unsigned int buf [2] = {0x000007FF, 0x0000800};
	double ticks_start_note = alt_nticks();
	double ticks_freq = ticks_start_note;

	while((alt_nticks() - ticks_start_note) / ticks_per_s < length){
		if (alt_up_audio_write_fifo_space(device, ALT_UP_AUDIO_LEFT) > 2){
			usleep(period);
			alt_up_audio_write_fifo (device, buf, 1, ALT_UP_AUDIO_LEFT);
			alt_up_audio_write_fifo (device, buf, 1, ALT_UP_AUDIO_RIGHT);
//			if((alt_nticks() - ticks_freq) / ticks_per_s < period){
//				// write audio buffer
//				alt_up_audio_write_fifo (device, buf, 1, ALT_UP_AUDIO_LEFT);
//				alt_up_audio_write_fifo (device, buf, 1, ALT_UP_AUDIO_RIGHT);
//				ticks_freq = alt_nticks();
//			}
		}else{
			continue;
		}
	}
}

void test_audio(){
	// open the Audio port;
	alt_up_audio_dev * audio_dev = alt_up_audio_open_dev("/dev/audio");
	if ( audio_dev == NULL)
		alt_printf ("Error: could not open audio device \n");
	else
		alt_printf ("Opened audio device \n");

	//Play a little tune on repeat
	while(1){
		play_note(.2, 1000, audio_dev);
		usleep(100000);
		play_note(.2, 1000, audio_dev);
		usleep(100000);
		play_note(.2, 5000, audio_dev);
		usleep(1000000);

		play_note(.5, 5000, audio_dev);
		usleep(100000);
		play_note(.5, 1000, audio_dev);
		usleep(100000);
		play_note(.5, 10000, audio_dev);
		usleep(100000);
		play_note(.2, 500, audio_dev);
		usleep(50000);
		play_note(.2, 500, audio_dev);
		usleep(50000);
		play_note(.5, 10000, audio_dev);
		usleep(1000000);

		play_note(.2, 5000, audio_dev);
		usleep(100000);
		play_note(.2, 5000, audio_dev);
		usleep(100000);
		play_note(.2, 1000, audio_dev);
		usleep(5000000);
	}

//	short int file_handle;
//	bool reading_data = false;
//	unsigned int pcm;
//	unsigned int read;
//	char* filename = "dingding.wav";
//
//	// open the Audio port;
//	alt_up_audio_dev * audio_dev = alt_up_audio_open_dev("/dev/audio");
//	if ( audio_dev == NULL)
//		alt_printf ("Error: could not open audio device \n");
//	else
//		alt_printf ("Opened audio device \n");
//
//	file_handle = sdcard_fopen(filename, 0);
//	if (file_handle == -1){
//		printf("File could not be opened.\n");
//	}else if (file_handle == -2){
//		printf("File is already open.\n");
//	}
//
//	while(1){
//		read = sdcard_read(file_handle);
//		pcm = read % 0xF00;
//		read = sdcard_read(file_handle);
//		pcm = read % 0xF00 + pcm*16*16;
//		read = sdcard_read(file_handle);
//		pcm = read % 0xF00 + pcm*16*16;
//		read = sdcard_read(file_handle);
//		pcm = read % 0xF00 + pcm*16*16;
//		printf("%X\n", pcm);
//
//		if (reading_data){
//			while(1){
//				if (alt_up_audio_write_fifo_space(audio_dev, ALT_UP_AUDIO_LEFT) > 2){
//					alt_up_audio_write_fifo (audio_dev, pcm, 1, ALT_UP_AUDIO_LEFT);
//					alt_up_audio_write_fifo (audio_dev, pcm, 1, ALT_UP_AUDIO_RIGHT);
//					break;
//				}else{
//					continue;
//				}
//			}
//		}else if (pcm == 0x64647461){
//			//From this point on we are reading actual data
//			reading_data = true;
//		}
//	}

}

#endif /* AUDIO_H_ */
