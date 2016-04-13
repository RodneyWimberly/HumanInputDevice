
#ifndef _HUMANINPUTDEVICE_h
#define _HUMANINPUTDEVICE_h

#include "Property.h"
#include "RfPacket.h"
#include "LiquidCrystal.h"
#include "DFR_Key.h"
#include <Arduino.h>
#include <SPI.h>
#include "EEPROMex.h"
#include "ConfigRegister.h"
#include "Radio.h"
#include "printf.h"
#include "RfPacket.h"

#define USE_UNO							1
#define DEBUG							1

#define DEFAULT_KEY_RATE				10

#define SUCCESS							0
#define ERROR_UNKNOWN_INSTRUCTION		1

#define USE_HID							1

// IRQ Usage
#define RADIO1_IRQ			0 // nRF24L01+ Interrupt Request (IRQ 0 must be used with pin 2)
#define RADIO2_IRQ			1 // nRF24L01+ Interrupt Request (IRQ 1 must be used with pin 3)

// Digital Pin Usage
#if USE_UNO
	#define EEPROM_MEM_SIZE		EEPROMSizeUno
	#define RADIO1_CE_PIN   	A0 // Serial Peripheral Interface - Chip Enable 1 (nRF24L01+)
	#define RADIO1_CS_PIN  		A1 // Serial Peripheral Interface - Chip Select 1 (nRF24L01+)
	#define UART_TX_PIN     	0 // Universal Asynchronous Receiver Transmitter - Transmit
	#define UART_RX_PIN     	1 // Universal Asynchronous Receiver Transmitter - Receive
	#define RADIO1_IRQ_PIN		2 // nRF24L01+ Interrupt Request (IRQ 0 must be used with pin 2)
	#define LCD_DATA_0			4 // LCD
	#define LCD_DATA_1			5 // LCD
	#define LCD_DATA_2			6 // LCD
	#define LCD_DATA_3			7 // LCD
	#define LCD_RS				8 // LCD
	#define LCD_ENABLE			9 // LCD
	#define SPI_MOSI_PIN    	11 // Serial Peripheral Interface - Master Output, Slave Input (output from master)
	#define SPI_MISO_PIN    	12 // Serial Peripheral Interface - Master Input, Slave Output (output from slave)
	#define SPI_SCLK_PIN     	13 // Serial Peripheral Interface - Serial Clock (output from master)
#else
	#define EEPROM_MEM_SIZE			EEPROMSizeUno
	#define UART_TX_PIN     	0 // Universal Asynchronous Receiver Transmitter - Transmit
	#define UART_RX_PIN     	1 // Universal Asynchronous Receiver Transmitter - Receive
	#define LCD_RS				8 // LCD
	#define LCD_ENABLE			9 // LCD
	#define LCD_DATA_0			4 // LCD
	#define LCD_DATA_1			5 // LCD
	#define LCD_DATA_2			6 // LCD
	#define LCD_DATA_3			7 // LCD
	#define RADIO1_CS_PIN  		8 // Serial Peripheral Interface - Chip Select 1 (nRF24L01+)
	#define RADIO1_CE_PIN   	9 // Serial Peripheral Interface - Chip Enable 1 (nRF24L01+)
	#define SPI_MOSI_PIN    	11 // Serial Peripheral Interface - Master Output, Slave Input (output from master)
	#define SPI_MISO_PIN    	12 // Serial Peripheral Interface - Master Input, Slave Output (output from slave)
	#define SPI_SCLK_PIN     	13 // Serial Peripheral Interface - Serial Clock (output from master)
#endif


#endif

