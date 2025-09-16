#define TX_PIN 4
#define RX_PIN 5
#define DE_RE_PIN 2  // Controls direction

void setup() {
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);  // Always in receive mode
  Serial1.setTX(TX_PIN);
  Serial1.setRX(RX_PIN);
  Serial1.begin(9600);
  Serial.begin(115200); // USB serial for debugging
}

void loop() {
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    Serial.println("Received: " + data);
  }
}
