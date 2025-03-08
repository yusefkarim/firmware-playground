//------------PLL_Init------------//
//Gets 80MHz clock from PLL
void PLL_Init(void);

//------------PortF_Init------------//
void PortF_Init(void);

//------------UART_Init------------//
//Initialize the UART for 9600 baud rate (assuming 80 MHz UART clock),
//8 bit word length, no parity bits, one stop bit, FIFOs enabled
//Using UART0 PA1(Tx),PA0(Rx)
void UART_Init(void);

//------------UART_InChar------------//
//Wait for new serial port input, accepts and returns an 8 bit char in ASCII
unsigned char UART_InChar(void);

//------------UART_OutChar------------//
//Input: letter is an 8-bit ASCII character to be transferred
//Output 8-bit to serial port
void UART_OutChar(char data);

//------------UART_OutString------------//
//Input: pointer to a NULL-terminated string to be transferred
//Output String (NULL termination)
void UART_OutString(char buffer[]);

