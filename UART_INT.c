#include <LPC21xx.H>  /* LPC21xx definitions         */
#include <string.h>
#include "lcd_1.h"
#define UART_INT_ENABLE 1
#include "delay_1.h"
#include "spi_eeprom.h"
#include "spi.h"
#include "spi_defines.h"
#include "types.h"
#include "pincfg_1.h"
#include "rtc.h"
#include "rtc_defines.h"
u8 rByte[11];
u8 id[11]="12623314";	
s32 hour,min,sec,date,month,year,day;

int spi_val,spi_flag=1;
void InitUART0 (void); /* Initialize Serial Interface       */ 
void UART0_Tx(char ch);  
char UART0_Rx(void);    

int first_call=2;
char buff[15],dummy;
char buff1[11];
unsigned char i=0,ch,r_flag=0;


void UART0_isr(void) __irq{
  if((U0IIR & 0x04)) //check if receive interrupt
  {
		ch = U0RBR;	/* Read to Clear Receive Interrupt */
		if(ch != '@')
			buff[i++] = ch; 
		else
		{
			buff[i]='\0';
			i=0;
			r_flag = 1;
		}
  }
  else
  {
      dummy=U0IIR; //Read to Clear transmit interrupt
  
  }
   VICVectAddr = 0; /* dummy write */
}
void UART1_isr(void) __irq
{
  if((U1IIR & 0x04)) //check if receive interrupt
  {
		ch = U1RBR;	/* Read to Clear Receive Interrupt */
		if(ch == 0X02){
			//buff[i++] = ch;
			i=1;	
		} 
		else if(ch==0X03)	{
			i=0;
			r_flag=1;
		}
		else{
			buff1[i++] = ch;
			//r_flag = 1;
		}
  }
  else
  {
      dummy=U0IIR; //Read to Clear transmit interrupt
  
  }
   VICVectAddr = 0; /* dummy write */
}
void InitUART0 (void) /* Initialize Serial Interface       */ 
{  
            		
  //PINSEL0 = 0x00000005; /* Enable RxD0 and TxD0              */
  //cfgPortPinFunc(u32 portNo,u32 pinNo,u32 pinFunc);
	cfgPortPinFunc(0,9,1);
	cfgPortPinFunc(0,1,1);
	cfgPortPinFunc(0,0,1);
	U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
  U0LCR = 0x03;         /* DLAB = 0  */
  U1LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U1DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
  U1LCR = 0x03; 
  #if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr1 = (unsigned)UART0_isr;
  VICVectCntl1 = 0x20 | 6; /* UART0 Interrupt */
  VICIntEnable = 1 << 6;   /* Enable UART0 Interrupt */
 
 // U0IIR = 0xc0;
 // U0FCR = 0xc7;
  U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   
  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr0 = (unsigned)UART1_isr;
  VICVectCntl0 = 0x20 | 7; /* UART0 Interrupt */
  VICIntEnable = 1 << 7;   /* Enable UART0 Interrupt */
 
 // U0IIR = 0xc0;
 // U0FCR = 0xc7;
  U1IER = 0x03;            
  #endif
						
}

void UART0_Tx(char ch)  /* Write character to Serial Port    */  
{ 
  while (!(U0LSR & 0x20));
  U0THR = ch;                
}

char UART0_Rx(void)    /* Read character from Serial Port   */
{                     
  while (!(U0LSR & 0x01));
  return (U0RBR);
}

void UART0_Str(char *s)
{
   while(*s)
       UART0_Tx(*s++);
}

void UART0_Int(unsigned int n)
{
  unsigned char a[10]={0,0,0,0,0,0,0,0,0,0};
  int i=0;
  if(n==0)
  {
    UART0_Tx('0');
	return;
  }
  else
  {
     while(n>0)
	 {
	   a[i++]=(n%10)+48;
	   n=n/10;
	 }
	 --i;
	 for(;i>=0;i--)
	 {
	   UART0_Tx(a[i]);
	 }
   }
}

void UART0_Float(float f)
{
  int x;
  float temp;
  x=f;
  UART0_Int(x);
  UART0_Tx('.');
  temp=(f-x)*100;
  x=temp;
  UART0_Int(x);
}		

void  DelayS(unsigned int  dly)
{  unsigned int  i;

   for(; dly>0; dly--) 
      for(i=1200000; i>0; i--);
}
								
int main(){
  
	InitLCD();
  InitUART0();
  Init_SPI0();
  RTC_Init();
  for(i=0;id[i];i++)
	{
		ByteWrite_25LC512(i,id[i]);
	}
	for(i=0;id[i];i++)
	{
		rByte[i]=ByteRead_25LC512(i);
	}

	for (i = 0;rByte[i]; i++)
	{
      CharLCD(rByte[i]);
    }
	delay_ms(5000);
  while(1)
  {	
  //	CharLCD('A');
  	//	delay_s(5);
  		StrLCD("hello");

		i=0;
		CmdLCD(0x01);
		//r_flag=0;
		while(r_flag == 0){
			GetRTCTimeInfo(&hour,&min,&sec);
		    DisplayRTCTime(hour,min,sec);
		    GetRTCDateInfo(&date,&month,&year);
		    DisplayRTCDate(date,month,year);
		    GetRTCDay(&day);
		    DisplayRTCDay(day);
		}
		CmdLCD(0x01);
		/*if(first_call==1){
			StrLCD(buff);
			first_call=0;
			 memset(buff,'\0',15);
			  r_flag=0;
			continue;
		} */
		if(first_call==2){
			StrLCD(buff);
				year=0;
		        for(i=0;i<4;i++){
		            year=year*10+(buff[i]-48);
		        }
		        month=((buff[5]-48)*10)+(buff[6]-48);
		        date=((buff[8]-48)*10)+(buff[9]-48);
		        day=buff[10]-48;
			first_call=1;
			//SetRTCTimeInfo(23,59,50);

	                SetRTCDateInfo(date,month,year);

	                  SetRTCDay(day);
			 memset(buff,'\0',15);
			  r_flag=0;
			continue;
		} 
		if(first_call==1){
			StrLCD(buff);
		        hour=((buff[0]-48)*10)+(buff[1]-48);
		        min=((buff[3]-48)*10)+(buff[4]-48);
		        sec=((buff[6]-48)*10)+(buff[7]-48);
			first_call=0;
			SetRTCTimeInfo(hour,min,sec);
			 memset(buff,'\0',15);
			  r_flag=0;
			continue;
		}
		for(spi_val=0;spi_val<8;spi_val++){
			if(rByte[spi_val]==buff1[spi_val+1]);
			else{
				spi_flag=0;
				break;
			}
		}
		if(spi_flag==1){
				buff1[0]='A';
				buff1[9]='$';
		}
	//buff1[0]='A';
	   else{
		buff1[0]='U';
		buff1[9]='$';
	   }
		UART0_Str(buff1);
		//uart sending data and wait purpose delay is used
		//i am giving delay due to reason getting the reply from code
		delay_s(3);
		//UART0_Str("\n\r");
		//UART0_Float(245.21);
		//UART0_Str("\n\r");*/
		CmdLCD(0xC0);
		StrLCD(buff);
		//delay_ms(1000);
	//	memset(buff,'\0',15);
		r_flag=0;
		spi_flag=1;
		//DelayS(1);
  }
}

