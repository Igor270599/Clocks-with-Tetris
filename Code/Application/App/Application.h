/*
 * Application.h
 *
 *  Created on: Sep 15, 2024
 *      Author: Игорь
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <stdint.h>
#include <stdbool.h>
#include <stm32f103xb.h>
#include <stm32f1xx_hal.h>

#include "../MAX_7219_Driver/MAX_7219.h"
#include "../Buttons/Buttons.h"
#include "../RTC_DS3231/RTC_DS3231_Driver.h"
#include "../MPU6050_Driver/mpu6050.h"
#include "../MP3_Player_TD5580/MP3_Player.h"

typedef struct Timers_t
{
	uint8_t 	Timer_1_ms;
	uint8_t 	Timer_10_ms;
	uint8_t 	Timer_100_ms;
	uint32_t 	Timer_1_s;
	bool		blink;
}Timers_t;

#define Get_Timer_1_ms()	Timers.Timer_1_ms
#define Get_Timer_10_ms()	Timers.Timer_10_ms
#define Get_Timer_100_ms()	Timers.Timer_100_ms
#define Get_Timer_1_s()		Timers.Timer_1_s

#define ACCELERARTION_LIMIT_MAX 12000
#define ACCELERARTION_LIMIT_MIN 3000

#define BRIGHTNESS_DARK_LEVEL 	1200
#define BRIGHTNESS_MAX_LEVEL 	3200

#define INTENSIVITY_DEFAULT_VALUE	4
#define INTENSIVITY_DARK_VALUE		0
#define INTENSIVITY_NORMAL_VALUE	2
#define INTENSIVITY_HIGHT_VALUE		5

#define TIMER_ALARM_TIMEOUT		300		//(60*5=300) Время через которое сигналавтоматически выключится

typedef struct Clock_Def_t
{
	uint8_t			hours;
	uint8_t			minutes;
	uint8_t			seconds;
	t_clock_style 	clock_style;
	t_font_style	font_style;
	bool	 		isBorderEnabled;
	bool	 		isDotsBlinked;
}Clock_Def_t;

typedef enum t_animationType
{
    DISABLED          =0,
    RANDOM_DOTS       =1,// delay(500)
    FALLING_DOTS      =2,// delay(100)
    CURRENT_TIME      =3,
	MOVEING_DOTS	  =4,
} t_animationType;

typedef enum t_alarm_type
{
    ONCE          	=0,
    EVERY_DAY       =1,// delay(500)
} t_alarm_type;


typedef struct Mp3_Player_Def_t
{
	uint16_t			prevTrack;
	t_animationType		animationVariant;
}Mp3_Player_Def_t;

typedef struct Tetris_Def_t
{
	uint16_t		score;
	uint8_t			speed;
	uint8_t			Startspeed;
	uint8_t			Melody;
	bool			isSoundEnabled;
	t_font_style	font_style;
	uint8_t			HEIGHT;
	uint8_t			WIDTH;
	uint8_t			figure;
	uint8_t			angle;
}Tetris_Def_t;

typedef struct Rouser_Def_t
{
	uint8_t			hours;
	uint8_t			minutes;
	t_font_style	font_style;
	bool	 		isAlarmEnabled;
	bool	 		isAlarmChecked;
	bool	 		isSignalEnabled;
	bool	 		isBorderEnabled;
	uint16_t		timeout;
	t_alarm_type 	alarmType;
}Rouser_Def_t;

typedef struct Timer_Def_t
{
	uint8_t			hours;
	uint8_t			minutes;
	t_font_style	font_style;
	bool	 		isTimerEnabled;
	bool	 		isSignalEnabled;
	bool	 		isBorderEnabled;
	bool	 		isDotsBlinked;
	uint8_t 		remainder;			//остаток секунд в минуте для повышения точности
	uint32_t 		startTime;			//остаток секунд в минуте для повышения точности
	uint16_t		timeout;
}Timer_Def_t;

typedef enum light_level_t
{
	DARK	=0,
	NORMAL	=1,
	BRIGHT	=2,
} light_level_t;

typedef struct Light_Sensor_t
{
	uint16_t		data[10];
	uint16_t		value;
	light_level_t	level;
	uint8_t			dataIndex;
	bool	 		isDataReady;
}Light_Sensor_t;


typedef enum Mode_t				//не менять порядок
{
	NONE_MODE				=0,
	CLOCK					=1,
	ROUSER					=2,
	TIMER					=3,
	MP3_PLAYER				=4,
	TETRIS					=5,
	AADITIONAL_FUNCTIONS	=6,
}Mode_t;

typedef enum addtionalFunctions_t
{
	NO_FUNCTION					=0,
	SET_BRIGHTNESS				=1,
	SET_VOLUME					=2,
	COUNT_DOWN					=3,
	SHOWN_ALARM_TYPE			=4,
	SHOWN_DATE					=5,
	SHOWN_SET					=6,
	SHOWN_AUTO_BRIGHTNESS		=7,
}addtionalFunctions_t;

typedef struct Application_t
{
	Mode_t					mode;
	Mode_t					returnMode;
	addtionalFunctions_t	addtionalFunctions;
	Key_Code_t				keyCode;
	t_rotation 				rotation;
	Clock_Def_t 			clockDefinition;
	Timer_Def_t				timerDefinition;
	Rouser_Def_t			rouserDefinition;
	Tetris_Def_t			tetrisDefinition;
	Mp3_Player_Def_t		mp3_PlayerDefinition;
	uint8_t					additionalFunctionsTimer;
	uint8_t					intensivity;
	bool					isAutoBrightnessEnabled;
	bool					forceUpdate;
	uint8_t 				volume;
	uint8_t 				saved_volume;
}Application_t;

void AppInit(void);
void AppTask(void);
void CreateImage(void);
void SetOrintation(void);

void ChangeModeButton(void);
void ChangeFontButton(void);
void ClockChangeStyleButton(void);
void AnimationButton(void);
void BorderButton(void);
void RouserIncreaseMinutesButton(void);
void RouserIncreaseHoursButton(void);
void RouserAlarmOffButton(void);
void RouserAlarmOnButton(void);
void TimerOffButton(void);
void TimerOnButton(void);
void TimerIncreaseHoursButton(void);
void TimerIncreaseMinutesButton(void);
void IncreaseMinutesButton(void);
void IncreaseHoursButton(void);
void EnterAditionalMode(void);
void DecreaseBrightness(void);
void IncreaseBrightness(void);
void DecreaseVolume(void);
void IncreaseVolume(void);
void ChangeMP3_PlayerAnimationButton(void);
void UpdateTime(void);
void SayTimeButton(void);
void MP3_PlayNextTrack();
void MP3_PlayPrevTrack();
void SetClockTimeButton(void);
void CountDownButton(void);
void AlarmTimerDisableButton(void);
void RouserChangeAlarmTypeButton (void);
void LightSensorAverage (void);
void ChangeTetrisMelodyButton(void);
void EnableDisabTetetrisMelody(void);
void ChangeTetrisStartSpeed();
void ClockShownDateButton(void);
void ClockSayDateButton(void);
void ReturnToClockModeButton(void);
void TimerAlarmOffButton(void);
void EnableDisableAutoBrightness(void);

uint16_t Random(uint16_t minValue, uint16_t maxValue);

extern Timers_t Timers;
extern Application_t Application;
extern Light_Sensor_t Light_Sensor;
extern uint16_t  ADC_Data_X;
extern uint16_t  ADC_Data_Y;
extern I2C_HandleTypeDef hi2c1;

extern ADC_HandleTypeDef hadc1;
extern Joestick_t Joestick;
extern UART_def_t UART_1;

#endif /* APPLICATION_H_ */
