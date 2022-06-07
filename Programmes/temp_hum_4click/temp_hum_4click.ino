#include <Wire.h>

#define pinSDA 38
#define pinSCL 37

// -------------------------------------------------------------

void setup()
{
  Wire1.setSDA(pinSDA);
  Wire1.setSCL(pinSCL);
  Wire1.begin();             // join i2c bus (address optional for master)
  Serial.begin(9600);       // start serial for output
  delay(10000);         
}

// -------------------------------------------------------------

void loop()
{
  Wire1.beginTransmission(0x40); //adresse I2C du composant
  Wire1.write(0x02);    //pointeur configuration
  Wire1.write(0x1000); //reset configuration
  Wire1.endTransmission();
  Wire1.requestFrom(0x40,2);
  
  Wire1.beginTransmission(0x40); //adresse I2C du composant
  Wire1.write(0x00); //pointeur Temperature
  Wire1.endTransmission();
  delay(8); //delai de 8ms
  Wire1.requestFrom(0x40,uint16_t(1)); //data Temperature
  uint16_t msb_lsb = Wire.read();
  
  float temp=((msb_lsb*256)/(pow(2,16)))*160-40; 
  
  Serial.print("Température : ");
  Serial.print(temp); //affichage de la température en °C
  Serial.print("°C   ");
  
  Wire1.beginTransmission(0x40); //adresse I2C du composant
  Wire1.write(0x01); //pointeur Humidité
  Wire1.endTransmission();
  delay(8); //délai de 8ms
  Wire1.requestFrom(0x40,2); //data Humidité
  byte msb_h = Wire.read();
  byte lsb_h = Wire.read();
  
  float hum=((msb_h*256+lsb_h)/(pow(2,16)))*100;
  
  Serial.print("Humidité : ");
  Serial.print(hum); //affichage de l'humidité en %
  Serial.print(" %");
  Serial.println();
  delay(500);
}
