#include <Arduino.h>
#include "DHT.h"
#include <NewRemoteTransmitter.h>
#include <NewRemoteReceiver.h>

#define DHTPIN 9
#define DHTTYPE DHT21 //DHT11, DHT21, DHT22
#define ledPin LED_BUILTIN


int incomingByte;
DHT dht(DHTPIN, DHTTYPE);
unsigned long i_time;
unsigned long mygroup=14360270UL;
boolean b_debug=false;

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

void setup() {

  Serial.begin(9600);
  Serial.println("----------- Start Serial Monitor ---------");
  Serial.println("Type 0 or 1 to control the LED");
  Serial.println("Type q or a to control switch 0");
  Serial.println("Type w or s to control switch 0");
  Serial.println("Type e or d to control switch 0");
  Serial.println("Type t to show temperature and humidity");
  Serial.println("Type x to turn debuging on or off");
  Serial.println();
  pinMode(ledPin, OUTPUT);
  dht.begin();
  NewRemoteReceiver::init(0, 2, showCode);

}

void loop() {
  if (Serial.available() > 0) {

    incomingByte = Serial.read();

    if(b_debug) {
    Serial.print(incomingByte);
    Serial.print("        ");
    Serial.print(incomingByte, HEX);
    Serial.print("       ");
    Serial.print(char(incomingByte));
    Serial.println();
    }
    if (incomingByte == '1')
    {
      digitalWrite(ledPin, HIGH);
      debuglog("LED ist eingeschaltet!");
    }

    if (incomingByte == '0')
    {
      digitalWrite(ledPin, LOW);
      debuglog("LED ist ausgeschaltet!");
    }
    if (incomingByte == 'q')
    {
        sendCode(mygroup, 0, true);
    }
    if (incomingByte == 'a')
    {
      sendCode(mygroup, 0, false);
    }
    if (incomingByte == 'w')
    {
      sendCode(mygroup, 1, true);
    }
    if (incomingByte == 's')
    {
      sendCode(mygroup, 1, false);
    }
    if (incomingByte == 'e')
    {
      sendCode(mygroup, 2, true);
    }
    if (incomingByte == 'd')
    {
      sendCode(mygroup, 2, false);
    }
    if (incomingByte == 'x')
    {
      if (b_debug){
        b_debug = false;
      } else {
        b_debug = true;
      }
    }

    if (incomingByte == 't')
    {
      float h = dht.readHumidity();     //Luftfeuchte auslesen
      float t = dht.readTemperature();  //Temperatur auslesen
      if (isnan(t) || isnan(h))
      {
        Serial.println("DHT22 konnte nicht ausgelesen werden");
      }
      else
      {
        Serial.print("Luftfeuchte: ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperatur: ");
        Serial.print(t);
        Serial.println(" C");
      }
    }
    i_time=millis();
    if(b_debug) {
    Serial.print("Runtime: ");
    Serial.println(i_time);
  }
  }
}
