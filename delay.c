                       /* delay.c */
#include "types.h"
void delay_ms(u32 i)
{
  unsigned int j;
  for(;i>0;i--)
  {
   for(j=11150;j>0;j--);
  }
}
void delay_us(u32 i)
{
  unsigned int j;
  for(;i>0;i--)
  {
   for(j=12;j>0;j--);
  }
}


