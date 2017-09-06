/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * Example sketch showing how to control physical relays.
 * This example will remember relay state after power failure.
 * http://www.mysensors.org/build/relay
 */

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Enable repeater functionality for this node
//#define MY_REPEATER_FEATURE

#define MY_BAUD_RATE 9600
#define MY_NODE_ID 2
#define CHILD_ID_TEMP 2
bool initialValueSent = false;

#include <MySensors.h>

#define RELAY_1  4  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay

MyMessage msg(CHILD_ID_TEMP, V_STATUS);

void before()
{
}

void setup()
{
    
    // Then set relay pins in output mode
    pinMode(RELAY_1, OUTPUT);
    // Set relay to last known state (using eeprom storage)
    digitalWrite(RELAY_1, RELAY_OFF);
    
    Serial.println("Sending initial value");
    send(msg.set(RELAY_OFF));
    Serial.println("Requesting initial value from controller");
    request(CHILD_ID_TEMP, V_STATUS);
    wait(2000, C_SET, V_STATUS);
}

void presentation()
{
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Relay", "1.0");

    // Register all sensors to gw (they will be created as child devices)
    present(CHILD_ID_TEMP, S_BINARY);
}


void loop()
{
}

void receive(const MyMessage &message)
{
    // We only expect one type of message from controller. But we better check anyway.
    if (message.type==V_STATUS) {
         // Change relay state
        digitalWrite(RELAY_1, message.getBool()?RELAY_ON:RELAY_OFF);
        // Store state in eeprom
        saveState(message.sensor, message.getBool());
        // Write some debug info
        Serial.print("Incoming change for sensor:");
        Serial.print(message.sensor);
        Serial.print(", New status: ");
        Serial.println(message.getBool());
        send(msg.set(message.getBool()?RELAY_ON:RELAY_OFF));
    }
}
