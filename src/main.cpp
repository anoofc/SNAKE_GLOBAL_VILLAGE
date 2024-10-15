#define DEBUG           1

#define SENSOR          23
#define RELAY           22

#define DEBOUNCE_DELAY  500
#define START_DELAY     1000
#define FOG_DELAY       2000
#define LIGHT_DELAY     20000
#define RELAY_DELAY     2000

#define RELAY_ON              digitalWrite(RELAY,0)
#define RELAY_OFF             digitalWrite(RELAY,1)

#define SENSOR_DETECTED       digitalRead(SENSOR) == 0
#define SENSOR_NOT_DETECTED   digitalRead(SENSOR) == 1

#define FOG_ON                dmx.write(1,255)
#define FOG_OFF               dmx.write(1,0)
#define LIGHT_ON              dmx.write(4,255)
#define LIGHT_OFF             dmx.write(4,0)


#include <Arduino.h>
#include <SparkFunDMX.h>


SparkFunDMX dmx;

uint32_t startUpdateMillis    = 0;
uint32_t fogUpdateMillis      = 0;
uint32_t lightUpdateMillis    = 0;
uint32_t relayUpdateMillis    = 0;
uint32_t debounceUpdateMillis = 0;
bool startStatus  = false;
bool relayStatus  = false;
bool fogStatus    = false;
bool lightStatus  = false;
bool activated    = false;


void dmxUpdate(){
  dmx.update();
}

void checkSensor(){
  if (!activated && SENSOR_DETECTED){
    if (millis() - debounceUpdateMillis < DEBOUNCE_DELAY){ return; }
    startStatus = true;  activated = true;
    startUpdateMillis = millis();
    if (DEBUG) { Serial.println("Sensor Triggered!");}
  }
  if (activated && SENSOR_NOT_DETECTED){
    activated = false;
    if (DEBUG) { Serial.println("Sensor Not Triggered!");}
  }
}

void checkTimer(){
  if (startStatus == true){
    if (millis() - startUpdateMillis < START_DELAY) { return; }
    FOG_ON; LIGHT_ON; RELAY_ON;
    fogUpdateMillis = millis();
    lightUpdateMillis = millis();
    relayUpdateMillis = millis();
    fogStatus = true; lightStatus = true; relayStatus = true;
    startStatus = false; 
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)); }
  }
  if (fogStatus == true){
    if (millis() - fogUpdateMillis < FOG_DELAY) { return; }
    FOG_OFF;    fogStatus = false;
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)); }
  }
  if (relayStatus == true){
    if (millis() - relayUpdateMillis < RELAY_DELAY) { return; }
    RELAY_OFF;  relayStatus = false;
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)); }
  }
  if (lightStatus == true){
    if (millis() - lightUpdateMillis < LIGHT_DELAY) { return; } 
    LIGHT_OFF;  lightStatus = false;
    if (DEBUG) { Serial.println("Start: " + String(startStatus) + "\t Fog: " + String(fogStatus) + "\t Light: " + String(lightStatus) + "\t Relay: " + String(relayStatus)); }
  }
}


void init(){
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(RELAY,OUTPUT);
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
  dmxUpdate();
  checkSensor();
  checkTimer();
}
