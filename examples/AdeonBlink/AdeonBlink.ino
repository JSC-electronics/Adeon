/**
 *  
 *  @brief This example demonstrates switching a LED on or off by the incoming SMS message in the 
 * Adeon format. 
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

const char* incommingMsg1 = "e4917: LED = 1;"; // LED on
const char* incommingMsg2 = "dec36: LED = 0;"; // LED off

const char* paramLED = "LED";

Adeon adeon;
char msgBuf[MSG_BUFFER_LENGTH]; 

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    //adding user, from now this user/number is authorized to send commands from Adeon app
    adeon.addUser(sender1pn, ADEON_ADMIN);

    /* adding parameter, from now this parameter value can be changed from 
    Adeon app by authorized user(s) */
    adeon.addParam(paramLED, 0);

    delay(1000);
}

void loop() {
    static bool inMsg1 = true;
    memset(msgBuf,0,sizeof(msgBuf));

    // with every loop incoming message is swapped
    if(inMsg1){
        strcpy(msgBuf, incommingMsg1);
        inMsg1 = false;
    }
    else{
        strcpy(msgBuf, incommingMsg2);
        inMsg1 = true;
    }

    if(adeon.isAdeonReady()) {
        if (adeon.isUserInAdeon(sender1pn)) {
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

    if (adeon.getParamValue(paramLED) == 1) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED on.");
    } 
    else {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED off.");
    }
    delay(2000);
}
