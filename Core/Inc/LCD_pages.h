/*
 * LCD_pages.h
 *
 *  Created on: Oct 7, 2025
 *      Author: ACER
 */

#ifndef INC_LCD_PAGES_H_
#define INC_LCD_PAGES_H_

#include "ili9341.h"
#include "ili9341_fonts.h"
#include "state.h"

extern ILI9341_HandleTypeDef ili9341;
void renderPage(uint8_t state);

#endif /* INC_LCD_PAGES_H_ */
