#include "AdeonMem.h"

AdeonMem::AdeonMem(){

}

bool AdeonMem::isConfigAvailable(){
	for(int i = 0; i < ADMIN_RECORD_LEN; i++){
		if(EEPROM.read(IDX_ADMIN_PN + 0) == BLANK_CELL){
			deleteDatabase();
			return false;
		}
	}
	_numOfUsers = readNumOfUsers();
	return true;
}

uint8_t AdeonMem::searchUser(const char* userPn){
	uint8_t j, i;
	char tmp;
	for(i = 0; i < _numOfUsers; i++){
		for(j = 0; j < USER_RECORD_LEN; j++){
			tmp = EEPROM.read(IDX_DATA_PART + i * USER_RECORD_LEN + j);
			if(tmp != userPn[j]) break;
			if(j == USER_RECORD_LEN - 1) return IDX_DATA_PART + i * USER_RECORD_LEN;
		}
	}
	return NOT_FOUND;
}

uint8_t AdeonMem::readNumOfUsers(){
	uint8_t num = EEPROM.read(IDX_NUM_USERS);
	if(num == BLANK_CELL) return 0;
	return num;
}

char* AdeonMem::readUserRecord(uint8_t numOfUserOrder){
	if(numOfUserOrder > MAX_NUM_OF_USERS){
		uint8_t i = 0;
		uint8_t idx = IDX_DATA_PART + numOfUserOrder * USER_RECORD_LEN;
		setReturnBuffer(USER_RECORD_LEN);
		for(i = 0; i < USER_RECORD_LEN; i++){
			_returnBuffer[i] = EEPROM.read(idx + i);
		}
		_returnBuffer[i] = '\0';
		return _returnBuffer;
	}
	return nullptr;
}

char* AdeonMem::readPin(){
	uint8_t i = 0;
	setReturnBuffer(PIN_RECORD_LEN);
	for(i = 0; i < PIN_RECORD_LEN; i++){
		_returnBuffer[i] = EEPROM.read(IDX_PIN + i);
	}
	_returnBuffer[i] = '\0';
	return _returnBuffer;
}

char* AdeonMem::readAdminPn(){
	uint8_t i = 0;
	setReturnBuffer(ADMIN_RECORD_LEN);
	for(i = 0; i < ADMIN_RECORD_LEN; i++){
		_returnBuffer[i] = EEPROM.read(IDX_ADMIN_PN + i);
	}
	_returnBuffer[i] = '\0';
	return _returnBuffer;
}

void AdeonMem::updatePin(const char* pin){
	for(uint8_t i = 0; i < PIN_RECORD_LEN; i++){
		EEPROM.update(IDX_PIN + i, pin[i]);
	}
}

void AdeonMem::updateAdmin(const char* adminPn){
	for(uint8_t i = 0; i < ADMIN_RECORD_LEN; i++){
		EEPROM.update(IDX_ADMIN_PN + i, adminPn[i]);
	}
} 

void AdeonMem::updateUsers(const char* userPn){
	for(uint8_t i = 0; i < USER_RECORD_LEN; i++){
		EEPROM.update(IDX_DATA_PART + USER_RECORD_LEN * _numOfUsers + i, userPn[i]);
	}
	_numOfUsers++;
	updateNumOfUsers(_numOfUsers);
}

void AdeonMem::updateUsersRights(const char* userPn, uint8_t rights){
	uint8_t idx = searchUser(userPn);
}

void AdeonMem::deleteUser(const char* userPn){

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
	if(_returnBuffer == nullptr) free(_returnBuffer);
	_returnBuffer = (char*) malloc (size + 1);
}

