// Author: Yusef Karim
#include "stm32l476xx.h"


/*
 * Accepts GPIO_Typedef struct defined for GPIO ports in stm32l476xx.h and
 * and a pin number. Initializes that pin as an output with the characteristics
 * defined in the source file.
 *
 * FUNCTION USAGE:
 * ## Step 1: Initialize the clock for the port you want
 *    Ex. To enable the clock for port B, set the bit in AHB2ENR register
 *    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
 * ## Step 2: Use this function to initialize the GPIO pin with defined settings
 *    Ex. Initializing pin PB2 as output
 *    GPIO_Pin_Output_Init(GPIOB, 2);
 * ## Step 3: Set or clear the specific pin you have initialized to turn on and off
 *    Ex. Turning pin PB2 on
 *    GPIO->ODR |= 1UL<<2
*/
void GPIO_Pin_Output_Init(GPIO_TypeDef *GPIO, unsigned short pin) {
    // Set mode of pin as digital output
    // 00 = digital input       01 = digital output
    // 10 = alternate function  11 = analog (default)
    GPIO->MODER &= ~(3UL<<(2*pin));         // Clear mode bits
    GPIO->MODER |= 1UL<<(2*pin);            // Set mode = 01 (digital output)

    // Set output type of pin as push-pull
    // 0 = push-pull (default)
    // 1 = open-drain
    GPIO->OTYPER &= ~(1<<pin);

    // Set output speed of pin as low
    // 00 = low speed      01 = medium speed
    // 10 = fast speed     11 = high speed
    GPIO->OSPEEDR &= ~(3UL<<(2*pin));      // Clear speed bits

    // Set pin as no pull-up, no pull-down
    // 00 = no pull-up, no pull-down      01 = pull-up
    // 10 = pull-down                     11 = reserved
    GPIO->PUPDR &= ~(3UL<<(2*pin));        // no pull-up, no pull-down
}


/*
 * Accepts GPIO_Typedef struct defined for GPIO ports in stm32l476xx.h and
 * and a pin number. Initializes that pin as an input with the characteristics
 * defined in the source file.
 *
 * FUNCTION USAGE:
 * ## Step 1: Initialize the clock for the port you want
 *    Ex. To enable the clock for port B, set the bit in AHB2ENR register
 *    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
 * ## Step 2: Use this function to initialize the GPIO pin with defined settings
 *    Ex. Initializing pin PB8 as input
 *    GPIO_Pin_Input_Init(GPIOB, 8);
 * ## Step 3: Read the initialized pin by masking it with a logical AND
 *    Ex. Reading the value of pin PB8 (bit eight)
 *    GPIOB->IDR & 0x100
*/
void GPIO_Pin_Input_Init(GPIO_TypeDef *GPIO, unsigned short pin) {
    // Set mode of pin as digital input
    // 00 = digital input       01 = digital output
    // 10 = alternate function  11 = analog (default)
    GPIO->MODER &= ~(3UL<<(2*pin));         // Clear mode bits (also sets as input)

    // Set pin as no pull-up, no pull-down
    // 00 = no pull-up, no pull-down      01 = pull-up
    // 10 = pull-down                     11 = reserved
    GPIO->PUPDR &= ~(3UL<<(2*pin));        // no pull-up, no pull-down
}


