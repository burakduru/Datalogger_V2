const int relay1pin = 16;

void setup() {
  pinMode(relay1pin, OUTPUT);
}

void loop() {
  digitalWrite(relay1pin, HIGH);
  delay(500);
  digitalWrite(relay1pin, LOW);
  delay(500);
}
