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
            adeonMem.updateAdmin(_adminPn);//přesunout funkci do Adeon addUser()
            _pAdeon->addUser(_adminPn, ADEON_ADMIN);
            _firstConfig = true;
        }
        else{
            _firstConfig = false;
        }
        adeonMem.updatePin(_pin);
    }
}

bool AdeonConfig::isFirstConfig(){
    return _firstConfig;
}

bool AdeonConfig::isConfigMsg(char* pMsg){
    if(pMsg[0] == configSymbol) return true;
    return false;
}

bool AdeonConfig::isMsgFromAdmin(char* pPn){
    if(strcmp(pPn, _adminPn) == 0) return true;
    return false;
}

void AdeonConfig::parseBuf(char* pMsg){

}

void AdeonConfig::setDefaultConfig(){
    _firstConfig = false;
    adeonMem.deleteDatabase();
    strcpy(_pin, defaultPin);
    memset(_adminPn, 0, ADMIN_PN_LEN);
    adeonMem.updatePin(_pin);
}

//move to AdeonGSM.h
void AdeonConfig::readUsersFromEeprom(){
    uint8_t numOfUsers = adeonMem.getNumOfUsers();
    if(adeonMem.getNumOfUsers() > 0){
        for (uint8_t i = 0; i < numOfUsers; i++){
            _pAdeon->addUser(adeonMem.readUserRecord(i), adeonMem.readUserRights(i));
        }
    }
}

void AdeonConfig::setPin(const char* pin){
    if(strlen(pin) == PIN_LEN){
        strcpy(_pin, pin);
        adeonMem.updatePin(_pin);
    }
}

void AdeonConfig::setAdmin(const char* adminPn){
     if(strlen(adminPn) <= ADMIN_PN_LEN){
        strcpy(_adminPn, adminPn);
        adeonMem.updateUsers(_adminPn, ADEON_ADMIN); //přesunout funkci do Adeon addUser()
        _pAdeon->addUser(_adminPn, ADEON_ADMIN);
     }
}

void AdeonConfig::setUserRights(const char* userPn, uint8_t rights){
    if(rights == (ADEON_ADMIN || ADEON_USER || ADEON_HOST)){
        adeonMem.updateUsersRights(userPn, rights); //přesunout funkci do Adeon addUser()
        _pAdeon->editUserRights(userPn, rights);
    }
}