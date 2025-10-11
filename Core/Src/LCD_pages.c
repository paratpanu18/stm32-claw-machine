/*
 * LCD_pages.c
 *
 *  Created on: Oct 7, 2025
 *      Author: ACER
 */
#include "LCD_pages.h"
#include "usart.h"

void renderPage(uint8_t state){
	switch (state){
		case IDLE:
			ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
			HAL_Delay(500);
			ILI9341_WriteString(&ili9341, 20, ili9341.height/2, "PLEASE INSERT COIN", ILI9341_Font_Spleen16x32, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 1, 1, 0, 0);
			ILI9341_WriteString(&ili9341, 90, ili9341.height/2 + 50, "5 Baht x 2", ILI9341_Font_Spleen16x32, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, 1, 1, 0, 0);
			HAL_Delay(500);
			//			static uint8_t rendered = 0;
//			if (!rendered){
//				ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
//				ILI9341_WriteString(&ili9341, 20, ili9341.height/2, "PLEASE INSERT COIN", ILI9341_Font_Spleen16x32, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 1, 1, 0, 0);
//				rendered = 1;
//			}
			break;
		case WAIT_COIN:
			break;
		case WAIT_CONFIRM:
			break;
		case GAME:
			break;
		case DEPOSIT:
			break;
	}
}
