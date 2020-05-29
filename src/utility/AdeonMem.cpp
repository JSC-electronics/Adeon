#include "AdeonMem.h"

AdeonMem::AdeonMem(){

}

bool AdeonMem::isConfigAvailable(){
	for(int i = 0; i < ADMIN_RECORS_LEN; i++){
		if(EEPROM.read(IDX_ADMIN_PN + 0) == BLANK_CELL){
			deleteDatabase();
			return false;
		}
	}
	return true;
}

uint8_t AdeonMem::searchUser(){
	
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
		if(_returnBuffer == nullptr) free(_returnBuffer);
		_returnBuffer = (char*) malloc (USER_RECORD_LEN + 1);
		for(i = 0; i < USER_RECORD_LEN; i++){
			_returnBuffer[i] = EEPROM.read(idx + i);
		}
		_returnBuffer[i] = '\0';
		return _returnBuffer;
	}
	return nullptr;
}

char* AdeonMem::readPin(){

}

char* AdeonMem::readAdminPn(){

}

void AdeonMem::updatePin(const char* pin){

}

void AdeonMem::updateAdmin(const char* adminPn){

} 

void AdeonMem::updateUsers(const char* userPn){

}

void AdeonMem::updateUsersRights(const char* userPn, uint8_t rights){

}

void AdeonMem::deleteUser(const char* userPn){

}

void AdeonMem::deleteDatabase(){
	for(int i = 0; i < EEPROM_LEN; i++){
		EEPROM.update(i, BLANK_CELL);
	}
}

void AdeonMem::setNumOfUsers(uint8_t numOfUsers){
	EEPROM.update(IDX_NUM_USERS, numOfUsers);
}

