/*
 * Display.h
 *
 *  Created on: Oct 20, 2024
 *      Author: Игорь
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "../App/Application.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stm32f103xb.h>
#include <stm32f1xx_hal.h>
#include "../MAX_7219_Driver/MAX_7219.h"


typedef struct t_Point
{
  char x;
  char y;
} t_Point ;

typedef struct FigureDef
{
	uint8_t figure;
	uint8_t angle;
}FigureDef;

void WriteMP3_PlayerAnimation(t_animationType animationType,t_rotation rotation);
void WriteRouser(Rouser_Def_t Rouser_Def,t_rotation rotation);
void WriteTimer(Timer_Def_t Timer_Def,t_rotation rotation);
void WriteTimeSmallSixDigits(uint8_t hours,uint8_t minutes,uint8_t secunds,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked);
void WriteTimeSmallFourDigits(uint8_t hours,uint8_t minutes,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked);
void WriteTimeSmallSixDigitsTwoStrings(uint8_t hours,uint8_t minutes,uint8_t secunds,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked);
void WriteTimeBigFourDigits(uint8_t hours,uint8_t minutes,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked);
void WriteTime(Clock_Def_t Clock_Def, t_rotation rotation);
void WriteAdditionalMode(t_rotation rotation);
void WriteTetrisMode(t_rotation rotation);
void WriteScore(uint16_t score, t_rotation rotation,bool endGame);
void WriteFixedFigures(void);
void WriteQueue(void);

extern FigureDef queue[4];

#endif /* DISPLAY_H_ */
