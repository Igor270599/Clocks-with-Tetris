/*
 * SPI.h
 *
 *  Created on: Oct 6, 2024
 *      Author: Игорь
 */

#ifndef SPI_H_
#define SPI_H_

#include "main.h"
//#include "stm32f0xx_hal_conf.h"
#include "stdint.h"
#include "stdbool.h"


#define SPI_BUFFER_SIZE 32

typedef enum SPI_status_t
{
	SPI_IDLE =0,
	SPI_BYSY =1,
}SPI_status_t;

typedef struct SPI_def_t
{
	SPI_status_t status;
	uint8_t transmitBuffer[SPI_BUFFER_SIZE];
	uint8_t resiveBuffer[SPI_BUFFER_SIZE];
	uint8_t count;
}SPI_def_t;


void SPI_2_Init(void);
void SPI_2_ClearBuffers(void);
bool SPI_2_Transmit (void);

extern SPI_def_t SPI_2;
extern SPI_HandleTypeDef hspi2;
#endif /* SPI_H_ */
