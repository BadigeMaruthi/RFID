//interrupt.c
#include<LPC21xx.h>
#include "interrupt_defines.h"
#include "types.h"
#include "pincfg_1.h"

void eint0_isr(void) __irq;
void Init_Intr(void)
{
	//PINSEL1|=((PINSEL1&~(3<<((EINT0_PIN%16)*2)))|(PIN_FUNC<<((EINT0_PIN%16)*2)));
	cfgPortPinFunc(0, 16, 1);
	VICIntEnable=1<<EINT0_VIC_CHNO;
	VICVectCntl2=(1<<5)|EINT0_VIC_CHNO;
	VICVectAddr2=(u32)eint0_isr;
	EXTMODE=1<<0;
}
