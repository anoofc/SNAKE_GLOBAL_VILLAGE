#define DEBUG           1

#define SENSOR          23
#define RELAY           22
#define ANIMAT_RELAY    26

#define DEBOUNCE_DELAY      500
#define START_DELAY         1000
#define FOG_DELAY           3000
#define LIGHT_DELAY         20000
#define RELAY_DELAY         2000
#define SPOTLIGHT_DELAY     
#define ANIMATRONICS_DELAY  110000



#define ANIMATRONIC_ON        digitalWrite(ANIMAT_RELAY, 0)
#define ANIMATRONIC_OFF       digitalWrite(ANIMAT_RELAY, 1)

#define RELAY_ON              digitalWrite(RELAY, 0)
#define RELAY_OFF             digitalWrite(RELAY, 1)

#define SENSOR_DETECTED       digitalRead(SENSOR) == 0
#define SENSOR_NOT_DETECTED   digitalRead(SENSOR) == 1

#define FOG_ON                dmx.write(1,255)
#define FOG_OFF               dmx.write(1,0)
#define LIGHT_ON              dmx.write(4,255)
#define LIGHT_OFF             dmx.write(4,0)


#include <Arduino.h>
#include <SparkFunDMX.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;


SparkFunDMX dmx;

uint32_t startUpdateMillis    = 0;
uint32_t fogUpdateMillis      = 0;
uint32_t lightUpdateMillis    = 0;
uint32_t relayUpdateMillis    = 0;
uint32_t debounceUpdateMillis = 0;
uint32_t animatUpdateMillis   = 0;

bool animatStatus = false;
bool startStatus  = false;
bool relayStatus  = false;
bool fogStatus    = false;
bool lightStatus  = false;
bool activated    = false;


void dmxUpdate(){
  dmx.update();
}

void activateSnake(){
  if (millis() - debounceUpdateMillis < DEBOUNCE_DELAY){ return; }
  startStatus = true;  activated = true;
  startUpdateMillis = millis();
  if (DEBUG) { Serial.println("Sensor Triggered!");}
  if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)+ "\t Animat: " + String(animatStatus));}
}

void checkSensor(){
  if (!activated && SENSOR_DETECTED){ activateSnake();}
  if (activated && SENSOR_NOT_DETECTED){
    activated = false;
    if (DEBUG) { Serial.println("Sensor Not Triggered!");}
  }
}

void checkTimer(){
  if (startStatus == true){
    if (millis() - startUpdateMillis < START_DELAY) { return; }
    FOG_ON; LIGHT_ON; RELAY_ON; ANIMATRONIC_ON;
    fogUpdateMillis = millis();
    lightUpdateMillis = millis();
    relayUpdateMillis = millis();
    animatUpdateMillis = millis();
    fogStatus = true; lightStatus = true; relayStatus = true; animatStatus = true;
    startStatus = false; 
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)+ "\t Animat: " + String(animatStatus));}
  }
  if (fogStatus == true){
    if (millis() - fogUpdateMillis < FOG_DELAY) { return; }
    FOG_OFF;    fogStatus = false;
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)+ "\t Animat: " + String(animatStatus));}
  }
  if (relayStatus == true){
    if (millis() - relayUpdateMillis < RELAY_DELAY) { return; }
    RELAY_OFF;  relayStatus = false;
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)+ "\t Animat: " + String(animatStatus));}
  }
  if (lightStatus == true){
    if (millis() - lightUpdateMillis < LIGHT_DELAY) { return; } 
    LIGHT_OFF;  lightStatus = false;
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)+ "\t Animat: " + String(animatStatus));}
  }
  if (animatStatus == true) {
    if (millis() - animatUpdateMillis < ANIMATRONICS_DELAY) { return; }
    ANIMATRONIC_OFF; animatStatus = false;
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)+ "\t Animat: " + String(animatStatus));}
  }
}


void readSerial(){
  if (Serial.available()){
    char incoming = Serial.read();
    if (incoming=='A'){ activateSnake(); }
  }
}

void init(){
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(RELAY,OUTPUT);
  pinMode(ANIMAT_RELAY, OUTPUT);
  ANIMATRONIC_OFF;
  RELAY_OFF;
  FOG_OFF;
  LIGHT_OFF;
}

void setup() {
  Serial.begin(9600);
  dmx.initWrite(512);
  init();
  if(DEBUG){ Serial.println("Initializing..!!!");}
}

void loop() {
  readSerial();
  dmxUpdate();
  checkSensor();
  checkTimer();
}
