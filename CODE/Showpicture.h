#include <REGX52.H>
#include "Delay.h"
#include "MatrixLED.h"
#include "MatrixLED_ShowPicture.h"
#include "Timer0Init.h"
void Showpicture()
{
    unsigned char k=1;	
	  MatrixLED_ShowColumn(0,Animation[Place*8]);
		for(k=1;k<8;k++)	
		{
			if(k!=7-numt)				
			   MatrixLED_ShowColumn(k,0x00);
			else
				MatrixLED_ShowColumn(k,ball_position);
		}
}