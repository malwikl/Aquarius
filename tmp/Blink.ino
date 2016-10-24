const int LED = 13;

// Die setup Methode wird nur einmal ausgeführt und bei reset
void setup() {
  // Digitaler PIN mit der LED ist ein Ausgang
  pinMode(LED, OUTPUT);
}

// Die Endlosschleife
void loop() {
  digitalWrite(LED, HIGH); // LED anschalten
  delay(50); // 50 ms warten 
  digitalWrite(LED, LOW); // LED ausschalten
  delay(1000); // 1 Sekunden warten
}
