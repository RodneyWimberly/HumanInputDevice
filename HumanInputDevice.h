#ifndef _HUMANINPUTDEVICE_h
#define _HUMANINPUTDEVICE_h

//#define SERIAL_DEBUG

#include <Arduino.h>
#include <SPI.h>
#include <Sync.h>
#include <RF24Network_config.h>
#include <RF24Network.h>
#include <RF24_config.h>
#include <RF24.h>
#include <printf.h>
#include <nRF24L01.h>
#include <elapsedMillis.h>
#include <EEPROMex.h>
#include <DfrLcdKeypad.h>
#include "ConfigRegister.h"

enum HeaderTypes
{
	/* These types will NOT receive network acknowledgements */
	notSet = 0,
	rePowerLineEvent = 1,
	reSerialDataEvent = 2,
	reInfaredEvent = 3,
	reRadioFrequencyEvent = 4,

	/* These types will receive network acknowledgements */
	hidGetKey = 66,
	hidWaitForKey = 67,
	hidLcdClear = 68,
	hidLcdSetCursor = 69,
	hidLcdWrite = 70,
	hidLcdWriteAt = 71,
	hidLcdCustomCharacter = 72,
	hidLcdCustomCharacterAt = 73,
	hidLcdCommand = 74,
	reX10Standard = 75,
	reX10ModuleStatus = 76,
	reEnvironmentSensor = 77,
	reSetClock = 78,
	reReadClock = 79,
};

#define USE_UNO							1
#define DEBUG							1

#define RADIO_CHANNEL					90
#define RE_RADIO_NODE_ID				00
#define HID_RADIO_NODE_ID				01
#define CLEAR_DISPLAY_INTERVAL			30000UL

#define USE_HID							1

// IRQ Usage
#define RADIO1_IRQ			0 // nRF24L01+ Interrupt Request (IRQ 0 must be used with pin 2)
#define RADIO2_IRQ			1 // nRF24L01+ Interrupt Request (IRQ 1 must be used with pin 3)

// Digital Pin Usage
#if USE_UNO
	#define EEPROM_MEM_SIZE		EEPROMSizeUno
	#define RADIO1_CE_PIN   	A3 // Serial Peripheral Interface - Chip Enable 1 (nRF24L01+)
	#define RADIO1_CS_PIN  		A1 // Serial Peripheral Interface - Chip Select 1 (nRF24L01+)
	#define KEYPAD_ANALOG_PIN	0 // DFRobot LCD Button Shield - Button input
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

struct X10Data_s
{
	char houseCode;
	uint8_t unitCode;
	uint8_t commandCode;
	uint8_t extendedData;
	uint8_t extendedCommand;
	bool success;
};


#endif

