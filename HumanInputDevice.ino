#include "HumanInputDevice.h"

void wirelessEvent(Radio* radio, RfPacket* rfPacket, uint8_t pipeNumber);

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DATA_0, LCD_DATA_1, LCD_DATA_2, LCD_DATA_3);
DFR_Key keypad;
Radio radio(RADIO1_CE_PIN, RADIO1_CS_PIN, RADIO1_IRQ, &wirelessEvent); 
ConfigRegister configRegister;

void setup()
{
	Serial.begin(115200);
	printf_begin();

	configRegister.begin();

	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);

	keypad.setRate(DEFAULT_KEY_RATE);

	radio.begin(configRegister.HidAddress, HUMAN_INPUT_DEVICE_PIPE_ADDRESS, HOST_PIPE_ADDRESS, ROOM_EXTENDER_PIPE_ADDRESS, true);

	Serial.println("HID is loaded and monitoring for new requests.");
}

void wirelessEvent(Radio* radio, RfPacket* rfPacket, uint8_t pipeNumber)
{
	printWirelessEvent(rfPacket, pipeNumber);
	
	rfPacket->Success = true;

	// If this is an ACK packet then we don't care...
	if (pipeNumber == 0)
		return;

	if (rfPacket->Type == hidSetKeyRate)
	{
		const int* current = reinterpret_cast<const int*>(rfPacket->readBody());
		keypad.setRate(current[0]);
	}
	else if (rfPacket->Type == hidGetKey)
	{
		int key[1] = { keypad.getKey() };
		memcpy(rfPacket->readBody(), key, 1);
	}
	else if (rfPacket->Type == hidLcdClear)
	{
		lcd.clear();
	}
	else if (rfPacket->Type == hidLcdSetCursor)
	{
		const uint8_t* current = reinterpret_cast<const uint8_t*>(rfPacket->readBody());
		lcd.setCursor(current[0], current[1]);
	}
	else if (rfPacket->Type == hidLcdPrint)
	{
		const char* current = reinterpret_cast<const char*>(rfPacket->readBody());
		lcd.print(current);
	}
	else if (rfPacket->Type == hidLcdPrintLine)
	{
		const char* current = reinterpret_cast<const char*>(rfPacket->readBody());
		lcd.println(current);
	}
	else if (rfPacket->Type == hidLcdWrite)
	{
		const char* current = reinterpret_cast<const char*>(rfPacket->readBody());
		lcd.write(current);
	}
	else if (rfPacket->Type == hidLcdCommand)
	{
		const uint8_t* current = reinterpret_cast<const uint8_t*>(rfPacket->readBody());
		lcd.command(current[0]);
	}
	else
	{
		rfPacket->Success = false;
	}
	Serial.println("sendAckPacket");
	radio->sendAckPacket(rfPacket);
}

void printWirelessEvent(RfPacket* rfPacket, uint8_t pipeNumber)
{
	byte* buffer = reinterpret_cast<byte*>(rfPacket->readBody());

	Serial.print(millis());
	Serial.println(": wirelessEvent Fired!");

	Serial.print("Pipe Number: ");
	Serial.println(pipeNumber);

	Serial.print("Source Address: ");
	Serial.println(rfPacket->SourceAddress);

	Serial.print("Destination Address: ");
	Serial.println(rfPacket->DestinationAddress); 
	
	uint8_t type = rfPacket->Type;
	Serial.print("PacketType: ");
	Serial.println((PacketTypes)type);

	uint8_t length = sizeof(buffer);
	for (int i = 0; i < rfPacket->BodyLength; i++)
	{
		Serial.print("Buffer [");
		Serial.print(i);
		Serial.print("]: ");
		Serial.println(buffer[i]);
	}
}

void loop() 
{
}

