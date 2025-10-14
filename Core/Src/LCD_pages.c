/*
 * LCD_pages.c
 *
 *  Created on: Oct 7, 2025
 *      Author: ACER
 */
#include "LCD_pages.h"
#include "usart.h"
#include "state.h"

uint8_t previousState = 255;
uint8_t previousTime = 255;

void renderPage(uint8_t state){
	char coinAmountBuf[60];
	extern uint8_t currentCoinAmount;
	extern uint8_t TARGET_COIN_AMOUNT;

	switch (state){
		case IDLE:
			if (state != previousState) {
				ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
				ILI9341_WriteString(&ili9341, 20, ili9341.height/2, "PLEASE INSERT COIN", ILI9341_Font_Spleen16x32, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 1, 1, 0, 0);

				sprintf(coinAmountBuf, "%d Baht Remaining", TARGET_COIN_AMOUNT - currentCoinAmount);
				ILI9341_WriteString(&ili9341, 90, ili9341.height/2 + 50, coinAmountBuf, ILI9341_Font_Spleen16x32, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, 1, 1, 0, 0);
			}
			break;

		case WAIT_COIN:
			if (state != previousState) {
				ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
				ILI9341_WriteString(&ili9341, 20, ili9341.height/2, "PLEASE INSERT COIN", ILI9341_Font_Spleen16x32, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE, 1, 1, 0, 0);

				sprintf(coinAmountBuf, "%d Baht Remaining", TARGET_COIN_AMOUNT - currentCoinAmount);
				ILI9341_WriteString(&ili9341, 90, ili9341.height/2 + 50, coinAmountBuf, ILI9341_Font_Spleen16x32, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, 1, 1, 0, 0);
			}
			break;

		case GAME:
			extern int timeCount;
			if (state != previousState || timeCount != previousTime) {
				ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
				char timeRemainingBuf[32];
				sprintf(timeRemainingBuf, "%d", 30-timeCount);
				ILI9341_WriteString(&ili9341, 90, ili9341.height/2 + 50, timeRemainingBuf, ILI9341_Font_Spleen16x32, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, 1, 1, 0, 0);
			}

			previousTime = timeCount;
			break;

		case DEPOSIT:
			if (state != previousState) {
				ILI9341_FillScreen(&ili9341, ILI9341_COLOR_WHITE);
				ILI9341_WriteString(&ili9341, 90, ili9341.height/2 + 50, "Game Ended. Depositing...", ILI9341_Font_Spleen16x32, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE, 1, 1, 0, 0);
			}
			break;
	}

	previousState = state;
}
