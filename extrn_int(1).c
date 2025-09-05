#include "pincfg.h"
#include "extrn_int.h"
#include "delay.h"
#include "int_defines.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm_defines.h"
#include "kpm.h"
//#include "uart.h"
#include<lpc21xx.h>
u32 val,time,flag=0;
void ext_int(void){
				cfgPortPinFunc(0,16,EINT0_INPUT_PIN);
				VICIntEnable=1<<EINT0_VIC_CHNO;
				VICVectCntl0=(1<<5)|EINT0_VIC_CHNO;
				VICVectAddr0=(u32 )eint0_isr;
				
				EXTMODE=1<<0;
				//IODIR0|=1<<EINT0_STATUS_LED;
		IODIR0|=1<<3;
}
void eint0_isr(void) __irq
//void eint0_isr(void)
{
	//toggle EINT0 status LED for ISR activity
	IOSET0=1<<3;
	//U0_TxStr("***TIME EDETING MODE ACTIVATED***\n");
	CmdLCD(0x01);
	StrLCD("1.H 2.M 3.S 4.D");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("5.DAY 6.M 7.Y8.E");
	while(1){
		if(flag==1){
			break;
		}
		//while(colScan());
		val=keyScan();
		CmdLCD(0x01);
		StrLCD("val is ");
		U32LCD(val);
		delay_s(1);
		CmdLCD(0X01);
		switch(val){
			case '1':StrLCD("HOUR");
								time=readNum();
								//cmdLCD(0x01);
								
								U32LCD(time);
								delay_s(1);
								HOUR = time;
								break;
			case '2':StrLCD("MINUTES");
								time=readNum();
								//cmdLCD(0x01);
								U32LCD(time);
								MIN = time;
								break;
			case '3':StrLCD("SECONDS");
								time=readNum();
								//cmdLCD(0x01);
								S32LCD(time);
								MIN = time;
								break;
			case '4':StrLCD("DAY");
								time=readNum();
								//cmdLCD(0x01);
								U32LCD(time);
								DOW =time;
								break;
			case '5':StrLCD("DATE");
								time=readNum();
								//cmdLCD(0x01);
								U32LCD(time);
								DOM=time;
								break;
			case '6':StrLCD("MONTH");
								time=readNum();
								//cmdLCD(0x01);
								S32LCD(time);
								MONTH=time;
								break;
			case '7':StrLCD("YEAR");
								time=readNum();
								//cmdLCD(0x01);
								U32LCD(time);
								YEAR=time;
								break;
			case '8'://cmdLCD(0x01);
								StrLCD("exit key ");
								flag=1;
								break;
		}
	}	
	delay_ms(2000);
	//clear EINT0 status flag in Ext Int Peripheral
	EXTINT=1<<0;
	//clear EINT0 status in VIC Peripheral
	VICVectAddr=0;
	CmdLCD(0x01);
	flag=0;
}
