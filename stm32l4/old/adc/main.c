// Author: Yusef Karim
#include "stm32l476xx.h"


int main(void)
{
    // Set mode of PA1 as analog
    // 00 = digital input       01 = digital output
    // 10 = alternate function  11 = analog (default)
    GPIOA->MODER |= 3UL<<2;

    // Set PA1 as pull-down
    // 00 = no pull-up, no pull-down      01 = pull-up
    // 10 = pull-down                     11 = reserved
    GPIOA->PUPDR &= ~(3UL<<2);
    GPIOA->PUPDR |= 2UL<<2;

    // Set port analog switch control register (ASCR) for PA1
    // 0 = Disconnect analog switch from the ADC input (reset state)
    // 1 = Connect analog switch to the ADC input
    GPIOA->ASCR |= 1U<<1;

    // TODO: REREAD CHAPTER 20 AND FINISH THIS CODE
}

