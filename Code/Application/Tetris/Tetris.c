/*
 * Tetris.c
 *
 *  Created on: Oct 27, 2024
 *      Author: Игорь
 */
#include "Tetris.h"

const int8_t figures[7][12][2]  = {
  {
    { -1, 0}, {1, 0}, {2, 0},
    {0, 1}, {0, 2}, {0, 3},
    { -1, 0}, {1, 0}, {2, 0},
    {0, 1}, {0, 2}, {0, 3},
  },
  {
    {0, 1}, {1, 0}, {1, 1},
    {0, 1}, {1, 0}, {1, 1},
    {0, 1}, {1, 0}, {1, 1},
    {0, 1}, {1, 0}, {1, 1},
  },
  {
    { -1, 0}, { -1, 1}, {1, 0},
    {0, 1}, {0, 2}, {1, 2},
    { -2, 1}, { -1, 1}, {0, 1},
    { -1, 0}, {0, 1}, {0, 2},
  },
  {
    { -1, 0}, {1, 0}, {1, 1},
    {0, 1}, {0, 2}, {1, 0},
    {0, 1}, {1, 1}, {2, 1},
    {0, 1}, {0, 2}, { -1, 2},
  },
  {
    { -1, 0}, {0, 1}, {1, 1},
    {0, 1}, { -1, 1}, { -1, 2},
    { -1, 0}, {0, 1}, {1, 1},
    {0, 1}, { -1, 1}, { -1, 2},
  },
  {
    { -1, 1}, {0, 1}, {1, 0},
    {0, 1}, {1, 1}, {1, 2},
    { -1, 1}, {0, 1}, {1, 0},
    {0, 1}, {1, 1}, {1, 2},
  },
  {
    { -1, 0}, {0, 1}, {1, 0},
    {0, 1}, {0, 2}, {1, 1},
    { -1, 1}, {0, 1}, {1, 1},
    { -1, 1}, {0, 1}, {0, 2},
  }
};


void RestartGame(void)
{
	MP3_Stop();
	HAL_Delay(500);
	MP3_PlayTrack(TETRIS_END_GAME);
	HAL_Delay(500);
	ClearMax7219();
	WriteScore(Application.tetrisDefinition.score,Application.rotation,true);
	UpdateMax7219();
	HAL_Delay(3000);
	StartGame();

}

void StartGame(void)
{
uint16_t seed;
uint8_t i;

seed= getSeed();
	srand(seed);
	Application.tetrisDefinition.score=0;
	Application.tetrisDefinition.speed=Application.tetrisDefinition.Startspeed;
	for(i=0;i<=22;i++)
	{
		fixedFigures[i]=0;
	}
	for(i=0;i<4;i++)
	{
		queue[i].figure=Random(0, 6);
		queue[i].angle=0;
	}
	RestartRound();
	Application.forceUpdate=true;
	ClearMax7219();
	MP3_PlayTrack(TETRIS_START_GAME);
	CreateImage();
	UpdateMax7219();
	HAL_Delay(3000);
	Application.forceUpdate=false;
}

uint16_t getSeed(void)
{
uint16_t result;
	result=(Get_Timer_1_ms()+1)*(Get_Timer_10_ms()+1)*(Get_Timer_100_ms()+1)*(Get_Timer_1_s()+1);
return result;
}

void ClearLines(void)
{
int8_t i,j,k,linesForClear=0,startLine,score;
uint8_t linesForClearIndex[4]={48,48,48,48};
	for(i=22;i>=0;i--)
	{
		if(fixedFigures[i]==FULL_LINE)
		{
			linesForClearIndex[linesForClear]=i;
			linesForClear++;
		}
	}

	if(linesForClear!=0)
	{
		for(j=0;j<2;j++)
		{
			for(i=0;i<linesForClear;i++)
			{
				fixedFigures[linesForClearIndex[i]]=0;
			}
			WriteFixedFigures();
			UpdateMax7219();
			HAL_Delay(500);
			for(i=0;i<linesForClear;i++)
			{
				fixedFigures[linesForClearIndex[i]]=FULL_LINE;
			}
			WriteFixedFigures();
			UpdateMax7219();
			HAL_Delay(500);
		}
	}

	for(i=0;i<linesForClear;i++)
	{
		fixedFigures[linesForClearIndex[i]]=0;
	}

	WriteFixedFigures();
	UpdateMax7219();
	HAL_Delay(500);

	if(linesForClear!=0)
	{
		for(i=22;i>=0;i--)
		{
			if(linesForClearIndex[k]==i)
			{
				startLine=i;
				k++;
				for(j=startLine;j<22;j++)
				{
					fixedFigures[j]=fixedFigures[j+1];
				}
			}
		}
	}

	if(linesForClear!=0)
	{
		switch(linesForClear)
		{
			case 1: 	score=1; 	break;
			case 2: 	score=4; 	break;
			case 3: 	score=12; 	break;
			case 4: 	score=32; 	break;
			default: 	score=0;	break;
		}

		Application.tetrisDefinition.score=Application.tetrisDefinition.score+score*(4-Application.tetrisDefinition.speed);
	}

	WriteFixedFigures();
	UpdateMax7219();
}

void drawFigure(int8_t figure, int8_t angle, int8_t x, int8_t y, bool State,t_rotation rotation)
{
  SetPixel(x, y, State );        // рисуем точку начала координат фигуры
  int8_t X, Y;                      // вспомогательные
  for (int8_t i = 0; i < 3; i++) {    // рисуем 4 точки фигуры
    // что происходит: рисуем фигуру относительно текущей координаты падающей точки
    // просто прибавляем "смещение" из массива координат фигур
    // для этого идём в прогмем (функция pgm_read_byte)
    // обращаемся к массиву по адресу &figures
    // преобразовываем число в int8_t (так как progmem работает только с "unsigned"
    // angle * 3 + i - обращаемся к координатам согласно текущему углу поворота фигуры

    X = x + figures[figure][angle * 3 + i][0];
    Y = y + figures[figure][angle * 3 + i][1];

    if(rotation==ROTATION_90_DEGREES)
    {
    	if (Y < Application.tetrisDefinition.HEIGHT-1)
    		continue;   // если выходим за пределы поля, пропустить отрисовку
    }

    SetPixel(X, Y, State);
  }

  //UpdateMax7219();
}

// check type:
// 0 - проверка пола
// 1 - проверка лежащих фигур и пола
// 2 - проверка стенки слева и фигур при смещении фигуры в лево
// 3 - проверка стенки справа и фигур при смещении фигуры в право
// 4 - проверка при вращении

bool checkArea(uint8_t x_Pos,uint8_t y_Pos,CheckType_t check_type)
{
uint8_t X, Y, pos, height, this_ang,i;

pos 		= x_Pos;
height 		= y_Pos;
this_ang 	= Application.tetrisDefinition.angle;

	for(i=0;i<4;i++)
	{
	    if (i == 0)
	    { // стартовая точка фигуры (начало отсчёта)
	      Y = height;
	      X = pos;
	    }
	    else
	    { // остальные три точки
	      Y = height + (int8_t)figures[Application.tetrisDefinition.figure][this_ang * 3 + i - 1][1];
	      X = pos + (int8_t)figures[Application.tetrisDefinition.figure][this_ang * 3 + i - 1][0];
	    }

	    if (check_type == 0)
	    {
	      if (Y >= 31)
	      {
	        return true;
	      }
	    }

	    if (check_type == 1)
	    {
	      if (Y >= 31)
	      {
	        return true;
	      }

	      if(GetFixedPixel((31-Y-1),X)==true)
	      {
	    	  return true;
	      }
	    }

	    if (check_type == 2)
	    {
	      if (X <=0)
	        return true;

	      if(GetFixedPixel((31-Y),X-1)==true)
	      {
	    	  return true;
	      }
	    }

	    if (check_type == 3)
	    {
	      if (X >=9)
	        return true;

	      if(GetFixedPixel((31-Y),X+1)==true)
	      {
	    	  return true;
	      }
	    }

	    if (check_type == 4)
	    {
	      if (X >=9)
	        return true;
	      if (X <=0)
	        return true;
	      if(GetFixedPixel((31-Y),X)==true)
	      {
	    	  return true;
	      }
	    }

	}
return false;
}
