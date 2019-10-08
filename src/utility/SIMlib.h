/**
 *  @file       SIMlib.h
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

#include <Arduino.h>
#include <SoftwareSerial.h>

constexpr static auto RX_BUFFER = 255;
constexpr static auto MSG_LENGTH = 147;
constexpr static auto MAX_CMD_LENGTH = 20;
constexpr static auto TEL_LENGTH = 16;
constexpr static auto PERIODIC_READ_TIME = 150; //ms 

class GSM{
  private:
    class ParserGSM;
    class SerialHandler;

  public:
    GSM(SoftwareSerial* pGsmSerial);
    void begin();
    void checkGsmOutput();
    bool isNewMsgAvailable();
    char* getMsg();
    char* getPhoneNum();

  private:
    bool sendCommand(const char* cmd);
    void deleteMsg();
    void deleteMsgGsmStack();

  private:
    const char* confirmFeedback = "OK";
    const char* basicCommand = "AT";
    const char* pinCheck = "AT+CPIN?";
    const char* checkSimCard = "AT+CPIN?";
    const char* plainTextMode = "AT+CMGF=1";
    const char* gsmMode = "AT+CFUN=1";
    const char* smsReading = "AT+CMGR=";
    const char* deleteSms = "AT+CMGD=";
    const char* incomingSms = "CMTI";

    GSM::ParserGSM* _pParser;
    GSM::SerialHandler* _pSerialHandler;

    char* _pMsgBuffer;
    char* _pTelBuffer;
    uint8_t _lastMsgIndex = 0;
    uint8_t _pwrPin = 0;

    bool _newMsg = false; //if GSM recieve new message, it will be checked by timer

  class ParserGSM{
    public:
      ParserGSM(GSM::SerialHandler* pSerialHandler, bool* newMsg, uint8_t* lastMsgIndex);
      bool getResponse(const char* searchedChar);
      bool identifyIncomingMsg(const char* command);
      char* makeDynamicCmd(const char* command, uint8_t id);
      void getMsg();
      void getTelNum();
      char* getPointMsgBuf();
      char* getPointTelBuf();

    private:
      uint8_t GetIndex(char* buffer, char startSym);
      const char* incomingSms = "CMTI";

      GSM::SerialHandler* _pSerialHandler;

      bool* _pNewMsg;
      char* _pRxBuffer = nullptr;
      char* _msgBuffer = nullptr;
      char* _cmdBuffer = nullptr;
      char _telBuffer[TEL_LENGTH];
      uint8_t* _pLastMsgIndex;
  };

  class SerialHandler{
    public:
    SerialHandler(SoftwareSerial* pGsmSerial);
    void serialWrite(const char* command);
    void periodicSerialCheck(); //get num of received bytes
    void feedbackSerialCheck();
    char* getRxBufferP();
    bool isRxBufferAvailable();
    void setRxBufferAvailability(bool var);
    void serialRead(uint8_t incomingBytes);

    SoftwareSerial* _pGsmSerial;

    bool _periodicReading = true; //peridical serial monitor reading, can be used in timer or loop as well
    bool _rxBufferAvailable = false;
    unsigned long _lastReadTime = 0;
    bool _periodicReadingFlag = true;

    char* _rxBuffer = nullptr;
  };
};

