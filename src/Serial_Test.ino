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

void sendCode(unsigned long i_group, byte i_unit, boolean i_on) {

  // Disable the receiver; otherwise it might pick up the retransmit as well.
  //NewRemoteReceiver::disable();

  // Need interrupts for delay()
  //interrupts();

  // Wait 5 seconds before sending.
  //delay(5000);
  NewRemoteTransmitter transmitter(i_group, 11, 257);
  transmitter.sendUnit(i_unit, i_on);
  //NewRemoteReceiver::enable();
}
void showCode(NewRemoteCode receivedCode) {
  // Note: interrupts are disabled. You can re-enable them if needed.

  // Print the received code.
  Serial.print("Addr ");
  Serial.print(receivedCode.address);

  if (receivedCode.groupBit) {
    Serial.print(" group");
  }
  else {
    Serial.print(" unit ");
    Serial.print(receivedCode.unit);
  }

  switch (receivedCode.switchType) {
    case NewRemoteCode::off:
    Serial.print(" off");
    break;
    case NewRemoteCode::on:
    Serial.print(" on");
    break;
    case NewRemoteCode::dim:
    Serial.print(" dim");
    break;
  }

  if (receivedCode.dimLevelPresent) {
    Serial.print(", dim level: ");
    Serial.print(receivedCode.dimLevel);
  }

  Serial.print(", period: ");
  Serial.print(receivedCode.period);
  Serial.println("us.");
}

void adresseAusgeben(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  Serial.print("Suche 1-Wire-Devices...\n\r");// "\n\r" is NewLine
  while(ourWire.search(addr)) {
    Serial.print("\n\r\n\r1-Wire-Device gefunden mit Adresse:\n\r");
    for( i = 0; i < 8; i++) {
      Serial.print("0x");
      if (addr[i] < 16) {
        Serial.print('0');
      }
      Serial.print(addr[i], HEX);
      if (i < 7) {
        Serial.print(", ");
      }
    }
    if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n\r");
      return;
    }
  }
  Serial.println();
  ourWire.reset_search();
  return;
}

void getTimedReading(){

  adresseAusgeben();
}
void setup() {

  Serial.begin(9600);

  SCmd.addCommand("getstate",process_getstate_command);  // Converts two arguments to integers and echos them back
  SCmd.addCommand("setstate",process_setstate_command);  // Converts two arguments to integers and echos them back
  SCmd.addCommand("getreading",process_getreading_command);  // Converts two arguments to integers and echos them back
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
