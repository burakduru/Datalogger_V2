float phase = 0.0;
float twopi = 3.14159 * 2;
elapsedMicros usec = 0;

void setup() {
  analogWriteResolution(12); //resolution 2^12 = 4096 donc analogWrite compris entre 0 et 4095
}

void loop() {
  float val = sin(phase)*2000.0 + 2050.0;
  analogWrite(A22, (int)val); //A22 correspond au pin DAC1
  phase = phase + 0.02;
  if (phase >= twopi) phase = 0;
  while (usec < 1000) ; // wait
  usec = usec - 1000;
}
