// RS485 Master (Pico + Pico-2CH-RS485, channel 0)
// Serial = USB (debug), Serial1 = UART0 -> GP0/TX, GP1/RX -> RS485 channel 0
#include <Arduino.h>

#define RS485 Serial1
const unsigned long BAUD = 115200;
const uint8_t ADDR_MASTER = 1;
const uint8_t ADDR_SLAVE  = 2;

// helper: compute simple XOR checksum
uint8_t checksum(const uint8_t *buf, size_t len){
  uint8_t cs = 0;
  for(size_t i=0;i<len;i++) cs ^= buf[i];
  return cs;
}

// build+send frame: STX, ADDR, CMD, LEN, PAYLOAD..., CHK, ETX
void sendFrame(uint8_t addr, uint8_t cmd, const uint8_t *payload, uint8_t len){
  uint8_t hdr[4] = {0x02, addr, cmd, len};
  RS485.write(hdr, 4);
  if(len) RS485.write(payload, len);

  // checksum over addr..payload
  uint8_t cs = 0;
  cs ^= addr; cs ^= cmd; cs ^= len;
  for(uint8_t i=0;i<len;i++) cs ^= payload[i];
  RS485.write(cs);
  RS485.write((uint8_t)0x03);
  RS485.flush(); // wait for peripheral buffer to be sent

  // safe turnaround: compute approximate time to send bytes and add margin
  unsigned long bytesTotal = 4 + len + 1 + 1; // hdr + payload + chk + ETX
  unsigned long bitTime_us = (1000000UL * 10UL) / BAUD; // 10 bit times per byte (start+8+stop)
  unsigned long txTime_us = bytesTotal * bitTime_us;
  delayMicroseconds(txTime_us + 2000UL); // +2ms margin
}

// blocking read with timeout; returns true if valid frame was read
bool readFrame(uint8_t *outAddr, uint8_t *outCmd, uint8_t *outPayload, uint8_t *outLen, unsigned long timeout_ms){
  unsigned long start = millis();
  bool seenSTX = false;
  while(millis() - start < timeout_ms){
    if(RS485.available()){
      int b = RS485.read();
      if(!seenSTX){
        if(b == 0x02) seenSTX = true;
        else continue;
      } else {
        *outAddr = (uint8_t)b;
        while(!RS485.available() && (millis()-start)<timeout_ms);
        *outCmd = RS485.read();
        while(!RS485.available() && (millis()-start)<timeout_ms);
        *outLen = RS485.read();
        for(uint8_t i=0;i<*outLen;i++){
          while(!RS485.available() && (millis()-start)<timeout_ms);
          outPayload[i] = RS485.read();
        }
        while(!RS485.available() && (millis()-start)<timeout_ms);
        uint8_t cs = RS485.read();
        while(!RS485.available() && (millis()-start)<timeout_ms);
        uint8_t etx = RS485.read();
        if(etx != 0x03) return false;
        uint8_t cs_calc = 0;
        cs_calc ^= *outAddr; cs_calc ^= *outCmd; cs_calc ^= *outLen;
        for(uint8_t i=0;i<*outLen;i++) cs_calc ^= outPayload[i];
        return (cs_calc == cs);
      }
    }
  }
  return false;
}

void setup(){
  Serial.begin(115200);
  RS485.begin(BAUD);
  Serial.println("RS485 Master ready");
}

void loop(){
  const char req[] = "REQ";
  sendFrame(ADDR_SLAVE, 0x10, (const uint8_t*)req, sizeof(req)-1);
  Serial.println("Sent request, waiting for reply...");

  uint8_t addr, cmd, len;
  uint8_t payload[64];
  if(readFrame(&addr, &cmd, payload, &len, 1500UL)){
    Serial.print("Reply from ");
    Serial.print(addr);
    Serial.print(", cmd=0x"); Serial.print(cmd, HEX);
    Serial.print(", data: ");
    for(uint8_t i=0;i<len;i++) Serial.write(payload[i]);
    Serial.println();
  } else {
    Serial.println("No reply or invalid frame");
  }

  delay(1000);
}
