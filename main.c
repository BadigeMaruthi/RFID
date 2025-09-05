                       /* main.c */
#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "spi.h"
#include "spi_eeprom.h"
#include "spi_eeprom_defines.h"
#include "lcd_defines.h"
#include "lcd.h"

#define GLED (1<<16)
#define RLED (1<<17)
u8  rxData[26] __attribute__((at(0x40000010)));

int main(void)
{
	 u8 i,t;
	 Init_SPI0();
	 InitLCD();
	 CharLCD('1');
		IODIR1 |= (GLED|RLED);
	
	 for(i=0;i<26;i++)
	 {
     ByteWrite_25LC512(i,'A'+i);
     t=ByteRead_25LC512(i);
		 rxData[i]=t;
	   if(rxData[i]=='A'+i)	 
        IOSET1 = GLED;

		 else
			 IOSET1 = RLED; 	 
		 delay_ms(100);
		 		IOCLR1 = (GLED|RLED);
		 delay_ms(50);
		 
 	}
	for(i=0;i<26;i++){
		//if(i>14){
		//	 CharCD(rxData[i]);
		//}
		CharLCD(rxData[i]);
		
	}
   while(1);
}
          /* end of main25LC512.c */                                                    
  
