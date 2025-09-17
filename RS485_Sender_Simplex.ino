#include <SoftwareSerial.h>
 
// Define the pins for the MAX485
#define DE 3
#define RE 2
 
// Create a SoftwareSerial object to communicate with the MAX485
SoftwareSerial RS485Serial(10, 11); // RX, TX
 
void setup() {
  // Initialize the serial communication
  Serial.begin(9600);
  RS485Serial.begin(9600);
 
  // Set the DE and RE pins as outputs
  pinMode(DE, OUTPUT);
  pinMode(RE, OUTPUT);
 
  // Set DE and RE high to enable transmission mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
}
 
void loop() {
  // Generate random data
  int data = random(0, 100);
 
  // Send data over RS485
  RS485Serial.write(data);
  delay(1);
  RS485Serial.flush();
 
  // Print the sent data to the serial monitor
  Serial.print("Data sent: ");
  Serial.println(data);
 
  // Wait for a while before sending the next data
  delay(2000);
}