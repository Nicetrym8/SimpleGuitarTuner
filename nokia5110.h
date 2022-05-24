/* Nokia 5110 LCD AVR Library
 *
 * Few functions by Nicetrym8
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */

#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <avr/pgmspace.h>
#include <stdint.h>

/*
 * LCD's port
 */
#define F_CPU 4000000UL
#define PORT_LCD PORTD
#define DDR_LCD DDRD

/*
 * LCD's pins
 */
#define LCD_SCE PD4
#define LCD_RST PD5
#define LCD_DC PD6
#define LCD_DIN PD1
#define LCD_CLK PD0

#define LCD_CONTRAST 0x40

/*
 * Must be called once before any other function, initializes display
 */
void nokia_lcd_init(void);

/*
 * Clear screen
 */
void nokia_lcd_clear(void);

/**
 * Power of display
 * @lcd: lcd nokia struct
 * @on: 1 - on; 0 - off;
 */
void nokia_lcd_power(uint8_t on);

/**
 * Set single pixel
 * @x: horizontal pozition
 * @y: vertical position
 * @value: show/hide pixel
 */
void nokia_lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value);
/**
 * Draw horizontal line
 * @x: horizontal position
 * @y: vertical position
 * @l: length of line
 */
void nokia_lcd_draw_xline(uint8_t x, uint8_t y, uint8_t l);
/**
 * Draw vertical line
 * @x: horizontal position
 * @y: vertical position
 * @l: length of line
 */
void nokia_lcd_draw_yline(uint8_t x, uint8_t y, uint8_t l);
/**
 * Clear rectangular shape area
 * @x_st: first vertex horizontal position
 * @y_st: first vertex vertical position
 * @x_end: second vertex horizontal position
 * @y_end: second vertex vertical position
 */
void nokia_lcd_clear_area(uint8_t x_st, uint8_t y_st, uint8_t x_end, uint8_t y_end);
/**
 * Invert rectangular shape area
 * @x_st: first vertex horizontal position
 * @y_st: first vertex vertical position
 * @x_end: second vertex horizontal position
 * @y_end: second vertex vertical position
 */
void nokia_lcd_inverse_area(uint8_t x_st, uint8_t y_st, uint8_t x_end, uint8_t y_end);
/**
* Drawing an image from array 
* @x: horizontal pos
* @y: vertical pos
* @*img: ptr to bitmap array
* @hght: height of image
* @wdth: width of image
*/
void nokia_lcd_draw_bitmap( const uint8_t *img,uint8_t x,uint8_t y,uint8_t wdth,uint8_t hght);
/**
 * Draw single char with 1-6 scale
 * @code: char code
 * @scale: size of char
 */
void nokia_lcd_write_char(char code, uint8_t scale);

/**
 * Draw string. Example: writeString("abc",3);
 * @str: sending string
 * @scale: size of text
 */
void nokia_lcd_write_string(const char *str, uint8_t scale);

/**
 * Set cursor position
 * @x: horizontal position
 * @y: vertical position
 */
void nokia_lcd_set_cursor(uint8_t x, uint8_t y);

/*
 * Render screen to display
 */
void nokia_lcd_render(void);


#endif
