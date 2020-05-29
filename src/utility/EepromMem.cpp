#include "EepromMem.h"

EepromMem::EepromMem(){

}

uint8_t EepromMem::readEEPROM(uint8_t index) {
	return EEPROM.read(index);
}

uint16_t EepromMem::readEEPROM16bit(uint8_t index) {
	fourth = EEPROM.read(index);
	third = EEPROM.read(index + 1);

	return (uint16_t)((fourth << 0) & 0xFF) + ((third << 8) & 0xFFFF);
}

long EepromMem::readEEPROM32bit(uint8_t index) {
	fourth = EEPROM.read(index);
	third = EEPROM.read(index + 1);
	second = EEPROM.read(index + 2);
	first = EEPROM.read(index + 3);

	return ((fourth << 0) & 0xFF) + ((third << 8) & 0xFFFF) + ((second << 16) & 0xFFFFFF) + ((first << 24) & 0xFFFFFFFF);
}

void EepromMem::updateEEPROM(uint8_t index, uint8_t value) {
	EEPROM.update(index, value);
}

void EepromMem::updateEEPROM16bit(uint8_t index, uint16_t value) {
	four = (value & 0xFF);
	three = ((value >> 8) & 0xFF);

	//Write the 4 bytes into the eeprom memory.
	EEPROM.update(index, four);
	EEPROM.update(index + 1, three);
}

void EepromMem::updateEEPROM32bit(uint8_t index, long value) {
	four = (value & 0xFF);
	three = ((value >> 8) & 0xFF);
	two = ((value >> 16) & 0xFF);
	one = ((value >> 24) & 0xFF);

	EEPROM.update(index, four);
	EEPROM.update(index + 1, three);
	EEPROM.update(index + 2, two);
	EEPROM.update(index + 3, one);
}

