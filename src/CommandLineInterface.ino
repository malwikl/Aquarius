/*
Contains handling of command line interface

*/


void unrecognized()
{
  Serial.println("INFO What? You can try the following, of course without the INFO");
  Serial.println("INFO setstate <device> <ON:OFF>");
  Serial.println("INFO getstate <device>");
  Serial.println("INFO getreading <device>");
}

inline const char * const BoolToString(bool b)
{
  return b ? "ON" : "OFF";
}

void process_getreading_command()
{
  char *arg;
  arg = SCmd.next();    // Get the next argument from the SerialCommand object buffer

  if (arg != NULL)      // As long as it existed, take it
  {
    if (strcmp(arg, "DHT") == 0) {
        getDHTReading();
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
        getDS18B20Reading();
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
