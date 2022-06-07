#define HWSERIAL Serial2 //RX2 TX2 

// -------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(9600);
}

// -------------------------------------------------------------

void loop() {
    char incomingByte;
        
  if (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.println(incomingByte);
  }
}
