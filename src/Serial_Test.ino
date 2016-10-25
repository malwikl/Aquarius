#include <Arduino.h>
#include "DHT.h"
#include <NewRemoteTransmitter.h>
#include <NewRemoteReceiver.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>

#define DHTPIN 9
#define DHTTYPE DHT21 //DHT11, DHT21, DHT22
#define ledPin LED_BUILTIN


int incomingByte;
DHT dht(DHTPIN, DHTTYPE);
unsigned long i_time;
unsigned long mygroup=14360270UL;
boolean b_debug=false;
SerialCommand SCmd;

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


void unrecognized()
{
  Serial.println("What? You can try the following");
  Serial.println("set <device> <state>");
  Serial.println("get <device> <attr>");

}


void process_get_command()
{
  char *arg;
  arg = SCmd.next();    // Get the next argument from the SerialCommand object buffer

  if (arg != NULL)      // As long as it existed, take it
  {
    if (strcmp(arg, "DHT") == 0) {
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
    if (strcmp(arg, "IT") == 0) {
      Serial.println(arg);
      arg = SCmd.next();
      Serial.println(arg);
    }
  }
  else {
    Serial.println("Hello, whoever you are");
  }
}


void process_set_command()
{
  int aNumber;
  char *arg;

  Serial.println("We're in process_command");
  arg = SCmd.next();
  if (arg != NULL)
  {
    aNumber=atoi(arg);    // Converts a char string to an integer
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


void setup() {

  Serial.begin(9600);

  SCmd.addCommand("get",process_get_command);  // Converts two arguments to integers and echos them back
  SCmd.addCommand("set",process_set_command);  // Converts two arguments to integers and echos them back
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")
  Serial.println("Ready");
  pinMode(ledPin, OUTPUT);
  dht.begin();
  NewRemoteReceiver::init(0, 2, showCode);

}

void loop() {
  SCmd.readSerial();
  /*if (Serial.available() > 0) {

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
*/
}
