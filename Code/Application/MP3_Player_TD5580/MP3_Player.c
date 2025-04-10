/*
 * MP3_Player.c
 *
 *  Created on: Dec 15, 2024
 *      Author: Игорь
 */

#include "MP3_Player.h"

uint16_t CalcCrc(uint8_t buf[],uint8_t count)
{
uint16_t	result=0;
uint8_t		i=0;
	for(i=0;i<count;i++)
	{
		result=result-buf[i+1];
	}

	return result;
}


void SayTime(void)
{
	MP3_Player.function=SAY_TIME;
	MP3_Player.delay=1;
}

void MP3_Task(void)
{
static bool done=false;
static uint16_t trackNumber=0;

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)==1)
	{
		MP3_Player.state=MP3_IDLE;
		if(Application.rouserDefinition.isSignalEnabled==true)
		{
			MP3_PlayTrack(ROUSER_SIGNAL);
		}

		if(Application.timerDefinition.isSignalEnabled==true)
		{
			MP3_PlayTrack(TIMER_SIGNAL);
		}

	}
	else
	{
		MP3_Player.state=MP3_BYSY;
		if(MP3_Player.isPaused==true)
		{
			MP3_Player.isPaused=false;
		}
	}

	if((MP3_Player.function==SAY_TIME)||(MP3_Player.function==SAY_DATE))
	{
		if(MP3_Player.delay!=0)
		{
			MP3_Player.delay--;
		}
		else
		{
			MP3_Player.phase++;
			done=false;
		}
	}

	if (MP3_Player.function==SAY_DATE)
	{
		if(done==false)
		{
			switch(MP3_Player.phase)
			{
				case 0:
					MP3_PlayTrack(RTC_data.DayOfWeek+DAY_OF_WEEK_OFFSET-1);
					MP3_Player.delay=0;	//10*100 ms=1000 ms= 1s
					done=true;
				break;
				case 1:
					MP3_PlayTrack(RTC_data.Day+DAY_OFFSET-1);
					MP3_Player.delay=0;	//10*100 ms=1000 ms= 1s
					done=true;
				break;
				case 2:
					MP3_PlayTrack(RTC_data.Mounth+MOUNTH_OFFSET-1);
					MP3_Player.delay=0;	//10*100 ms=1000 ms= 1s
					done=true;
				break;
				case 3:
					MP3_PlayTrack(_2000_TRACK_NUMBER);
					MP3_Player.delay=0;	//10*100 ms=1000 ms= 1s
					done=true;
				break;
				case 4:
					MP3_PlayTrack(RTC_data.Year+YEAR_OFFSET-2025);
					MP3_Player.delay=0;	//10*100 ms=1000 ms= 1s
					done=true;
				break;
				case 5:
					MP3_PlayTrack(_YEAR_TRACK_NUMBER);
					MP3_Player.phase=0;
					MP3_Player.function=NO_SOUND;
					Application.volume=Application.saved_volume;
					done=false;
				break;
			}
		}
	}

	if (MP3_Player.function==SAY_TIME)
	{
		if(done==false)
		{
			switch(MP3_Player.phase)
			{
				case 0:
					MP3_PlayTrack(RTC_data.Hours+HOURS_OFFSET);
					MP3_Player.delay=1;	//10*100 ms=1000 ms= 1s
					done=true;
				break;
				case 1:
					MP3_PlayTrack(RTC_data.Minutes+MINUTES_OFFSET);
					MP3_Player.phase=0;
					MP3_Player.function=NO_SOUND;
					done=false;
				break;
			}
		}
	}

	if(Application.mode==MP3_PLAYER)
	{
		if(MP3_Player.state==MP3_IDLE)
		{
			if(MP3_Player.isPaused==false)
			{
				Application.mp3_PlayerDefinition.prevTrack=MP3_Player.trackNumber;
				trackNumber=GenarateTrack();
				MP3_PlayTrack(trackNumber);
			}

		}
	}


	if(Application.mode==TETRIS)
	{
		if(MP3_Player.state==MP3_IDLE)
		{
			if(Application.tetrisDefinition.isSoundEnabled==true)
			{
				MP3_PlayTrack(TETRIS_OFFSET+Application.tetrisDefinition.Melody);
			}

		}
	}
}

uint16_t GenarateTrack(void)
{
	uint16_t result;

	result=Random(MUSIC_START, MUSIC_END);

	return result;
}

void MP3_SetVolume(uint8_t volume)
{
uint16_t	crc;

	while(UART_1.status!=UART_IDLE){};

	UART_1.transmitBuffer[0]=0x7E;
	UART_1.transmitBuffer[1]=0xFF;
	UART_1.transmitBuffer[2]=0x06;
	UART_1.transmitBuffer[3]=0x06;
	UART_1.transmitBuffer[4]=0x00;
	UART_1.transmitBuffer[5]=0x00;
	UART_1.transmitBuffer[6]=volume;
	crc=CalcCrc(UART_1.transmitBuffer,6);
	UART_1.transmitBuffer[7]=(crc&HIGHT_BYTE_MASK)>>8;;
	UART_1.transmitBuffer[8]=(uint8_t)(crc&LOW_BYTE_MASK);
	UART_1.transmitBuffer[9]=0xEF;
	UART_1.count=10;
	UART_1_Transmit();
}

void MP3_Pause(void)
{
uint16_t	crc;

	while(UART_1.status!=UART_IDLE){};

	MP3_Player.isPaused=true;
	UART_1.transmitBuffer[0]=0x7E;
	UART_1.transmitBuffer[1]=0xFF;
	UART_1.transmitBuffer[2]=0x06;
	UART_1.transmitBuffer[3]=0x0E;
	UART_1.transmitBuffer[4]=0x00;
	UART_1.transmitBuffer[5]=0x00;
	UART_1.transmitBuffer[6]=0x00;
	crc=CalcCrc(UART_1.transmitBuffer,6);
	UART_1.transmitBuffer[7]=(crc&HIGHT_BYTE_MASK)>>8;
	UART_1.transmitBuffer[8]=(uint8_t)(crc&LOW_BYTE_MASK);
	UART_1.transmitBuffer[9]=0xEF;
	UART_1.count=10;
	UART_1_Transmit();
}

void MP3_Play(void)
{
uint16_t	crc;

	while(UART_1.status!=UART_IDLE){};
	MP3_Player.isPaused=false;
	UART_1.transmitBuffer[0]=0x7E;
	UART_1.transmitBuffer[1]=0xFF;
	UART_1.transmitBuffer[2]=0x06;
	UART_1.transmitBuffer[3]=0x0D;
	UART_1.transmitBuffer[4]=0x00;
	UART_1.transmitBuffer[5]=0x00;
	UART_1.transmitBuffer[6]=0x00;
	crc=CalcCrc(UART_1.transmitBuffer,6);
	UART_1.transmitBuffer[7]=(crc&HIGHT_BYTE_MASK)>>8;;
	UART_1.transmitBuffer[8]=(uint8_t)(crc&LOW_BYTE_MASK);
	UART_1.transmitBuffer[9]=0xEF;
	UART_1.count=10;
	UART_1_Transmit();
}

void MP3_Stop(void)
{
uint16_t	crc;

	while(UART_1.status!=UART_IDLE){};

	UART_1.transmitBuffer[0]=0x7E;
	UART_1.transmitBuffer[1]=0xFF;
	UART_1.transmitBuffer[2]=0x06;
	UART_1.transmitBuffer[3]=0x16;
	UART_1.transmitBuffer[4]=0x00;
	UART_1.transmitBuffer[5]=0x00;
	UART_1.transmitBuffer[6]=0x00;
	crc=CalcCrc(UART_1.transmitBuffer,6);
	UART_1.transmitBuffer[7]=(crc&HIGHT_BYTE_MASK)>>8;;
	UART_1.transmitBuffer[8]=(uint8_t)(crc&LOW_BYTE_MASK);
	UART_1.transmitBuffer[9]=0xEF;
	UART_1.count=10;
	UART_1_Transmit();
}


void MP3_Reset(void)
{
uint16_t	crc;

	while(UART_1.status!=UART_IDLE){};

	UART_1.transmitBuffer[0]=0x7E;
	UART_1.transmitBuffer[1]=0xFF;
	UART_1.transmitBuffer[2]=0x06;
	UART_1.transmitBuffer[3]=0x0C;
	UART_1.transmitBuffer[4]=0x00;
	UART_1.transmitBuffer[5]=0x00;
	UART_1.transmitBuffer[6]=0x00;
	crc=CalcCrc(UART_1.transmitBuffer,6);
	UART_1.transmitBuffer[7]=(crc&HIGHT_BYTE_MASK)>>8;;
	UART_1.transmitBuffer[8]=(uint8_t)(crc&LOW_BYTE_MASK);
	UART_1.transmitBuffer[9]=0xEF;
	UART_1.count=10;
	UART_1_Transmit();
}

void MP3_PlayTrack(uint16_t number)
{
uint16_t	crc;

	while(UART_1.status!=UART_IDLE){};

	MP3_Player.trackNumber=number;

	UART_1.transmitBuffer[0]=0x7E;
	UART_1.transmitBuffer[1]=0xFF;
	UART_1.transmitBuffer[2]=0x06;
	UART_1.transmitBuffer[3]=0x12;
	UART_1.transmitBuffer[4]=0x00;
	UART_1.transmitBuffer[5]=(number&HIGHT_BYTE_MASK)>>8;
	UART_1.transmitBuffer[6]=(uint8_t)(number&LOW_BYTE_MASK);
	crc=CalcCrc(UART_1.transmitBuffer,6);
	UART_1.transmitBuffer[7]=(crc&HIGHT_BYTE_MASK)>>8;;
	UART_1.transmitBuffer[8]=(uint8_t)(crc&LOW_BYTE_MASK);
	UART_1.transmitBuffer[9]=0xEF;
	UART_1.count=10;
	UART_1_Transmit();
}
