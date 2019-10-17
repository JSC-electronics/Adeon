/**
 * @brief This example demonstrates basic processing of the incoming SMS message in the 
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

#define RELAY 5
#define RX 10
#define TX 11

SoftwareSerial gsmSerial = SoftwareSerial(RX, TX);
GSM* gsm;
Adeon adeon;

char* msgBuf; 
char* pnBuf; 

//these variables needs to be changed according to users preferences
char sender1pn[LIST_ITEM_LENGTH]; 
char sender2pn[LIST_ITEM_LENGTH]; 

//parameter names of paramN variables must be identical with parameters names in incoming message - if not nothing going to happen
char param1Name[LIST_ITEM_LENGTH]; 
char param2Name[LIST_ITEM_LENGTH]; 

void setStrings(){
    strcpy(sender1pn, "420606771068");
    strcpy(sender2pn, "420159753456");

    strcpy(param1Name, "LED");
    strcpy(param2Name, "REL");    
}

void numOfItems(){
  Serial.print(F("NUM OF USERS: "));
  Serial.println(adeon.getNumOfUsers()); //number of saved users

  Serial.print(F("NUM OF PARAMS: "));
  Serial.println(adeon.getNumOfParams()); //number of saved parameters
  Serial.println();
}

void callbackLed(uint16_t val){
    //callback function which is called if value of parameter is edited
    Serial.print(F("LED VAL: "));
    Serial.println(val);
    Serial.println();

    (val == 0) ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH); 
}

void callbackRel(uint16_t val){
    //callback function which is called if value of parameter is edited
    Serial.print(F("REL VAL: "));
    Serial.println(val);
    Serial.println();

    (val == 0) ? digitalWrite(RELAY, HIGH) : digitalWrite(RELAY, LOW); 
}

void userInit(){
    //add users with ADMIN, USER or HOST rights
    adeon.addUser(sender1pn, ADEON_ADMIN);
    adeon.addUser(sender2pn, ADEON_HOST);
    adeon.printUsers();
}

void paramInit(){
    //add parameters
    adeon.addParamWithCallback(callbackLed, param1Name, 0);
    adeon.addParamWithCallback(callbackRel, param2Name, 0);
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

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAY, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY, HIGH);

    gsm = new GSM(&gsmSerial);
    gsm->begin();

    setStrings();
    userInit();
    paramInit();
    numOfItems();    
}

void loop() {
    gsm->checkGsmOutput();
    if(gsm->isNewMsgAvailable()){
        pnBuf = gsm->getPhoneNum();
        msgBuf = gsm->getMsg();
        processMsg();
    }
}
