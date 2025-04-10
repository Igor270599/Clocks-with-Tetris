/*
 * Tetris.h
 *
 *  Created on: Oct 27, 2024
 *      Author: Игорь
 */

#ifndef TETRIS_H_
#define TETRIS_H_

#include "../App/Application.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../Display/Display.h"
#include <stm32f1xx_hal.h>

#define FULL_LINE 65472

typedef enum CheckType_t
{
	FLOOR				=	0,
	FLOOR_AND_FIGURES	=	1,
	LEFT_AND_FIGURES	=	2,
	RIGHT_AND_FIGURES	=	3,
	ROTATION			=	4,
}CheckType_t;

void drawFigure(int8_t figure, int8_t angle, int8_t x, int8_t y, bool State,t_rotation rotation);
bool checkArea(uint8_t x_Pos,uint8_t y_Pos,CheckType_t check_type);
void FixPixel(uint8_t i, uint8_t j, bool state);
bool GetFixedPixel(uint8_t i, uint8_t j);
uint16_t getSeed(void);
void RestartGame(void);
void RestartRound(void);
void StartGame(void);
void ClearLines(void);

extern const int8_t figures[7][12][2];
extern uint16_t fixedFigures[23];
#endif /* TETRIS_H_ */
