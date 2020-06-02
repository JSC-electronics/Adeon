#include <AdeonConfig.h>

AdeonConfig::AdeonConfig(Adeon *pAdeon){
    this->_pAdeon = pAdeon;
    _pAdeon->setEepromEnabled(true);
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
        if(strlen(adminPn) <= PN_LEN){
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
        if(strlen(adminPn) <= PN_LEN){
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
    if(*pMsg == configSymbol) return true;
    return false;
}

bool AdeonConfig::isMsgFromAdmin(char* pPn){
    if(strcmp(pPn, _adminPn) == 0) return true;
    return false;
}

void AdeonConfig::readConfigMsg(char* pMsg, char* userPn){
    if(strlen(pMsg) <= MSG_BUFFER_LENGTH && isConfigMsg(pMsg)){
        _msgBuf = (char*) malloc (strlen(pMsg));
        strcpy(_msgBuf, pMsg);
        parseMsg(userPn);
        free(_msgBuf);
    }
}

void AdeonConfig::parseMsg(char* userPn){
    char* tmp;
    if(isPinValid()){
        selectFunction(strchr(_msgBuf, separatorSymbol) + 1);
    }
    
    if(_function != NONE){        
        switch(_function){
            NONE:
                break;
            ADMIN_INIT:
                if(!_firstConfig){
                    setAdmin(userPn);
                    _firstConfig = !_firstConfig;
                }
                break;
            NEW_USER:
                parseNewUser(strchr(_msgBuf, endSymbol) + 1);
                break;
            DELETE_USER:
                parseDeleteUser(strchr(_msgBuf, endSymbol) + 1);
                break;
            NEW_PIN:
                parseNewPin(strchr(_msgBuf, endSymbol) + 1);
                break;
            DELETE_ALL:
                setDefaultConfig();
                break;
            default:
                break;
        }
        _function = NONE;
    }
}

void AdeonConfig::parseNewPin(char* pMsg){
    char* _parsBuf;
    if (isNumber(pMsg, PIN_LEN)) {
        _parsBuf = (char*)malloc(sizeof(char) * (PIN_LEN + 1));
        strncpy(_parsBuf, pMsg, PIN_LEN);
        setPin(_parsBuf);
        free(_parsBuf);
    }
}

void AdeonConfig::parseNewUser(char* pMsg){
    char* pn = nullptr;
    char* rights = nullptr;
    char* tmp = nullptr;
    bool separatorDetected = false;
    uint8_t i;
    tmp = strchr(pMsg, separatorSymbol) - 1;
    pn = (char*)malloc(sizeof(char)*(PN_LEN + 1));
    for (i = 0; i < PN_LEN; i++) {
        pn[i] = pMsg[i];
        if (&pMsg[i] == tmp) {
            separatorDetected = true;
            break;
        }
    }
    if (separatorDetected) {
        pn[i + 1] = '\0';
        separatorDetected = false;
    }
    else {
        pn[i] = '\0';
    }
    if(isNumber(pn, strlen(pn))){
        rights = (char*) malloc (sizeof(char) * (RIGHTS_LEN + 1));
        strncpy(rights, strchr(pMsg, separatorSymbol) + 1, RIGHTS_LEN);
        if (isNumber(rights, RIGHTS_LEN)) {
            _pAdeon->addUser(pn, atoi(rights));
        }
        free(rights);
    }
    free(pn);
    tmp = strchr(pMsg, endSymbol);
    if (tmp != nullptr) {
        tmp += 1;
        return parseNewUser(tmp);
    }
}

void AdeonConfig::parseDeleteUser(char* pMsg){
    char* pn = nullptr;
    char* tmp = nullptr;
    uint8_t i;
    bool separatorDetected = false;
    tmp = strchr(pMsg, endSymbol);
    if (tmp != NULL) {
        tmp -= 1;
    }
    else {
        tmp = &pMsg[PN_LEN - 1];
    }
    pn = (char*)malloc(sizeof(char) * (PN_LEN + 1));
    for (i = 0; i < PN_LEN; i++) {
        pn[i] = pMsg[i];
        if (&pMsg[i] == tmp) {
            separatorDetected = true;
            break;
        }
    }
    if (separatorDetected) {
        pn[i + 1] = '\0';
        separatorDetected = false;
    }
    else {
        pn[i] = '\0';
    }
    if (isNumber(pn, strlen(pn))) {
        _pAdeon->deleteUser(pn);
    }
    free(pn);
    tmp = strchr(pMsg, endSymbol);
    if (tmp != nullptr) {
        tmp += 1;
        return parseDeleteUser(tmp);
    }
}

bool isNumber(char* pStr, uint8_t n){
    for (uint8_t i = 0; i < n; i++){
        if(pStr[i] < 0x30 || pStr[i] > 0x39){
            return false;
        }
    }
    return true;
}

bool AdeonConfig::isPinValid(){
    if(strncmp(strchr(_msgBuf, configSymbol) + 1, _pin, strlen(_pin)) == 0){
        return true;
    }
    return false;
}

void AdeonConfig::selectFunction(char* pMsg){
    if(strncmp(pMsg, adminInicialization, strlen(adminInicialization)) == 0){
        _function = ADMIN_INIT;
    }
    else if(strncmp(pMsg, newUser, strlen(newUser)) == 0){
        _function = NEW_USER;
    }
    else if(strncmp(pMsg, deleteUser, strlen(deleteUser)) == 0){
        _function = DELETE_USER;
    }
    else if(strncmp(pMsg, newPin, strlen(newPin)) == 0){
        _function = NEW_PIN;
    }
    else if(strncmp(pMsg, deleteAll, strlen(deleteAll)) == 0){
        _function = DELETE_ALL;
    }
    else{
        _function = NONE;
    }
}

void AdeonConfig::setDefaultConfig(){
    _firstConfig = false;
    adeonMem.deleteDatabase();
    strcpy(_pin, defaultPin);
    memset(_adminPn, 0, PN_LEN);
    adeonMem.updatePin(_pin);
}

void AdeonConfig::readUsersFromEeprom(){
    uint8_t numOfUsers = adeonMem.getNumOfUsers();
    if(adeonMem.getNumOfUsers() > 0){
        for (uint8_t i = 0; i < numOfUsers; i++){
            _pAdeon->addUser(adeonMem.readUserRecord(i), adeonMem.readUserRights(i), true);
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
     if(strlen(adminPn) <= PN_LEN){
        strcpy(_adminPn, adminPn);
        adeonMem.updateAdmin(_adminPn);
        _pAdeon->addUser(_adminPn, ADEON_ADMIN);
     }
}

void AdeonConfig::setUserRights(const char* userPn, uint16_t rights){
    if(rights == (ADEON_ADMIN || ADEON_USER || ADEON_HOST)){
        _pAdeon->editUserRights(userPn, rights);
    }
}