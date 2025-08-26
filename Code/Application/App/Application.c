/*
 * Application.c
 *
 *  Created on: Sep 15, 2024
 *      Author: Игорь
 */
#include "Application.h"
#include "../Display/Display.h"
#include "../Tetris/Tetris.h"

Timers_t Timers={0};
Application_t Application;
Buttons_t Buttons;
RTC_t RTC_data;
MPU6050_t MPU6050;
MP3_Player_t MP3_Player;
Joestick_t Joestick;
Light_Sensor_t Light_Sensor;

void AppInit(void)
{
	HAL_NVIC_EnableIRQ(SPI2_IRQn);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
	/*
	HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
	HAL_ADC_Start_IT(&hadc);
	*/
	Application.mode=CLOCK;
	Application.rotation=ROTATION_90_DEGREES;
	Application.clockDefinition.clock_style=BIG_FOUR_DIGITS;
	Application.clockDefinition.font_style=NEOCLASSIC;
	Application.clockDefinition.hours=20;
	Application.clockDefinition.minutes=3;
	Application.clockDefinition.seconds=13;
	Application.clockDefinition.isBorderEnabled=true;
	Application.clockDefinition.isDotsBlinked=true;
	Application.volume=4;
	Application.mp3_PlayerDefinition.animationVariant=FALLING_DOTS;
	Application.intensivity=0;
	Application.isAutoBrightnessEnabled=true;
	Application.rouserDefinition.hours=20;
	Application.rouserDefinition.minutes=25;
	Application.tetrisDefinition.isSoundEnabled=true;
	Application.tetrisDefinition.Startspeed=3;
	HAL_Delay(500);
	while (MPU6050_Init(&hi2c1) == 1);
	MPU6050_Read_All(&hi2c1, &MPU6050);
	DS3231_Init(&hi2c1);
	DS3231_Collect_data(&RTC_data);
	InitMax7219(8);
	SetIntensivity(Application.intensivity);
	UpdateMax7219();
	HAL_ADC_Start_IT(&hadc1);
	MP3_Reset();
	MP3_SetVolume(Application.volume);
	HAL_Delay(1000);
	MP3_Stop();
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET );
	//StartGame();
	//DS3231_SetFullTime(13, 35, 0);
	//DS3231_SetFullDate(16, 11, 6, 2024);
	while(1)
	{
		AppTask();
	}
}

void AppTask(void)
{
static uint8_t	prevTick=0;
static uint8_t	prevIntensivity=0;
static uint8_t	prevVolume=0;
bool keyResetFlag=true;

	if(prevTick!=Get_Timer_1_ms())
	{
//---------------------------------------------------------------------------- 1 ms
		prevTick=Get_Timer_1_ms();

//---------------------------------------------------------------------------- 1 ms
		if(Get_Timer_1_ms()>=10)
		{
//---------------------------------------------------------------------------- 10 ms
			Timers.Timer_10_ms++;
			Timers.Timer_1_ms=0;
			ButtonsTask();
//---------------------------------------------------------------------------- 10 ms
			if(Get_Timer_10_ms()==10)
			{
//---------------------------------------------------------------------------- 100 ms
				Timers.Timer_100_ms++;
				Timers.Timer_10_ms=0;
				LightSensorAverage();
				CreateImage();
				ProcessJoestick();
				if((Get_Timer_100_ms()%2)==0)
				{
					if(Application.keyCode!=NONE_BUTTON)
					{
						switch(Application.keyCode)
						{
							case INCREASE_HOURS_CONTINUOUSLY: 		IncreaseHoursButton(); break;
							case INCREASE_MINUTES_CONTINUOUSLY: 	IncreaseMinutesButton(); break;

							case INCREASE_BRIGHTNESS_CONTINUOUSLY:
							case DECREASE_BRIGHTNESS_CONTINUOUSLY:
							case DECREASE_VOLUME_CONTINUOUSLY:
							case INCREASE_VOLUME_CONTINUOUSLY:
								if(Application.mode!=AADITIONAL_FUNCTIONS)
								{
									EnterAditionalMode();
								}

								if(Application.addtionalFunctions==SET_BRIGHTNESS)
								{
									if(Application.keyCode==INCREASE_BRIGHTNESS_CONTINUOUSLY)
									{
										IncreaseBrightness();
										Application.additionalFunctionsTimer=5;
									}

									if(Application.keyCode==DECREASE_BRIGHTNESS_CONTINUOUSLY)
									{
										DecreaseBrightness();
										Application.additionalFunctionsTimer=5;
									}
								}

								if(Application.addtionalFunctions==SET_VOLUME)
								{
									if(Application.keyCode==INCREASE_VOLUME_CONTINUOUSLY)
									{
										IncreaseVolume();
										Application.additionalFunctionsTimer=5;
									}

									if(Application.keyCode==DECREASE_VOLUME_CONTINUOUSLY)
									{
										DecreaseVolume();
										Application.additionalFunctionsTimer=5;
									}
								}
							break;
							default:			break;
						}
					}
				}
//---------------------------------------------------------------------------- 100 ms
				if(Get_Timer_100_ms()==10)
				{
					Timers.Timer_1_s++;
					Timers.blink=!(Timers.blink);
					Timers.Timer_100_ms=0;
				}
//---------------------------------------------------------------------------- 1 s
					switch(Get_Timer_100_ms()%10)
					{
					case	1:

						if((Application.keyCode>CONTINUOUSLY_MARK)||(Application.keyCode==FAST_FALL_BUTTON))
						{
							keyResetFlag=false;
						}

						if(Application.keyCode!=NONE_BUTTON)
						{
							switch(Application.keyCode)
							{                                           
								case CHANGE_MODE_BUTTON:				ChangeModeButton();		break;

								case CLOCK_CHANGE_FONT_BUTTON:
								case ROUSER_CHANGE_FONT_BUTTON:
								case TIMER_CHANGE_FONT_BUTTON:
								case TETRIS_CHANGE_FONT_BUTTON:
									ChangeFontButton();
								break;

								case CLOCK_ANIMATION_BUTTON:
								case TIMER_ANIMATION_BUTTON:
									AnimationButton();
								break;

								case CLOCK_BORDER_BUTTON:
								case ROUSER_BORDER_BUTTON:
								case TIMER_BORDER_BUTTON:
									BorderButton();
								break;

								case ROUSER_INCREASE_HOURS_BUTTON:
								case TIMER_INCREASE_HOURS_BUTTON:
									IncreaseHoursButton();
								break;

								case ROUSER_INCREASE_MINUTES_BUTTON:
								case TIMER_INCREASE_MINUTES_BUTTON:
									IncreaseMinutesButton();
								break;

								case CLOCK_SWOWN_DATE_BUTTON: 			ClockShownDateButton();			break;
								case CLOCK_SAY_DATE_BUTTON: 			ClockSayDateButton();			break;
								case CLOCK_CHANGE_STYLE_BUTTON: 		ClockChangeStyleButton();		break;
								case SAY_TIME_BUTTON: 					SayTimeButton();				break;
								case COUNT_DOWN_BUTTON: 				CountDownButton();				break;
								case AUTO_BRIGHTNESS_BUTTON: 			EnableDisableAutoBrightness();	break;

								case ALARM_TIMER_DISABLE_BUTTON:		AlarmTimerDisableButton();		break;

								case DECREASE_BRIGHTNESS_BUTTON:
								case INCREASE_BRIGHTNESS_BUTTON:
									if(Application.addtionalFunctions==SET_BRIGHTNESS)
									{
										if(Application.keyCode==DECREASE_BRIGHTNESS_BUTTON)
											DecreaseBrightness();

										if(Application.keyCode==INCREASE_BRIGHTNESS_BUTTON)
											IncreaseBrightness();

										Application.additionalFunctionsTimer=5;
									}
								break;

								case DECREASE_VOLUME_BUTTON:
								case INCREASE_VOLUME_BUTTON:
									if(Application.addtionalFunctions==SET_VOLUME)
									{
										if(Application.keyCode==DECREASE_VOLUME_BUTTON)
											DecreaseVolume();

										if(Application.keyCode==INCREASE_VOLUME_BUTTON)
											IncreaseVolume();

										Application.additionalFunctionsTimer=5;
									}
								break;

								case ALARM_OFF_BUTTON: 					RouserAlarmOffButton();				break;
								case ALARM_ON_BUTTON: 					RouserAlarmOnButton();				break;
								case CHANGE_ALARM_TYPE_BUTTON: 			RouserChangeAlarmTypeButton();		break;
								case RETURN_TO_CLOCK_MODE_BUTTON: 		ReturnToClockModeButton();			break;
								case SET_CLOCK_TIME_BUTTON: 			SetClockTimeButton();				break;
								case TIMER_OFF_BUTTON: 					TimerOffButton();					break;
								case TIMER_ON_BUTTON:					TimerOnButton();					break;
								case TIMER_ALARM_OFF_BUTTON:			AlarmTimerDisableButton();				break;

								case CHANGE_MP3_PLAYER_ANIMATION_BUTTON: 	ChangeMP3_PlayerAnimationButton(); 			break;
								case NEXT_TRECK_BUTTON: 					MP3_PlayNextTrack();						break;
								case PREV_TRECK_BUTTON: 					MP3_PlayPrevTrack();						break;
								case PAUSE_PLAY_BUTTON:
									if(MP3_Player.state==MP3_BYSY)
									{
										MP3_Pause();
									}
									else
									{
										MP3_Play();
									}
								break;

								case CHANGE_TETRIS_MELODY_BUTTON: 				ChangeTetrisMelodyButton();			break;
								case ENABLE_DISABLE_TETRIS_MELODY_BUTTON: 		EnableDisabTetetrisMelody();		break;
								case RESTART_GAME_BUTTON: 						RestartGame();						break;
								case CHANGE_START_SPEED_BUTTON:					ChangeTetrisStartSpeed();			break;
								default:																			break;
							}
						}
						if(keyResetFlag)
						{
							Application.keyCode=NONE_BUTTON;
							if((Get_Timer_1_s()%5)==0)
							{
								KeysReset();				//сбрасываем кнопки раз в 5 секунд для безопасности
							}
						}
					break;

					case	3:
						if(Application.additionalFunctionsTimer!=0)
						{
							Application.additionalFunctionsTimer--;
						}
						else
						{
							if(Application.mode==AADITIONAL_FUNCTIONS)
							{
								Application.mode=Application.returnMode;
								Application.returnMode=NONE_MODE;
								Application.addtionalFunctions=NO_FUNCTION;
							}
						}

						if(Application.rouserDefinition.timeout != 0)
						{
							Application.rouserDefinition.timeout--;
						}
						else
						{
							if(Application.rouserDefinition.isSignalEnabled==true)
							{
								AlarmTimerDisableButton();
							}
						}

						if(Application.timerDefinition.timeout != 0)
						{
							Application.timerDefinition.timeout--;
						}
						else
						{
							if(Application.timerDefinition.isSignalEnabled==true)
							{
								AlarmTimerDisableButton();
							}
						}

						UpdateTime();
						MPU6050_Read_All(&hi2c1, &MPU6050);
						SetOrintation();

						MP3_Task();
					break;

					case	5:
						if((Application.clockDefinition.hours==Application.rouserDefinition.hours)&&(Application.clockDefinition.minutes==Application.rouserDefinition.minutes))
						{
							if((Application.rouserDefinition.isSignalEnabled==false)&&(Application.rouserDefinition.isAlarmChecked==false)&&(Application.rouserDefinition.isAlarmEnabled==true))
							{
								Application.rouserDefinition.isSignalEnabled=true;
								Application.rouserDefinition.timeout=TIMER_ALARM_TIMEOUT;
								Application.saved_volume=Application.volume;
								Application.volume=10;
								MP3_SetVolume(Application.volume);
								HAL_Delay(500);
								MP3_PlayTrack(ROUSER_SIGNAL);
							}
						}
						else
						{
							Application.rouserDefinition.isAlarmChecked=false;
						}

						if(Application.timerDefinition.isTimerEnabled == true)
						{
							if((Get_Timer_1_s()%60==Application.timerDefinition.remainder)&&(Get_Timer_1_s() != Application.timerDefinition.startTime))
							{
								if(Application.timerDefinition.minutes==0)
								{
									if(Application.timerDefinition.hours!=0)
									{
										Application.timerDefinition.hours--;
										Application.timerDefinition.minutes=59;
									}
								}
								else
								{
									Application.timerDefinition.minutes--;
								}
							}
						}

						if((Application.timerDefinition.isTimerEnabled == true)&&(Application.timerDefinition.hours == 0)&&(Application.timerDefinition.minutes == 0))
						{
							if(Application.timerDefinition.isSignalEnabled==false)
							{
								Application.timerDefinition.isSignalEnabled=true;
								Application.timerDefinition.timeout=TIMER_ALARM_TIMEOUT;
								Application.saved_volume=Application.volume;
								Application.volume=10;
								MP3_SetVolume(Application.volume);
								HAL_Delay(500);
								MP3_PlayTrack(TIMER_SIGNAL);
							}
						}
					break;

					case	7:

					break;

					case	9:

					break;

					case	0:
					case	2:
					case	4:
					case	6:
					case	8:

						if(Application.isAutoBrightnessEnabled==true)
						{
							switch (Light_Sensor.level)
							{
								case DARK:
									Application.intensivity=INTENSIVITY_DARK_VALUE;
								break;

								case NORMAL:
									Application.intensivity=INTENSIVITY_NORMAL_VALUE;
								break;

								case BRIGHT:
									Application.intensivity=INTENSIVITY_HIGHT_VALUE;
								break;

								default:
									Application.intensivity=INTENSIVITY_NORMAL_VALUE;
								break;
							}
						}

						if(prevIntensivity!=Application.intensivity)
						{
							SetIntensivity(Application.intensivity);
							prevIntensivity=Application.intensivity;
						}

						if(prevVolume!=Application.volume)
						{
							MP3_SetVolume(Application.volume);
							prevVolume=Application.volume;
						}

					break;
					}
//---------------------------------------------------------------------------- 1 s
			}
		}
	}
}

void EnableDisableAutoBrightness(void)
{
	if(Application.isAutoBrightnessEnabled == true)
	{
		Application.isAutoBrightnessEnabled=false;
		Application.intensivity=INTENSIVITY_DEFAULT_VALUE;
	}
	else
	{
		Application.isAutoBrightnessEnabled=true;
	}

	Application.addtionalFunctions=SHOWN_AUTO_BRIGHTNESS;
	Application.returnMode=Application.mode;
	Application.mode=AADITIONAL_FUNCTIONS;
	Application.additionalFunctionsTimer=5;
}

void ReturnToClockModeButton(void)
{
	MP3_Stop();
	Application.mode=CLOCK;
}

void ClockShownDateButton(void)
{
	if(Application.addtionalFunctions!=SHOWN_DATE)
	{
		Application.addtionalFunctions=SHOWN_DATE;
		Application.returnMode=Application.mode;
		Application.mode=AADITIONAL_FUNCTIONS;
		Application.additionalFunctionsTimer=5;
	}
}

void ClockSayDateButton(void)
{
	MP3_Player.function=SAY_DATE;
	MP3_Player.delay=1;
	Application.saved_volume=Application.volume;
	Application.volume=10;
	MP3_SetVolume(Application.volume);
	HAL_Delay(300);
}

void ChangeTetrisStartSpeed()
{
	if(Application.tetrisDefinition.Startspeed==0)
		Application.tetrisDefinition.Startspeed=3;
	else
		Application.tetrisDefinition.Startspeed--;
}

void EnableDisabTetetrisMelody(void)
{
	if(Application.tetrisDefinition.isSoundEnabled == true)
	{
		MP3_Stop();
		Application.tetrisDefinition.isSoundEnabled=false;
	}
	else
	{
		MP3_PlayTrack(TETRIS_OFFSET+Application.tetrisDefinition.Melody);
		Application.tetrisDefinition.isSoundEnabled = true;
	}
}

void ChangeTetrisMelodyButton(void)
{
	MP3_Stop();
	HAL_Delay(500);
	Application.tetrisDefinition.Melody++;

	if(Application.tetrisDefinition.Melody==TETRIS_MELODY_COUNTER)
		Application.tetrisDefinition.Melody=0;

	MP3_PlayTrack(TETRIS_OFFSET+Application.tetrisDefinition.Melody);
}

void ShowLightSensorData(void)
{
	uint8_t a,b,c,d;
	a=Light_Sensor.value/1000;
	b=(Light_Sensor.value-a*1000)/100;
	c=(Light_Sensor.value-a*1000-b*100)/10;
	d=(Light_Sensor.value-a*1000-b*100-c*10);

	DrowDigit(6,12,a,CLASSIC,SMALL,ROTATION_90_DEGREES);
	DrowDigit(12,12,b,CLASSIC,SMALL,ROTATION_90_DEGREES);
	DrowDigit(6,21,c,CLASSIC,SMALL,ROTATION_90_DEGREES);
	DrowDigit(12,21,d,CLASSIC,SMALL,ROTATION_90_DEGREES);
	}

void LightSensorAverage (void)
{
uint32_t summ=0;
	if(Light_Sensor.isDataReady==true)
	{
		for(uint8_t i=0;i<10;i++)
		{
			summ=summ+Light_Sensor.data[i];
		}
		Light_Sensor.value=summ/10;

		if(Light_Sensor.value<BRIGHTNESS_DARK_LEVEL)
			Light_Sensor.level=DARK;

		if((Light_Sensor.value>=BRIGHTNESS_DARK_LEVEL)&&(Light_Sensor.value<=BRIGHTNESS_MAX_LEVEL))
			Light_Sensor.level=NORMAL;

		if(Light_Sensor.value>BRIGHTNESS_MAX_LEVEL)
			Light_Sensor.level=BRIGHT;

		Light_Sensor.isDataReady=false;
	}
}

void CreateImage(void)
{
	if(!((Application.mode==MP3_PLAYER)||(Application.mode==TETRIS)))
	{
		ClearMax7219();	// для плеера очистку мартцы вызывает функциия анимации
	}
	switch(Application.mode)
	{
		case CLOCK:
			WriteTime(Application.clockDefinition,Application.rotation);
		break;

		case ROUSER:
			WriteRouser(Application.rouserDefinition,Application.rotation);
			//ShowLightSensorData(); // только ROTATION_90_DEGREES
		break;

		case TIMER:
			WriteTimer(Application.timerDefinition,Application.rotation);
		break;

		case TETRIS:
			WriteTetrisMode(Application.rotation);
		break;

		case AADITIONAL_FUNCTIONS:
			 WriteAdditionalMode(Application.rotation);
		break;

		case MP3_PLAYER:
			 WriteMP3_PlayerAnimation(Application.mp3_PlayerDefinition.animationVariant,Application.rotation);
		break;

		default:		break;
	}
	UpdateMax7219();
}

void SetOrintation(void)
{
	if((MPU6050.Accel_X_RAW>ACCELERARTION_LIMIT_MAX)&&(abs(MPU6050.Accel_Y_RAW)<ACCELERARTION_LIMIT_MIN)&&(abs(MPU6050.Accel_Z_RAW)<ACCELERARTION_LIMIT_MIN))
	{
		MPU6050.orientation=DOWN_SIDE;
	}

	if((MPU6050.Accel_X_RAW<(-1)*ACCELERARTION_LIMIT_MAX)&&(abs(MPU6050.Accel_Y_RAW)<ACCELERARTION_LIMIT_MIN)&&(abs(MPU6050.Accel_Z_RAW)<ACCELERARTION_LIMIT_MIN))
	{
		MPU6050.orientation=UP_SIDE;
	}

	if((MPU6050.Accel_Z_RAW>ACCELERARTION_LIMIT_MAX)&&(abs(MPU6050.Accel_Y_RAW)<ACCELERARTION_LIMIT_MIN)&&(abs(MPU6050.Accel_X_RAW)<ACCELERARTION_LIMIT_MIN))
	{
		MPU6050.orientation=LEFT_SIDE;
	}

	if((MPU6050.Accel_Z_RAW<(-1)*ACCELERARTION_LIMIT_MAX)&&(abs(MPU6050.Accel_Y_RAW)<ACCELERARTION_LIMIT_MIN)&&(abs(MPU6050.Accel_X_RAW)<ACCELERARTION_LIMIT_MIN))
	{
		MPU6050.orientation=RIGHT_SIDE;
	}

	if((MPU6050.Accel_Y_RAW>ACCELERARTION_LIMIT_MAX)&&(abs(MPU6050.Accel_X_RAW)<ACCELERARTION_LIMIT_MIN)&&(abs(MPU6050.Accel_Z_RAW)<ACCELERARTION_LIMIT_MIN))
	{
		MPU6050.orientation=TOP_SIDE;
	}

	if((MPU6050.Accel_Y_RAW<(-1)*ACCELERARTION_LIMIT_MAX)&&(abs(MPU6050.Accel_X_RAW)<ACCELERARTION_LIMIT_MIN)&&(abs(MPU6050.Accel_Z_RAW)<ACCELERARTION_LIMIT_MIN))
	{
		MPU6050.orientation=BOTTOM_SIDE;
	}


	switch (MPU6050.orientation)
	{
		case DOWN_SIDE:
			Application.rotation=ROTATION_0_DEGREES;
		break;

		case UP_SIDE:
			Application.rotation=ROTATION_180_DEGREES;
		break;

		case LEFT_SIDE:
			Application.rotation=ROTATION_90_DEGREES;
		break;

		case RIGHT_SIDE:
			Application.rotation=ROTATION_270_DEGREES;
		break;

		case TOP_SIDE:
		case BOTTOM_SIDE:
			Application.rotation=Application.rotation;
		break;

		default:	break;
	}
}

void RouserChangeAlarmTypeButton (void)
{
	if(Application.rouserDefinition.alarmType==ONCE)
	{
		Application.rouserDefinition.alarmType=EVERY_DAY;
	}
	else
	{
		Application.rouserDefinition.alarmType=ONCE;
	}
	Application.rouserDefinition.isAlarmEnabled=false;
	RouserAlarmOnButton();
}

void AlarmTimerDisableButton(void)
{


	if(Application.rouserDefinition.isSignalEnabled==true)
	{
		if(Application.rouserDefinition.alarmType==ONCE)
		{
			Application.rouserDefinition.isAlarmEnabled=false;
		}

		Application.rouserDefinition.isSignalEnabled=false;
		Application.rouserDefinition.isAlarmChecked=true;
	}

	if(Application.timerDefinition.isSignalEnabled==true)
	{
		Application.timerDefinition.isSignalEnabled=false;
		Application.timerDefinition.isTimerEnabled=false;
	}

	MP3_Stop();
	Application.volume=Application.saved_volume;
}

void SayTimeButton(void)
{
	SayTime();
}

void SetClockTimeButton(void)
{
	if(Application.addtionalFunctions!=SHOWN_SET)
	{
		Application.addtionalFunctions=SHOWN_SET;
		Application.returnMode=Application.mode;
		Application.mode=AADITIONAL_FUNCTIONS;
		Application.additionalFunctionsTimer=3;
		DS3231_SetFullTime(Application.rouserDefinition.hours, Application.rouserDefinition.minutes, 0);
		Application.rouserDefinition.hours=0;
		Application.rouserDefinition.minutes=0;
	}
}

void CountDownButton(void)
{
uint16_t trackNumber,a;
	if(Application.addtionalFunctions!=COUNT_DOWN)
	{
		Application.addtionalFunctions=COUNT_DOWN;
		Application.returnMode=Application.mode;
		Application.mode=AADITIONAL_FUNCTIONS;
		a=Random(0, 1);
		MP3_Stop();
		HAL_Delay(500);
		if(a==0)
			trackNumber=COUNT_DOWN_VARIANT_ONE;
		else
		{
			trackNumber=COUNT_DOWN_VARIANT_TWO;
		}
		MP3_PlayTrack(trackNumber);
		while(MP3_Player.state!=MP3_IDLE)
		{
			MP3_Task();
		}
		Application.additionalFunctionsTimer=11;
	}
}
void MP3_PlayNextTrack()
{

uint16_t trackNumber;
	MP3_Stop();
	HAL_Delay(1000);
	Application.mp3_PlayerDefinition.prevTrack=MP3_Player.trackNumber;
	trackNumber=GenarateTrack();
	MP3_PlayTrack(trackNumber);
}

void MP3_PlayPrevTrack()
{
	if(Application.mp3_PlayerDefinition.prevTrack!=0)
	{
		MP3_Stop();
		HAL_Delay(1000);
		MP3_PlayTrack(Application.mp3_PlayerDefinition.prevTrack);
		Application.mp3_PlayerDefinition.prevTrack=0;
	}
}

void ChangeMP3_PlayerAnimationButton(void)
{

	if(Application.mp3_PlayerDefinition.animationVariant==CURRENT_TIME)
		Application.mp3_PlayerDefinition.animationVariant=RANDOM_DOTS;
	else
		Application.mp3_PlayerDefinition.animationVariant++;
}

void IncreaseBrightness(void)
{
	if(Application.intensivity<15)
	{
		Application.intensivity++;
	}
}

void DecreaseBrightness(void)
{
	if(Application.intensivity>0)
	{
		Application.intensivity--;
	}
}

void IncreaseVolume(void)
{
	if(Application.volume<30)
	{
		Application.volume++;
	}
}

void DecreaseVolume(void)
{
	if(Application.volume>0)
	{
		Application.volume--;
	}
}

void EnterAditionalMode(void)
{
	Application.returnMode=Application.mode;
	Application.additionalFunctionsTimer=5;
	Application.mode=AADITIONAL_FUNCTIONS;

	if((Application.keyCode==DECREASE_BRIGHTNESS_CONTINUOUSLY)||(Application.keyCode==INCREASE_BRIGHTNESS_CONTINUOUSLY))
	{
		Application.addtionalFunctions=SET_BRIGHTNESS;
	}

	if((Application.keyCode==DECREASE_VOLUME_CONTINUOUSLY)||(Application.keyCode==INCREASE_VOLUME_CONTINUOUSLY))
	{
		Application.addtionalFunctions=SET_VOLUME;
	}

	if(Application.keyCode==CHANGE_ALARM_TYPE_BUTTON)
	{
		Application.addtionalFunctions=SHOWN_ALARM_TYPE;
	}
}

void ChangeMode(Mode_t newMode)
{
uint16_t seed;

	switch(newMode)
	{
		case CLOCK:
			MP3_Stop();
		break;

		case ROUSER:
			MP3_Stop();
		break;

		case TIMER:
			MP3_Stop();
		break;

		case MP3_PLAYER:
			seed= getSeed();
			srand(seed);
			Application.mp3_PlayerDefinition.prevTrack=0;
			MP3_Player.trackNumber=0;
			MP3_Player.isPaused=false;
			MP3_Stop();
		break;

		case TETRIS:
		 MP3_Stop();
		 HAL_Delay(500);
		 StartGame();
		break;

		case AADITIONAL_FUNCTIONS:

		break;

		default: 	break;
	}
}

void TimerOffButton(void)
{
	if(Application.timerDefinition.isTimerEnabled==true)
	{
		Application.timerDefinition.isTimerEnabled=false;
		AlarmTimerDisableButton();
	}
}

void TimerOnButton(void)
{
	if(Application.timerDefinition.isTimerEnabled==false)
	{
		if(!((Application.timerDefinition.hours==0)&&(Application.timerDefinition.minutes==0)))
		{
			MP3_Stop();
			HAL_Delay(500);
			MP3_PlayTrack(TIMER_SET);
			MP3_Player.state=MP3_BYSY;
			HAL_Delay(500);
			while(MP3_Player.state!=MP3_IDLE)
			{
				MP3_Task();
			}
			HAL_Delay(250);
			MP3_PlayTrack(Application.timerDefinition.hours+HOURS_OFFSET);
			HAL_Delay(1500);
			MP3_PlayTrack(Application.timerDefinition.minutes+MINUTES_OFFSET);

			Application.timerDefinition.isTimerEnabled=true;
			Application.timerDefinition.remainder=Get_Timer_1_s()%60;
			Application.timerDefinition.startTime=Get_Timer_1_s();
		}
	}
}

void IncreaseHoursButton(void)
{
	switch(Application.mode)
	{
		case ROUSER:
			if(Application.rouserDefinition.hours==23)
			{
				Application.rouserDefinition.hours=0;
			}
			else
			{
				Application.rouserDefinition.hours++;
			}
		break;

		case TIMER:
			if(Application.timerDefinition.isTimerEnabled==false)
			{
				if(Application.timerDefinition.hours==23)
				{
					Application.timerDefinition.hours=0;
				}
				else
				{
					Application.timerDefinition.hours++;
				}
			}
		break;

		default: break;
	}
}

void IncreaseMinutesButton(void)
{
	switch(Application.mode)
	{
		case ROUSER:
			if(Application.rouserDefinition.minutes==59)
			{
				Application.rouserDefinition.minutes=0;
			}
			else
			{
				Application.rouserDefinition.minutes++;
			}
		break;

		case TIMER:
			if(Application.timerDefinition.isTimerEnabled==false)
			{
				if(Application.timerDefinition.minutes==59)
				{
					Application.timerDefinition.minutes=0;
				}
				else
				{
					Application.timerDefinition.minutes++;
				}
			}
		break;

		default: break;
	}
}

void RouserAlarmOffButton(void)
{
	if(Application.rouserDefinition.isAlarmEnabled==true)
	{
		Application.rouserDefinition.isAlarmEnabled=false;
	}
}

void RouserAlarmOnButton(void)
{
	if(Application.rouserDefinition.isAlarmEnabled==false)
	{
		Application.rouserDefinition.isAlarmEnabled=true;
		MP3_Stop();
		HAL_Delay(500);
		MP3_PlayTrack(ROUSER_SET);
		MP3_Player.state=MP3_BYSY;
		HAL_Delay(500);
		while(MP3_Player.state!=MP3_IDLE)
		{
			MP3_Task();
		}
		HAL_Delay(250);
		MP3_PlayTrack(Application.rouserDefinition.hours+HOURS_OFFSET);
		HAL_Delay(1500);
		MP3_PlayTrack(Application.rouserDefinition.minutes+MINUTES_OFFSET);
		HAL_Delay(1500);
		if(Application.rouserDefinition.alarmType==EVERY_DAY)
		{
			MP3_PlayTrack(ROUSER_EVERYDAY);
		}
		else
		{
			MP3_PlayTrack(ROUSER_ONCE);
		}
		Application.returnMode=Application.mode;
		Application.additionalFunctionsTimer=3;
		Application.mode=AADITIONAL_FUNCTIONS;
		Application.addtionalFunctions=SHOWN_ALARM_TYPE;
		Application.rouserDefinition.isAlarmChecked=false;
	}
}

void ChangeModeButton(void)
{
	if(Application.mode==TETRIS)
	{
		Application.mode=CLOCK;
	}
	else
	{
		Application.mode++;
	}

	ChangeMode(Application.mode);
}

void AnimationButton(void)
{
	switch(Application.mode)
	{
		case CLOCK:
		case MP3_PLAYER:
			if(Application.clockDefinition.isDotsBlinked==true)
				Application.clockDefinition.isDotsBlinked=false;
			else
				Application.clockDefinition.isDotsBlinked=true;
		break;

		case TIMER:
			if(Application.timerDefinition.isDotsBlinked==true)
				Application.timerDefinition.isDotsBlinked=false;
			else
				Application.timerDefinition.isDotsBlinked=true;
		break;

		default: break;
	}

}

void BorderButton(void)
{
	switch(Application.mode)
	{
		case CLOCK:
		case MP3_PLAYER:
			if(Application.clockDefinition.isBorderEnabled==true)
				Application.clockDefinition.isBorderEnabled=false;
			else
				Application.clockDefinition.isBorderEnabled=true;
		break;

		case ROUSER:
			if(Application.rouserDefinition.isBorderEnabled==true)
				Application.rouserDefinition.isBorderEnabled=false;
			else
				Application.rouserDefinition.isBorderEnabled=true;
		break;

		case TIMER:
			if(Application.timerDefinition.isBorderEnabled==true)
				Application.timerDefinition.isBorderEnabled=false;
			else
				Application.timerDefinition.isBorderEnabled=true;
		break;

		default: break;
	}
}

void ClockChangeStyleButton(void)
{
	Application.clockDefinition.clock_style++;
	if(Application.clockDefinition.clock_style>=4)
	{
		Application.clockDefinition.clock_style=0;
	}
}

void ChangeFontButton(void)
{
	switch(Application.mode)
	{
		case CLOCK:
		case MP3_PLAYER:
			if(Application.clockDefinition.font_style==NEOCLASSIC)
			{
				Application.clockDefinition.font_style=CLASSIC;
			}
			else
			{
				Application.clockDefinition.font_style=NEOCLASSIC;
			}
		break;

		case ROUSER:
			if(Application.rouserDefinition.font_style==NEOCLASSIC)
			{
				Application.rouserDefinition.font_style=CLASSIC;
			}
			else
			{
				Application.rouserDefinition.font_style=NEOCLASSIC;
			}
		break;

		case TIMER:
			if(Application.timerDefinition.font_style==NEOCLASSIC)
			{
				Application.timerDefinition.font_style=CLASSIC;
			}
			else
			{
				Application.timerDefinition.font_style=NEOCLASSIC;
			}
		break;

		case TETRIS:
			if(Application.tetrisDefinition.font_style==NEOCLASSIC)
			{
				Application.tetrisDefinition.font_style=CLASSIC;
			}
			else
			{
				Application.tetrisDefinition.font_style=NEOCLASSIC;
			}
		break;

		default: break;
	}
}


void UpdateTime(void)
{
	DS3231_Collect_data(&RTC_data);
	Application.clockDefinition.hours=RTC_data.Hours;
	Application.clockDefinition.minutes=RTC_data.Minutes;
	Application.clockDefinition.seconds=RTC_data.Secounds;
}


uint16_t Random(uint16_t minValue, uint16_t maxValue)
{
int16_t result;
int16_t buf;

	buf=maxValue-minValue+1;
	result=rand();
	result=abs(result);
	result=(result%buf)+minValue;

	return (uint16_t)result;
}
