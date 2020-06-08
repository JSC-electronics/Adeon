/**
 *  @file       AdeonGSM.cpp
 *  Project     AdeonGSM
 *  @brief      Adeon library for Arduino
 *  @author     JSC electronics
 *  License     Apache-2.0 - Copyright (c) 2019 JSC electronics
 *
 *  Copyright (c) 2019 JSC electronics
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <AdeonGSM.h>

/**
 * @brief Constructor for the class Adeon.
 * Create instances of parser and user/parameter list.
 */
Adeon::Adeon(){
    parser = new Parser(_msg);
    userList = new UserList();
    paramList = new ParameterList();
}

/**
 * @brief Add user into Adeon.
 * @param pTelNum is pointer to telephone number constant string.
 * @param userGroup is variable which defines user rights.
 */
void Adeon::addUser(const char* pTelNum, uint16_t userGroup, bool disableEepromSaving){
    userList->addItem(pTelNum, userGroup);
    if(_eepromEna && !disableEepromSaving) adeonMem.updateUsers(pTelNum, (uint8_t)userGroup);
}

/**
 * @brief Delete user from Adeon.
 * @param pTelNum is pointer to telephone number constant string.
 */
void Adeon::deleteUser(const char* pTelNum){
    userList->deleteItem(userList->findItem(pTelNum));
    if(_eepromEna) adeonMem.deleteUser(pTelNum);
}

/**
 * @brief Delete whole Adeon list.
 */
void Adeon::deleteList(){
    userList->deleteHead();
}

/**
 * @brief Edit user telephone number.
 * @param pActualTelNum is pointer to actual telephone number constant string.
 * @param pNewTelNum is pointer to new telephone number constant string.
 */
char* Adeon::editUserPhone(const char* pActualTelNum, const char* pNewTelNum){
    return userList->editItemId(userList->findItem(pActualTelNum), pNewTelNum);
}

/**
 * @brief Edit user rights.
 * @param pTelNum is pointer to telephone number constant string.
 * @param userGroup is variable which defines user rights.
 */
void Adeon::editUserRights(const char* pTelNum, uint16_t userGroup){
    userList->editItemVal(userList->findItem(pTelNum), userGroup);
    if(_eepromEna)adeonMem.updateUsersRights(pTelNum, (uint8_t)userGroup);
}

/**
 * @brief Check if user is in Adeon.
 * @param pTelNum is pointer to telephone number constant string.
 * @return userList->isInList(userList->findItem(pTelNum)) <code>true</code> if user is in the list, <code>false</code> otherwise.
 */
bool Adeon::isUserInAdeon(const char* pTelNum){
    return userList->isInList(userList->findItem(pTelNum));
}

/**
 * @brief Get number of users in a list.
 * @return userList->getNumOfItems() - return value is number of users in the list.
 */
uint8_t Adeon::getNumOfUsers(){
    return userList->getNumOfItems();
}

/**
 * @brief Get user rights value.
 * @param pTelNum is pointer to telephone number constant string.
 * @return userList->getItemVal(userList->findItem(pTelNum)) - return value is user rights value.
 */
uint16_t Adeon::getUserRightsLevel(const char* pTelNum){
    return userList->getItemVal(userList->findItem(pTelNum));
}

/**
 * @brief Print content of the user list.
 * 
 * It calls method of ItemList class.
 * To carry out this metod is necessary to initialize serial terminal in setup (Serial.begin).
 */
void Adeon::printUsers(){
    userList->printData();
}

/**
 * @brief Add parameter into Adeon.
 * @param pName is pointer to name constant string.
 * @param val is variable which defines value of parameter.
 */
void Adeon::addParam(const char* pName, uint16_t val){
    paramList->addItem(pName, val);
}

/**
 * @brief Add parameter into Adeon.
 * @param pName is pointer to name constant string.
 * @param val is variable which defines value of parameter.
 * @param *callback is function callback which is triggered after changing 
 */
void Adeon::addParamWithCallback(void (*callback)(uint16_t), const char* pName, uint16_t val){
    paramList->addItemWithCallback(pName, val, callback);
}

/**
 * @brief Set parameter access rights.
 * @param pName is pointer to name constant string.
 * @param access is variable which defines user access level to parameter.
Default access rights for parameter is level ADMIN
 */
void Adeon::setParamAccess(const char* pName, uint8_t access){
    paramList->setParamAccess(paramList->findItem(pName), access);
}

/**
 * @brief Delete parameter from Adeon.
 * @param pName is pointer to name constant string.
 */
void Adeon::deleteParam(const char* pName){
    paramList->deleteItem(paramList->findItem(pName));
}

/**
 * @brief Edit parameter name.
 * @param pActualName is pointer to actual name constant string. 
 * @param pNewName is pointer to new name constant string.
 */
char* Adeon::editParamName(const char* pActualName, const char* pNewName){
    return paramList->editItemId(paramList->findItem(pActualName), pNewName);
}

/**
 * @brief Edit parameter value.
 * @param pName is pointer to name constant string.
 * @param val is variable which defines new value of parameter.
 */
void Adeon::editParamValue(const char* pName, uint16_t val){
    paramList->editItemVal(paramList->findItem(pName), val);
}

/**
 * @brief Check if parameter is in Adeon.
 * @param pName is pointer to name constant string.
 * @return paramList->isInList(paramList->findItem(pName)) <code>true</code> if parameter is in the list, <code>false</code> otherwise.
 */
bool Adeon::isParamInAdeon(const char* pName){
    return paramList->isInList(paramList->findItem(pName));
}

/**
 * @brief Get number of parameters in a list.
 * @return userList->getNumOfItems() - return value is number of parameters in the list.
 */
uint8_t Adeon::getNumOfParams(){
    return paramList->getNumOfItems();
}

/**
 * @brief Get parameter value.
 * @param pName is pointer to name constant string.
 * @return paramList->getItemVal(paramList->findItem(pName)); - return value is parameter value.
 */
uint16_t Adeon::getParamValue(const char* pName){
    return paramList->getItemVal(paramList->findItem(pName));
}

/**
 * @brief Print content of the parameter list.
 * 
 * It calls method of ItemList class.
 * To carry out this metod is necessary to initialize serial terminal in setup (Serial.begin).
 */
void Adeon::printParams(){
    paramList->printData();
}

/**
 * @brief Call parsing process of a message.
 * @param pMsg is pointer to incoming message.
 * 
 * 1. Check if message length is valid and if parser is ready.
 * 2. Set Adeon state to <code>false</code> and copy message into internal Adeon buffer.
 * 3. Check if message is valid (validity of hash and symbols order).
 * 4. Parse parameter names and values until all received data has been processed.
 * 5. Set Adeon state to <code>true</code>.
 */
void Adeon::parseBuf(char* pMsg, uint8_t userGroup){
    char* tmpName;
    if(strlen(pMsg) <= MSG_BUFFER_LENGTH && parser->isParserReady() && !paramList->isListEmpty()){
        _ready = false;
        memset(_msg, 0, sizeof(_msg));
        strcpy(_msg, pMsg);
        if(parser->isMsgValid()){
            while(parser->isNameAvailable()){
                parser->parse();
                tmpName = parser->getTmpName();
                if(isParamInAdeon(tmpName) && (getParamAccess(tmpName) >= userGroup)){
                    editParamValue(tmpName, parser->getValue());
                }
            }
        }
        _ready = true;
    }
}

/**
 * @brief Check if Adeon is ready for incoming message.
 * @return _ready <code>true</code> if Adeon is ready, <code>false</code> otherwise.
 */
bool Adeon::isAdeonReady(){
    return _ready;
}

/**
 * @brief Enable eeprom reading/writing.
 * @return _eepromEna <code>true</code> if Adeon is ready, <code>false</code> otherwise.
 */
void Adeon::setEepromEnabled(bool state){
    _eepromEna = state;
}

/**
 * @brief Get parameter access rights.
 * @param pName is pointer to name constant string.
 */
uint8_t Adeon::getParamAccess(char* pName){
    return paramList->getParamAccess(paramList->findItem(pName));
}

/**
 * @brief Constructor for nested class Parser.
 * @param pMsg is a pointer to string which carrying content of received message in class Adeon.
 * 
 * Constructor should be called only once.
 */
Adeon::Parser::Parser(char* pMsg){
    _pMsg = pMsg;
    _pHash = new Hash(pMsg, _tmpHash);
}

/**
 * @brief Check if parser is ready to process a message.
 * @return <code>true</code> if parser is ready, <code>false</code> otherwise.
 * 
 * Must be called always before isMsgValid().
 */
bool Adeon::Parser::isParserReady(){
    return parsState == State::READY;
}

/**
 * @brief Check if received message is valid.
 * @return <code>true</code> if message is valid, <code>false</code> otherwise.
 * 
 * Must be called always before isNameAvailable().
 * If message is valid, state will be changed to INIT and initialization is carried out.
 * Message is validated by checking incoming hash.
 */
bool Adeon::Parser::isMsgValid(){
    if(isHashParsingValid()){
        parsState = State::INIT;
        parse();
        return true;
    }
    return false;
}

/**
 * @brief Check if next parameter is available.
 * @return <code>true</code> if parameter is available, <code>false</code> otherwise.
 * 
 * Must be called always before parse().
 * If there is no available parameter, parser's state is set to READY (waiting for new message).
 */
bool Adeon::Parser::isNameAvailable(){
    if(_processedNames < _numberOfNames){
        return true;
    }
    parsState = State::READY;    
    return false;
}

/**
 * @brief Pars actual parameter from the message
 * 
 * Init state sets numbers of parameters in the message and sets counter of processed parameters to zero.
 * The state of parser is changed to PROCESSING.
 * Processing state parses name and value from actual parameter
 */
void Adeon::Parser::parse(){
    switch(parsState){
    case State::INIT:
        _processedNames = 0;
        _numberOfNames = getNumberOfNames();        
        parsState = State::PROCESSING;
        break;
    case State::PROCESSING:
        char* tmpStr;
        tmpStr = parseName();
        parseValue(tmpStr);
        _processedNames++;
        break;
        default:
        break;
    }
}

/**
 * @brief Get actual parsed parameter name.
 * @return _pTmpName is pointer to name string
 */
char* Adeon::Parser::getTmpName(){
    return _pTmpName;
}

/**
 * @brief Get actual parsed parameter value.
 * @return _tmpValue is value variable
 */
uint16_t Adeon::Parser::getValue(){
    return _tmpValue;
}

/**
 * @brief Pars hash from message and check its validity.
 * @return <code>true</code> if hash is valid, <code>false</code> otherwise.
 * 
 * After hash parsing is called method from class Hash which carries out if hash is valid or not.
 */
bool Adeon::Parser::isHashParsingValid(){
    char* pEndSymbol = strchr(_pMsg, _hashEndSymbol);
    uint8_t hashLen = strlen(_pMsg) - strlen(pEndSymbol);
    
    if(hashLen == SHORT_HASH_LENGTH){
        memset(_tmpHash, 0, sizeof(_tmpHash));
        for(int i = 0; i < hashLen; i++){
            _tmpHash[i] = _pMsg[i];
        }
        _tmpHash[hashLen] = _nullChar;
        return _pHash->isHashValid();
    }
    return false;
}

/**
 * @brief Get number of available parameters from the message.
 * @return Number of semicolons in message (semicolons separate parameters).
 */
uint8_t Adeon::Parser::getNumberOfNames(){
    uint8_t msgLen = strlen(_pMsg);
    uint8_t semicolonCount = 0;

    for(int i = 0; i < msgLen; i++){
        if(_pMsg[i] == _semicolon){
            semicolonCount++;
        }
    }
    return semicolonCount;
}

/**
 * @brief Pars name of actual parameter from the message.
 * @return Pointer to actual parameter in the message string.
 * 
 * If number of processed names of parameters is zero, start symbol is a gap.
 * If number of processed names od parameters is higher than zero, start symbol is a semicolon.
 */
char* Adeon::Parser::parseName(){
    char* tmp;
    if(_processedNames == 0){
        tmp = positionOfStr(_pMsg, 1, _gap);
        strcpy(_pTmpName,getCharsUntilEndSym(tmp, _gap));
    }
    else{
        tmp = positionOfStr(_pMsg, _processedNames, _semicolon);
        strcpy(_pTmpName, getCharsUntilEndSym(tmp, _gap));
    }
    return tmp;
}

/**
 * @brief Pars value of actual parameter from the message.
 * @param pActualParam is pointer to actual parameter in the message string.
 * 
 * After parsing it carries out conversion value data type from char to uint16_t.
 */
void Adeon::Parser::parseValue(char* pActualParam){
    char* tmp;
    tmp = positionOfStr(pActualParam, 1, _equal) + 1; //address plus one because of the gap
    tmp = getCharsUntilEndSym(tmp, _semicolon);
    _tmpValue = (uint16_t)atoi(tmp);
}

/**
 * @brief Pars value of actual parameter from the message.
 * @param pActualParam is pointer to actual substring in the message string.
 * @param endSymbol defines end symbol for substring in the message string.
 * @return Pointer to substring.
 * 
 * Substring is dynamic created according to number of characters.
 * After using string must be deleted.
 */
char* Adeon::Parser::getCharsUntilEndSym(char* pActualParam, char endSymbol){
    int i = 0;
    char* endPointer;
    while(pActualParam[i] != endSymbol){
        i++;
    }
    endPointer = &pActualParam[i];

    memset(_subStr, 0, sizeof(_subStr));
    i = 0;
    while(&pActualParam[i] != endPointer){
      _subStr[i] = pActualParam[i];
      i++;
    }
    _subStr[i] = _nullChar;
    return _subStr;
}

/**
 * @brief Search start position of substring in the message string.
 * @param pStr is pointer to the actual position in the message string.
 * @param pos defines how many start symbols is the string (from start), searched symbol place included.
 * @param stertSymbol defines start symbol for substring in the message string.
 * @return Pointer to substring.
 */
char* Adeon::Parser::positionOfStr(char* pStr, uint8_t pos, char startSymbol){
    uint8_t symCount = 0;
    uint8_t i = 0;
    while(symCount != pos){
        if(pStr[i] == startSymbol){
            symCount++;
        }
        i++;
    }
    return &pStr[i];
}

/**
 * @brief Constructor for the class Hash. Constructor is allowed to call only in class Parser.
 * @param pMsg is a pointer to received message without hash.
 * @param pMsgHash is a pointer to parsed hash from the message.
 * 
 * @see MD5.cpp 
 * @see MD5.h
 */
Adeon::Parser::Hash::Hash(char* pMsg, char* pMsgHash){
    _pMsg = pMsg;
    _pMsgHash = pMsgHash;
}

/**
 * @brief Evaluate if calculated hash (from pMsg) is matching with parsed hash (from pMsgHash).
 * @param hashLen is length variable of hash from message. 
 * @return <code>true</code> if hash is matching, <code>false</code> otherwise.
 */
bool Adeon::Parser::Hash::isHashValid(){
    _pTmpMsg = strchr(_pMsg, ':') + 2;
    makeHashFromStr();
    return(strcmp(_shortHash, _pMsgHash) == 0);
}

/**
 * @brief Make hash from message content.
 * @param hashLen is length variable of hash from message. 
 * 
 * Using MD5 encryption algorithm.
 */
void Adeon::Parser::Hash::makeHashFromStr(){
    unsigned char* hash = MD5::make_hash(_pTmpMsg);
    char* md5Str = MD5::make_digest(hash, 16);   
    free(hash);
    makeShortHash(md5Str);
    free(md5Str);
}

/**
 * @brief Takes last bytes from hash as many as length of the hash from recieved message.
 * @param str is pointer to hash string.
 * @param hashLen is length variable of hash from message.  
 */
void Adeon::Parser::Hash::makeShortHash(char* str){
    uint8_t strLength = strlen(str); 
    memset(_shortHash, 0, sizeof(_shortHash));
    strcpy(_shortHash, &str[strLength - SHORT_HASH_LENGTH]);
}

/**
 * @brief Empty constructor for nested class UserList.
 */
Adeon::UserList::UserList(){

}

/**
 * @brief Empty constructor for nested class ParameterList.
 */
Adeon::ParameterList::ParameterList(){

}

/**
 * @brief Add item into a list with callback.
 * @param pId is pointer to costant id string.
 * @param val is value of item.
 * @param callback is pointer to callback function
 * 
 * Call addItem method and saves pointer to callback function
 */
void Adeon::ParameterList::addItemWithCallback(const char* pId, uint16_t val, void (*callback)(uint16_t)){
    Item* pItem = addItem(pId, val);
    pItem->_pCallback = callback;
}

/**
 * @brief Set parameter access rights.
 * @param pItem is pointer to item object.
 * @param access is variable which defines user access level to parameter.
 */
void Adeon::ParameterList::setParamAccess(Item* pItem, uint8_t access){
    pItem->accessRights = access;
}

/**
 * @brief Get parameter access rights.
 * @param pItem is pointer to item object.
 */
uint8_t Adeon::ParameterList::getParamAccess(Item* pItem){
    return pItem->accessRights;
}