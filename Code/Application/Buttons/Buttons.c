/*
 * Buttons.c
 *
 *  Created on: Oct 13, 2024
 *      Author: Игорь
 */
#include "Buttons.h"
#include "../App/Application.h"

void ButtonsTask(void)
{
uint8_t i=0, pressedButtons=0;
	ReadButtons();
	for(i=0;i<BTN_COUNT;i++)
	{
		if(Buttons.level[i]==LOW)
		{
			pressedButtons++;
			switch(Buttons.state[i])
			{
				case INIT:
					if(Buttons.BtnPressCounter[i]>DEBOUNCE_TIMEOUT)
					{
						Buttons.state[i]=DOWN;
					}
				break;

				case DOWN:
					if(Buttons.BtnPressCounter[i]>PRESS_TIMEOUT_3S)
					{
						Buttons.state[i]=PRESS;
						Buttons.action[i]=LONG;
					}
				break;

				case UP:
					if(Buttons.BtnPressCounter[i]>DEBOUNCE_TIMEOUT)
					{
						Buttons.state[i]=COUNT;
						Buttons.action[i]=NO_ACTION;
						Buttons.clickCounter[i]++;
					}
				break;

				default: break;
			}
		}
		else
		{
			switch(Buttons.state[i])
			{
				case DOWN:
						Buttons.state[i]=UP;
				break;

				case UP:
					if(Buttons.BtnIdleCounter[i]>CLICK_TIMEOUT)
					{
						Buttons.state[i]=INIT;
						Buttons.action[i]=CLICK;
					}
				break;

				case COUNT:
					Buttons.state[i]=UP;
					Buttons.action[i]=NO_ACTION;
				break;

				case PRESS:
					Buttons.state[i]=INIT;
					Buttons.action[i]=NO_ACTION;
				break;

				default: break;
			}

		}
	}
	ProcessButtons(pressedButtons);
}

void ProcessButtons(uint8_t count)
{
Key_Code_t result=NONE_BUTTON;
uint8_t i;
bool isManyClicks=false;

	if(count==0)
	{
		for(i=0;i<BTN_COUNT;i++)
		{
			if(Buttons.clickCounter[i]!=0)
			{
				isManyClicks=true;
				break;
			}
		}

		if(isManyClicks==false)		//одиночный клик
		{
			if(Buttons.action[LEFT_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:			result=CLOCK_CHANGE_FONT_BUTTON;				break;
					case ROUSER:		result=ROUSER_INCREASE_HOURS_BUTTON;			break;
					case TIMER: 		result=TIMER_INCREASE_HOURS_BUTTON;				break;
					case MP3_PLAYER: 	result=CHANGE_MP3_PLAYER_ANIMATION_BUTTON;		break;
					case TETRIS: 		result=CHANGE_START_SPEED_BUTTON;				break;
					case AADITIONAL_FUNCTIONS:result=DECREASE_BRIGHTNESS_BUTTON;		break;
					default:			result=NONE_BUTTON;	KeysReset();				break;
				}
			}

			if(Buttons.action[RIGHT_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:	result=CLOCK_CHANGE_STYLE_BUTTON;		break;
					case ROUSER:result=ROUSER_INCREASE_MINUTES_BUTTON;	break;
					case TIMER: result=TIMER_INCREASE_MINUTES_BUTTON;	break;
					case MP3_PLAYER: result=CLOCK_CHANGE_STYLE_BUTTON;	break;
					case AADITIONAL_FUNCTIONS:result=INCREASE_BRIGHTNESS_BUTTON;break;
					default:	result=NONE_BUTTON;	KeysReset();		break;
				}

			}

			if(Buttons.action[UP_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:			result=SAY_TIME_BUTTON;			break;
					case ROUSER:		result=ALARM_ON_BUTTON;			break;
					case TIMER: 		result=TIMER_ON_BUTTON;			break;
					case MP3_PLAYER: 	result=NEXT_TRECK_BUTTON;		break;
					case AADITIONAL_FUNCTIONS:result=INCREASE_VOLUME_BUTTON;break;
					default:	result=NONE_BUTTON;	KeysReset();		break;
				}
			}

			if(Buttons.action[DOWN_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:			result=CLOCK_SAY_DATE_BUTTON;		break;
					case ROUSER:		result=ALARM_OFF_BUTTON;			break;
					case TIMER: 		result=TIMER_OFF_BUTTON;			break;
					case MP3_PLAYER: 	result=PREV_TRECK_BUTTON;			break;
					case AADITIONAL_FUNCTIONS:result=DECREASE_VOLUME_BUTTON;break;
					default:			result=NONE_BUTTON;	KeysReset();	break;
				}
			}

			if(Buttons.action[MENU_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:
					case ROUSER:
					case TIMER:
					case MP3_PLAYER:
					case TETRIS:
						result=CHANGE_MODE_BUTTON;
					break;
					default:			result=NONE_BUTTON;	KeysReset();	break;
				}
			}

			if(Buttons.action[JOESTICK_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:
					case ROUSER:
					case TIMER:
					case MP3_PLAYER:
						result=NONE_BUTTON;	KeysReset();
					break;

					case TETRIS:
						result=FAST_FALL_BUTTON;
					break;

					default:			result=NONE_BUTTON;	KeysReset();	break;
				}
			}

		}
		else	//двойной клик
		{
			if(Buttons.action[UP_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:		 result=CLOCK_ANIMATION_BUTTON;			break;
					case ROUSER:	 result=CHANGE_ALARM_TYPE_BUTTON; 		break;
					case TIMER: 	 result=TIMER_ANIMATION_BUTTON;			break;
					case MP3_PLAYER: result=CLOCK_ANIMATION_BUTTON;		break;
					default:		 result=NONE_BUTTON; KeysReset();		break;
				}
			}

			if(Buttons.action[DOWN_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:	result=CLOCK_BORDER_BUTTON;				break;
					case ROUSER:result=ROUSER_BORDER_BUTTON;			break;
					case TIMER: result=TIMER_BORDER_BUTTON;				break;
					case MP3_PLAYER: result=CLOCK_BORDER_BUTTON;		break;
					default:	result=NONE_BUTTON;	 KeysReset();		break;
				}
			}

			if(Buttons.action[LEFT_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:		 result=COUNT_DOWN_BUTTON;				break;
					case ROUSER:	 result=ROUSER_CHANGE_FONT_BUTTON;		break;
					case TIMER: 	 result=TIMER_CHANGE_FONT_BUTTON;		break;
					case MP3_PLAYER: result=CLOCK_CHANGE_FONT_BUTTON;		break;
					case TETRIS: 	 result=TETRIS_CHANGE_FONT_BUTTON;		break;
					default:	result=NONE_BUTTON;	KeysReset();			break;
				}
			}

			if(Buttons.action[RIGHT_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:	result=CLOCK_SWOWN_DATE_BUTTON;				break;
					default:	result=NONE_BUTTON;		KeysReset();		break;
				}
			}

			if(Buttons.action[MENU_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case CLOCK:			result=AUTO_BRIGHTNESS_BUTTON;			break;
					case ROUSER:		result=RETURN_TO_CLOCK_MODE_BUTTON;		break;
					case TIMER: 		result=RETURN_TO_CLOCK_MODE_BUTTON;		break;
					case MP3_PLAYER:	result=PAUSE_PLAY_BUTTON;				break;
					case TETRIS:		result=CHANGE_TETRIS_MELODY_BUTTON;		break;
					default:	result=NONE_BUTTON;		KeysReset();	break;
				}
			}

			if(Buttons.action[JOESTICK_BUTTON]==CLICK)
			{
				switch(Application.mode)
				{
					case TETRIS:		result=RESTART_GAME_BUTTON;		break;
					default:	result=NONE_BUTTON;		KeysReset();	break;
				}
			}

		}
	}


	if(count==1)
	{
		if(Buttons.action[MENU_BUTTON]==LONG)
		{
			switch(Application.mode)
			{
				case CLOCK:			result=ALARM_TIMER_DISABLE_BUTTON;				break;
				case ROUSER:		result=SET_CLOCK_TIME_BUTTON;					break;
				case TIMER: 		result=TIMER_ALARM_OFF_BUTTON;					break;
				case MP3_PLAYER:	result=RETURN_TO_CLOCK_MODE_BUTTON;				break;
				case TETRIS:		result=ENABLE_DISABLE_TETRIS_MELODY_BUTTON;		break;
				default:			result=NONE_BUTTON; KeysReset();				break;
			}
		}

		if(Buttons.action[DOWN_BUTTON]==LONG)
		{
			switch(Application.mode)
			{
				default:	result=DECREASE_VOLUME_CONTINUOUSLY;	 	break;
			}
		}

		if(Buttons.action[LEFT_BUTTON]==LONG)
		{
			switch(Application.mode)
			{
				case CLOCK:
				case MP3_PLAYER:
				case TETRIS:
				case AADITIONAL_FUNCTIONS:
					result=DECREASE_BRIGHTNESS_CONTINUOUSLY;
				break;

				case ROUSER:
				case TIMER:
					result=INCREASE_HOURS_CONTINUOUSLY;
				break;

				default:	result=NONE_BUTTON;	KeysReset();		break;
			}
		}

		if(Buttons.action[RIGHT_BUTTON]==LONG)
		{
			switch(Application.mode)
			{
				case CLOCK:
				case MP3_PLAYER:
				case TETRIS:
				case AADITIONAL_FUNCTIONS:
					result=INCREASE_BRIGHTNESS_CONTINUOUSLY;
				break;

				case ROUSER:
				case TIMER:
					result=INCREASE_MINUTES_CONTINUOUSLY;
				break;
				default:	result=NONE_BUTTON;	KeysReset();		break;
			}
		}

		if(Buttons.action[UP_BUTTON]==LONG)
		{
			switch(Application.mode)
			{
				default:	result=INCREASE_VOLUME_CONTINUOUSLY;		break;
			}
		}
	}

	if(count==0)
	{
		switch(Application.keyCode)
		{
			case INCREASE_BRIGHTNESS_CONTINUOUSLY:
			case DECREASE_BRIGHTNESS_CONTINUOUSLY:
			case DECREASE_VOLUME_CONTINUOUSLY:	
			case INCREASE_VOLUME_CONTINUOUSLY:	
			case INCREASE_HOURS_CONTINUOUSLY:		
			case INCREASE_MINUTES_CONTINUOUSLY:	
				Application.keyCode=NONE_BUTTON;
			default: break;
		}
	}

	if(result!=NONE_BUTTON)
	{
		Application.keyCode=result;
		KeysReset();
	}

}

void ReadButtons(void)
{
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)==0)
	{
		if(Buttons.BtnPressCounter[LEFT_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnPressCounter[LEFT_BUTTON]++;
		Buttons.level[LEFT_BUTTON]=LOW;
		Buttons.BtnIdleCounter[LEFT_BUTTON]=0;
	}
	else
	{
		if(Buttons.BtnIdleCounter[LEFT_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnIdleCounter[LEFT_BUTTON]++;
		Buttons.level[LEFT_BUTTON]=HIGHT;
		Buttons.BtnPressCounter[LEFT_BUTTON]=0;
	}

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)==0)
	{
		if(Buttons.BtnPressCounter[UP_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnPressCounter[UP_BUTTON]++;
		Buttons.level[UP_BUTTON]=LOW;
		Buttons.BtnIdleCounter[UP_BUTTON]=0;
	}
	else
	{
		if(Buttons.BtnIdleCounter[UP_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnIdleCounter[UP_BUTTON]++;
		Buttons.level[UP_BUTTON]=HIGHT;
		Buttons.BtnPressCounter[UP_BUTTON]=0;
	}

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)==0)
	{
		if(Buttons.BtnPressCounter[RIGHT_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnPressCounter[RIGHT_BUTTON]++;
		Buttons.level[RIGHT_BUTTON]=LOW;
		Buttons.BtnIdleCounter[RIGHT_BUTTON]=0;
	}
	else
	{
		if(Buttons.BtnIdleCounter[RIGHT_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnIdleCounter[RIGHT_BUTTON]++;
		Buttons.level[RIGHT_BUTTON]=HIGHT;
		Buttons.BtnPressCounter[RIGHT_BUTTON]=0;
	}

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==0)
	{
		if(Buttons.BtnPressCounter[DOWN_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnPressCounter[DOWN_BUTTON]++;
		Buttons.level[DOWN_BUTTON]=LOW;
		Buttons.BtnIdleCounter[DOWN_BUTTON]=0;
	}
	else
	{
		if(Buttons.BtnIdleCounter[DOWN_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnIdleCounter[DOWN_BUTTON]++;
		Buttons.level[DOWN_BUTTON]=HIGHT;
		Buttons.BtnPressCounter[DOWN_BUTTON]=0;
	}

	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==0)
	{
		if(Buttons.BtnPressCounter[MENU_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnPressCounter[MENU_BUTTON]++;
		Buttons.level[MENU_BUTTON]=LOW;
		Buttons.BtnIdleCounter[MENU_BUTTON]=0;
	}
	else
	{
		if(Buttons.BtnIdleCounter[MENU_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnIdleCounter[MENU_BUTTON]++;
		Buttons.level[MENU_BUTTON]=HIGHT;
		Buttons.BtnPressCounter[MENU_BUTTON]=0;
	}

	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)==0)
	{
		if(Buttons.BtnPressCounter[JOESTICK_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnPressCounter[JOESTICK_BUTTON]++;
		Buttons.level[JOESTICK_BUTTON]=LOW;
		Buttons.BtnIdleCounter[JOESTICK_BUTTON]=0;
	}
	else
	{
		if(Buttons.BtnIdleCounter[JOESTICK_BUTTON]<COUNTER_LIMIT)
			Buttons.BtnIdleCounter[JOESTICK_BUTTON]++;
		Buttons.level[JOESTICK_BUTTON]=HIGHT;
		Buttons.BtnPressCounter[JOESTICK_BUTTON]=0;
	}
}

void ProcessJoestick(void)
{
joestick_command_t result=NONE_ACTION;

	if((Joestick.ADC_Data_X>=0)&&(Joestick.ADC_Data_X<150))
		result=LEFT_ACTION;
	if((Joestick.ADC_Data_X>3900)&&(Joestick.ADC_Data_X<4095))
		result=RIGHT_ACTION;
	if((Joestick.ADC_Data_Y>=0)&&(Joestick.ADC_Data_Y<150))
		result=UP_ACTION;
	if((Joestick.ADC_Data_Y>3900)&&(Joestick.ADC_Data_Y<4095))
		result=DOWN_ACTION;

	/*
	if((result==UP_ACTION)||(result==DOWN_ACTION))
	{
		while(true)
		{
			if(((Joestick.ADC_Data_X>1900)&&(Joestick.ADC_Data_X<2100))&&((Joestick.ADC_Data_Y>1900)&&(Joestick.ADC_Data_Y<2100)))
			{
				break;
			}
		}
	}
	*/

	//if(!((result==NONE_ACTION)&&(Joestick.command!=NONE_ACTION)))
		Joestick.command=result;
}


void KeysReset(void)
{
	for(uint8_t i=0;i<BTN_COUNT;i++)
	{
		Buttons.action[i]=NO_ACTION;
		Buttons.clickCounter[i]=0;
	}
}
