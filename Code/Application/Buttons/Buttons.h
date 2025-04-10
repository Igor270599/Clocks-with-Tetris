/*
 * Buttons.h
 *
 *  Created on: Oct 13, 2024
 *      Author: Игорь
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stm32f1xx_hal.h>

#define BTN_COUNT			6
#define DEBOUNCE_TIMEOUT	5
#define CLICK_TIMEOUT		35
#define PRESS_TIMEOUT_3S	300

#define CONTINUOUSLY_MARK	200

#define LEFT_BUTTON				0
#define UP_BUTTON				1
#define RIGHT_BUTTON			2
#define DOWN_BUTTON				3
#define MENU_BUTTON				4
#define JOESTICK_BUTTON			5

#define COUNTER_LIMIT		65000

typedef enum Key_Code_t
{
	NONE_BUTTON									= 0,
	CHANGE_MODE_BUTTON							= 1,
	CLOCK_CHANGE_FONT_BUTTON					= 2,
	CLOCK_CHANGE_STYLE_BUTTON					= 3,
	SAY_TIME_BUTTON								= 4,
	COUNT_DOWN_BUTTON							= 5,
	AUTO_BRIGHTNESS_BUTTON						= 6,
	CLOCK_ANIMATION_BUTTON						= 7,
	CLOCK_BORDER_BUTTON							= 8,
	ALARM_TIMER_DISABLE_BUTTON					= 9,
	DECREASE_BRIGHTNESS_BUTTON					= 10,
	INCREASE_BRIGHTNESS_BUTTON					= 11,
	DECREASE_VOLUME_BUTTON						= 12,
	INCREASE_VOLUME_BUTTON						= 13,
												  
	ROUSER_INCREASE_HOURS_BUTTON				= 14,
	ROUSER_INCREASE_MINUTES_BUTTON				= 15,
	ROUSER_CHANGE_FONT_BUTTON					= 16,
	ROUSER_BORDER_BUTTON						= 17,
	ALARM_OFF_BUTTON							= 18,
	ALARM_ON_BUTTON								= 19,
	CHANGE_ALARM_TYPE_BUTTON					= 20,
	RETURN_TO_CLOCK_MODE_BUTTON					= 21,
	SET_CLOCK_TIME_BUTTON						= 22,
												  
	TIMER_INCREASE_HOURS_BUTTON					= 23,
	TIMER_INCREASE_MINUTES_BUTTON				= 24,
	TIMER_CHANGE_FONT_BUTTON					= 25,
	TIMER_ANIMATION_BUTTON						= 26,
	TIMER_BORDER_BUTTON							= 27,
	TIMER_OFF_BUTTON							= 28,
	TIMER_ON_BUTTON								= 29,
	TIMER_ALARM_OFF_BUTTON						= 30,

	CHANGE_MP3_PLAYER_ANIMATION_BUTTON			= 31,
	NEXT_TRECK_BUTTON							= 32,
	PREV_TRECK_BUTTON							= 33,
	PAUSE_PLAY_BUTTON							= 34,
												  
	CHANGE_TETRIS_MELODY_BUTTON					= 35,
	ENABLE_DISABLE_TETRIS_MELODY_BUTTON			= 36,
	TETRIS_CHANGE_FONT_BUTTON 					= 37,
	FAST_FALL_BUTTON							= 38,
	RESTART_GAME_BUTTON							= 39,
	CHANGE_START_SPEED_BUTTON					= 40,

	CLOCK_SWOWN_DATE_BUTTON						= 41,
	CLOCK_SAY_DATE_BUTTON						= 42,

	INCREASE_HOURS_CONTINUOUSLY					= 201,
	INCREASE_MINUTES_CONTINUOUSLY				= 202,
	DECREASE_BRIGHTNESS_CONTINUOUSLY			= 203,
	INCREASE_BRIGHTNESS_CONTINUOUSLY			= 204,
	DECREASE_VOLUME_CONTINUOUSLY				= 205,
	INCREASE_VOLUME_CONTINUOUSLY				= 206,
}Key_Code_t;

typedef enum joestick_command_t
{
	NONE_ACTION                        = 0,
    UP_ACTION                          = 1,
    DOWN_ACTION                        = 2,
    LEFT_ACTION                        = 3,
    RIGHT_ACTION                       = 4,
} joestick_command_t ;

typedef struct Joestick_t
{
	uint16_t  				ADC_Data_X;
	uint16_t  				ADC_Data_Y;
	joestick_command_t		command;
}Joestick_t;

typedef enum State_t
{
	INIT		= 0x00,
	DOWN		= 0x01,
	UP			= 0x02,
	COUNT		= 0x03,
	PRESS		= 0x04,
	PRESS_END	= 0x05,
}State_t;

typedef enum Level_t
{
	LOW		= 0x00,
	HIGHT	= 0x01,
}Level_t;

typedef enum Action_t
{
	NO_ACTION	= 0x00,
	CLICK		= 0x01,
	LONG		= 0x02,
}Action_t;

typedef struct Buttons_t
{
	uint16_t BtnPressCounter[BTN_COUNT];
	uint16_t BtnIdleCounter[BTN_COUNT];
	Level_t	level[BTN_COUNT];
	State_t	 state[BTN_COUNT];
	Action_t action[BTN_COUNT];
	uint8_t clickCounter[BTN_COUNT];
}Buttons_t;


void ReadButtons(void);
void ProcessButtons(uint8_t count);
void ButtonsTask(void);
void KeysReset(void);
void ProcessJoestick(void);

extern Buttons_t Buttons;

#endif /* BUTTONS_H_ */
