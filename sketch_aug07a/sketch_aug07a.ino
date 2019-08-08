#include <SoftwareSerial.h>
 
SoftwareSerial espSerial(12, 13); // RX, TX
 
void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
}
 
void loop() { // run over and over
  if (espSerial.available()) {
    Serial.write(espSerial.read());
  }
  if (Serial.available()) {
    espSerial.write(Serial.read());
  }
}
