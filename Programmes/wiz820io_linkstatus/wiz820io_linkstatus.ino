#include <SPI.h>
#include <Ethernet.h>

#define PWDN 26
#define RST 39
#define CS_WIZ 31

void setup() {
  pinMode(PWDN, OUTPUT); // PWDN Pin
  pinMode(RST, OUTPUT); // RESET Pin
  digitalWrite(PWDN, LOW);
  digitalWrite(RST, HIGH);
  Ethernet.init(31);  // CS WIZ820_io

  Serial.begin(9600);
}

void loop() {
  Ethernet.hardwareStatus();
  auto link = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (link) {
    case Unknown:
      Serial.println("Unknown");
      break;
    case LinkON:
      Serial.println("ON");
      break;
    case LinkOFF:
      Serial.println("OFF");
      break;
  }
  delay(1000);
}
