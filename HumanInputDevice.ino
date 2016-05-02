#include "HumanInputDevice.h"

elapsedMillis p_elapsedTimer, p_x10StatusShown;
bool p_x10StatusVisible = false, 
	success = true;

// nRF24L01+ 2.4 GHz Wireless Transceiver
RF24 radio(RADIO1_CE_PIN, RADIO1_CS_PIN);

// Add OSI layer 3 network to the nRF24L01+ radio
RF24Network network(radio);

// LCD Button Shield
DfrLcdKeyPad lcd(LCD_RS, LCD_ENABLE, LCD_DATA_0, LCD_DATA_1, LCD_DATA_2, LCD_DATA_3, KEYPAD_ANALOG_PIN);

// Configuration Register loaded from EEPROM
ConfigRegister configRegister;

void setup()
{
	// Initialize Serial and PrintF buffer
	Serial.begin(115200);
	printf_begin();

	configRegister.begin();

	// Initialize the LCD Button Shield
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(0, 0);

	// Start the nRF24L01+ library
	SPI.begin();
	radio.begin();
	radio.setPALevel(RF24_PA_HIGH);
	radio.setDataRate(RF24_250KBPS);
	network.begin(RADIO_CHANNEL, HID_RADIO_NODE_ID);

	p_x10StatusVisible = true;
	p_x10StatusShown = CLEAR_DISPLAY_INTERVAL;
	
	// Write out that we started
	Serial.println(F("HID is loaded and monitoring for new requests."));
}

void loop()
{
	network.update();
	while (network.available())
		processIncommingRequests();
	processOutgoingRequests();
	clearX10Status();
}

void processIncommingRequests()
{
	RF24NetworkHeader header;
	uint16_t length = network.peek(header);
	IF_SERIAL_DEBUG(printf_P(PSTR("%lu: APP Received request from 0%o type %d\n\r"), millis(), header.from_node, header.type));
	switch ((HeaderTypes)header.type)
	{
	case HeaderTypes::hidGetKey:
		getKey(header, length);
		break;
	case HeaderTypes::hidWaitForKey:
		waitForKey(header, length);
		break;
	case HeaderTypes::hidLcdClear:
		lcdClear(header, length);
		break;
	case HeaderTypes::hidLcdCommand:
		lcdCommand(header, length);
		break;
	case HeaderTypes::hidLcdCustomCharacter:
		lcdCustomCharacter(header, length);
		break;
	case HeaderTypes::hidLcdCustomCharacterAt:
		lcdCustomCharacterAt(header, length);
		break;
	case HeaderTypes::hidLcdSetCursor:
		lcdSetCursor(header, length);
		break;
	case HeaderTypes::hidLcdWrite:
		lcdWrite(header, length);
		break;
	case HeaderTypes::hidLcdWriteAt:
		lcdWriteAt(header, length);
		break;
	case HeaderTypes::rePowerLineEvent:
	case HeaderTypes::reSerialDataEvent:
	case HeaderTypes::reInfaredEvent:
	case HeaderTypes::reRadioFrequencyEvent:
		printX10Status(header, length);
		break;
	default:
		network.read(header, 0, 0);
		break;
	}
}

void processOutgoingRequests()
{
}

void clearX10Status()
{
	if (p_x10StatusVisible && p_x10StatusShown > CLEAR_DISPLAY_INTERVAL)
	{
		char x10msg[] = "    ";
		lcd.setCursor(10, 0);
		lcd.write(x10msg);
		p_x10StatusVisible = false;
	}
}

void printX10Status(RF24NetworkHeader& header, uint16_t length)
{
	X10Data_s x10Data;
	network.read(header, &x10Data, sizeof(X10Data_s));
	Serial.print("X10 Status: ");
	Serial.print(x10Data.houseCode);
	Serial.print(x10Data.unitCode);
	Serial.print("_");
	Serial.println(x10Data.commandCode);

	char x10msg[] = "A1_O";
	sprintf(x10msg, "%c%X_%X", x10Data.houseCode, x10Data.unitCode, x10Data.commandCode);
	lcd.setCursor(10, 0);
	lcd.write(x10msg);

	p_x10StatusShown = 0;
	p_x10StatusVisible = true;
}

void getKey(RF24NetworkHeader& header, uint16_t length)
{
	network.read(header, 0, 0);
	KeyPadButtons keyPadButton = lcd.getKeypadButton();
	writeToNetwork(header.from_node, header.type, &keyPadButton, sizeof(keyPadButton));
}

void waitForKey(RF24NetworkHeader& header, uint16_t length)
{
	uint8_t data[1];
	network.read(header, &data, sizeof(data));
	unsigned long timeout = data[0] * 100;
	elapsedMillis elapsedTimer;
	KeyPadButtons keyPadButton = KeyPadButtons::None;
	while (true)
	{ 
		keyPadButton = lcd.getKeypadButton();
		if (keyPadButton != KeyPadButtons::None)
			break;
		if (elapsedTimer > timeout)
			break;
	}
	writeToNetwork(header.from_node, header.type, &keyPadButton, sizeof(KeyPadButtons));
}

void lcdClear(RF24NetworkHeader& header, uint16_t length)
{
	network.read(header, 0, 0);
	lcd.clear();
	writeToNetwork(header.from_node, header.type, &success, sizeof(bool));
}

void lcdCommand(RF24NetworkHeader& header, uint16_t length)
{
	uint8_t data[1];
	network.read(header, &data, sizeof(data));
	lcd.command(data[0]);
	writeToNetwork(header.from_node, header.type, &success, sizeof(bool));
}

void lcdCustomCharacter(RF24NetworkHeader& header, uint16_t length)
{
	uint8_t data[1];
	network.read(header, &data, sizeof(data));
	lcd.writeCustomCharacter((CustomCharacters)data[0]);
	writeToNetwork(header.from_node, header.type, &success, sizeof(bool));
}

void lcdCustomCharacterAt(RF24NetworkHeader& header, uint16_t length)
{
	uint8_t data[length];
	network.read(header, &data, length);
	lcd.setCursor(data[0], data[1]);
	
	lcd.writeCustomCharacter((CustomCharacters)data[2]);
	writeToNetwork(header.from_node, header.type, &success, sizeof(bool));
}

void lcdSetCursor(RF24NetworkHeader& header, uint16_t length)
{
	uint8_t data[2];
	network.read(header, &data, sizeof(data));
	lcd.setCursor(data[0], data[1]);
	writeToNetwork(header.from_node, header.type, &success, sizeof(bool));
}

void lcdWrite(RF24NetworkHeader& header, uint16_t length)
{
	char data[length];
	network.read(header, &data, length);
	lcd.write(data, length);
	writeToNetwork(header.from_node, header.type, &success, sizeof(bool));
}

void lcdWriteAt(RF24NetworkHeader& header, uint16_t length)
{
	uint8_t data[length];
	network.read(header, &data, length);
	lcd.setCursor(data[0], data[1]);

	char message[length - 2];
	memcpy(message, data + 2, length - 2);
	lcd.write(message, length - 2);
	writeToNetwork(header.from_node, header.type, &success, sizeof(bool));
}

bool writeToNetwork(uint16_t fromNode, unsigned char type, const void* message, uint16_t length)
{
	RF24NetworkHeader header = RF24NetworkHeader(fromNode, type);
	return network.write(header, message, length);
}

