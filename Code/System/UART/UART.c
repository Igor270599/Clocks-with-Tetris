/*
 * UART.c
 *
 *  Created on: Dec 15, 2024
 *      Author: Игорь
 */

#include "UART.h"

UART_def_t UART_1;

void UART_1_Init(void)
{
	UART_1.status=UART_IDLE;
	for(uint8_t i=0;i<UART_BUFFER_SIZE;i++)
	{
		UART_1.resiveBuffer[i]=0;
		UART_1.transmitBuffer[i]=0;
		UART_1.count=0;
	}
}

void UART_1_ClearBuffers(void)
{
	for(uint8_t i=0;i<UART_BUFFER_SIZE;i++)
	{
		UART_1.resiveBuffer[i]=0;
		UART_1.transmitBuffer[i]=0;
		UART_1.count=0;
	}
}

bool UART_1_Transmit (void)
{
bool result=false;

	if(UART_1.status==UART_IDLE)
	{
		UART_1.status=UART_BYSY;
		HAL_UART_Transmit_IT(&huart1, UART_1.transmitBuffer, UART_1.count);
		result=true;
	}
return result;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *hUART)	//Функция вызывается по завершению передачи
{
	UART_1.status=UART_IDLE;
	UART_1_ClearBuffers();
}
