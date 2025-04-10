/*
 * UART.h
 *
 *  Created on: Dec 15, 2024
 *      Author: Игорь
 */

#ifndef UART_H_
#define UART_H_

#include "main.h"
#include "stdint.h"
#include "stdbool.h"


#define UART_BUFFER_SIZE 16
#define HIGHT_BYTE_MASK 0xFF00
#define LOW_BYTE_MASK 	0x00FF

typedef enum UART_status_t
{
	UART_IDLE =0,
	UART_BYSY =1,
}UART_status_t;

typedef struct UART_def_t
{
	UART_status_t status;
	uint8_t transmitBuffer	[UART_BUFFER_SIZE];
	uint8_t resiveBuffer	[UART_BUFFER_SIZE];
	uint8_t count;
}UART_def_t;


void UART_1_Init(void);
void UART_1_ClearBuffers(void);
bool UART_1_Transmit (void);

extern UART_def_t UART_1;
extern UART_HandleTypeDef huart1;



#endif /* UART_H_ */
