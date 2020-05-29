#include <AdeonConfig.h>

AdeonConfig::AdeonConfig(Adeon *pAdeon){
    this->_pAdeon = pAdeon;
    //set Adeon EEPROM regime
}

void AdeonConfig::begin(){
    if(adeonMem.isConfigAvailable()){
        strcpy(_pin, adeonMem.readPin());
        strcpy(_adminPn, adeonMem.readAdminPn());
        _firstConfig = true;
    }
    else{
        adeonMem.deleteDatabase();
        strcpy(_pin, defaultPin);
        adeonMem.updatePin(_pin);
        _firstConfig = false;
    }
}

void AdeonConfig::begin(const char* pin){
    if(adeonMem.isConfigAvailable()){
        if(strlen(pin) == PIN_LEN){
            strcpy(_pin, pin);
        }
        else{
            strcpy(_pin, adeonMem.readPin());
        }
        strcpy(_adminPn, adeonMem.readAdminPn());
        _firstConfig = true;
    }
    else{
        adeonMem.deleteDatabase();
        if(strlen(pin) == PIN_LEN){
            strcpy(_pin, pin);
        }
        else{
            strcpy(_pin, defaultPin);
        }
        adeonMem.updatePin(_pin);
        _firstConfig = false;
    }
}

void AdeonConfig::begin(const char* pin, const char* adminPn){
    if(adeonMem.isConfigAvailable()){
        if(strlen(pin) == PIN_LEN){
            strcpy(_pin, pin);
        }
        else{
            strcpy(_pin, adeonMem.readPin());
        }
        if(strlen(adminPn) <= ADMIN_PN_LEN){
            strcpy(_adminPn, adminPn);
        }
        else{
            strcpy(_adminPn, adeonMem.readAdminPn());
        }
        _firstConfig = true;
    }
    else{
        adeonMem.deleteDatabase();
        if(strlen(pin) == PIN_LEN){
            strcpy(_pin, pin);
        }
        else{
            strcpy(_pin, defaultPin);
        }
        if(strlen(adminPn) <= ADMIN_PN_LEN){
            strcpy(_adminPn, adminPn);
            adeonMem.updateAdmin(_adminPn);
            _firstConfig = true;
        }
        else{
            _firstConfig = false;
        }
        adeonMem.updatePin(_pin);
    }
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