const int relay2_setpin = 6;
const int relay2_resetpin = 5;

void setup() {
  pinMode(relay2_setpin, OUTPUT);
  pinMode(relay2_resetpin, OUTPUT);
}

void loop() {
  digitalWrite(relay2_setpin, HIGH);
  digitalWrite(relay2_resetpin, LOW);
  delay(2000);
  digitalWrite(relay2_setpin, LOW);
  digitalWrite(relay2_resetpin, HIGH);
  delay(2000);
}
