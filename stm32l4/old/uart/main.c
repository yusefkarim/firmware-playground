// Author: Yusef Karim
#include "stm32l476xx.h"

// TODO: TEST THIS CODE WITH SERIAL-TO-USB ADAPTER (I NEED A SECOND POWER CABLE)

void USART_Init(USART_TypeDef *USARTx);
void USART_Read(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t n_bytes);
void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t n_bytes);

int main(void)
{
    // Enable GPIO clock for PB then configure USART1_TX (PB6) and USART1_RX (PB7)
    // as Alternate Function, High Speed, Push-Pull, and Pull-up

    // Enable clock for GPIO port B
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    // Set mode of PB6-7 as alternative function
    GPIOA->MODER &= ~(0xF << (2*6));    // Clear mode bit for 6 and 7
    GPIOA->MODER |= 0xA << (2*6);       // Set alternate function for 6 and 7
    // Choose alternate function (7 = USART 1)
    GPIOB->AFR[0] |= 0x77 << (4*6);     // Set pin 6 and 7 to AF 7
    // Set GPIO speed for PB6-7 as high speed
    GPIOB->OSPEEDR |= 0xF << (2*6);
    // Set GPIO PB6-7 as pull-up
    GPIOB->PUPDR &= ~(0xF << (2*6));    // Clear push-pull bits
    GPIOB->PUPDR |= 0x5 << (2*6);       // Set pull-up bits
    // Set PB6-7 as push-pull
    GPIOB->OTYPER &= ~(0x3 << 6);

    // Enable clock for UART 1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // Select system clock (SYSCLK) for USART clcok source of UART 1
    // 00 = PCLK, 01 = System clock (SYSCLK), 10 = HSI16, 11 = LSE
    RCC->CCIPR &= ~RCC_CCIPR_USART1SEL;
    RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
    // Configure USART1
    USART_Init(USART1);
}

/*
 *
 */
void USART_Init(USART_TypeDef *USARTx) {
    // Disable USART
    USARTx->CR1 &= ~USART_CR1_UE;

    // Set data length to  8 bits
    // 00 = 8 data bits, 01 = 9 data bits, 10 = 7 data bits
    USARTx->CR1 &= ~USART_CR1_M;

    // Select 1 stop bit
    // 00 = 1 stop bit, 01 = 0.5 stop bit, 10 = 2 stop bits, 11 = 1.5 stop bit
    USARTx->CR2 &= ~USART_CR2_STOP;

    // Set parity control as no parity
    // 0 = no parity, 1 = parity enabled (Use PS bit to select even/odd parity)
    USARTx->CR1 &= ~USART_CR1_PCE;

    // Oversampling by 16
    // 0 = oversampling by 18, 1 = oversampling by 8
    USARTx->CR1 &= ~USART_CR1_OVER8;

    // Set Baud rate to be 9600 use APB frequency (80 MHz)
    // The below calculation assumes signal is oversampled by 16 and processer at 80MHz
    // USARTDIV = ((1+OVER8) * f_pc)/(Baud Rate) = 80Mhz/9600 = 8333.33 = BRR = 0x208D
    USARTx->BRR = 0x208D;

    // Enable transmission and reception
    USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // Enable USART
    USARTx->CR1 |= USART_CR1_UE;

    // Wait for USART to be ready for transmission
    // The hardware will set or reset the Transmit Enable Acknowledge flag (TEACK)
    while((USARTx->ISR & USART_ISR_TEACK) == 0);
    // Wait for USART to be ready for receiving
    // The hardware will set or reset the Receive Enable Acknowledge flag (REACK)
    while((USARTx->ISR & USART_ISR_REACK) == 0);
}

void USART_Read(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t n_bytes) {
    int i;

    for(i = 0; i < n_bytes; i++) {
        while(!(USARTx->ISR & USART_ISR_RXNE));     // Wait until hardware sets RXNE bit
        buffer[i] = USARTx->RDR;                    // Reading from RDR clears RXNE flag
    }
}

void USART_Write(USART_TypeDef *USARTx, uint8_t *buffer, uint32_t n_bytes) {
    int i;

    for(i = 0; i < n_bytes; i++) {
        while(!(USARTx->ISR & USART_ISR_TC));   // Wait until hardware sets TXE bit
        USARTx->TDR = buffer[i] & 0xFF;         // Writing to TDR clears TXE flag
    }

    // Wait until TC bit is set. TC is set by the hardware and cleared by software
    while(!(USARTx->ISR & USART_ISR_TC));       // TC = Transmission complete flag

    // Writing 1 to the TCCF bit in ICR clears the TC bit in ISR
    USARTx->ICR |= USART_ICR_TCCF;              // TCCF = Transmission complete flag clear
}

