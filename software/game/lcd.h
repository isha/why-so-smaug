/*
 * lcd.h
 *
 *  Created on: 2013-09-18
 *      Author: n1v7
 */

#ifndef LCD_H_
#define LCD_H_

#include "altera_up_avalon_character_lcd.h"

alt_up_character_lcd_dev * char_lcd_dev;

void initialize_lcd() {
	char_lcd_dev = alt_up_character_lcd_open_dev ("/dev/character_lcd_0");
	if ( char_lcd_dev == NULL)
		alt_printf ("\nError: could not open character LCD device\n");
	else
		alt_printf ("\nOpened character LCD device\n");
	alt_up_character_lcd_init (char_lcd_dev);
}
#endif /* LCD_H_ */
