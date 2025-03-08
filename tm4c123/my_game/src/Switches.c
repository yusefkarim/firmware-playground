#include <stdint.h>
#include "Switches.h"
#include "tm4c123gh6pm.h"

/*
//Setup port E, PE2-0
void Switches_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000010;   //E clock
    delay = SYSCTL_RCGC2_R;         //delay
    GPIO_PORTE_CR_R |= 0x07;        //Allow changes to PE2-0 registers
    GPIO_PORTE_DIR_R &= ~0x07;      //PE2-0 as input
    GPIO_PORTE_AFSEL_R &= ~0x07;    //Clear alt functions
    GPIO_PORTE_PCTL_R &= ~0x07;     //GPIO, clear bit PCTL
    GPIO_PORTE_AMSEL_R &= ~0x07;    //Clear analog functions
    GPIO_PORTE_PDR_R |= 0x07;       //Enable pull-down resistors on PE2-0
    GPIO_PORTE_DEN_R |= 0x07;       //Enable digital pin PE2-0
    GPIO_PORTE_IS_R &= ~0x07;       //PE2-0, edge triggered
    GPIO_PORTE_IBE_R &= ~0x07;      //PE2-0, not both edges
    GPIO_PORTE_IEV_R |= 0x07;       //PE2-0, rising edge trigger
    GPIO_PORTE_ICR_R |= 0x07;       //clear interrupt flags
    GPIO_PORTE_IM_R |= 0x07;        //arm interrupts
    //Give PortE Priority 1 by setting bit 6 in PRI1
    NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFF01F) | 0x00000040;
    NVIC_EN0_R |= 0x00000010;       //Enable interrupt 4 in NVIC
}
*/

//Setup port E, PE2-0
void Switches_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000010;   //E clock
    delay = SYSCTL_RCGC2_R;         //delay
    GPIO_PORTE_CR_R |= 0x07;        //Allow changes to PE2-0 registers
    GPIO_PORTE_DIR_R &= ~0x07;      //PE2-0 as input
    GPIO_PORTE_AFSEL_R &= ~0x07;    //Clear alt functions
    GPIO_PORTE_PCTL_R &= ~0x07;     //GPIO, clear bit PCTL
    GPIO_PORTE_AMSEL_R &= ~0x07;    //Clear analog functions
    GPIO_PORTE_PDR_R |= 0x07;       //Enable pull-down resistors on PE2-0
    GPIO_PORTE_DEN_R |= 0x07;       //Enable digital pin PE2-0
}

