/**
 *  
 * @brief This example demonstrates advanced processing of the incoming SMS message in the 
 * Adeon format. The target is to edit users and parameter entries and set the values of defined 
 * parameters via SMS in case that the SMS has been sent by authorized user (from authorized phone number) 
 * with sufficient user rights.
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
const char* sender1pn = "420856954231";
const char* sender2pn = "420159753456";
const char* sender3pn = "420852741963";
const char* sender4pn = "420197346843";

// Incoming messages (in Adeon format)
const char* testMsg1 = "02606: WaterPump = 123;OpenDoor = 1;OpenGate = 0;AirCondition = 200;";

/** 
 * Definition of the names of the parameters, which value can be changed via SMS (in Adeon format).
 * 
 * IMPORTANT NOTE: 
 * Parameter names of param(N)Name (param1Name, param2Name, ...) variables must be identical with the names of 
 * the parameters names sent in incoming message otherwise nothing is going to happen.
 */
const char* param1Name = "OpenGate";
const char* param2Name = "OpenDoor";
const char* param3Name = "Pump";
const char* param4Name = "AirConditioning";

Adeon adeon;

char msgBuf[MSG_BUFFER_LENGTH]; // incomming message buffer
char pnBuf[LIST_ITEM_LENGTH];   // phone number buffer

// phone numbers
char pn1[LIST_ITEM_LENGTH]; 
char pn2[LIST_ITEM_LENGTH]; 
char pn3[LIST_ITEM_LENGTH]; 
char pn4[LIST_ITEM_LENGTH]; 

//parameter names of paramN variables must be identical with parameters names in incoming message - if not nothing going to happen
char param1[LIST_ITEM_LENGTH]; 
char param2[LIST_ITEM_LENGTH]; 
char param3[LIST_ITEM_LENGTH]; 
char param4[LIST_ITEM_LENGTH]; 

// function declarations
void setDefaultValues(void);
void numOfItems(void);
void callbackFunction(uint16_t val);
void userTest(void);
void processMsg(void);

void setup() {
    // Setup the Serial port. See http://arduino.cc/en/Serial/IfSerial
    Serial.begin(9600);
    while (!Serial) { ; // wait for serial port to connect. Needed for Leonardo only
    }
    delay(200);

    // initialize built-in LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    setDefaultValues();

    userTest();
    paramTest();
    numOfItems();
    processMsg();
}

void loop() {
    // insert your code here
}

void setDefaultValues(void) {
    // set default user phone numbers
    strcpy(pn1, sender1pn);
    strcpy(pn2, sender2pn);
    strcpy(pn3, sender3pn);
    strcpy(pn4, sender4pn);

    // set defualt parameter names
    strcpy(param1, param1Name);
    strcpy(param2, param2Name);
    strcpy(param3, param3Name);
    strcpy(param4, param4Name);

    // set incomming message sender number and text
    strcpy(pnBuf, sender4pn);
    strcpy(msgBuf, testMsg1);
}

void numOfItems() {
  Serial.print("NUM OF USERS: ");
  Serial.println(adeon.getNumOfUsers()); //number of saved users

  Serial.print("NUM OF PARAMS: ");
  Serial.println(adeon.getNumOfParams()); //number of saved parameters
  Serial.println();
}

void callbackFunction(uint16_t val) {
    //callback function which is called if value of the parameter is edited/changed
    Serial.print("CALLBACK - ITEM VALUE: "); 
    Serial.print(val);
    Serial.println();

    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void userTest() {
    //temporary pointer to string - it is used when source string need to be changed
    char* tmpStr;
    //add users with ADMIN, USER or HOST rights
    adeon.addUser(pn1, ADEON_ADMIN);
    adeon.addUser(pn2, ADEON_HOST);
    adeon.addUser(pn3, ADEON_ADMIN);
    adeon.addUser(pn4, ADEON_USER);
    
    //edit user1 phone number
    tmpStr = adeon.editUserPhone(pn1, "420222555666");
    memset(pn1, 0, sizeof(pn1)); //delete array content
    strcpy(pn1, tmpStr); //copy new string to array

    //edit user3 phone number
    tmpStr = adeon.editUserPhone(pn3, "420800205305");
    memset(pn3, 0, sizeof(pn3)); //delete array content
    strcpy(pn3, tmpStr); //copy new string to array
    
    //edit user3 rights
    adeon.editUserRights(pn3, ADEON_HOST);
    
    //delete user2 
    adeon.deleteUser(pn2);

    // TEST
    Serial.print("IS USER ");
    Serial.print(pn1);
    Serial.print(" IN ADEON? ");
    adeon.isUserInAdeon(pn1) ? Serial.println("YES") : Serial.println("NO");
    
    Serial.print("IS USER ");
    Serial.print(pn2);
    Serial.print(" IN ADEON? ");
    adeon.isUserInAdeon(pn2) ? Serial.println("YES") : Serial.println("NO");

    Serial.print("IS USER ");
    Serial.print(pn3);
    Serial.print(" IN ADEON? ");
    adeon.isUserInAdeon(pn3) ? Serial.println("YES") : Serial.println("NO");

    Serial.print("GET USER RIGHTS LEVEL OF USER WITH NUBMER ");
    Serial.print(pn1);
    Serial.print(": ");
    Serial.println(adeon.getUserRightsLevel(pn1)); //get user rights group

    Serial.print("GET USER RIGHTS LEVEL OF USER WITH NUBMER ");
    Serial.print(pn2);
    Serial.print(": ");
    Serial.println(adeon.getUserRightsLevel(pn2)); //get user rights group
    
    Serial.print("GET USER RIGHTS LEVEL OF USER WITH NUBMER ");
    Serial.print(pn3);
    Serial.print(": ");
    Serial.println(adeon.getUserRightsLevel(pn3)); //get user rights group

    adeon.printUsers();
    Serial.println("USER TEST IS DONE\n");
}

void paramTest() {
    //temporary pointer to string - it is used when source string need to be changed
    char* tmpStr;
    //add parameters
    adeon.addParam(param1, 1);
    adeon.addParam(param2, 2);
    adeon.addParam(param4, 4);
    
    //add parameter with callback
    adeon.addParamWithCallback(callbackFunction, param3, 3);
    
    //edit parameter name
    tmpStr = adeon.editParamName(param3, "WaterPump");
    memset(param3, 0, sizeof(param3)); //delete array content
    strcpy(param3, tmpStr); //copy new string to array
    
    //edit parameter value
    adeon.editParamValue(param3, 100);
    
    //delete parameter 
    adeon.deleteParam(param2);

    Serial.print("IS PARAMETER ");
    Serial.print(param1);
    Serial.print(" IN ADEON? ");
    (adeon.isParamInAdeon(param1)) ? Serial.println("YES") : Serial.println("NO");

    Serial.print("IS PARAMETER ");
    Serial.print(param2);
    Serial.print(" IN ADEON? ");
    (adeon.isParamInAdeon(param2)) ? Serial.println("YES") : Serial.println("NO");
    
    Serial.print("GET PARAMETER ");
    Serial.print(param3);
    Serial.print(" VALUE: ");
    Serial.println(adeon.getParamValue(param3)); //get parameter value

    adeon.printParams();
    Serial.println("PARAMETER TEST IS DONE\n");
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
