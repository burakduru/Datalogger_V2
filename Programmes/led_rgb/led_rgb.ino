int redpin = 29;
int greenpin = 35;
int bluepin = 30;

void setup() {
  // put your setup code here, to run once:
  pinMode(redpin, OUTPUT); 
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
}

void loop() {
  analogWrite(redpin, 30); //(pin, valeur PWM comprise entre 0 et 256) 
  delay(500);             
  analogWrite(greenpin, 200); 
  delay(500);
  analogWrite(bluepin, 40);
  delay(500);
  analogWrite(redpin, 150);
  delay(500);
  analogWrite(greenpin, 0);
  delay(500);
  analogWrite(bluepin, 250);
  delay(500);
}
