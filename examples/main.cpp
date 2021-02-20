/*******************************************************************************
* AmbaSat-1 LMIC Library Test
* Filename: main.cpp
*
* Copyright (c) 2021 AmbaSat Ltd
* https://ambasat.com
*
* To use this code, set NWKSKEY, APPSKEY & DEVADDR values
* See the HowTo: https://ambasat.com/howto/kit-2/#/../unique-ids
*
* For ISM band configuration: See lmic/config.h eg. #define CFG_us915 1
* licensed under Creative Commons Attribution-ShareAlike 3.0
* ******************************************************************************/

#include <Arduino.h>
#include <LoraMessage.h>
#include <Debugging.h>
#include "AmbaSatLMIC.h"

#define SERIAL_BAUD         9600 
#define LED_PIN             9
#define SENSOR_10_TEST      10

// TTN *****************************  
// The Network Session Key
static const PROGMEM u1_t NWKSKEY[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// LoRaWAN AppSKey, application session key
static const u1_t PROGMEM APPSKEY[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x00000000 ;
/********************************/

AmbaSatLMIC *ambasatLMIC;


// ============================================================================

void setup()
{
    Serial.begin(9600);

    while (!Serial)
        delay(10);    

    // Turn the LED ON 
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);

    // Create the LMIC object
    ambasatLMIC = new AmbaSatLMIC();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    ambasatLMIC->setup(0x13, DEVADDR, appskey, nwkskey);

    // Turn the LED OFF
    digitalWrite(LED_PIN, LOW);

    PRINTLN_DEBUG("Setup complete");    
}

// ============================================================================

void loop()
{
    uint16_t testValue, len;
    LoraMessage message;

    testValue = 555;

    PRINT_DEBUG(F("Sending test value: "));
    PRINTLN_DEBUG(testValue);  

    message.addUint16(testValue);
    message.addUint8(SENSOR_10_TEST);

    len = message.getLength();

    PRINT_DEBUG(F("message length: "));
    PRINTLN_DEBUG(len);  

    // send payload 
    ambasatLMIC->sendSensorPayload(SENSOR_10_TEST, message);  

    delay(5000);

}
