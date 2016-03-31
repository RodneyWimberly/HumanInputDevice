
#ifndef Radio_h
#define Radio_h

#include <SPI.h>
#include "Arduino.h" 
#include "IPAddress.h"
#include "RF24_config.h"
#include "nRF24L01.h"
#include "RF24.h"

typedef enum {
	HidSetKeyRate,
	HidGetKey,
	HidLcdClear,
	HidLcdSetCursor,
	HidLcdPrint,
	HidLcdPrintLine,
	HidLcdWrite,
	HidLcdCommand
} hidTypes;

class Radio
{

public:
	typedef void(*radioCallback_t)(Radio* radio, hidTypes hidType, const void* buffer, uint8_t length);
	Radio(uint8_t cePin, uint8_t csPin, uint8_t irqPin, radioCallback_t radioCallback);
	// Public methods
	void begin(uint64_t receiveAddress, uint64_t sendAddress, bool useIrq);
	void begin(IPAddress receiveAddress, IPAddress sendAddress, bool useIrq);
	void receivePacket();
	void sendPacket(hidTypes hidType, const void* buffer, uint8_t length);
	void handleInterrupt(void);

private:
	RF24 *radio;
	byte payload[32];
	uint8_t irqPin;
	bool useIrq;
	radioCallback_t radioCallback;
};

#endif
