/*
 * MP3_Player.h
 *
 *  Created on: Dec 15, 2024
 *      Author: Игорь
 */

#ifndef MP3_PLAYER_H_
#define MP3_PLAYER_H_


#include "../../System/UART/UART.h"
#include "../RTC_DS3231/RTC_DS3231_Driver.h"
#include "../App/Application.h"

#define HOURS_OFFSET 		1
#define MINUTES_OFFSET 		25
#define DAY_OF_WEEK_OFFSET	85
#define DAY_OFFSET			92
#define MOUNTH_OFFSET		123
#define YEAR_OFFSET			138
#define _2000_TRACK_NUMBER	135
#define _YEAR_TRACK_NUMBER	137

#define TETRIS_START_GAME 			178
#define TETRIS_END_GAME				176
#define TETRIS_OFFSET				179
#define TETRIS_MELODY_COUNTER		9

#define TIMER_SIGNAL		173
#define ROUSER_SET			174
#define TIMER_SET			175
#define ROUSER_SIGNAL		167
#define MUSIC_START 		189
#define MUSIC_END 			190
#define TIMER_SIGNAL		173

#define ROUSER_EVERYDAY		165
#define ROUSER_ONCE			166

#define COUNT_DOWN_VARIANT_ONE		169
#define COUNT_DOWN_VARIANT_TWO		172

typedef enum MP3_Player_state_t
{
	MP3_IDLE,
	MP3_BYSY,
}MP3_Player_state_t;

typedef enum MP3_Player_function_t
{
	NO_SOUND,
	SAY_TIME,
	SAY_DATE,
	PLAY_TETRIS,
	PLAY_MP3,
	PLAY_TRACK,
}MP3_Player_function_t;

typedef struct MP3_Player_t
{
	MP3_Player_state_t 		state;
	MP3_Player_function_t	function;
	uint16_t 				trackNumber;
	uint8_t					volume;
	uint8_t					phase;
	int8_t	 				delay;
	bool 					isPaused;
}MP3_Player_t;

void MP3_PlayTrack(uint16_t number);
void MP3_SetVolume(uint8_t volume);
void MP3_Stop(void);
void MP3_Play(void);
void MP3_Pause(void);
void MP3_Reset(void);

void MP3_Task(void);
void SayTime(void);
uint16_t GenarateTrack(void);

extern MP3_Player_t MP3_Player;

#endif /* MP3_PLAYER_H_ */
