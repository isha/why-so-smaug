/*
 * timer.h
 *
 *  Created on: 2013-10-01
 *      Author: Kevin
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdio.h>
#include "sys/alt_alarm.h"
#include "sys/alt_timestamp.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"

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


#endif /* TIMER_H_ */
