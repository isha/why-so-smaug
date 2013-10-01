#include <stdio.h>
#include "vga.h"
#include "lcd.h"
#include <stdbool.h>
#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"

#define switches (volatile char *) SWITCHES_BASE
#define leds (char *) LEDS_BASE
#define keys (volatile char *) BUTTONS_BASE

alt_up_character_lcd_dev * char_lcd_dev;
alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev *char_buffer;


void init() {
	initialize_lcd(&char_lcd_dev);
	initialize_vga(&pixel_buffer, &char_buffer);
}

int main(void)
{
	int *buttons;
	bool bouncing = false;

	printf("Woohoo!\n");

	init();

	alt_up_pixel_buffer_dma_draw_line(pixel_buffer, 0, 240, 320, 0, 0xFFFF, 0);

	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "EECE 381", 60, 50);

	timer_test();

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
	return 0;
}

void timer_test(void) {

	int freq;
	int cycles;
	float duration;
	int ticks_start;
	int ticks_end;
	int ticks_per_s;
	int ticks_duration;
	int timer_period;
	int status;
	int done;

	printf("Timers\n");

	printf(" Sys Clock Timer\n");

	ticks_per_s = alt_ticks_per_second();
	printf(" Tick Freq: %d\n", ticks_per_s);

	printf(" Recording starting ticks\n");
	ticks_start = alt_nticks();

	printf(" Sleeping for 5 seconds\n");
	usleep(5000000);

	printf(" Recording ending ticks\n");
	ticks_end = alt_nticks();

	ticks_duration = ticks_end - ticks_start;
	duration = (float) ticks_duration / (float) ticks_per_s;
	printf(" The program slept for %d ticks (%f seconds)\n\n", ticks_duration,
	duration);

	printf(" Timestamp Timer\n");

	freq = alt_timestamp_freq();
	printf(" CPU Freq: %d\n", freq);

	printf(" Resetting Timestamp timer\n");
	alt_timestamp_start();

	printf(" ...Timing the print of this statement...\n");
	cycles = alt_timestamp();
	duration = (float) cycles / (float) freq;
	printf(" It took %d cycles (%f seconds) to print the statement\n\n",
	cycles, duration);

	printf(" Hardware-Only Timer\n");

	printf(" Setting timer period to 5 seconds.\n");
	timer_period = 5 * 50000000;
	// 50 MHz clock, so 50M cycles per second.
	// Therefore, multiply by 5 to get the period in cycles

	IOWR_16DIRECT(HW_ONLY_CLK_BASE, 0, timer_period >> 16);
	// IOWR_16DIRECT (timer_base_address, offset, input_data)
	// Offset 0x8 (8) is where the lower 8 bits of the period is stored
	// bitwise & converts int period into hex

	IOWR_16DIRECT(HW_ONLY_CLK_BASE, 8, timer_period & 0xFFFF);
	// Offset 0xC (12) is where the higher 8 bits of the period is stored
	// complete right shift casts int data to hex and clears the register

	IOWR_16DIRECT(HW_ONLY_CLK_BASE, 12, timer_period >> 16);
	// clear the status register at offset 0x0 (0)

	printf(" Stopping Timer\n");
	status = IORD_16DIRECT(HW_ONLY_CLK_BASE, 0);
	// IORD_16DIRECT (timer_base_address, offset)
	// Offset 0x0 (0) is where the current status of the timer can be read

	if (status & 0x2) {	// ie. if the timer is currently running
		IOWR_16DIRECT(HW_ONLY_CLK_BASE, 4, 1 << 3);
		// adjust the control register at offset 0x4 (4) to enable to the stop bit and disable the start bit
	}
	printf(" Starting Timer\n");
	IOWR_16DIRECT(HW_ONLY_CLK_BASE, 4, 1 << 2);
	// adjust the control register at offset 0x4 (4) to enable the start bit and disable the stop bit

	printf(" Waiting for timer to expire...\n");
	done = 0;

	while (! done) {
		status = IORD_16DIRECT(HW_ONLY_CLK_BASE, 0); // ie. continuously read the status of the timer at offset 0x0 (0)
		done = status & 0x1; // compare the expected done value with the current status to check if timer completes
	}
	printf(" 5 seconds timer is done\n");
}
