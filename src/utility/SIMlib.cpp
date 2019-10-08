/**
 *  @file       SIMlib.cpp
 *  Project     AdeonGSM
 *  @brief      Adeon library for GSM modules
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

#include <GSM/SIMlib.h>

/**
 * @brief Constructor for the class GSM.
 * @param pGsmSerial is a pointer to SoftwareSerial object
 * Creat instances of parser and serial hanfler.
 */
GSM::GSM(SoftwareSerial* pGsmSerial){
    _pSerialHandler = new SerialHandler(pGsmSerial);
    _pParser = new ParserGSM(_pSerialHandler, &_newMsg, &_lastMsgIndex);
    _pTelBuffer = _pParser->getPointTelBuf();   
}

/**
 * @brief Checks for incoming SMS.
Checks serial for new incoming message. If message is detected, it starts to checks its validity.
If message is valid phone number and message text are parsed.
When the message is processed, SMS is deleted from GSM buffer.
If GSM buffer keeps more than 10 SMS, whole buffer will be deleted.
 */
void GSM::runGsm(){
    _pSerialHandler->periodicSerialCheck();
    //check if SMS is received
    if(_pSerialHandler->isRxBufferAvailable()){
        if(_pParser->identifyIncomingMsg(incomingSms)){
            if(sendCommand(_pParser->makeDynamicCmd(smsReading, _lastMsgIndex))){
                _pParser->getTelNum();
                _pParser->getMsg();
                _pMsgBuffer = _pParser->getPointMsgBuf();
                if(_lastMsgIndex > 10){
                    deleteMsgGsmStack();
                }
                else{
                    deleteMsg();
                }
            }
            else{
                Serial.println(F("ERR"));
                deleteMsgGsmStack();
            }
        }
    }
    _pSerialHandler->setRxBufferAvailability(false);
}

/**
 * @brief Returns new message availability.
 * @return _newMsg <code>true</code> if new message is available, <code>false</code> otherwise.
 */
bool GSM::isNewMsgAvailable(){
    return _newMsg;
}

/**
 * @brief Returns pointer to message buffer array.
 * @return _pMsgBuffer is a pointer to an array.
 */
char* GSM::getMsg(){
    _newMsg = false; //sms is returned, preparing logical variable for new msg
    return _pMsgBuffer;
}

/**
 * @brief Returns pointer to phone number buffer array.
 * @return _pTelBuffer is a pointer to an array.
 */
char* GSM::getMsgPhone(){
    return _pTelBuffer;
}

/**
 * @brief Sets GSM module.
 * Performs standard AT test, sets GSM mode and message in plain text
 */
void GSM::begin(){
    while(sendCommand(basicCommand) != true){
        delay(1000);
    }
    Serial.println(F("GSM IS ONLINE"));
    delay(1000);
    while(sendCommand(gsmMode) != true){
        delay(1000);
    }
    Serial.println(F("GSM IS CONFIGURED"));
    delay(1000);
    while(sendCommand(plainTextMode) != true){
        delay(1000);
    }
    Serial.println(F("MSG SET TO TEXT"));
    delay(1000);
}

/**
 * @brief Send command from parameter to serial.
 * @param cmd is an pointer to a command array.
 * @return _pParser->getResponse(confirmFeedback) <code>true</code> if GSM answers OK, <code>false</code> otherwise.
 */
bool GSM::sendCommand(const char* cmd){
    _pSerialHandler->serialWrite(cmd);
    return _pParser->getResponse(confirmFeedback);
}

/**
 * @brief Send command from deleting message from GSM buffer.
 */
void GSM::deleteMsg(){
    if(sendCommand(_pParser->makeDynamicCmd(deleteSms, _lastMsgIndex))){
        Serial.println(F("MSG DELETED"));
        _lastMsgIndex--;
    }
    else{
        Serial.println(F("DELETE ERR"));
    }
}

/**
 * @brief Send command from deleting messages from GSM buffer until it won't be empty.
 */
void GSM::deleteMsgGsmStack(){
    while(_lastMsgIndex != 0){
        deleteMsg();
    }
}

/**
 * @brief Constructor for nested class ParserGSM.
 * @param pSerialHandler is a pointer to SerialHandler object
 * @param newMsg is a pointer to logical variable
 * @param lastMsgIndex is a pointer to GSM buffer number of last message 
 */
GSM::ParserGSM::ParserGSM(GSM::SerialHandler* pSerialHandler, bool* newMsg, uint8_t* lastMsgIndex){
    _pSerialHandler = pSerialHandler;
    _pNewMsg = newMsg;
    _pLastMsgIndex = lastMsgIndex;
}

/**
 * @brief Checks reaction of GSM to AT command.
 * @param searchedChar is a pointer to string
 * @return  <code>true</code> if GSM answer is the same like searchedChar, <code>false</code> otherwise.
 */
bool GSM::ParserGSM::getResponse(const char* searchedChar){
    _pSerialHandler->feedbackSerialCheck();
    _pRxBuffer = _pSerialHandler->getRxBufferP();
    if(_pSerialHandler->isRxBufferAvailable()){
        if(strstr(_pRxBuffer, "OK") != nullptr){
            _pSerialHandler->setRxBufferAvailability(false);
            return true;
        }
        else{
            return false;
        }
    }
    return false;
}

/**
 * @brief Gets a message from a new message from the GSM output.
 */
void GSM::ParserGSM::getMsg(){
    _pRxBuffer = _pSerialHandler->getRxBufferP();
    char* tmpStr = strrchr(_pRxBuffer, '\"') + 3;
    char* endMsgPointer = strrchr(tmpStr, ';') + 1;
    uint8_t counter = 0;

    if(_msgBuffer != nullptr){
        free(_msgBuffer);
    }
    _msgBuffer = (char*)malloc(strlen(tmpStr) - strlen(endMsgPointer));
    while(&tmpStr[counter] != endMsgPointer){
        _msgBuffer[counter] = tmpStr[counter];
        counter++;
    }
    _msgBuffer[counter] = '\0';
    *_pNewMsg = true;
}

/**
 * @brief Gets a phone number from a new message from the GSM output.
 */
void GSM::ParserGSM::getTelNum(){
    _pRxBuffer = _pSerialHandler->getRxBufferP();
    char* tmpStr = strstr(_pRxBuffer, "\"+") + 2;
    char* endMsgPointer = strstr(tmpStr, "\",\"\",\"");
    uint8_t counter = 0;

    memset(_telBuffer, 0, sizeof(_telBuffer));
    while(&tmpStr[counter] != endMsgPointer){
        _telBuffer[counter] = tmpStr[counter];
        counter++;
    }
    _telBuffer[counter] = '\0';
}

/**
 * @brief Gets a pointer to message buffer
 * @return _msgBuffer is a pointer to an array.
 */
char* GSM::ParserGSM::getPointMsgBuf(){
    return _msgBuffer;
}

/**
 * @brief Gets a pointer to phone number buffer
 * * @return _telBuffer is a pointer to an array.
 */
char* GSM::ParserGSM::getPointTelBuf(){
    return _telBuffer;
}

/**
 * @brief Identifies if new message is on the GSM output
 * @param command is a pointer to searching command
 * @return  <code>true</code> if new message is on the GSM output, <code>false</code> otherwise.
 */
bool GSM::ParserGSM::identifyIncomingMsg(const char* command){
    _pRxBuffer = _pSerialHandler->getRxBufferP();
    char *tmpStr = strstr(_pRxBuffer, command);
    if(tmpStr != nullptr){
        if(strncmp(tmpStr, incomingSms, strlen(incomingSms)) == 0){
            *_pLastMsgIndex = GetIndex(tmpStr, ',');
            _pSerialHandler->setRxBufferAvailability(false);
            return true;
        }
    }
    _pSerialHandler->setRxBufferAvailability(false);
    return false;    
}

/**
 * @brief Make dynamic command (str + num)
 * @param command is a pointer to command string
 * @param id is a pointer to number which is related to the command
 * @return _cmdBuffer - pointer to _cmdBuffer.
 */
char* GSM::ParserGSM::makeDynamicCmd(const char* command, uint8_t id){
    if(_cmdBuffer != nullptr){
        free(_cmdBuffer);
    }
    _cmdBuffer = (char*)malloc(strlen(command) + 2);
    strcpy(_cmdBuffer, command);
    sprintf(&_cmdBuffer[strlen(command)], "%d", id);
    _cmdBuffer[strlen(command) + 1] = '\0';
    return _cmdBuffer;
}

/**
 * @brief Gets index of incoming message
 * @param buffer is pointer to buffer which is carring the GSM output
 * @param startSym is a start symbol character
 * @return (uint8_t)atoi(tmpStr) - convert string to uint8_t
 */
uint8_t GSM::ParserGSM::GetIndex(char* buffer, char startSym){
    char *tmpStr = strchr(buffer, startSym) + 1;
    uint8_t indexLenCount = 0;
    while(tmpStr[indexLenCount] >= 0x31 && tmpStr[indexLenCount] <= 0x39){
        indexLenCount++;
    }
    tmpStr[indexLenCount] = '\0';
    return (uint8_t)atoi(tmpStr);
}

/**
 * @brief Constructor for the nested class SerialHandler.
 * @param pGsmSerial is a pointer to SoftwareSerial object
 */
GSM::SerialHandler::SerialHandler(SoftwareSerial* pGsmSerial){
    _pGsmSerial = pGsmSerial;
}

/**
 * @brief Writes command to serial.
 * @param command is a pointer to command constant
 */
void GSM::SerialHandler::serialWrite(const char* command){
    _periodicReading = false;
    _pGsmSerial->println(command);
    delay(200);
    _periodicReading = true;
}

/**
 * @brief Method which is able to periodicaly (150 ms) read serial output.
 * Searching for incoming GSM output
 */
void GSM::SerialHandler::periodicSerialCheck(){
    if(_periodicReading){
        if(_periodicReadingFlag){
            _lastReadTime = millis();
            _periodicReadingFlag = false;
        }

        if((millis() - _lastReadTime) >= PERIODIC_READ_TIME){
            uint16_t var;
            var = _pGsmSerial->available();
            delay(100);
            if(var > 0 && _periodicReading){
                serialRead(var);
            }
            _periodicReadingFlag = true;
        }
    }
}

/**
 * @brief Checks serial for feedback of GSM module to AT commands
 */
void GSM::SerialHandler::feedbackSerialCheck(){
    unsigned long timeFlag = millis();
    while(millis() < (timeFlag + 1000)){
        uint16_t var;
        var = _pGsmSerial->available();
        delay(100);
        if(var > 0){
            serialRead(var);
            break;
        }
    }
}

/**
 * @brief Gets pointer to rx buffer
 * @return  _rxBuffer is a pointer to _rxBuffer
 */
char* GSM::SerialHandler::getRxBufferP(){
    return _rxBuffer;
}

/**
 * @brief Availability of rx buffer
 * @return  _rxBufferAvailable <code>true</code> if rx buffer is available, <code>false</code> otherwise.
 */
bool GSM::SerialHandler::isRxBufferAvailable(){
    return _rxBufferAvailable;
}

/**
 * @brief Sets rx buffer state
 * @param var is a logical state
 */
void GSM::SerialHandler::setRxBufferAvailability(bool var){
    _rxBufferAvailable = var;
}

/**
 * @brief Reads serial output.
 * @param incomingBytes is a number of incoming bytes of actual string in stack
 */
void GSM::SerialHandler::serialRead(uint8_t incomingBytes){
    if(_rxBuffer != nullptr){
        free(_rxBuffer);
    }
    _rxBuffer = (char*)malloc(incomingBytes);
    _pGsmSerial->readBytes(_rxBuffer, incomingBytes);
    _rxBuffer[incomingBytes] = '\0';
    _rxBufferAvailable = true;
}

