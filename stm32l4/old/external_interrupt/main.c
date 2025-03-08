// Author: Yusef Karim
#include "stm32l476xx.h"

// Function prototypes
void EXTI1_Init(void);


// EXTI 1 interrupt handler
void EXTI1_IRQHandler(void)
{
    // Check for EXTI1 interrupt flag
    if((EXTI->PR1 & EXTI_PR1_PIF1) == EXTI_PR1_PIF1) {
        GPIOC->ODR ^= 1UL<<8;           // Toggle pin PC8
        //Clear interrupt pending request
        EXTI->PR1 |= EXTI_PR1_PIF1;     // Write 1 to clear
    }
}

int main(void)
{
    /* INITIALIZE PIN PC8 AS OUTPUT */
    // Enable the clock to GPIO port C
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    // Set mode of pin as digital output
    GPIOC->MODER &= ~(3UL<<16);         // Clear mode bits
    GPIOC->MODER |= 1UL<<16;            // Set mode = 01 (digital output)
    // Set output type of pin as push-pull
    GPIOC->OTYPER &= ~(1<<8);
    // Set output speed of pin as low
    GPIOC->OSPEEDR &= ~(3UL<<16);      // Clear speed bits
    // Set pin as no pull-up, no pull-down
    GPIOC->PUPDR &= ~(3UL<<16);        // no pull-up, no pull-down
    // Clear state (off)
    GPIOC->ODR &= ~(1UL<<8);

    /*  INITIALIZE PIN PA1 AS INPUT */
    // Enable the clock to GPIO port A
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    // Set mode of pin as digital input
    GPIOA->MODER &= ~(3UL<<2);       // Clear mode bits (also sets as input)
    // Set pin as pull-down
    GPIOA->PUPDR &= ~(3UL<<2);       // clear all options
    GPIOA->PUPDR |= 2UL<<2;          // set as pull-down

    // Initialize EXTI 1 to handle interrupts on rising edge
    EXTI1_Init();
    while(1);

}


void EXTI1_Init(void)
{
    // Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Select PA1 as the trigger source of  EXTI1
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
    SYSCFG->EXTICR[0] &= ~(0x000F);

    // Enable rising edge trigger for EXTI 1, via Rising trigger selection register (RSTR)
    // 0 = disabled, 1 = enabled
    EXTI->RTSR1 |= EXTI_RTSR1_RT1;
    // Disable rising edge trigger for EXTI 1, via Falling trigger selection register(FSTR)
    // 0 = disabled, 1 = enabled
    EXTI->FTSR1 &= ~EXTI_FTSR1_FT1;

    // Enable EXTI 1 interrupt
    // Interrupt mask register: 0 = masked, 1 = unmasked
    // Masked means that the processor ignores the corresponding interrupt.
    EXTI->IMR1 |= EXTI_IMR1_IM1;

    // Set EXTI1 priority to 1
    NVIC_SetPriority(EXTI1_IRQn, 1);

    // Enable EXTI1 interrupt
    NVIC_EnableIRQ(EXTI1_IRQn);
}
