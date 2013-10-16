/*
 * nunchuck.h
 *
 *  Created on: 2013-10-13
 *      Author: n1v7
 */



#ifndef NUNCHUCK_H_
#define NUNCHUCK_H_

#include "i2c_opencores.h"

int values[6];

typedef struct{
	char values[6];
} Nunchuck;

void initialize_nunchuck(Nunchuck *nunchuck) {
	I2C_init(I2C_BASE, 5000000, 100000);

	int ack;
	ack = I2C_start(I2C_BASE,0xA4,0);
	ack = I2C_write(I2C_BASE, 0x40, false);
	ack = I2C_write(I2C_BASE, 0x00, true);

	while(true) {
		conv();
		read();

		printf ("\n\nSx = %d, Sy = %d", values[0], values[1]);
		printf("\nAx = %d, Ay = %d, Az = %d", values[2], values[3],
				  values[4]);
	}
}

void conv() {
	I2C_start(I2C_BASE,0xA4,0);
	I2C_write(I2C_BASE, 0x00, true);
}

void read() {
	I2C_start(I2C_BASE,0xA5,1);

	int i, num;
	for (i=0; i<6; i++) {
		if (i==5) num = I2C_read(I2C_BASE, true);
		else num = I2C_read(I2C_BASE, false);

		values[i] = (num ^ 0x17) + 0x17;
	}
}

#endif /* NUNCHUCK_H_ */
