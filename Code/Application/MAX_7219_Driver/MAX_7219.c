/*
 * MAX_7219.c
 *
 *  Created on: Sep 15, 2024
 *      Author: Игорь
 */

#include "MAX_7219.h"
#include "Fonts.h"
#include "../App/Application.h"

matrix_t Matrix;

void InitMax7219(int count)
{
uint8_t i,j;

	for(j=0;j<count;j++)
	{
	  SPI_2.transmitBuffer[j*2]=OP_DISPLAYTEST;
	  SPI_2.transmitBuffer[j*2+1]=0;
	}
	  SPI_2.count=16;
	  SPI_2_Transmit();
	  HAL_Delay(1);
	for(j=0;j<count;j++)
	{
	  SPI_2.transmitBuffer[j*2]=OP_SCANLIMIT;
	  SPI_2.transmitBuffer[j*2+1]=7;
	}
	  SPI_2.count=16;
	  SPI_2_Transmit();
	  HAL_Delay(1);
	for(j=0;j<count;j++)
	{
	  SPI_2.transmitBuffer[j*2]=OP_DECODEMODE;
	  SPI_2.transmitBuffer[j*2+1]=0;
	}
	  SPI_2.count=16;
	  SPI_2_Transmit();
	  HAL_Delay(1);
	for(j=0;j<count;j++)
	{
	  SPI_2.transmitBuffer[j*2]=OP_SHUTDOWN;
	  SPI_2.transmitBuffer[j*2+1]=1;
	}
	  SPI_2.count=16;
	  SPI_2_Transmit();
	  HAL_Delay(1);
	for(j=0;j<count;j++)
	{
	  SPI_2.transmitBuffer[j*2]=OP_INTENSITY;
	  SPI_2.transmitBuffer[j*2+1]=0;
	}
	  SPI_2.count=16;
	  SPI_2_Transmit();
	  HAL_Delay(1);

	for(i=0;i<32;i++)
	{
		Matrix.data[i]=0;
	}
	UpdateMax7219();
}

void ClearMax7219(void)
{
	for(uint8_t i=0;i<32;i++)
	{
		Matrix.data[i]=0;
	}
}

void SetPixel(uint8_t x,uint8_t y,bool state)
{
	if((!((x>=0)&&(x<=15)))&&(!((y>=0)&&(y<=31))))
	{
		return;
	}
	else
	{	if(state)
		{
			Matrix.data[y]=Matrix.data[y]|(1<<(15-x));
		}
		else
		{
			Matrix.data[y]=Matrix.data[y]&(~(1<<(15-x)));
		}
	}
}

bool GetPixel(uint8_t x,uint8_t y)
{
bool result;

	if(Matrix.data[y]&(1<<(15-x)))
	{
		result=true;
	}
	else
	{
		result=false;
	}

	return result;
}

void UpdateMax7219(void)
{
uint8_t	byteIndex=0;
part_t	part;
uint8_t data,shift;

	if (SPI_2.status==SPI_BYSY)
	{
		return;
	}

	for(uint8_t i=0;i<8;i++)
	{
		for(uint8_t j=0;j<8;j++)
		{
			switch(j)
			{
				case 0: byteIndex=0;	part=RIGHT_PART;break;
				case 1: byteIndex=8; 	part=RIGHT_PART;break;
				case 2: byteIndex=16;	part=RIGHT_PART;break;
				case 3: byteIndex=24;	part=RIGHT_PART;break;
				case 4: byteIndex=0; 	part=LEFT_PART;break;
				case 5: byteIndex=8; 	part=LEFT_PART;break;
				case 6: byteIndex=16; 	part=LEFT_PART;break;
				case 7: byteIndex=24; 	part=LEFT_PART;break;
			}
			if(part==LEFT_PART)
				shift=8;
			else
				shift=0;

			data=0;
			for(uint8_t k=0;k<8;k++)
			{
				if(Matrix.data[byteIndex+k]&(1<<(i+shift)))
				{
					data=data|(1<<(7-k));
				}
			}

			SPI_2.transmitBuffer[j*2]=i+1;
			SPI_2.transmitBuffer[j*2+1]=data;
		}
	  	SPI_2.count=16;
	  	SPI_2_Transmit();

	  	while(SPI_2.status==SPI_BYSY)
	  	{
	  		AppTask();
	  	}

	}
}

void DrowDigit(uint8_t x,uint8_t y,uint8_t digit,t_font_style stile,t_font_size Size, t_rotation rotation)
{
uint8_t  fontIndex,matrixX,matrixY,fontHeight,fontWidth,data,startPos;

	if(stile==CLASSIC)
	{
	  fontIndex=0;
	}
	else
	{
	  fontIndex=10;
	}

	switch(Size)
	{
		case SMALL:
			fontHeight=FONT_4X7_HEIGHT;
			fontWidth=FONT_4X7_WIDTH;
			startPos=4;
		break;

		case BIG:
			fontHeight=FONT_6X12_HEIGHT;
			fontWidth=FONT_6X12_WIDTH;
			fontIndex=0;
			startPos=2;
		break;

		case LETTER:
			fontHeight=FONT_5X7_HEIGHT;
			fontWidth=FONT_5X7_WIDTH;
			fontIndex=0;
			startPos=3;
		break;
	}

	matrixX=x;
	matrixY=y;

	for(uint8_t i=0;i<fontHeight;i++)
	{
		switch(Size)
		{
			case SMALL:
				data=FontTable4x7[digit+fontIndex][i];
			break;

			case BIG:
				data=FontTable6x12[digit+fontIndex][i];
			break;

			case LETTER:
				data=FontTable5x7[digit+fontIndex][i];
			break;
		}

		for(uint8_t j=0;j<fontWidth;j++)
		{
			if(data&(1<<(startPos+j)))
			{
				SetPixel(matrixX,matrixY,true);
			}
			else
			{
				SetPixel(matrixX,matrixY,false);
			}

		    switch (rotation)
		    {
		      case ROTATION_0_DEGREES:
		         matrixY++;
		      break;

		      case ROTATION_90_DEGREES:
		        matrixX--;
		      break;

		      case ROTATION_180_DEGREES:
		        matrixY--;
		      break;

		      case ROTATION_270_DEGREES:
		        matrixX++;
		      break;
		     }
		}

	    switch (rotation)
	    {
	      case ROTATION_0_DEGREES:
	        matrixX++;
	        matrixY=y;
	      break;

	       case ROTATION_90_DEGREES:
	          matrixY++;
	          matrixX=x;
	       break;

	      case ROTATION_180_DEGREES:
	        matrixX--;
	        matrixY=y;
	      break;

	      case ROTATION_270_DEGREES:
	        matrixY--;
	        matrixX=x;
	      break;
	    }
	}
}

void SetIntensivity(uint8_t intensivity)
{
	if((intensivity>=0)&&(intensivity<=15))
	{
		while(SPI_2.status!= SPI_IDLE){};
		for(uint8_t i=0;i<8;i++)
		{
			SPI_2.transmitBuffer[i*2]=OP_INTENSITY;
			SPI_2.transmitBuffer[i*2+1]=intensivity;
		}
		SPI_2.count=16;
		SPI_2_Transmit();
		HAL_Delay(1);
	}
}
