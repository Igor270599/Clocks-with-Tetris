/*
 * SPI.c
 *
 *  Created on: Oct 6, 2024
 *      Author: Игорь
 */

#include "SPI.h"

SPI_def_t SPI_2;

void SPI_2_Init(void)
{
	SPI_2.status=SPI_IDLE;
	for(uint8_t i=0;i<SPI_BUFFER_SIZE;i++)
	{
		SPI_2.resiveBuffer[i]=0;
		SPI_2.transmitBuffer[i]=0;
		SPI_2.count=0;
	}
}

void SPI_2_ClearBuffers(void)
{
	for(uint8_t i=0;i<SPI_BUFFER_SIZE;i++)
	{
		SPI_2.resiveBuffer[i]=0;
		SPI_2.transmitBuffer[i]=0;
		SPI_2.count=0;
	}
}

bool SPI_2_Transmit (void)
{
bool result=false;

	if(SPI_2.status==SPI_IDLE)
	{
		SPI_2.status=SPI_BYSY;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET );
		HAL_SPI_Transmit_IT(&hspi2, SPI_2.transmitBuffer, SPI_2.count);
		result=true;
	}
return result;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)	//Функция вызывается по завершению передачи
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET );
	SPI_2.status=SPI_IDLE;
	SPI_2_ClearBuffers();
}
