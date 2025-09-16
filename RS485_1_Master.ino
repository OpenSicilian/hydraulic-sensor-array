#define TX_PIN 4
#define RX_PIN 5
#define DE_RE_PIN 2  // Controls direction

void setup() {
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, LOW);  // Start in receive mode
  Serial1.setTX(TX_PIN);
  Serial1.setRX(RX_PIN);
  Serial1.begin(9600);
  delay(1000);
}

void loop() {
  digitalWrite(DE_RE_PIN, HIGH);  // Enable transmit mode
  Serial1.println("Hello from Master!");
  delay(10);  // Small delay to ensure transmission
  digitalWrite(DE_RE_PIN, LOW);   // Back to receive
  delay(1000);  // Wait before next message
}
