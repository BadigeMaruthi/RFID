//kpm.c
#include "types.h"
#include<LPC21xx.h>
#include "kpm_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
const u8 kpmLUT[4][4]=
{
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};
void InitKPM()
{
	//cfg row lines as gpio out
	IODIR1|=15<<ROW0;
	//default all col lines are anyway high
}
u32 ColScan(void)
{
	u32 t;
	t=(((IOPIN1>>COL0)&15)<15)?0:1;
	return t;
}
u32 RowCheck(void)
{
	u32 rNo;
	for(rNo=0; rNo<=3; rNo++)
	{
		//ground every row at a time,
		//starting with 0th row
		IOPIN1=((IOPIN1&~(15<<ROW0))|(~(1<<rNo))<<ROW0);
		if(ColScan()==0)
		{
			break;
		}
	}
	//reinitialise all rows grounded
	IOCLR1=15<<ROW0;
	return rNo;
}
u32 ColCheck(void)
{
	u32 cNo;
	for(cNo=0; cNo<=3; cNo++)
	{
		if(((IOPIN1>>(COL0+cNo))&1)==0)
		{
			break;
		}
	}
	return cNo;
}
u32 keyScan(void)
{
	u32 rNo, cNo, keyV;
	//wait for any key press
	while(ColScan());
	//proceed to identify row in which key is pressed
	rNo=RowCheck();
	//proceed to identify column in which key is pressed
	cNo=ColCheck();
	//extract key value from look up table for pressed key
	keyV=kpmLUT[rNo][cNo];
	return keyV;
}
u32 ReadNum(void)
{
	u32 sum;
	s8 key;
	sum=0;
	while(1)
	{
		key=keyScan();
		delay_ms(200);
		CmdLCD(GOTO_LINE1_POS0);
		CharLCD(key);
		if((key>='0')&&(key<='9'))
		{
			sum=(sum*10)+(key-'0');
			CmdLCD(GOTO_LINE1_POS0);
			U32LCD(sum);
			while(ColScan()==0);
		}
		else if(key=='B')
		{
			sum=sum/10;
		}
		else
		{
			while(ColScan()==0);
			break;
		}
	}
	return sum;
}
