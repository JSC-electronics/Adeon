#include "AdeonMem.h"

AdeonMem::AdeonMem(){

}

bool AdeonMem::isConfigAvailable(){
	uint8_t val = 0;
	for (int i = 0; i < ADMIN_RECORD_LEN; i++) {
		val = EEPROM.read(IDX_ADMIN_PN + i);
		if (val == BLANK_CELL || (val < 0x30 || val > 0x39)) {
			deleteDatabase();
			return false;
		}
	}
	_numOfUsers = getNumOfUsers();
	memset(_freeIndexBuffer, 0, FREE_INDEX_BUF_LEN);
	return true;
}

uint8_t AdeonMem::searchUser(const char* userPn){
	uint8_t j, i;
	char tmp;
	for (i = 0; i < _numOfUsers; i++) {
		for (j = 0; j < USER_RECORD_LEN - 1; j++) {
			tmp = EEPROM.read(IDX_DATA_PART + i * USER_RECORD_LEN + j);
			if (tmp != userPn[j]) break;
			if (j == USER_RECORD_LEN - 2) return IDX_DATA_PART + i * USER_RECORD_LEN;
		}
	}
	return NOT_FOUND;
}

uint8_t AdeonMem::getNumOfUsers(){
	uint8_t num = EEPROM.read(IDX_NUM_USERS);
	if (num == BLANK_CELL) return 0;
	return num;
}

char* AdeonMem::readUserRecord(uint8_t numOfUserOrder){
	if (numOfUserOrder < MAX_NUM_OF_USERS) {
		while (isIdxInFreeBuffer(numOfUserOrder)) {
			numOfUserOrder++;
		}
		uint8_t i = 0;
		uint8_t idx = IDX_DATA_PART + numOfUserOrder * USER_RECORD_LEN;
		setReturnBuffer(USER_RECORD_LEN - 1);
		for (i = 0; i < USER_RECORD_LEN - 1; i++) {
			_returnBuffer[i] = EEPROM.read(idx + i);
		}
		_returnBuffer[i] = '\0';
		return _returnBuffer;
	}
	return nullptr;
}

uint8_t AdeonMem::readUserRights(uint8_t numOfUserOrder){
	if (numOfUserOrder < MAX_NUM_OF_USERS) {
		while (isIdxInFreeBuffer(numOfUserOrder)) {
			numOfUserOrder++;
		}
		uint8_t idx = IDX_DATA_PART + numOfUserOrder * USER_RECORD_LEN;
		return EEPROM.read(idx + USER_RECORD_LEN - 1);
	}
	return NOT_FOUND;
}

char* AdeonMem::readPin(){
	uint8_t i = 0;
	setReturnBuffer(PIN_RECORD_LEN);
	for (i = 0; i < PIN_RECORD_LEN; i++) {
		_returnBuffer[i] = EEPROM.read(IDX_PIN + i);
	}
	_returnBuffer[i] = '\0';
	return _returnBuffer;
}

char* AdeonMem::readAdminPn(){
	uint8_t i = 0;
	setReturnBuffer(ADMIN_RECORD_LEN);
	for (i = 0; i < ADMIN_RECORD_LEN; i++) {
		_returnBuffer[i] = EEPROM.read(IDX_ADMIN_PN + i);
	}
	_returnBuffer[i] = '\0';
	return _returnBuffer;
}

void AdeonMem::updatePin(const char* pin){
	for (uint8_t i = 0; i < PIN_RECORD_LEN; i++) {
		EEPROM.update(IDX_PIN + i, pin[i]);
	}
}

void AdeonMem::updateAdmin(const char* adminPn){
	for (uint8_t i = 0; i < ADMIN_RECORD_LEN; i++) {
		EEPROM.update(IDX_ADMIN_PN + i, adminPn[i]);
	}
} 

void AdeonMem::updateUsers(const char* userPn, uint8_t rights){
	uint8_t i;
	uint8_t freeIdx = getIndexFromFreeBuffer();
	if (freeIdx > 0) {
		for (i = 0; i < USER_RECORD_LEN - 1; i++) {
			EEPROM.update(freeIdx + i, userPn[i]);
		}
		EEPROM.update(freeIdx + i, rights);
	}
	else {
		for (i = 0; i < USER_RECORD_LEN - 1; i++) {
			EEPROM.update(IDX_DATA_PART + USER_RECORD_LEN * _numOfUsers + i, userPn[i]);
		}
		EEPROM.update(IDX_DATA_PART + USER_RECORD_LEN * _numOfUsers + i, rights);
	}
	_numOfUsers++;
	updateNumOfUsers(_numOfUsers);
}

void AdeonMem::updateUsersRights(const char* userPn, uint8_t rights){
	int16_t idx = searchUser(userPn);
	if (idx >= 0) {
		EEPROM.update(idx + USER_RECORD_LEN - 1, rights);
	}
}

void AdeonMem::deleteUser(const char* userPn){
	int16_t idx = searchUser(userPn);
	if (idx >= 0) {
		for (uint8_t i = 0; i < USER_RECORD_LEN; i++) {
			EEPROM.update(idx + i, BLANK_CELL);
		}
		setIndexToFreeBuffer(idx);
		_numOfUsers--;
		updateNumOfUsers(_numOfUsers);
	}
}

void AdeonMem::deleteDatabase(){
	for(int i = 0; i < EEPROM_LEN; i++){
		EEPROM.update(i, BLANK_CELL);
	}
}

void AdeonMem::updateNumOfUsers(uint8_t numOfUsers){
	EEPROM.update(IDX_NUM_USERS, numOfUsers);
}

void AdeonMem::setReturnBuffer(uint8_t size){
	if (_returnBuffer == nullptr) free(_returnBuffer);
	_returnBuffer = (char*)malloc(sizeof(char) * (size + 1));
}

void AdeonMem::setIndexToFreeBuffer(uint8_t idx){
	for (uint8_t i = 0; i < FREE_INDEX_BUF_LEN; i++) {
		if (_freeIndexBuffer[i] == 0) {
			_freeIndexBuffer[i] = idx;
			break;
		}
	}
}

uint8_t AdeonMem::getIndexFromFreeBuffer(){
	uint8_t idx;
	for (uint8_t i = 0; i < FREE_INDEX_BUF_LEN; i++) {
		if (_freeIndexBuffer[i] != 0) {
			idx = _freeIndexBuffer[i];
			_freeIndexBuffer[i] = 0;
			return idx;
		}
	}
	return 0;
}

bool AdeonMem::isIdxInFreeBuffer(uint16_t idx) {
	idx = IDX_DATA_PART + (idx * IDX_NUM_USERS);
	for (uint8_t i = 0; i < FREE_INDEX_BUF_LEN; i++) {
		if (_freeIndexBuffer[i] == idx) {
			return true;
		}
	}
	return false;
}

