
/******************************************************************************************
/* Includes
/******************************************************************************************/
#include "HumanInputDevice.h"
#include "LiquidCrystal.h"
#include "DFR_Key.h"
#include <Arduino.h>
#include <SPI.h>
#include "EEPROMex.h"
#include "EEPROMvar.h"
#include "IPAddress.h"
#include "Radio.h"



/******************************************************************************************
/* Forward declarations
/******************************************************************************************/
void wirelessEvent(Radio* radio, hidTypes hidType, const void* buffer, uint8_t length);

/******************************************************************************************
/* Variable declarations
/******************************************************************************************/
const uint64_t addresses[6] = { 0xABCDABCD00LL, 0xABCDABCD10LL, 0xABCDABCD20LL, 0xABCDABCD11LL, 0xABCDABCD21LL, 0xABCDABCD12LL };

/******************************************************************************************
/* Class Initialization
/******************************************************************************************/
// DFRobot LCD Keypad Shield
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DATA_0, LCD_DATA_1, LCD_DATA_2, LCD_DATA_3);
DFR_Key keypad;

// nRF24L01+ 2.4 GHz Wireless Transceiver
Radio radio(
	RADIO1_CE_PIN,
	RADIO1_CS_PIN,
	RADIO1_IRQ,
	&wirelessEvent); 

void setup()
{
	Serial.begin(115200);

	// start reading from position memBase (address 0) of the EEPROM. Set maximumSize to EEPROMSizeUno 
	// Writes before membase or beyond EEPROMSizeUno will only give errors when _EEPROMEX_DEBUG is set
	EEPROM.setMemPool(memBase, EEPROMSizeUno);

	// Set maximum allowed writes to maxAllowedWrites. 
	// More writes will only give errors when _EEPROMEX_DEBUG is set
	EEPROM.setMaxAllowedWrites(maxAllowedWrites);
	//EEPROMVar<float> eepromFloat(5.5);  // initial value 5.5
	//eepromFloat.save();     // store EEPROMVar to EEPROM
	//eepromFloat = 0.0;      // reset 
	//eepromFloat.restore();  // restore EEPROMVar to EEPROM

	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);

	radio.begin(addresses[1], addresses[0], true);
}


/******************************************************************************************
/* Process Loop 
/******************************************************************************************/
void loop()
{

  /* add main program code here */

}


/******************************************************************************************
/* Event Handlers
/******************************************************************************************/

void wirelessEvent(Radio* radio, hidTypes hidType, const void* buffer, uint8_t length)
{
	const byte* current = reinterpret_cast<const byte*>(buffer);
	
	if (hidType == HidSetKeyRate)
	{
		keypad.setRate(current[0]);
	}
}
