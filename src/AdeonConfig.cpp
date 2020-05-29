#include <AdeonConfig.h>

AdeonConfig::AdeonConfig(Adeon *_pAdeon){
    //set Adeon EEPROM regime
}

AdeonConfig::AdeonConfig(Adeon *_pAdeon, const char* pin){

}

AdeonConfig::AdeonConfig(Adeon *_pAdeon, const char* pin, const char* adminPn){

}

void AdeonConfig::begin(){

}

bool AdeonConfig::isFirstConfig(){

}

bool AdeonConfig::isConfigMsg(){

}

bool AdeonConfig::isMsgFromAdmin(){
    
}

void AdeonConfig::waitForConfig(){

}

void AdeonConfig::parseBuf(char* pMsg){

}

void AdeonConfig::setDefaultConfig(){

}

bool AdeonConfig::isConfigEmpty(){

}

void AdeonConfig::readConfigFromEeprom(){

}

void AdeonConfig::setPin(){

}

void AdeonConfig::setAdmin(){

}

void AdeonConfig::setNewUser(){

}

void AdeonConfig::deleteUser(){

}

void AdeonConfig::setUserRights(){

}