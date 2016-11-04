# 1 "/var/folders/69/5sr1wmls207b6sz7kydfqj000000gp/T/tmpAlKa5J"
#include <Arduino.h>
# 1 "/Volumes/Macintosh HD/Users/mkloeffer/Documents/Arduino/Aquarius/src/Serial_Test.ino"
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
#define DHTTYPE DHT21
#define ledPin LED_BUILTIN
#define ONE_WIRE_BUS 5

int readingfrequency=60;
int incomingByte;
DHT dht(DHTPIN, DHTTYPE);
unsigned long i_time;
unsigned long mygroup=14360270UL;
boolean b_debug=false;
SerialCommand SCmd;
OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);



typedef struct {
  unsigned long ITgroup;
  boolean debug;
  byte myledPin;
  byte dhtPin;
  byte txPin;
  byte rxPin;
  char* configVersion;
} myConfigType;


typedef struct {
  bool LED1;
  bool IT1;
  bool IT2;
  bool IT3;
  float temp;
  float hum;
} mystateType;

mystateType mystate;
void debuglog(char s_message[]);
void setup();
void loop();
void unrecognized();
void process_getreading_command();
void process_setstate_command();
void sendCode(unsigned long i_group, byte i_unit, boolean i_on);
void showCode(NewRemoteCode receivedCode);
void adresseAusgeben(void);
void getDS18B20Temperature ();
void getDHTTemperature ();
void getDHTHumidity ();
void getTimedReading();
#line 50 "/Volumes/Macintosh HD/Users/mkloeffer/Documents/Arduino/Aquarius/src/Serial_Test.ino"
void debuglog(char s_message[]){
  if(b_debug){
    Serial.println(s_message);
  }
}

void setup() {

  Serial.begin(9600);

  SCmd.addCommand("set",process_setstate_command);
  SCmd.addCommand("get",process_getreading_command);
  SCmd.addDefaultHandler(unrecognized);



  pinMode(ledPin, OUTPUT);
  dht.begin();
  NewRemoteReceiver::init(0, 2, showCode);
  sensors.begin();





}

void loop() {
  SCmd.readSerial();
}
# 1 "/Volumes/Macintosh HD/Users/mkloeffer/Documents/Arduino/Aquarius/src/CommandLineInterface.ino"






void unrecognized()
{
  Serial.println("INFO What? You can try the following, of course without the INFO");
  Serial.println("INFO set <device> <ON:OFF>");
  Serial.println("INFO get <device>");
}

inline const char * const BoolToString(bool b)
{
  return b ? "ON" : "OFF";
}

void process_getreading_command()
{
  char *arg;
  arg = SCmd.next();

  if (arg != NULL)
  {
    if (strcmp(arg, "DHT") == 0) {
        arg = SCmd.next();
        if (arg != NULL)
        {
          if (strcmp(arg, "temp") == 0) {
            getDHTTemperature();
          }
          else if (strcmp(arg, "hum") == 0) {
            getDHTHumidity();
          } else {
            Serial.println("READING DHT state DHT_ERROR_UNKOWN_READING");
          }
        } else {
          Serial.println("READING DHT state DHT_ERROR_UNKOWN_READING");
        }
    }
    else if (strcmp(arg, "IT1") == 0) {
      Serial.print("READING IT1 state ");
      Serial.println(BoolToString(mystate.IT1));
    }
    else if (strcmp(arg, "IT2") == 0) {
      Serial.print("READING IT2 state ");
      Serial.println(BoolToString(mystate.IT2));
    }
    else if (strcmp(arg, "IT3") == 0) {
      Serial.print("READING IT3 state ");
      Serial.println(BoolToString(mystate.IT3));
    }
    else if (strcmp(arg, "LED") == 0) {
      Serial.print("READING LED state ");
      Serial.println(BoolToString(mystate.LED1));
    }
    else if (strcmp(arg, "TEMP1") == 0) {
        getDS18B20Temperature();
    }
    else {
      Serial.println("INFO Unkown Device");
    }
  }
  else {
    Serial.println("INFO No Device specified");
  }
}


void process_setstate_command()
{
  int aNumber;
  char *arg;

  Serial.println("We're in process_command");
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber=atoi(arg);
    Serial.print("First argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No arguments");
  }

  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber=atol(arg);
    Serial.print("Second argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No second argument");
  }

}
# 1 "/Volumes/Macintosh HD/Users/mkloeffer/Documents/Arduino/Aquarius/src/SensorControl.ino"
void sendCode(unsigned long i_group, byte i_unit, boolean i_on) {
# 11 "/Volumes/Macintosh HD/Users/mkloeffer/Documents/Arduino/Aquarius/src/SensorControl.ino"
  NewRemoteTransmitter transmitter(i_group, 11, 257);
  transmitter.sendUnit(i_unit, i_on);

}

void showCode(NewRemoteCode receivedCode) {



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

  Serial.print("Suche 1-Wire-Devices...\n");
  while(ourWire.search(addr)) {
    Serial.print("1-Wire-Device gefunden mit Adresse:\n");
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
      Serial.print("CRC is not valid!\n");
      return;
    }
  }
  Serial.println();
  ourWire.reset_search();
  return;
}

void getDS18B20Temperature () {

  float t = sensors.getTempCByIndex(0);
  sensors.requestTemperatures();
  Serial.print("READING TEMP1 temperature ");
  Serial.println(t);
  delay(200);
}

void getDHTTemperature () {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h))
  {
    Serial.println("READING DHT DHT_READ_ERROR");
  }
  else
  {



    Serial.print("READING DHT temperature ");
    Serial.println(t);
    delay(200);
  }
}

void getDHTHumidity () {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(t) || isnan(h))
  {
    Serial.println("READING DHT state DHT_READ_ERROR");
  }
  else
  {
    Serial.print("READING DHT humidity ");
    Serial.println(h);
    delay(200);
  }
}

void getTimedReading(){

  getDS18B20Temperature();
  getDHTTemperature();
  getDHTHumidity();
}