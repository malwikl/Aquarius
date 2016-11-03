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

  Serial.print("Suche 1-Wire-Devices...\n");// "\n\r" is NewLine
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

void getDS18B20Temperarture () {

  float t = sensors.getTempCByIndex(0);
  sensors.requestTemperatures(); // Temp abfragen
  Serial.print("READING TEMP1 temperature ");
  Serial.println(t);
  delay(200);
}

void getDHTTemperature () {
  float h = dht.readHumidity();     //Luftfeuchte auslesen
  float t = dht.readTemperature();  //Temperatur auslesen
  if (isnan(t) || isnan(h))
  {
    Serial.println("READING DHT DHT_READ_ERROR");
  }
  else
  {
    //Serial.print("READING DHT humidity ");
    //Serial.println(h);
    //delay(200);
    Serial.print("READING DHT temperature ");
    Serial.println(t);
    delay(200);
  }
}

void getDHTHumidity () {
  float h = dht.readHumidity();     //Luftfeuchte auslesen
  float t = dht.readTemperature();  //Temperatur auslesen
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
