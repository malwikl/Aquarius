/*
Contains handling of command line interface

*/


void unrecognized()
{
  Serial.println("What? You can try the following");
  Serial.println("set <device> <state>");
  Serial.println("get <device> <attr>");

}


void process_getreading_command()
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
        Serial.println("READING DHT DHT_READ_ERROR");
      }
      else
      {
        Serial.print("READING DHT Humidity ");
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("READING DHT Temperature ");
        Serial.println(t);
      }
    }
    else if (strcmp(arg, "IT1") == 0) {
      Serial.println(arg);
      arg = SCmd.next();
      Serial.println(arg);
    }
    else if (strcmp(arg, "IT2") == 0) {
      Serial.println(arg);
      arg = SCmd.next();
      Serial.println(arg);
    }
    else if (strcmp(arg, "IT3") == 0) {
      Serial.println(arg);
      arg = SCmd.next();
      Serial.println(arg);
    }
    else {
      Serial.println("INFO Unkown Device");
    }
  }
  else {
    Serial.println("INFO No Device specified");
  }
}

void process_getstate_command()
{

}

void process_setstate_command()
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
