#define HWSERIAL1 Serial2 //RX2 TX2 

// -------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  HWSERIAL1.begin(9600);
}

// -------------------------------------------------------------

void loop() {
    char incomingByte;
        
  if (HWSERIAL1.available() > 0) {
    incomingByte = HWSERIAL1.read();
    Serial.println(incomingByte);
  }
}
