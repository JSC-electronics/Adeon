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
#include <AdeonGSM.h>
#include <utility/SIMlib.h>

#define getName(var)  #var 

#define RELAY 6

Adeon adeon = Adeon();
//SoftwareSerial default setting – RX 10, TX 11, BAUD 9600 (Uno, Nano, Mini, Micro...)
//HardwareSerial default setting – Serial2, BAUD 9600 (Mega 2560...)
GSM gsm = GSM();

uint16_t counter = 0;

char pnHost[LIST_ITEM_LENGTH];
char pnUser[LIST_ITEM_LENGTH];
char pnAdmin[LIST_ITEM_LENGTH];

char parRelay[LIST_ITEM_LENGTH];
char parClose[LIST_ITEM_LENGTH];
char parAccess[LIST_ITEM_LENGTH];

char* msgBuf; 
char* pnBuf; 

void setStrings();
void numOfItems();
void callbackRel(uint16_t val);
void accessManagement(uint16_t val);
void setupTimer();
void userInit();
void paramInit();
void processMsg();

void setStrings(){
  //ADD YOUR NUMBER IN HERE
    strcpy(pnAdmin, "420111111111");
    strcpy(pnUser, "420222222222");
    strcpy(pnHost, "420333333333");

    strcpy(parRelay, "RELAY");
    strcpy(parClose, "CLOSE");
    strcpy(parAccess, "ACCESS");
}

void numOfItems(){
  Serial.print(F("NUM OF USERS: "));
  Serial.println(adeon.getNumOfUsers()); //number of saved users

  Serial.print(F("NUM OF PARAMS: "));
  Serial.println(adeon.getNumOfParams()); //number of saved parameters
  Serial.println();
}

void callbackRel(uint16_t val){
    //callback function which is called if value of parameter is edited
    Serial.print(F("REL VAL: "));
    Serial.println(val);

    (val == 0) ? digitalWrite(RELAY, HIGH) : digitalWrite(RELAY, LOW); 
}

void accessManagement(uint16_t val){
    Serial.print("PARAM SET TO ");
    if(val == 0){
        adeon.setParamAccess(parRelay, ADEON_ADMIN);
        Serial.println(getName(ADEON_ADMIN));
    }
    else if(val == 1){
        adeon.setParamAccess(parRelay, ADEON_USER);
        Serial.println(getName(ADEON_USER));
    }
    else{
        adeon.setParamAccess(parRelay, ADEON_HOST);
        Serial.println(getName(ADEON_HOST));
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
    if(adeon.getParamValue(parRelay) == 1){
        if(adeon.getParamValue(parClose) > counter){
            counter++;
        }
        else{
            adeon.editParamValue(parRelay, 0);
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
    adeon.addParamWithCallback(callbackRel, parRelay, 0);
    adeon.addParam(parClose, 5);
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
    delay(200);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAY, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY, HIGH);

    gsm.begin();

    setStrings();
    userInit();
    paramInit();
    numOfItems();  
    setupTimer1();  
}

void loop() {
    gsm.checkGsmOutput();
    if(gsm.isNewMsgAvailable()){
        pnBuf = gsm.getPhoneNum();
        msgBuf = gsm.getMsg();
        processMsg();
    }
}
