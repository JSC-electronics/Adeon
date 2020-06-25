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

#define BAUD_RATE       9600

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
constexpr static auto TEL_LENGTH = 16;
constexpr static auto PERIODIC_READ_TIME = 150; //ms 

class GSM{
  private:
    class ParserGSM;
    class SerialHandler;

  public:
  #ifdef SW_SERIAL
    GSM(uint8_t rx = RX, uint8_t tx = TX, long baud = BAUD_RATE);
    GSM(SoftwareSerial* pGsmSerial);
  #else
    GSM(long baud = BAUD_RATE);
    GSM(HardwareSerial* pGsmSerial);
  #endif
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

    ParserGSM* _pParser;
    SerialHandler* _pSerialHandler;

    char* _pMsgBuffer;
    char* _pTelBuffer;
    uint8_t _lastMsgIndex = 0;
    uint8_t _pwrPin = 0;

    bool _newMsg = false; //if GSM recieve new message, it will be checked by timer

  class ParserGSM{
    public:
      ParserGSM(SerialHandler* pSerialHandler, bool* newMsg, uint8_t* lastMsgIndex);
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

      SerialHandler* _pSerialHandler;

      bool* _pNewMsg;
      char* _pRxBuffer = nullptr;
      char* _msgBuffer = nullptr;
      char* _cmdBuffer = nullptr;
      char _telBuffer[TEL_LENGTH];
      uint8_t* _pLastMsgIndex;
  };

  class SerialHandler{
    public:
  #ifdef SW_SERIAL
    SerialHandler(SoftwareSerial* pGsmSerial);
  #else
    SerialHandler(HardwareSerial* pGsmSerial);
  #endif
    void serialWrite(const char* command);
    void periodicSerialCheck(); //get num of received bytes
    void feedbackSerialCheck();
    char* getRxBufferP();
    bool isRxBufferAvailable();
    void setRxBufferAvailability(bool var);
    void serialRead(uint8_t incomingBytes);

    #ifdef SW_SERIAL
      SoftwareSerial* _pGsmSerial;
    #else
      HardwareSerial* _pGsmSerial;
    #endif

    bool _periodicReading = true; //peridical serial monitor reading, can be used in timer or loop as well
    bool _rxBufferAvailable = false;
    unsigned long _lastReadTime = 0;
    bool _periodicReadingFlag = true;

    char* _rxBuffer = nullptr;
  };
};

