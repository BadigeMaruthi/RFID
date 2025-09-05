//projectmain.c

#include<LPC21xx.h>
#include<string.h>
#include "projectmain_defines.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "rtc_defines.h"
#include "rtc.h"
#include "interrupt_defines.h"
#include "interrupt.h"
#include "types.h"
#include "spi.h"
#include "spi_eeprom.h"
#include "uart.h"
#include "uart1.h"
#include "uart_defines.h"

s32 hour,min,sec,day,date,month,year;
const s8 admin_card[9]="12535708";
s8 i, rByte[8],r_flag,buff[9],dummy,ch,receive,intr_flag;

void edit(void);
/*int mystrcmp(const s8 *p, const s8 *q)
{
	s8 i=0;
	for(i=0; p[i]&&q[i]; i++)
	{
		if(p[i]!=q[i])
		{
			return (p[i]-q[i]);
		}
	}
	return (p[i]-q[i]);
} */
int main()
{
	//initiatlizations
	InitKPM();
	Init_LCD();
	Init_Intr();
	RTC_Init();
	Init_SPI0();
	Init_UART0();
	Init_UART1();
	//displaying project name on LCD for certain time
	CmdLCD(CLEAR_LCD); //0x01
	CmdLCD(RET_CUR_HOME); //0x02
	CmdLCD(GOTO_LINE1_POS0); //0x80
	StrLCD("RFID Based");
	CmdLCD(GOTO_LINE2_POS0); //0xC0
	StrLCD("Attendance");
	delay_s(DLY);
	// Set the initial time (hours, minutes, seconds)
	SetRTCTimeInfo(HH,MM,SS);
	SetRTCDateInfo(DD,MO,YR);
	SetRTCDay(D);
	//saving the admin card into SPI_EEPROM-AT25LC512
	for(i=0; admin_card[i]; i++)
	{
		ByteWrite(0x00+i, admin_card[i]);
	}
	for(i=0; i<8; i++)
	{
		rByte[i]=ByteRead(0x00+i);
	}
	rByte[i]='\0';
	CmdLCD(CLEAR_LCD); //0x01
	CmdLCD(RET_CUR_HOME); //0x02
	CmdLCD(GOTO_LINE1_POS0); //0x80
	StrLCD(rByte);
	delay_s(3);
	while(1)
	{
		r_flag=0;
		CmdLCD(CLEAR_LCD); //0x01
		CmdLCD(RET_CUR_HOME); //0x02
		CmdLCD(GOTO_LINE1_POS0);
		/*StrLCD("Waiting for card");
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD(rByte);*/
		while(r_flag==0)
		{
			GetRTCTimeInfo(&hour,&min,&sec);
			DisplayRTCTime(hour,min,sec);
			GetRTCDay(&day);
			DisplayRTCDay(day);
			GetRTCDateInfo(&date,&month,&year);
			DisplayRTCDate(date,month,year);	
		}
		i=0;
		do
		{
			if(intr_flag==1)
			{
				edit();
				intr_flag=0;
			//	r_flag=0;
				break;
			}
			else if(r_flag==1)
			{
				break;
			}
			
		}while(r_flag==0&&intr_flag==0);
		CmdLCD(GOTO_LINE2_POS0);
		StrLCD(buff);
		//delay_s(2);
		if((strcmp(buff,rByte))==0)
		{
			U0_TxStr("A");
			U0_TxStr(buff);
			U0_TxStr("$");
			U0_TxStr("\r\n");
		}
		else
		{
			U0_TxStr("U");
			U0_TxStr(buff);
			U0_TxStr("$");
			U0_TxStr("\r\n");
		}
	}		
}

//External interrupt
void eint0_isr(void) __irq
{
	intr_flag=1;
/*	u8 key, key1;
L1:	CmdLCD(CLEAR_LCD); //0x01
	CmdLCD(RET_CUR_HOME); //0x02 
  	CmdLCD(GOTO_LINE1_POS0); //0x80
  	StrLCD("1.ADMIN CHANGE");
  	CmdLCD(GOTO_LINE2_POS0); //0xc0
  	StrLCD("2.RTC EDIT");
  	key=keyScan();
  	delay_ms(200);
  	while(ColScan()==0);
	switch(key)
  	{
		case '1':while(1)
				{
				 CmdLCD(CLEAR_LCD);
             	 CmdLCD(RET_CUR_HOME);
             	 CmdLCD(GOTO_LINE1_POS0);
            	 StrLCD("Scan the card");
				 r_flag=0;
				 while(r_flag==0);
				 if(r_flag==1)
				 {
				 	for(i=0;buff[i];i++)
					{
						ByteWrite(0x00+i, buff[i]);
						//rByte[i]=ByteRead(i);
					}
					for(i=0;i<8;i++)
					{
						//ByteWrite(i, buff[i]);
						rByte[i]=ByteRead(0x00+i);
					}
					rByte[i]='\0';
				 }
             	 CmdLCD(GOTO_LINE2_POS0);
				 StrLCD(rByte);
				 break;
				 }
			     break;
		case '2':
		L2: 	 CmdLCD(CLEAR_LCD);	//0x01
				 CmdLCD(RET_CUR_HOME); //0x02
            	 CmdLCD(GOTO_LINE1_POS0); //0x80
			     StrLCD("1.H 2.M 3.S 4.D");
			     CmdLCD(GOTO_LINE2_POS0); //0xc0
			     StrLCD("5.M 6.Y 7.DW 8.E");
            	 key1=keyScan();
            	 delay_ms(200);
            	 while(ColScan()==0);
			     switch(key1)
			     {
				 	case '1':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //hour
							 HOUR=ReadNum();                                                                                                                           
						     break;
      				case '2':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //minute
							 MIN=ReadNum();
							 //min=prev=MIN;
      					     break;
				    case '3':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //seconds
							 SEC=ReadNum();
						     break;
					case '4':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //date of month
							 DOM=ReadNum();
					         break;
					case '5':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //month
							 MONTH=ReadNum();
							 break;
					case '6':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //year
							 YEAR=ReadNum();
						     break;
				    case '7':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //date of week
							 DOW=ReadNum();
							 break;
					case '8':break;
					default :goto L2;
				 }
        		 CmdLCD(CLEAR_LCD); //0x01
        		 CmdLCD(RET_CUR_HOME); //0x02
	   			 break;
        default :goto L1;
	}
	CmdLCD(CLEAR_LCD); //0x01
    CmdLCD(RET_CUR_HOME); //0x02 */
    EXTINT=1<<0;
    VICVectAddr=0; 
}

#if (UART1_INTS_ENABLE!=0)
void U1_ISR(void) __irq
{
	if(U1IIR & 0x04) //check if received interrupt
	{
		ch=U1RBR;
		if(ch==0x02) //for RFID card checking need to compare with 0x02(in place of '.')
		{
			receive=1;
			i=0;
		}
		else if((ch==0x03)&&(receive==1))
		{
			buff[i]='\0'; //placing NULL character in place of 0x03(i.e., end of card number)
			r_flag=1;
			i=0;
			receive=0;
		}
		else if((ch!=0x03)&&(i<9)&&(receive==1)) //for RFID card checking need to compare with 0x03(in place of ',')
		{
			buff[i++]=ch;
		}
	}
	else
	{
		dummy=U1IIR;
	}
	VICVectAddr=0; //dummy write
}
#endif

void edit(void)
{
	u8 key, key1;
L1:	CmdLCD(CLEAR_LCD); //0x01
	CmdLCD(RET_CUR_HOME); //0x02 
  	CmdLCD(GOTO_LINE1_POS0); //0x80
  	StrLCD("1.ADMIN CHANGE");
  	CmdLCD(GOTO_LINE2_POS0); //0xc0
  	StrLCD("2.RTC EDIT");
  	key=keyScan();
  	delay_ms(200);
  	while(ColScan()==0);
		switch(key)
  	{
		case '1':CmdLCD(CLEAR_LCD);
             CmdLCD(RET_CUR_HOME);
             CmdLCD(GOTO_LINE1_POS0);
             StrLCD("Scan the card");
						 //r_flag=0;
						 while(r_flag==0);
						 if(r_flag==1)
				   	 {
				 	   for(i=0;buff[i];i++)
					   {
								ByteWrite(0x00+i, buff[i]);
					   }
					   for(i=0;i<8;i++)
					   {
								rByte[i]=ByteRead(0x00+i);
					   }
					   rByte[i]='\0';
				     }
             CmdLCD(GOTO_LINE2_POS0);
				     StrLCD(rByte);
			       break;
		case '2':
		L2: 	 CmdLCD(CLEAR_LCD);	//0x01
				 CmdLCD(RET_CUR_HOME); //0x02
            	 CmdLCD(GOTO_LINE1_POS0); //0x80
			     StrLCD("1.H 2.M 3.S 4.D");
			     CmdLCD(GOTO_LINE2_POS0); //0xc0
			     StrLCD("5.M 6.Y 7.DW 8.E");
            	 key1=keyScan();
            	 delay_ms(200);
            	 while(ColScan()==0);
			     switch(key1)
			     {
				 	case '1':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //hour
							 HOUR=ReadNum();                                                                                                                           
						     break;
      				case '2':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //minute
							 MIN=ReadNum();
							 //min=prev=MIN;
      					     break;
				    case '3':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //seconds
							 SEC=ReadNum();
						     break;
					case '4':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //date of month
							 DOM=ReadNum();
					         break;
					case '5':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //month
							 MONTH=ReadNum();
							 break;
					case '6':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //year
							 YEAR=ReadNum();
						     break;
				    case '7':CmdLCD(CLEAR_LCD);	//0x01
                        	 CmdLCD(RET_CUR_HOME); //0x02
							 CmdLCD(GOTO_LINE1_POS0); //0x80
                        	 //date of week
							 DOW=ReadNum();
							 break;
					case '8':break;
					default :goto L2;
				 }
        		 CmdLCD(CLEAR_LCD); //0x01
        		 CmdLCD(RET_CUR_HOME); //0x02
	   			 break;
        default :goto L1;
	}
	CmdLCD(CLEAR_LCD); //0x01
  	CmdLCD(RET_CUR_HOME); //0x02
	intr_flag=0;
}
