/**
 * @brief This example demonstrates advanced processing of the incoming SMS message in the 
 * Adeon format. The target is to set the values of defined parameters via SMS in case that
 * the SMS has been sent by authorized user (from authorized phone number).
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
#include <AdeonGSM.h>
#include <utility/SIMlib.h>

#define getName(var)  #var 

#define RED     6
#define GREEN   7
#define BLUE  8

#define RX      10
#define TX      11

SoftwareSerial gsmSerial = SoftwareSerial(RX, TX);
GSM* gsm;
Adeon adeon;

uint16_t counter = 0;

char pnHost[LIST_ITEM_LENGTH];
char pnUser[LIST_ITEM_LENGTH];
char pnAdmin[LIST_ITEM_LENGTH];

char parRed[LIST_ITEM_LENGTH];
char parGreen[LIST_ITEM_LENGTH];
char parBlue[LIST_ITEM_LENGTH];
char parAutoOff[LIST_ITEM_LENGTH];
char parAccess[LIST_ITEM_LENGTH];

char* msgBuf; 
char* pnBuf; 

void setStrings();
void numOfItems();
void callbackRed(uint16_t val);
void callbackGreen(uint16_t val);
void callbackBlue(uint16_t val);
void accessManagement();
void changeAccess(uint16_t val, const char* param);
void setupTimer();
void userInit();
void paramInit();
void processMsg();

void setStrings(){
	//ADD YOUR NUMBER IN HERE
    strcpy(pnAdmin, "420111111111");
    strcpy(pnUser, "420222222222");
    strcpy(pnHost, "420333333333");

    strcpy(parRed, "R");
    strcpy(parGreen, "G");
    strcpy(parBlue, "B");
    strcpy(parAutoOff, "AUTOOFF"); //AUTO OFF
    strcpy(parAccess, "ACCESS"); //
}

void numOfItems(){
  Serial.print(F("NUM OF USERS: "));
  Serial.println(adeon.getNumOfUsers()); //number of saved users

  Serial.print(F("NUM OF PARAMS: "));
  Serial.println(adeon.getNumOfParams()); //number of saved parameters
  Serial.println();
}

void callbackRed(uint16_t val){
    //callback function which is called if value of parameter is edited
    Serial.print(F("RED LED VAL: "));
    Serial.println(val);

    (val > 255) ? analogWrite(RED, 255) : analogWrite(RED, val); 
}

void callbackGreen(uint16_t val){
    //callback function which is called if value of parameter is edited
    Serial.print(F("GREEN LED VAL: "));
    Serial.println(val);

    (val > 255) ? analogWrite(GREEN, 255) : analogWrite(GREEN, val); 
}

void callbackBlue(uint16_t val){
    //callback function which is called if value of parameter is edited
    Serial.print(F("BLUE LED VAL: "));
    Serial.println(val);

    (val > 255) ? analogWrite(BLUE, 255) : analogWrite(BLUE, val); 
}

void accessManagement(uint16_t val){
    /*
    10 - RED ADMIN
    11 - RED USER
    12 - RED HOST 

    20 - GREEN ADMIN
    21 - GREEN USER
    22 - GREEN HOST 

    30 - BLUE ADMIN
    31 - BLUE USER
    32 - BLUE HOST 
    */

    uint8_t colorSelect = val / 10;

    switch(colorSelect){
        case 1:
        changeAccess(val % 10, parRed);
        break;
        case 2:
        changeAccess(val % 10, parGreen);
        break;
        case 3:
        changeAccess(val % 10, parBlue);
        break;
        default:
        Serial.println(F("UNKNOWN PARAM"));
        break;
    }
}

void changeAccess(uint16_t val, const char* param){
    Serial.print(F("PARAM "));
    Serial.print(param);
    Serial.print(F(" SET TO: "));
    switch(val){
        case 0:
        adeon.setParamAccess(param, ADEON_ADMIN);
        Serial.println(getName(ADEON_ADMIN));
        break;
        case 1:
        adeon.setParamAccess(param, ADEON_USER);
        Serial.println(getName(ADEON_USER));
        break;
        case 2:
        adeon.setParamAccess(param, ADEON_HOST);
        Serial.println(getName(ADEON_HOST));
        break;
        default:
        Serial.println(F("NO CHANGE"));
        break;
    }
}

void setupTimer1() {
  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 1 Hz (16000000/((15624+1)*1024))
  OCR1A = 15624;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

ISR(TIMER1_COMPA_vect) {
    uint16_t r = adeon.getParamValue(parRed);
    uint16_t g = adeon.getParamValue(parGreen);
    uint16_t y = adeon.getParamValue(parBlue);
    uint16_t autoOffTime = adeon.getParamValue(parAutoOff);

    if((r != 0 || g != 0 || y != 0) && autoOffTime != 0){
        if(autoOffTime > counter){
            counter++;
        }
        else{
            adeon.editParamValue(parRed, 0);
            adeon.editParamValue(parGreen, 0);
            adeon.editParamValue(parBlue, 0);
            adeon.printParams();
        }
    }
    else{
        counter = 0;
    }
}

void userInit(){
    //add users with ADMIN, USER or HOST rights
    adeon.addUser(pnAdmin, ADEON_ADMIN);
    adeon.addUser(pnUser, ADEON_USER);
    adeon.addUser(pnHost, ADEON_HOST);
    adeon.printUsers();
}

void paramInit(){
    //add parameters
    adeon.addParamWithCallback(callbackRed, parRed, 0);
    adeon.addParamWithCallback(callbackGreen, parGreen, 0);
    adeon.addParamWithCallback(callbackBlue, parBlue, 0);
    adeon.addParam(parAutoOff, 10);
    adeon.addParamWithCallback(accessManagement, parAccess, 0);
    adeon.printParams();
}

void processMsg(){
    Serial.println("\nPROCESSING INCOMMING MSG.");
    //simulation of incomming message 
    if(adeon.isAdeonReady()){
        if(adeon.isUserInAdeon(pnBuf)){
            Serial.println(F("PHONE NUMBER IS AUTHORIZED"));
            Serial.println(msgBuf);
            //parameters are parsed and their values are saved into list
            adeon.parseBuf(msgBuf, adeon.getUserRightsLevel(pnBuf));
            adeon.printParams();
        }
        else {
            Serial.println(F("PHONE NUMBER IS NOT AUTHORIZED\n"));
        }
    }
    else{
        Serial.println(F("ADEON IS NOT READY TO PROCCESS NEW MESSAGE."));
    }
}

void setup() {
    // Setup the Serial port. See http://arduino.cc/en/Serial/IfSerial
    Serial.begin(9600);
    gsmSerial.begin(9600);
    delay(200);

    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    gsm = new GSM(&gsmSerial);
    gsm->begin();

    setStrings();
    userInit();
    paramInit();
    numOfItems();  
    setupTimer1();  
}

void loop() {
    gsm->checkGsmOutput();
    if(gsm->isNewMsgAvailable()){
        pnBuf = gsm->getPhoneNum();
        msgBuf = gsm->getMsg();
        processMsg();
    }
}