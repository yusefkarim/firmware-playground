//Author: Yusef Karim
//Project: UART_to_PC

//This program enables UART0 and allows serial communication via usb
//The user can change the on-board LED by sending specific characters via
//serial connection

#include "../inc/uart.h"

//For PortF GPIO Pins
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))

int main(void)
{
    PLL_Init();
    PortF_Init();
    UART_Init();
    
    unsigned char input; 

    while(1)
    {   
        //Asks and reads input from user
        UART_OutString("Input: ");
        input = UART_InChar();
        UART_OutChar(input);
        UART_OutString("\n\r");
        
        //Checks user input and turns Onboard LED on accordingly
        if(input == 'r')
            GPIO_PORTF_DATA_R = 0x02;   //red
        else if(input == 'b')
            GPIO_PORTF_DATA_R = 0x04;   //blue
        else if(input == 'g')
            GPIO_PORTF_DATA_R = 0x08;   //green
        else if(input == 'y')
            GPIO_PORTF_DATA_R = 0x0A;   //yellow
        else
            GPIO_PORTF_DATA_R = 0x00;   //off

    }
}
