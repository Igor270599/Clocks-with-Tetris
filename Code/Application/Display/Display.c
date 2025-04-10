/*
 * Display.c
 *
 *  Created on: Oct 20, 2024
 *      Author: Игорь
 */

#include "Display.h"
#include "../Tetris/Tetris.h"

t_Point previusPosition={4,8};
bool stopFlag=false;
FigureDef queue[4];

//#define START_WITH_SPECIAL_FIELD	//!!! ДЛЯ КОРРЕКТНОЙ РАБОТЫ УБРАТЬ ЗАКОМЕНТИРОВАТЬ fixedFigures[i]=0 В StartGame();

#ifdef START_WITH_SPECIAL_FIELD
uint16_t fixedFigures[23] =
{
	65472,
	65471,
	32768,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};
#else
	uint16_t fixedFigures[23] = {0};
#endif

void WriteMP3_PlayerAnimation(t_animationType animationType,t_rotation rotation)
{
uint8_t dotsCount,randLimitMax,randLimitMin,x,y;
int8_t i,j;
bool buf;

static uint8_t delay;
static uint8_t currentDelay=0;

	if(delay!=currentDelay)
	{
		delay++;
		return;
	}

	delay=0;

	switch(animationType)
	{
		case RANDOM_DOTS:
			randLimitMax=100;
			randLimitMin=10;
			dotsCount=(uint8_t)Random((uint16_t)randLimitMin,(uint16_t)randLimitMax);
			currentDelay=5;
			ClearMax7219();
			for(i=0;i<dotsCount;i++)
			{
				x=(uint8_t)Random(0,15);
				y=(uint8_t)Random(0,31);
				SetPixel(x, y, true);
			}
		break;

		case FALLING_DOTS:
			if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES))
			{
				randLimitMax=4;
				randLimitMin=0;
				dotsCount=(uint8_t)Random((uint16_t)randLimitMin,(uint16_t)randLimitMax);
				currentDelay=2;

				if(rotation==ROTATION_0_DEGREES)
				{
					for(i=14;i>=0;i--)
					{
						for(j=0;j<32;j++)
						{
							buf=GetPixel(i,j);
							if(buf==true)
							{
								SetPixel(i+1, j, true);
							}
							else
							{
								SetPixel(i+1, j, false);
							}
							SetPixel(i, j, false);
						}
					}

					for(i=0;i<dotsCount;i++)
					{
						y=(uint8_t)Random(0,31);
						SetPixel(0, y, true);
					}
				}


				if(rotation==ROTATION_180_DEGREES)
				{
					for(i=1;i<16;i++)
					{
						for(j=0;j<32;j++)
						{
							buf=GetPixel(i,j);
							if(buf==true)
							{
								SetPixel(i-1, j, true);
							}
							else
							{
								SetPixel(i-1, j, false);
							}
							SetPixel(i, j, false);
						}
					}

					for(i=0;i<dotsCount;i++)
					{
						y=(uint8_t)Random(0,31);
						SetPixel(15, y, true);
					}
				}


			}

			if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
			{
				randLimitMax=3;
				randLimitMin=0;
				dotsCount=(uint8_t)Random((uint16_t)randLimitMin,(uint16_t)randLimitMax);
				currentDelay=2;

				if(rotation==ROTATION_270_DEGREES)
				{
					for(i=1;i<32;i++)
					{
						Matrix.data[i-1]=Matrix.data[i];
						Matrix.data[i]=0;
					}

					for(i=0;i<dotsCount;i++)
					{
						y=(uint8_t)Random(0,15);
						SetPixel(y, 31, true);
					}
				}

				if(rotation==ROTATION_90_DEGREES)
				{
					for(i=30;i>=0;i--)
					{
						Matrix.data[i+1]=Matrix.data[i];
						Matrix.data[i]=0;
					}

					for(i=0;i<dotsCount;i++)
					{
						y=(uint8_t)Random(0,15);
						SetPixel(y, 0, true);
					}
				}
			}
		break;

		case CURRENT_TIME:
			ClearMax7219();
			WriteTime(Application.clockDefinition,rotation);
		break;

		case MOVEING_DOTS:

		break;

		default:
		break;
	}
}

void FixFigures(void)
{
uint8_t i,j;
bool buf;
	for(i=0;i<10;i++)
	{
		for(j=31;j>=9;j--)
		{
			buf=GetPixel(i, j);
			FixPixel(i,31-j,buf);
		}
	}
}

void FixPixel(uint8_t i, uint8_t j, bool state)
{
	if(state==true)
	{
		fixedFigures[j]=fixedFigures[j]|(1<<(15-i));
	}
	else
	{
		fixedFigures[j]=fixedFigures[j]&(~(1<<(15-i)));
	}
}

bool GetFixedPixel(uint8_t i, uint8_t j)
{
bool result;

	if(fixedFigures[i]&(1<<(15-j)))
	{
		result=true;
	}
	else
	{
		result=false;
	}

	return result;
}

void WriteFixedFigures(void)
{
uint8_t i,j;
bool buf;

	for(i=0;i<22;i++)
	{
		for(j=0;j<10;j++)
		{
			buf=GetFixedPixel(i, j);
			SetPixel(j,31-i,buf);
		}
	}
}

void RestartRound(void)
{
uint8_t i;

	previusPosition.x=4;
	previusPosition.y=8;

	for(i=0;i<3;i++)
	{
		queue[i].figure=queue[i+1].figure;
		queue[i].angle=queue[i+1].angle;
	}

	queue[3].figure=Random(0, 6);
	queue[3].angle=0;

	Application.tetrisDefinition.figure=queue[0].figure;
	Application.tetrisDefinition.angle=Random(0,3);
}

void WriteTetrisMode(t_rotation rotation)
{
uint8_t i,j,currentSpeed;
bool result=false;
static uint8_t delay,steps=0;
static bool	 fastFall=false;

if(fastFall==true)
{
	currentSpeed=0;
}
else
{
	currentSpeed=Application.tetrisDefinition.speed;
}

if(delay!=currentSpeed)
{
	delay++;
	if(delay>10)	//защита от изменения скорости программатором
		delay=0;

	if(Application.forceUpdate==false)	//если ускоренное обновление, то не выходим из функции
		return;

}

	delay=0;
	ClearMax7219();

	switch(rotation)
	{
		case ROTATION_0_DEGREES:
			DrowDigit(4,26,E_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(4,20,R_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(4,14,R_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(4,8,O_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(4,2,R_LETTER,CLASSIC,LETTER,rotation);
		break;

		case ROTATION_90_DEGREES:
			Application.tetrisDefinition.HEIGHT=10;
			Application.tetrisDefinition.WIDTH=24;
			for(i=0;i<=10;i++)
			{
				SetPixel(i, 8, true);
			}

			for(j=8;j<=31;j++)
			{
				SetPixel(10, j, true);
			}

			DrowDigit(15,25,(4-Application.tetrisDefinition.speed),Application.tetrisDefinition.font_style,SMALL,rotation);
			WriteScore(Application.tetrisDefinition.score,rotation,false);
			WriteFixedFigures();
			WriteQueue();
			if(Joestick.command==LEFT_ACTION)
			{
				if(checkArea(previusPosition.x,previusPosition.y,LEFT_AND_FIGURES)==false)
					previusPosition.x--;
			}

			if(Joestick.command==RIGHT_ACTION)
			{
				if(checkArea(previusPosition.x,previusPosition.y,RIGHT_AND_FIGURES)==false)
					previusPosition.x++;
			}


			if(Joestick.command==UP_ACTION)
			{
				if(checkArea(previusPosition.x,previusPosition.y,ROTATION)==false)
				{
					Application.tetrisDefinition.angle++;
					if (Application.tetrisDefinition.angle == 4)
						Application.tetrisDefinition.angle = 0;
				}
			}

			if(Application.keyCode==FAST_FALL_BUTTON)
			{
				Application.keyCode=NONE_BUTTON;
				KeysReset();
				fastFall=true;
			}

			if(Joestick.command==DOWN_ACTION)
			{
				previusPosition.x=4;
				previusPosition.y=8;
			}

			Joestick.command=NONE_ACTION;

			if(Application.forceUpdate==false)
			{
				drawFigure(Application.tetrisDefinition.figure,Application.tetrisDefinition.angle,previusPosition.x,previusPosition.y,true,rotation);
			}
			result=checkArea(previusPosition.x,previusPosition.y,FLOOR_AND_FIGURES);

			if(result)
			{
				if(steps==0)
				{
					RestartGame();
					RestartRound();
					return;
				}

				FixFigures();
				ClearLines();
				RestartRound();
				fastFall=false;
				steps=0;
				return;
			}
			else
			{
				previusPosition.y++;
				steps++;
			}

		break;

		case ROTATION_180_DEGREES:
			DrowDigit(11,5,E_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(11,11,R_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(11,17,R_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(11,23,O_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(11,29,R_LETTER,CLASSIC,LETTER,rotation);
		break;

		case ROTATION_270_DEGREES:

			DrowDigit(6,27,E_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(6,19,R_LETTER,CLASSIC,LETTER,rotation);
			DrowDigit(6,11,R_LETTER,CLASSIC,LETTER,rotation);


			 //рисование рамки для тетриса
			for(i=5;i<=15;i++)
			{
				SetPixel(i, 23, true);
			}

			for(j=0;j<=23;j++)
			{
				SetPixel(5, j, true);
			}

			DrowDigit(0,6,Application.tetrisDefinition.speed,Application.tetrisDefinition.font_style,SMALL,rotation);
			WriteScore(Application.tetrisDefinition.score,rotation,false);
		break;
	}
}

void WriteQueue(void)
{
	drawFigure(queue[1].figure,queue[1].angle,13 ,10 ,true,ROTATION_90_DEGREES);
	drawFigure(queue[2].figure,queue[2].angle,13 ,15 ,true,ROTATION_90_DEGREES);
	drawFigure(queue[3].figure,queue[3].angle,13 ,20 ,true,ROTATION_90_DEGREES);
}

void WriteScore(uint16_t score, t_rotation rotation,bool endGame)
{
uint8_t a,b,c,counter=1;

a=score/100;
b=(score-a*100)/10;
c=score-a*100-b*10;

if(a!=0)
{
	counter=3;
}

if((b!=0)&&(a==0))
{
	counter++;
}

	switch(rotation)
	{
		case ROTATION_90_DEGREES:
			if(endGame==false)
			{
				switch(counter)
				{
					case 1:
						DrowDigit(9,0,c,Application.tetrisDefinition.font_style,SMALL,rotation);
					break;

					case 2:
						DrowDigit(6,0,b,Application.tetrisDefinition.font_style,SMALL,rotation);
						DrowDigit(12,0,c,Application.tetrisDefinition.font_style,SMALL,rotation);
					break;

					case 3:
						DrowDigit(4,0,a,Application.tetrisDefinition.font_style,SMALL,rotation);
						DrowDigit(9,0,b,Application.tetrisDefinition.font_style,SMALL,rotation);
						DrowDigit(14,0,c,Application.tetrisDefinition.font_style,SMALL,rotation);
					break;

					default:

					break;
				}
			}
			else
			{
				DrowDigit(4,8,a,Application.tetrisDefinition.font_style,SMALL,rotation);
				DrowDigit(9,8,b,Application.tetrisDefinition.font_style,SMALL,rotation);
				DrowDigit(14,8,c,Application.tetrisDefinition.font_style,SMALL,rotation);
			}
		break;

		case ROTATION_270_DEGREES:
			switch(counter)
			{
				case 1:
					DrowDigit(6,31,c,Application.tetrisDefinition.font_style,SMALL,rotation);
				break;

				case 2:
					DrowDigit(9,31,b,Application.tetrisDefinition.font_style,SMALL,rotation);
					DrowDigit(3,31,c,Application.tetrisDefinition.font_style,SMALL,rotation);
				break;

				case 3:
					DrowDigit(11,31,a,Application.tetrisDefinition.font_style,SMALL,rotation);
					DrowDigit(6,31,b,Application.tetrisDefinition.font_style,SMALL,rotation);
					DrowDigit(1,31,c,Application.tetrisDefinition.font_style,SMALL,rotation);
				break;

				default:

				break;
			}
		break;

		default:

		break;
	}
}

void WriteAdditionalMode(t_rotation rotation)
{
uint8_t value,letter,v,vv,j,i;
t_font_style style;
bool isNeedBorder=false;

	if(Application.addtionalFunctions==SHOWN_DATE)
	{
		WriteTimeSmallSixDigits(RTC_data.Day, RTC_data.Mounth, (RTC_data.Year%100), Application.rotation, Application.clockDefinition.clock_style, Application.clockDefinition.isBorderEnabled, false);
	}

	if(Application.addtionalFunctions==SHOWN_AUTO_BRIGHTNESS)
	{
		style=Application.clockDefinition.font_style;
		if(Application.isAutoBrightnessEnabled == true)
			v=1;
		else
			v=0;

		switch(rotation)
		{
			case ROTATION_0_DEGREES:
				DrowDigit(4,22,A_LETTER,style,LETTER,rotation);
				DrowDigit(4,16,B_LETTER,style,LETTER,rotation);
				for(i=10;i<15;i++)
				{
					SetPixel(7,i,true);
				}
				DrowDigit(4,5,v,style,SMALL,rotation);
			break;

			case ROTATION_90_DEGREES:
				DrowDigit(6,8,A_LETTER,style,LETTER,rotation);
				DrowDigit(13,8,B_LETTER,style,LETTER,rotation);
				DrowDigit(9,17,v,style,SMALL,rotation);
			break;

			case ROTATION_180_DEGREES:
				DrowDigit(11,9,A_LETTER,style,LETTER,rotation);
				DrowDigit(11,15,B_LETTER,style,LETTER,rotation);
				for(i=17;i<22;i++)
				{
					SetPixel(8,i,true);
				}
				DrowDigit(11,26,v,style,SMALL,rotation);
			break;

			case ROTATION_270_DEGREES:
				DrowDigit(9,23,A_LETTER,style,LETTER,rotation);
				DrowDigit(2,23,B_LETTER,style,LETTER,rotation);
				DrowDigit(6,14,v,style,SMALL,rotation);
			break;
		}
	}

	if(Application.addtionalFunctions==SHOWN_SET)
	{
		style=Application.clockDefinition.font_style;
		switch(rotation)
		{
			case ROTATION_0_DEGREES:
				DrowDigit(4,20,S_LETTER,style,LETTER,rotation);
				DrowDigit(4,14,E_LETTER,style,LETTER,rotation);
				DrowDigit(4,8,T_LETTER,style,LETTER,rotation);
			break;

			case ROTATION_90_DEGREES:
				DrowDigit(9,5,S_LETTER,style,LETTER,rotation);
				DrowDigit(9,13,E_LETTER,style,LETTER,rotation);
				DrowDigit(9,21,T_LETTER,style,LETTER,rotation);
			break;

			case ROTATION_180_DEGREES:
				DrowDigit(11,11,S_LETTER,style,LETTER,rotation);
				DrowDigit(11,17,E_LETTER,style,LETTER,rotation);
				DrowDigit(11,23,T_LETTER,style,LETTER,rotation);
			break;

			case ROTATION_270_DEGREES:
				DrowDigit(6,26,S_LETTER,style,LETTER,rotation);
				DrowDigit(6,18,E_LETTER,style,LETTER,rotation);
				DrowDigit(6,10,T_LETTER,style,LETTER,rotation);
			break;
		}
	}

	if((Application.addtionalFunctions==SET_BRIGHTNESS)||(Application.addtionalFunctions==SET_VOLUME))
	{
		style=Application.clockDefinition.font_style;

		if(Application.addtionalFunctions==SET_BRIGHTNESS)
		{
			letter=B_LETTER;
			value=Application.intensivity;
		}

		if(Application.addtionalFunctions==SET_VOLUME)
		{
			letter=V_LETTER;
			value=Application.volume;
		}

		v=value/10;
		vv=value-v*10;

		switch(rotation)
		{
			case ROTATION_0_DEGREES:
				DrowDigit(4,22,letter,style,LETTER,rotation);
				DrowDigit(4,10,v,style,SMALL,rotation);
				DrowDigit(4,5,vv,style,SMALL,rotation);

				for(j=15;j<=20;j++)
				{
					SetPixel(7, j, true);
				}

			break;

			case ROTATION_90_DEGREES:
				DrowDigit(10,8,letter,style,LETTER,rotation);
				DrowDigit(6,17,v,style,SMALL,rotation);
				DrowDigit(12,17,vv,style,SMALL,rotation);
			break;

			case ROTATION_180_DEGREES:
				DrowDigit(11,9,letter,style,LETTER,rotation);
				DrowDigit(11,21,v,style,SMALL,rotation);
				DrowDigit(11,26,vv,style,SMALL,rotation);

				for(j=11;j<=16;j++)
				{
					SetPixel(8, j, true);
				}
			break;

			case ROTATION_270_DEGREES:
				DrowDigit(5,23,letter,style,LETTER,rotation);
				DrowDigit(9,14,v,style,SMALL,rotation);
				DrowDigit(3,14,vv,style,SMALL,rotation);
			break;
		}
	}

	if(Application.addtionalFunctions==COUNT_DOWN)
	{
		value=Application.additionalFunctionsTimer;

		if(value==11)	//Для синхронизации звука и картинки
			value=10;

		v=value/10;
		vv=value-v*10;

		switch(rotation)
		{
			case ROTATION_0_DEGREES:
				if(Application.clockDefinition.font_style==CLASSIC)
				{
					DrowDigit(2,17,v,style,BIG,rotation);
					DrowDigit(2,9,vv,style,BIG,rotation);
				}
				else
				{
					if(v == 0)
					{
						DrowDigit(2,13,vv,style,BIG,rotation);
					}
					else
					{
						DrowDigit(2,17,v,style,BIG,rotation);
						DrowDigit(2,9,vv,style,BIG,rotation);
					}
				}
			break;

			case ROTATION_90_DEGREES:
				if(Application.clockDefinition.font_style==CLASSIC)
				{
					DrowDigit(6,10,v,style,BIG,rotation);
					DrowDigit(14,10,vv,style,BIG,rotation);
				}
				else
				{
					if(v == 0)
					{
						DrowDigit(10,10,vv,style,BIG,rotation);
					}
					else
					{
						DrowDigit(6,10,v,style,BIG,rotation);
						DrowDigit(14,10,vv,style,BIG,rotation);
					}
				}
			break;

			case ROTATION_180_DEGREES:
				if(Application.clockDefinition.font_style==CLASSIC)
				{
					DrowDigit(13,14,v,style,BIG,rotation);
					DrowDigit(13,22,vv,style,BIG,rotation);
				}
				else
				{
					if(v == 0)
					{
						DrowDigit(13,18,vv,style,BIG,rotation);
					}
					else
					{
						DrowDigit(13,14,v,style,BIG,rotation);
						DrowDigit(13,22,vv,style,BIG,rotation);
					}
				}
			break;

			case ROTATION_270_DEGREES:
				if(Application.clockDefinition.font_style==CLASSIC)
				{
					DrowDigit(9,21,v,style,BIG,rotation);
					DrowDigit(1,21,vv,style,BIG,rotation);
				}
				else
				{
					if(v == 0)
					{
						DrowDigit(5,21,vv,style,BIG,rotation);
					}
					else
					{
						DrowDigit(9,21,v,style,BIG,rotation);
						DrowDigit(1,21,vv,style,BIG,rotation);
					}
				}
			break;
		}
	}


	if(Application.addtionalFunctions==SHOWN_ALARM_TYPE)
	{

		if(Application.rouserDefinition.alarmType==ONCE)
			letter=O_LETTER;

		if(Application.rouserDefinition.alarmType==EVERY_DAY)
			letter=E_LETTER;

		switch(rotation)
		{
			case ROTATION_0_DEGREES:
				DrowDigit(5,14,letter,style,LETTER,rotation);
			break;

			case ROTATION_90_DEGREES:
				DrowDigit(9,13,letter,style,LETTER,rotation);
			break;

			case ROTATION_180_DEGREES:
				DrowDigit(10,17,letter,style,LETTER,rotation);
			break;

			case ROTATION_270_DEGREES:
				DrowDigit(6,18,letter,style,LETTER,rotation);
			break;
		}
	}

	if((Application.clockDefinition.isBorderEnabled)&&(Application.addtionalFunctions==COUNT_DOWN))
		isNeedBorder=true;

	if((Application.rouserDefinition.isBorderEnabled)&&(Application.addtionalFunctions==SHOWN_ALARM_TYPE))
		isNeedBorder=true;


	if(isNeedBorder==true)
	{
		if(Application.addtionalFunctions==COUNT_DOWN)
		{
			if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES))
			{
				if(Application.clockDefinition.isBorderEnabled)
				{
					for(j=0;j<32;j++)
					{
						SetPixel(0, j, true);
						SetPixel(15, j, true);
					}
				}
			}

			if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
			{
				if(Application.clockDefinition.isBorderEnabled)
				{
					for(i=0;i<16;i++)
					{
						SetPixel(i, 0, true);
						SetPixel(i, 31, true);
					}
				}
			}
		}

		if(Application.addtionalFunctions==SHOWN_ALARM_TYPE)
		{
			if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES)) //90 и 270 градусов недопустимый режим работы, происходит подмена фунции
			{
				for(i=0;i<16;i++)
				{
					SetPixel(i, 0, true);
					SetPixel(i, 1, true);
					SetPixel(i, 31, true);
					SetPixel(i, 30, true);
				}
			}

			if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
			{
				for(i=0;i<16;i++)
				{
					SetPixel(i, 0, true);
					SetPixel(i, 1, true);
					SetPixel(i, 30, true);
					SetPixel(i, 31, true);
				}
			}
		}
	}
}


void WriteRouser(Rouser_Def_t Rouser_Def,t_rotation rotation)
{
uint8_t h,hh,m,mm,i,j,state;

h=Rouser_Def.hours/10;
hh=Rouser_Def.hours-h*10;
m=Rouser_Def.minutes/10;
mm=Rouser_Def.minutes-m*10;
state=0;

if(Rouser_Def.isAlarmEnabled)
{
	state=1;
}
else
{
	state=0;
}

	switch(rotation)
	{
		case ROTATION_0_DEGREES:
			DrowDigit(0,19,A_LETTER,Rouser_Def.font_style,LETTER,rotation);
			DrowDigit(0,8,state,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(9,23,h,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(9,18,hh,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(9,10,m,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(9,5,mm,Rouser_Def.font_style,SMALL,rotation);

		    for(i=13;i<=14;i++)
		      for(j=15;j<=16;j++)
		        SetPixel(i, j, true);

		    for(i=10;i<=11;i++)
		    {
		      for(j=15;j<=16;j++)
		      {
		    	 SetPixel(i, j, true);
		      }
		    }

		    for(j=13;j<=17;j++)
		    {
		    	SetPixel(3, j, true);
		    }

		break;

		case ROTATION_90_DEGREES:
			DrowDigit(5,3,A_LETTER,Rouser_Def.font_style,LETTER,rotation);
			DrowDigit(14,3,state,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(6,12,h,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(12,12,hh,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(6,21,m,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(12,21,mm,Rouser_Def.font_style,SMALL,rotation);

		    for(i=7;i<=9;i++)
		    {
		    	SetPixel(i, 6, true);
		    }

		break;

		case ROTATION_180_DEGREES:
			DrowDigit(15,12,A_LETTER,Rouser_Def.font_style,LETTER,rotation);
			DrowDigit(15,23,state,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(6,8,h,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(6,13,hh,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(6,21,m,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(6,26,mm,Rouser_Def.font_style,SMALL,rotation);

		    for(i=1;i<=2;i++)
		      for(j=15;j<=16;j++)
		        SetPixel(i, j, true);

		    for(i=4;i<=5;i++)
		    {
		      for(j=15;j<=16;j++)
		      {
		    	 SetPixel(i, j, true);
		      }
		    }

		    for(j=14;j<=18;j++)
		    {
		    	SetPixel(12, j, true);
		    }

		break;

		case ROTATION_270_DEGREES:
			DrowDigit(10,28,A_LETTER,Rouser_Def.font_style,LETTER,rotation);
			DrowDigit(1,28,state,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(9,19,h,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(3,19,hh,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(9,10,m,Rouser_Def.font_style,SMALL,rotation);
			DrowDigit(3,10,mm,Rouser_Def.font_style,SMALL,rotation);

		    for(i=6;i<=8;i++)
		    {
		    	SetPixel(i, 25, true);
		    }

		break;
	}

	if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES)) //90 и 270 градусов недопустимый режим работы, происходит подмена фунции
	{
		if(Rouser_Def.isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 31, true);
				SetPixel(i, 30, true);
			}
		}
	}

	if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
	{
		if(Rouser_Def.isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 30, true);
				SetPixel(i, 31, true);
			}
		}
	}
}

void WriteTimer(Timer_Def_t Timer_Def,t_rotation rotation)
{
uint8_t h,hh,m,mm,i,j,state;

h=Timer_Def.hours/10;
hh=Timer_Def.hours-h*10;
m=Timer_Def.minutes/10;
mm=Timer_Def.minutes-m*10;
state=0;

if(Timer_Def.isTimerEnabled)
{
	state=1;
}
else
{
	state=0;
}

	switch(rotation)
	{
		case ROTATION_0_DEGREES:
			DrowDigit(0,19,T_LETTER,Timer_Def.font_style,LETTER,rotation);
			DrowDigit(0,8,state,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(9,23,h,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(9,18,hh,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(9,10,m,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(9,5,mm,Timer_Def.font_style,SMALL,rotation);

			for(i=13;i<=14;i++)
			  for(j=15;j<=16;j++)
				SetPixel(i, j, true);

			for(i=10;i<=11;i++)
			{
			  for(j=15;j<=16;j++)
			  {
				 SetPixel(i, j, true);
			  }
			}

			if((Application.timerDefinition.isDotsBlinked==true)&&(Application.timerDefinition.isTimerEnabled==true))
			{
				if(Timers.blink==false)
				{
					for(i=13;i<=14;i++)
					  for(j=15;j<=16;j++)
						SetPixel(i, j, false);

					for(i=10;i<=11;i++)
					{
					  for(j=15;j<=16;j++)
					  {
						 SetPixel(i, j, false);
					  }
					}
				}
			}

		    for(j=13;j<=17;j++)
		    {
		    	SetPixel(3, j, true);
		    }

		break;

		case ROTATION_90_DEGREES:
			DrowDigit(5,3,T_LETTER,Timer_Def.font_style,LETTER,rotation);
			DrowDigit(14,3,state,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(6,12,h,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(12,12,hh,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(6,21,m,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(12,21,mm,Timer_Def.font_style,SMALL,rotation);

		    for(i=7;i<=9;i++)
		    {
		    	SetPixel(i, 6, true);
		    }

		break;

		case ROTATION_180_DEGREES:
			DrowDigit(15,12,T_LETTER,Timer_Def.font_style,LETTER,rotation);
			DrowDigit(15,23,state,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(6,8,h,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(6,13,hh,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(6,21,m,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(6,26,mm,Timer_Def.font_style,SMALL,rotation);

			if((Application.timerDefinition.isDotsBlinked==true)&&(Application.timerDefinition.isTimerEnabled==true))
			{
				if(Timers.blink==false)
				{
					for(i=13;i<=14;i++)
					  for(j=15;j<=16;j++)
						SetPixel(i, j, false);

					for(i=10;i<=11;i++)
					{
					  for(j=15;j<=16;j++)
					  {
						 SetPixel(i, j, false);
					  }
					}
				}
			}

		    for(j=14;j<=18;j++)
		    {
		    	SetPixel(12, j, true);
		    }

		break;

		case ROTATION_270_DEGREES:
			DrowDigit(10,28,T_LETTER,Timer_Def.font_style,LETTER,rotation);
			DrowDigit(1,28,state,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(9,19,h,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(3,19,hh,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(9,10,m,Timer_Def.font_style,SMALL,rotation);
			DrowDigit(3,10,mm,Timer_Def.font_style,SMALL,rotation);

		    for(i=6;i<=8;i++)
		    {
		    	SetPixel(i, 25, true);
		    }

		break;
	}

	if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES)) //90 и 270 градусов недопустимый режим работы, происходит подмена фунции
	{
		if(Timer_Def.isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 31, true);
				SetPixel(i, 30, true);
			}
		}
	}

	if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
	{
		if(Timer_Def.isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 30, true);
				SetPixel(i, 31, true);
			}
		}
	}
}

void WriteTime(Clock_Def_t Clock_Def, t_rotation rotation)
{
  switch(Clock_Def.clock_style)
  {
    case SMALL_SIX_DIGITS_TWO_STRINGS:
      WriteTimeSmallSixDigitsTwoStrings(Clock_Def.hours,Clock_Def.minutes,Clock_Def.seconds,rotation,Clock_Def.font_style,Clock_Def.isBorderEnabled,Clock_Def.isDotsBlinked);
    break;

    case SMALL_SIX_DIGITS:
      WriteTimeSmallSixDigits(Clock_Def.hours,Clock_Def.minutes,Clock_Def.seconds,rotation,Clock_Def.font_style,Clock_Def.isBorderEnabled,Clock_Def.isDotsBlinked);
    break;

    case SMALL_FOUR_DIGITS:
      WriteTimeSmallFourDigits(Clock_Def.hours,Clock_Def.minutes,rotation,Clock_Def.font_style,Clock_Def.isBorderEnabled,Clock_Def.isDotsBlinked);
    break;

    case BIG_FOUR_DIGITS:
      WriteTimeBigFourDigits(Clock_Def.hours,Clock_Def.minutes,rotation,Clock_Def.font_style,Clock_Def.isBorderEnabled,Clock_Def.isDotsBlinked);
    break;
  }
}

void WriteTimeSmallFourDigits(uint8_t hours,uint8_t minutes,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked)
{
uint8_t h,hh,m,mm,i,j;

	h=hours/10;
	hh=hours-h*10;
	m=minutes/10;
	mm=minutes-m*10;

	if(rotation==ROTATION_0_DEGREES)
	{
		DrowDigit(4,23,h,style,SMALL,rotation);
		DrowDigit(4,18,hh,style,SMALL,rotation);
		DrowDigit(4,10,m,style,SMALL,rotation);
		DrowDigit(4,5,mm,style,SMALL,rotation);

	    for(i=5;i<=6;i++)
	      for(j=15;j<=16;j++)
	        SetPixel(i, j, true);

	    for(i=8;i<=9;i++)
	      for(j=15;j<=16;j++)
	    	 SetPixel(i, j, true);

	    if(isDotsBlinked==true)
	    {
	    	if(Timers.blink==false)
	    	{
	    	    for(i=5;i<=6;i++)
	    	      for(j=15;j<=16;j++)
	    	        SetPixel(i, j, false);

	    	    for(i=8;i<=9;i++)
	    	      for(j=15;j<=16;j++)
	    	    	 SetPixel(i, j, false);
	    	}
	    }
	}


	if(rotation==ROTATION_90_DEGREES)
	{
		DrowDigit(6,8,h,style,SMALL,rotation);
		DrowDigit(12,8,hh,style,SMALL,rotation);
		DrowDigit(6,17,m,style,SMALL,rotation);
		DrowDigit(12,17,mm,style,SMALL,rotation);
	}

	if(rotation==ROTATION_180_DEGREES)
	{
		DrowDigit(11,8,h,style,SMALL,rotation);
		DrowDigit(11,13,hh,style,SMALL,rotation);
		DrowDigit(11,21,m,style,SMALL,rotation);
		DrowDigit(11,26,mm,style,SMALL,rotation);

	    for(i=6;i<=7;i++)
	      for(j=15;j<=16;j++)
	        SetPixel(i, j, true);

	    for(i=9;i<=10;i++)
	      for(j=15;j<=16;j++)
	    	 SetPixel(i, j, true);

	    if(isDotsBlinked==true)
	    {
	    	if(Timers.blink==false)
	    	{
	    	    for(i=6;i<=7;i++)
	    	      for(j=15;j<=16;j++)
	    	        SetPixel(i, j, false);

	    	    for(i=9;i<=10;i++)
	    	      for(j=15;j<=16;j++)
	    	    	 SetPixel(i, j, false);
	    	}
	    }

	}

	if(rotation==ROTATION_270_DEGREES)
	{
		DrowDigit(9,23,h,style,SMALL,rotation);
		DrowDigit(3,23,hh,style,SMALL,rotation);
		DrowDigit(9,14,m,style,SMALL,rotation);
		DrowDigit(3,14,mm,style,SMALL,rotation);
	}

	if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES))
	{
		if(isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 31, true);
				SetPixel(i, 30, true);
			}

			for(j=0;j<32;j++)
			{
				SetPixel(0, j, true);
				SetPixel(1, j, true);
				SetPixel(14, j, true);
				SetPixel(15, j, true);
			}
		}
	}

	if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
	{
		if(isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 31, true);
				SetPixel(i, 30, true);
			}
		}
	}
}

void WriteTimeBigFourDigits(uint8_t hours,uint8_t minutes,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked)
{
  uint8_t h,hh,m,mm,i,j;
  h=hours/10;
  hh=hours-h*10;
  m=minutes/10;
  mm=minutes-m*10;

  if(rotation==ROTATION_0_DEGREES)
  {
	DrowDigit(2,26,h,style,BIG,rotation);
	DrowDigit(2,19,hh,style,BIG,rotation);
	DrowDigit(2,7,m,style,BIG,rotation);
	DrowDigit(2,0,mm,style,BIG,rotation);
  }

  if(rotation==ROTATION_90_DEGREES)
  {
	DrowDigit(6,2,h,style,BIG,rotation);
	DrowDigit(14,2,hh,style,BIG,rotation);
	DrowDigit(6,18,m,style,BIG,rotation);
	DrowDigit(14,18,mm,style,BIG,rotation);
  }

  if(rotation==ROTATION_180_DEGREES)
  {
	DrowDigit(13,5,h,style,BIG,rotation);
	DrowDigit(13,12,hh,style,BIG,rotation);
	DrowDigit(13,24,m,style,BIG,rotation);
	DrowDigit(13,31,mm,style,BIG,rotation);
  }

  if(rotation==ROTATION_270_DEGREES)
  {
	DrowDigit(9,29,h,style,BIG,rotation);
	DrowDigit(1,29,hh,style,BIG,rotation);
	DrowDigit(9,13,m,style,BIG,rotation);
	DrowDigit(1,13,mm,style,BIG,rotation);
  }

  if((rotation==ROTATION_180_DEGREES)||(rotation==ROTATION_0_DEGREES))
  {

	  for(i=4;i<=5;i++)
	    for(j=15;j<=16;j++)
	    	SetPixel(i, j, true);

	  for(i=10;i<=11;i++)
	    for(j=15;j<=16;j++)
	    	SetPixel(i, j, true);

	  if(isDotsBlinked==true)
	  {
	    if(Timers.blink==false)
	    {
	  	  for(i=4;i<=5;i++)
	  	    for(j=15;j<=16;j++)
	  	    	SetPixel(i, j, false);

	  	  for(i=10;i<=11;i++)
	  	    for(j=15;j<=16;j++)
	  	    	SetPixel(i, j, false);
	    }

	 }
  }

  if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES))
  {
	if(isBorderEnabled)
	{
		for(j=0;j<32;j++)
		{
			SetPixel(0, j, true);
			SetPixel(15, j, true);
		}
	}
  }

  if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
  {
	if(isBorderEnabled)
	{
		for(i=0;i<16;i++)
		{
				SetPixel(i, 0, true);
				SetPixel(i, 31, true);
		}
	}
  }

}

void WriteTimeSmallSixDigitsTwoStrings(uint8_t hours,uint8_t minutes,uint8_t secunds,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked)
{
uint8_t h,hh,m,mm,s,ss,i,j;
	h=hours/10;
	hh=hours-h*10;
	m=minutes/10;
	mm=minutes-m*10;
	s=secunds/10;
	ss=secunds-s*10;

	if ((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))   //недопустимый режим работы, происходит подмена фунции
	{
		WriteTimeSmallSixDigits(hours,minutes,secunds,rotation,style,isBorderEnabled,isDotsBlinked);
	}

	if (rotation==ROTATION_0_DEGREES)
	{
		DrowDigit(0,23,h,style,SMALL,rotation);
		DrowDigit(0,18,hh,style,SMALL,rotation);
		DrowDigit(0,10,m,style,SMALL,rotation);
		DrowDigit(0,5,mm,style,SMALL,rotation);
		DrowDigit(9,17,s,style,SMALL,rotation);
		DrowDigit(9,11,ss,style,SMALL,rotation);


	    for(i=1;i<=2;i++)
	      for(j=15;j<=16;j++)
	        SetPixel(i, j, true);

	    for(i=4;i<=5;i++)
	      for(j=15;j<=16;j++)
	    	 SetPixel(i, j, true);

	    if(isDotsBlinked==true)
	    {
	    	if(Timers.blink==false)
	    	{
	    	    for(i=1;i<=2;i++)
	    	      for(j=15;j<=16;j++)
	    	        SetPixel(i, j, false);

	    	    for(i=4;i<=5;i++)
	    	      for(j=15;j<=16;j++)
	    	    	 SetPixel(i, j, false);
	    	}
	    }
	}

	if (rotation==ROTATION_180_DEGREES)
	{
		DrowDigit(15,8,h,style,SMALL,rotation);
		DrowDigit(15,13,hh,style,SMALL,rotation);
		DrowDigit(15,21,m,style,SMALL,rotation);
		DrowDigit(15,26,mm,style,SMALL,rotation);
		DrowDigit(6,14,s,style,SMALL,rotation);
		DrowDigit(6,20,ss,style,SMALL,rotation);

	    for(i=13;i<=14;i++)
	      for(j=15;j<=16;j++)
	        SetPixel(i, j, true);

	    for(i=10;i<=11;i++)
	      for(j=15;j<=16;j++)
	    	 SetPixel(i, j, true);

	    if(isDotsBlinked==true)
	    {
	    	if(Timers.blink==false)
	    	{
	    	    for(i=13;i<=14;i++)
	    	      for(j=15;j<=16;j++)
	    	        SetPixel(i, j, false);

	    	    for(i=10;i<=11;i++)
	    	      for(j=15;j<=16;j++)
	    	    	 SetPixel(i, j, false);
	    	}
	    }

	}

	if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES)) //90 и 270 градусов недопустимый режим работы, происходит подмена фунции
	{
		if(isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 31, true);
				SetPixel(i, 30, true);
			}
		}
	}
}

void WriteTimeSmallSixDigits(uint8_t hours,uint8_t minutes,uint8_t secunds,t_rotation rotation,t_font_style style,bool isBorderEnabled,bool isDotsBlinked)
{
uint8_t h,hh,m,mm,s,ss,j,i;

  h=hours/10;
  hh=hours-h*10;
  m=minutes/10;
  mm=minutes-m*10;
  s=secunds/10;
  ss=secunds-s*10;

  if(rotation==ROTATION_0_DEGREES)
  {
	DrowDigit(4,28,h,style,SMALL,rotation);
	DrowDigit(4,23,hh,style,SMALL,rotation);
	DrowDigit(4,17,m,style,SMALL,rotation);
	DrowDigit(4,12,mm,style,SMALL,rotation);
	DrowDigit(4,6,s,style,SMALL,rotation);
	DrowDigit(4,1,ss,style,SMALL,rotation);
  }

  if(rotation==ROTATION_90_DEGREES)
  {
	DrowDigit(6,3,h,style,SMALL,rotation);
	DrowDigit(12,3,hh,style,SMALL,rotation);
	DrowDigit(6,12,m,style,SMALL,rotation);
	DrowDigit(12,12,mm,style,SMALL,rotation);
	DrowDigit(6,21,s,style,SMALL,rotation);
	DrowDigit(12,21,ss,style,SMALL,rotation);
  }

  if(rotation==ROTATION_180_DEGREES)
  {
	  DrowDigit(11,3,h,style,SMALL,rotation);
	  DrowDigit(11,8,hh,style,SMALL,rotation);
	  DrowDigit(11,14,m,style,SMALL,rotation);
	  DrowDigit(11,19,mm,style,SMALL,rotation);
	  DrowDigit(11,25,s,style,SMALL,rotation);
	  DrowDigit(11,30,ss,style,SMALL,rotation);
  }

  if(rotation==ROTATION_270_DEGREES)
  {
	DrowDigit(9,28,h,style,SMALL,rotation);
	DrowDigit(3,28,hh,style,SMALL,rotation);
	DrowDigit(9,19,m,style,SMALL,rotation);
	DrowDigit(3,19,mm,style,SMALL,rotation);
	DrowDigit(9,10,s,style,SMALL,rotation);
	DrowDigit(3,10,ss,style,SMALL,rotation);
  }

	if((rotation==ROTATION_0_DEGREES)||(rotation==ROTATION_180_DEGREES))
	{
		if(isBorderEnabled)
		{
			for(j=0;j<32;j++)
			{
				SetPixel(0, j, true);
				SetPixel(1, j, true);
				SetPixel(14, j, true);
				SetPixel(15, j, true);
			}
		}
	}

	if((rotation==ROTATION_90_DEGREES)||(rotation==ROTATION_270_DEGREES))
	{
		if(isBorderEnabled)
		{
			for(i=0;i<16;i++)
			{
				SetPixel(i, 0, true);
				SetPixel(i, 1, true);
				SetPixel(i, 30, true);
				SetPixel(i, 31, true);
			}
		}
	}

}
