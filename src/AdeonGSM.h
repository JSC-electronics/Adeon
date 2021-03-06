/**
 *  @file       AdeonGSM.h
 *  Project     AdeonGSM
 *  @brief      Adeon library for Arduino
 *  @author     JSC electronics
 *  License     Apache-2.0 - Copyright (c) 2019 JSC electronics
 *
 *  @section License
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

#ifndef ADEON_GSM_H
#define ADEON_GSM_H

#include <Arduino.h>
#include "utility/MD5.h"
#include "utility/list.h"

#define ADEON_ADMIN 1
#define ADEON_USER 2
#define ADEON_HOST 3

constexpr static auto SHORT_HASH_LENGTH = 5;
constexpr static auto MSG_BUFFER_LENGTH = 140; /* - Maximum number of characters in one SMS is 160
                                                  - Each character takes 7 bits of memory
                                                    (7/8 of one byte)
                                                  - Min. buffer size should be 160*7/8 = 140 bytes 
                                                */

class Adeon {
    public:
        void addUser(const char* phoneNum, uint16_t userGroup = 1);
        void deleteUser(const char* phoneNum);
        void deleteList();
        char* editUserPhone(const char* actualPhoneNum, const char* newPhoneNum);
        void editUserRights(const char* phoneNum, uint16_t userGroup = 1);
        bool isUserInAdeon(const char* phoneNum);
        uint8_t getNumOfUsers();
        uint16_t getUserRightsLevel(const char* phoneNum);
        void printUsers();

        void addParam(const char* pName, uint16_t val = 1);
        void addParamWithCallback(void (*callback)(uint16_t), const char* pName, uint16_t val = 1);
        void setParamAccess(const char* pName, uint8_t access);
        void deleteParam(const char* pName);
        char* editParamName(const char* pActualName, const char* pNewName);
        void editParamValue(const char* pName, uint16_t val = 1);
        bool isParamInAdeon(const char* pName);
        uint8_t getNumOfParams();
        uint16_t getParamValue(const char* pName);
        void printParams();

        void parseBuf(const char* pMsg, uint8_t userGroup);
        bool isAdeonReady();
    
    private:
        class Parser {
            public:
                Parser(char* pMsg);
                bool isParserReady();
                bool isMsgValid();
                bool isNameAvailable();
                void parse();

                char* getTmpName();
                uint16_t getValue();

            private:
                enum class State{
                    READY,
                    INIT,
                    PROCESSING
                };
                const char _hashEndSymbol = ':';
                const char _semicolon = ';';
                const char _equal = '=';
                const char _gap = ' ';
                const char _nullChar = '\0';

                class Hash {
                    public:
                        Hash();
                        bool isHashValid(char* msg, char* hash);

                    private:
                        char* _pTmpMsg = nullptr;
                        char _shortHash[SHORT_HASH_LENGTH + 1];

                        void makeHashFromStr(char* msg);
                        void makeShortHash(char* str);
                };

                Hash _pHash = Hash();
                State parsState = State::READY;
            
                char _tmpHash[SHORT_HASH_LENGTH + 1]; // Reserve space for \0 character
                char _subStr[LIST_ITEM_LENGTH];
                char _pTmpName[LIST_ITEM_LENGTH];
                uint16_t _tmpValue;
                char* _pMsg = nullptr;
                uint8_t _numberOfNames = 0; // get by getNumberOfParams(char* pMsg) function
                uint8_t _processedNames = 0;

                bool isHashParsingValid(); 
                uint8_t getNumberOfNames();
                char* parseName();
                void parseValue(char* pActualParam);
                char* getCharsUntilEndSym(char* pActualParam, char endSymbol);
                char* positionOfStr(char* pStr, uint8_t pos, char startSymbol);
        };

        class UserList : public ItemList{
        };

        class ParameterList : public ItemList{
            public:
                void addItemWithCallback(const char* pId, uint16_t val, void (*callback)(uint16_t));
                void setParamAccess(Item* pItem, uint8_t access);
                uint8_t getParamAccess(Item* pItem);
        };

        uint8_t getParamAccess(const char* pName); 

        char _msg[MSG_BUFFER_LENGTH];
        bool _ready = true; // indicator, that Adeon is ready to process new data

        Parser parser = Parser(_msg);
        UserList userList;
        ParameterList paramList;
};

#endif // ADEON_GSM_H