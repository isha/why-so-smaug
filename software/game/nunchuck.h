/*
 * nunchuck.h
 *
 *  Created on: 2013-10-13
 *      Author: n1v7
 */

#ifndef NUNCHUCK_H_
#define NUNCHUCK_H_

typedef struct{
	char values[6];
} Nunchuck;

void initialize_nunchuck(Nunchuck *nunchuck) {
	I2C_init(I2C_BASE, 100, 100);
}

#endif /* NUNCHUCK_H_ */
