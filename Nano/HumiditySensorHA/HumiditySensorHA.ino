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
 * Version 1.0: Henrik EKblad
 * Version 1.1 - 2016-07-20: Converted to MySensors v2.0 and added various improvements - Torben Woltjen (mozzbozz)
 * 
 * DESCRIPTION
 * This sketch provides an example of how to implement a humidity/temperature
 * sensor using a DHT11/DHT-22.
 *  
 * For more information, please visit:
 * http://www.mysensors.org/build/humidity
 * 
 */

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached 
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69
//#define MY_RS485

#define MY_BAUD_RATE 9600

// Set this to the pin you connected the DHT's data pin to
#define DHT_DATA_PIN 16

#define BMP_SCK 5
#define BMP_MISO 4
#define BMP_MOSI 3 
#define BMP_CS 2


// Set this offset if the sensor has a permanent small offset to the real temperatures
#define SENSOR_TEMP_OFFSET 0

// Sleep time between sensor updates (in milliseconds)
// Must be >1000ms for DHT22 and >2000ms for DHT11
static const uint64_t UPDATE_INTERVAL = 200000;

// Force sending an update of the temperature after n sensor reads, so a controller showing the
// timestamp of the last update doesn't show something like 3 hours in the unlikely case, that
// the value didn't change since;
// i.e. the sensor would force sending an update every UPDATE_INTERVAL*FORCE_UPDATE_N_READS [ms]
static const uint8_t FORCE_UPDATE_N_READS = 10;

#define MY_NODE_ID 1
#define CHILD_ID_TEMP 1

float lastTemp;
float lastHum;
uint8_t nNoUpdatesTemp;
uint8_t nNoUpdatesHum;


#include <SPI.h>
#include <Wire.h>
#include <MySensors.h>  
#include <Adafruit_BMP280.h>

MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);


void presentation()  
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("Temperature", "1.1");

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_TEMP, S_TEMP);
}


void setup()
{
  if (!bmp.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  sleep(UPDATE_INTERVAL);
}


void loop()      
{
  
  float temperature = bmp.readTemperature();
  send(msgTemp.set(temperature, 1));

  #ifdef MY_DEBUG
  Serial.print("T: ");
  Serial.println(temperature);
  #endif

  sleep(UPDATE_INTERVAL);
}
