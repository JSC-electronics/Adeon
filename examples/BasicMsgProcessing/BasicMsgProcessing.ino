/**
 *  
 *  @brief This example demonstrates basic processing of the incoming SMS message in the 
 * Adeon format. The target is to set the values of defined parameters via SMS in case that
 * the SMS has been sent by authorized user (from authorized phone number).
 *  
 * For simplicity, real incoming SMS message(s) from real sender(s) is substituted with 
 * constant string(s).
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

#include "AdeonGSM.h"

// Phone numbers of senders
const char* sender1pn = "420598632485";
const char* sender2pn = "420423587465";

// Incoming messages (in Adeon format)
const char* testMsg1 = "c2c60: WaterPump = 1;OpenDoor = 2;OpenGate = 3;AirConditioning = 4;";
const char* testMsg2 = "da711: WaterPump = 10;OpenDoor = 20;OpenGate = 30;AirConditioning = 40;";
const char* testMsg3 = "75c66: WaterPump = 100;OpenDoor = 200;OpenGate = 300;AirConditioning = 400;";

/** 
 * Definition of the names of the parameters, which value can be changed via SMS (in Adeon format).
 * 
 * IMPORTANT NOTE: 
 * Parameter names of param(N)Name (param1Name, param2Name, ...) variables must be identical with the names of 
 * the parameters names sent in incoming message otherwise nothing is going to happen.
 */
const char* param1Name = "OpenGate";
const char* param2Name = "OpenDoor";
const char* param3Name = "WaterPump";
const char* param4Name = "AirConditioning";

Adeon adeon;
char msgBuf[MSG_BUFFER_LENGTH]; 
char pnBuf[LIST_ITEM_LENGTH];

// function declarations
void loadNextMsg(void);
void processMsg(void);

void setup() {
    // Setup the Serial port. See http://arduino.cc/en/Serial/IfSerial
    Serial.begin(115200);
    while (!Serial) { ; // wait for serial port to connect. Needed for Leonardo only
    }
    
    //adding user, from now this user/number is authorized to send commands from Adeon app.
    adeon.addUser(sender1pn, ADEON_ADMIN); 
    adeon.printUsers();

    /* adding parameters, from now these parameters values can be changed from 
    Adeon app by authorized user(s) */
    adeon.addParam(param1Name, 1);
    adeon.addParam(param2Name, 0);
    adeon.addParam(param3Name, 0);
    adeon.addParam(param4Name, 0);
    adeon.printParams();

    delay(5000);
}

void loop() {
    static bool activeSender1 = true;
    memset(pnBuf,0,sizeof(pnBuf));
    memset(msgBuf,0,sizeof(msgBuf));
    
    // with every loop the sender is swapped
    if(activeSender1){
        strcpy(pnBuf, sender1pn);
        activeSender1 = false;
    }
    else{
        strcpy(pnBuf, sender2pn);
        activeSender1 = true;
    }
    //every loop the message is changed
    loadNextMsg();
    //message processing    
    processMsg();
    delay(5000);
}

void loadNextMsg() {
    static uint8_t loopCounter = 0;
    switch(loopCounter){
        case 0:
        strcpy(msgBuf, testMsg1);
        break;
        case 1:
        strcpy(msgBuf, testMsg2);
        break;
        case 2:
        strcpy(msgBuf, testMsg3);
        break;
        default:
        break;
    }
    loopCounter++;
    if(loopCounter >= 3){
        loopCounter = 0;
    }
}

void processMsg() {
	Serial.println("\nPROCESSING INCOMMING MSG.");
    //simulation of incomming message 
    if(adeon.isAdeonReady()) {
        if (adeon.isUserInAdeon(pnBuf)) {
            Serial.println("PHONE NUMBER IS AUTHORIZED");
            //parameters are parsed and their values are saved into list
            adeon.parseBuf(msgBuf);
            adeon.printParams();
        }
        else {
            Serial.println("PHONE NUMBER IS NOT AUTHORIZED\n");
        }
    } 
    else Serial.println("ADEON IS NOT READY TO PROCCESS NEW MESSAGE.");
}
