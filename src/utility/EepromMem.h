#include <Arduino.h>
#include <EEPROM.h>

class EepromMem
{
private:
	byte four;
	byte three;
	byte two;
	byte one;

	long fourth;
	long third;
	long second;
	long first;

public:
	EepromMem();
	uint8_t readEEPROM(uint8_t index);
	uint16_t readEEPROM16bit(uint8_t index);
	long readEEPROM32bit(uint8_t index);

	void updateEEPROM(uint8_t index, uint8_t value);
	void updateEEPROM16bit(uint8_t index, uint16_t value);
	void updateEEPROM32bit(uint8_t index, long value);
};

static EepromMem Mem;

