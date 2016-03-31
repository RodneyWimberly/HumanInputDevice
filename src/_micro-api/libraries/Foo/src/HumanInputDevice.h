
#ifndef _HUMANINPUTDEVICE_h
#define _HUMANINPUTDEVICE_h

#define USE_UNO					1
#define DEBUG					1

// IRQ Usage
#define RADIO1_IRQ			0 // nRF24L01+ Interrupt Request (IRQ 0 must be used with pin 2)
#define RADIO2_IRQ			1 // nRF24L01+ Interrupt Request (IRQ 1 must be used with pin 3)

// Digital Pin Usage
#if USE_UNO
	#define UART_TX_PIN     	0 // Universal Asynchronous Receiver Transmitter - Transmit
	#define UART_RX_PIN     	1 // Universal Asynchronous Receiver Transmitter - Receive
	#define RADIO1_CS_PIN  		7 // Serial Peripheral Interface - Chip Select 1 (nRF24L01+)
	#define RADIO1_CE_PIN   	6 // Serial Peripheral Interface - Chip Enable 1 (nRF24L01+)
	#define SPI_MOSI_PIN    	11 // Serial Peripheral Interface - Master Output, Slave Input (output from master)
	#define SPI_MISO_PIN    	12 // Serial Peripheral Interface - Master Input, Slave Output (output from slave)
	#define SPI_SCLK_PIN     	13 // Serial Peripheral Interface - Serial Clock (output from master)
#else
	#define UART_TX_PIN     	0 // Universal Asynchronous Receiver Transmitter - Transmit
	#define UART_RX_PIN     	1 // Universal Asynchronous Receiver Transmitter - Receive
	#define RADIO1_CS_PIN  		8 // Serial Peripheral Interface - Chip Select 1 (nRF24L01+)
	#define RADIO1_CE_PIN   	9 // Serial Peripheral Interface - Chip Enable 1 (nRF24L01+)
	#define SPI_MOSI_PIN    	11 // Serial Peripheral Interface - Master Output, Slave Input (output from master)
	#define SPI_MISO_PIN    	12 // Serial Peripheral Interface - Master Input, Slave Output (output from slave)
	#define SPI_SCLK_PIN     	13 // Serial Peripheral Interface - Serial Clock (output from master)
#endif

const int maxAllowedWrites = 20;
const int memBase = 120;

#endif

