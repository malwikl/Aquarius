#include <Arduino.h>
#include "DHT.h"
#include <NewRemoteTransmitter.h>
#include <NewRemoteReceiver.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "TimerOne.h"

#define DHTPIN 9
#define DHTTYPE DHT21 //DHT11, DHT21, DHT22
#define ledPin LED_BUILTIN
#define ONE_WIRE_BUS 5 /* Digitalport Pin 5 definieren */

int readingfrequency=60;
int incomingByte;
DHT dht(DHTPIN, DHTTYPE);
unsigned long i_time;
unsigned long mygroup=14360270UL;
boolean b_debug=false;
SerialCommand SCmd;
OneWire ourWire(ONE_WIRE_BUS); /* Ini oneWire instance */
DallasTemperature sensors(&ourWire);


//Preparation for Configuration
typedef struct {
  unsigned long ITgroup;
  boolean debug;
  byte myledPin;
  byte dhtPin;
  byte txPin;
  byte rxPin;
  char* configVersion;
} myConfigType;

//Internal State Structure for storing last state set or received
typedef struct {
  bool LED1;
  bool IT1;
  bool IT2;
  bool IT3;
  float temp;
  float hum;
} mystateType;

mystateType mystate;

void debuglog(char s_message[]){
  if(b_debug){
    Serial.println(s_message);
  }
}

void setup() {

  Serial.begin(9600);

  SCmd.addCommand("set",process_setstate_command);  // Converts two arguments to integers and echos them back
  SCmd.addCommand("get",process_getreading_command);  // Converts two arguments to integers and echos them back
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")
  //Serial.println("Ready");

  //Initialize Sensors and Actors
  pinMode(ledPin, OUTPUT);
  dht.begin();
  NewRemoteReceiver::init(0, 2, showCode);
  sensors.begin();/* Inizialisieren der Dallas Temperature library */
  //adresseAusgeben(); /* Adresse der OneWire Devices ausgeben */

  //Triggered readings
  //Timer1.initialize(readingfrequency*1000000);
  //Timer1.attachInterrupt(getTimedReading);
}

void loop() {
  SCmd.readSerial();
}
