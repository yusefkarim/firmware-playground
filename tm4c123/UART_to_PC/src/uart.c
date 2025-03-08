#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/uart.h"

//------------PLL_Init------------//
//Gets 80MHz clock from PLL
//Using the 400MHz PLL we get the bus frequency by dividing
//400MHz / (SYSDIV2+1). Therefore, 400MHz/(4+1) = 80 MHz bus frequency
#define SYSDIV2 4
void PLL_Init(void)
{
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;   //Use RCC2 for advanced features
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;   //Bypass PLL during initialization
    SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;     //Clear XTAL field
    SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;  //Use 16Mhz crystal
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;//Clear oscillator source field
    SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;//Use main oscillator source field
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;   //Activate PLL by clearing PWRDN

    //As explained above we use 400MHz PLL and specified SYSDIV2 for 80MHz clock
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;    //Use 400MHz PLL
    //Clear system clock divider field then configure 80Mhz clock
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000) + (SYSDIV2<<22);
    //Wait for the PLL to lock by polling PLLLRIS
    while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS) == 0){};
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;  //Enable PLL by clearing BYPASS
}

//Setup port F
//------------PortF_Init------------//
void PortF_Init(void)
{
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000020;   //F clock
    delay = SYSCTL_RCGC2_R;         //delay
    GPIO_PORTF_LOCK_R = 0x4C4F434B; //Unlock Port F
    GPIO_PORTF_CR_R |= 0x1F;        //Allow changes to PF4-0
    GPIO_PORTF_AMSEL_R &= ~0x1F;    //Disable analog functions
    GPIO_PORTF_PCTL_R &= ~0x1F;     //GPIO clear bit PCTL
    GPIO_PORTF_DIR_R |= 0x1F;       //PF4-0 output
    GPIO_PORTF_AFSEL_R &= ~0x1F;    //Clear alt functions
    //GPIO_PORTF_PUR_R |= 0x11;       //Enable pull-up resistor on PF4 and PF0
    GPIO_PORTF_DEN_R |= 0x1F;       //Enable digital pin PF4, PF0 and PF2-1
}


//------------UART0Init------------//
//Initialize the UART for 9600 baud rate (assuming 80 MHz UART clock),
//8 bit word length, no parity bits, one stop bit, FIFOs enabled
//Using UART0 PA1(Tx),PA0(Rx)
void UART_Init(void)
{
    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; //Activate UART0
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; //Activate port A
    UART0_CTL_R &= ~UART_CTL_UARTEN;      //Disable UART to config
    //IBRD = int(80,000,000 / (16 * 9600)) = int(520.83)
    UART0_IBRD_R = 520;
    //FBRD = round(0.83 * 64) = 60
    UART0_FBRD_R = 60;
    //8 bit word length (no parity bits, one stop bit, FIFO)
    UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
    UART0_CC_R = UART_CC_CS_SYSCLK;       //Use system clock for UART
    //Enable UART0 and Tx, Rx functions
    UART0_CTL_R = (UART_CTL_UARTEN|UART_CTL_RXE|UART_CTL_TXE);
    GPIO_PORTA_AFSEL_R |= 0x03;           //Enable alt funct on PA1,PA0
    GPIO_PORTA_DEN_R |= 0x03;             //Enable digital I/O on PA1,PA0
    //configure PA1,PA0 as UART0
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
    GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA1,PA0
}

//------------UART_InChar------------//
//Wait for new serial port input, returns an ASCII char
unsigned char UART_InChar(void)
{
    //Waits for the RXFE bit in UART Flag register to be 0, FIFO full
    while((UART0_FR_R&UART_FR_RXFE) != 0);
    //Once FIFO full read and return the char
    return((unsigned char)(UART0_DR_R&0xFF));
}

//------------UART_OutChar------------//
//Input: letter is an 8-bit ASCII character to be transferred
//Output 8-bit to serial port
void UART_OutChar(char data)
{
    //Waits for the TXFF bit in UART Flag register to be 0, FIFO empty
    while((UART0_FR_R&UART_FR_TXFF) != 0);
    //Wrtie data to UART Data register
    UART0_DR_R = data;
}

//------------UART_OutString------------//
//Input: pointer to a NULL-terminated string to be transferred
//Output String (NULL termination)
void UART_OutString(char buffer[])
{
    //Uses above function to print each char in the passed string
	unsigned long i = 0;
	while(buffer[i])
	{
		UART_OutChar(buffer[i]);
		i++;
	}
}


