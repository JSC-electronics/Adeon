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

#ifndef ADEON_SIM_LIB_H
#define ADEON_SIM_LIB_H

#include <Arduino.h>

#define DEFAULT_BAUD_RATE       9600
#define GSM_SMS_BUFFER_LEN      70
#define GSM_SMS_PART_BUFFER_LEN      20

#if defined(__AVR_ATmega2560__)
    #define HW_SERIAL
#elif defined(__AVR_ATmega328P__)       
    #define SW_SERIAL
    #define RX          10
    #define TX          11
#elif defined(ESP8266)
    #define SW_SERIAL
    #define RX          14  //D5
    #define TX          12  //D6
    #define RX_BUF_SIZE 256       
#elif defined(ESP32)       
    #define HW_SERIAL
    #define RX          16
    #define TX          17
#else
    #error "Unsupported board"
#endif

#ifdef SW_SERIAL
  #include <SoftwareSerial.h>
#endif

constexpr static auto RX_BUFFER = 255;
constexpr static auto MSG_LENGTH = 147;
constexpr static auto MAX_CMD_LENGTH = 20;
constexpr static auto PHONE_NUMBER_LENGTH = 16;
constexpr static auto PERIODIC_READ_TIME = 150; //ms 

class GSM {
  public:

    #ifdef HW_SERIAL
    GSM(long baud = DEFAULT_BAUD_RATE);
    #endif

    #if defined(RX) && defined(TX)
    GSM(uint8_t rx = RX, uint8_t tx = TX, long baud = DEFAULT_BAUD_RATE);
    #else
    GSM(uint8_t rx, uint8_t tx, long baud = DEFAULT_BAUD_RATE);
    #endif
    GSM(Stream* pGsmSerial);
    
    void begin();
    bool checkGsmOutput();
    bool isNewMsgAvailable();
    void checkNextGsmBufferIdx();
    char* getMsg();
    char* getPhoneNum();
    void deleteSmsStack();

  private:
    class SerialHandler{
      public:
      SerialHandler(Stream* pGsmSerial);

      void serialWrite(const char* command);
      void periodicSerialCheck(); //get num of received bytes
      void feedbackSerialCheck();
      char* getRxBufferP();
      bool isRxBufferAvailable();
      void setRxBufferAvailability(bool var);
      void serialRead(uint8_t incomingBytes);

      Stream* _pGsmSerial;

      bool _periodicReading = true; //peridical serial monitor reading, can be used in timer or loop as well
      bool _rxBufferAvailable = false;
      unsigned long _lastReadTime = 0;
      bool _periodicReadingFlag = true;

      char* _rxBuffer = nullptr;
    };

    class ParserGSM{
      public:
        ParserGSM(SerialHandler* pSerialHandler, bool* newMsg, uint8_t* lastMsgIndex);
        bool getResponse(const char* searchedChar);
        bool identifyIncomingMsg(const char* command);
        char* makeDynamicCmd(const char* command, uint8_t id);
        void getMsg();
        void getPhoneNumber();
        char* getPointMsgBuf();
        char* getPointPhoneBuf();

      private:
        uint8_t GetIndex(char* buffer, char startSym);

        SerialHandler* _pSerialHandler;

        bool* _pNewMsg;
        char* _pRxBuffer = nullptr;
        char* _msgBuffer = nullptr;
        char* _cmdBuffer = nullptr;
        char _phoneBuffer[PHONE_NUMBER_LENGTH];
        uint8_t* _pLastMsgIndex;
    };

    bool sendCommand(const char* cmd);
    void deleteReadMsgs();
    void deleteMsg();
    void deleteMsgGsmStack();

    static constexpr const char* confirmFeedback = "OK";
    static constexpr const char* basicCommand = "AT";
    static constexpr const char* pinCheck = "AT+CPIN?";
    static constexpr const char* checkSimCard = "AT+CPIN?";
    static constexpr const char* plainTextMode = "AT+CMGF=1";
    static constexpr const char* gsmMode = "AT+CFUN=1";
    static constexpr const char* smsReading = "AT+CMGR=";
    static constexpr const char* deleteSms = "AT+CMGD=";
    static constexpr const char* deleteReadSms = "AT+CMGD=1,1";
    static constexpr const char* deleteSmsBuffer = "AT+CMGD=1,4";
    static constexpr const char* incomingSms = "CMTI";

    ParserGSM* _pParser;
    SerialHandler* _pSerialHandler;

    char* _pMsgBuffer;
    char* _pPhoneBuffer;
    uint8_t _lastMsgIndex = 0;
    uint8_t _pwrPin = 0;
    uint8_t _msgSearchIndexer = 1;

    bool _newMsg = false; //if GSM recieve new message, it will be checked by timer
};

#endif // ADEON_SIM_LIB_H