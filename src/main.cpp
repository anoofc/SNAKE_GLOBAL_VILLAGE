#define DEBUG 0

#define SENSOR  23
#define RELAY   22

#define FOG_DELAY     2000
#define LIGHT_DELAY   20000
#define RELAY_DELAY   2000

#define RELAY_ON        digitalWrite(RELAY,1)
#define RELAY_OFF       digitalWrite(RELAY,0)

#define SENSOR_DETECTED digitalRead(SENSOR)==LOW

#include <Arduino.h>
#include <SparkFunDMX.h>

SparkFunDMX dmx;

void checkSensor(){
  if (SENSOR_DETECTED){
    
  }
}


void setup() {
  Serial.begin(9600);
  dmx.initWrite(10);

  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(RELAY,OUTPUT);
  RELAY_OFF;

}

void loop() {

}
