#include <stdint.h>
#include "SysTick.h"
#include "tm4c123gh6pm.h"

//Enables SysTick with passed in reload value
//Enables interrupts with Priority 0 for SysTick
void SysTick_Init(unsigned long reload)
{

    NVIC_ST_CTRL_R = 0;								//Disable SysTick during setup
    NVIC_ST_RELOAD_R = reload - 1;					//Number of counts to wait
    NVIC_ST_CURRENT_R = 0;							//Any write clears current
    //Give SysTick Priority 2
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF) | 0x40000000;
    //Enable Systick with core clock and interrupts
    //NVIC_ST_CTRL_ENABLE(bit0)+NVIC_ST_CTRL_CLK_SRC(bit1)+NVIC_ST_CTRL_INTEN(bit2)
    NVIC_ST_CTRL_R |= 0x07;
}

/*
//Uses SysTick to count down 1ms*(passed value)
void SysTick_Wait1ms(unsigned long ms)
{
    //Clock is set to 80MHz, each Systick takes 1/80MHz = 12.5ns
    //To get 1ms count down delay, take (1ms)/(12.5ns)
    unsigned long delay = 80000;
    unsigned long i;

    for(i = 0; i < ms; i++)
    {
        NVIC_ST_RELOAD_R = delay - 1;   //Reload value is the number of counts to wait
        NVIC_ST_CURRENT_R = 0;          //Clears current
        //Bit 16 of STCTRL is set to 1 if SysTick timer counts down to zero
        while((NVIC_ST_CTRL_R&0x00010000) == 0){}
    }
}
*/
