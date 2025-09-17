#include <SoftwareSerial.h>
 
#define DE 3
#define RE 2
SoftwareSerial RS485Serial(11, 10); // RX, TX
 
void setup() {
  Serial.begin(9600);
  RS485Serial.begin(9600);
 
  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
 
  Serial.println("Enter data to send or wait to receive:");
}
 
void loop() {

  if (Serial.available()) {
    String dataToSend = Serial.readStringUntil('\n');
    digitalWrite(DE, HIGH);
    digitalWrite(RE, HIGH);
    RS485Serial.println(dataToSend);
    Serial.print("Data sent: ");
    Serial.println(dataToSend);
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
  }
 
  // Check if data is available to receive over RS485
  if (RS485Serial.available()) {
    String receivedData = RS485Serial.readStringUntil('\n');
    Serial.print("Data received: ");
    Serial.println(receivedData);
    Serial.println("Data successfully received.");
  }
}