/*
 * MAX7219.h
 *
 *  Created on: Sep 15, 2024
 *      Author: Игорь
 */

#ifndef MAX_7219_H_
#define MAX_7219_H_

#include "main.h"
//#include "stm32f0xx_hal_conf.h"
#include "stdint.h"
#include "stdbool.h"
#include "../../System/SPI/SPI.h"

#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10 //A
#define OP_SCANLIMIT   11 //B
#define OP_SHUTDOWN    12 //C
#define OP_DISPLAYTEST 15 //F

#define HIGH_BIT_MASK   0xFF00
#define LOW_BIT_MASK 	0x00FF

typedef struct matrix_t
{
	uint16_t data[32];
}matrix_t;

typedef enum part_t
{
	LEFT_PART 	=0,
	RIGHT_PART 	=1,
}part_t;

typedef enum t_font_style {
CLASSIC   =0,
NEOCLASSIC  =1,
}t_font_style;

typedef enum t_font_size {
SMALL  		=0,
BIG  		=1,
LETTER		=2,
}t_font_size;

typedef enum t_letter_index {
	A_LETTER  =0,
	B_LETTER  =1,
	T_LETTER  =2,
	V_LETTER  =3,
	E_LETTER  =4,
	O_LETTER  =5,
	R_LETTER  =6,
	S_LETTER  =7,
}t_letter_index;

typedef enum t_clock_style {
SMALL_SIX_DIGITS_TWO_STRINGS    =0,
SMALL_SIX_DIGITS                =1,
SMALL_FOUR_DIGITS               =2,
BIG_FOUR_DIGITS                 =3,
}t_clock_style;

typedef enum t_rotation {
ROTATION_0_DEGREES   =0,
ROTATION_90_DEGREES  =1,
ROTATION_180_DEGREES =2,
ROTATION_270_DEGREES =3,
}t_rotation;

void InitMax7219(int count);
void UpdateMax7219(void);
bool SPI_2_Transmit (void);
void DrowDigit(uint8_t x,uint8_t y,uint8_t digit,t_font_style stile,t_font_size Size, t_rotation rotation);
void SetIntensivity(uint8_t intensivity);
void SetPixel(uint8_t x,uint8_t y,bool state);
bool GetPixel(uint8_t x,uint8_t y);
void ClearMax7219(void);

extern matrix_t Matrix;

//extern SPI_HandleTypeDef hspi2;

#endif /* MAX_7219_H_ */
