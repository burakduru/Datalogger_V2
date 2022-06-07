/************************************************************************
*                                                                                                                                        
*    Test du module "LPS22HB Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2665                                                
* La température et la pression atmosphérique sont affichées 
* dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "LPS22HB Click" 
*        3. Shifter Level TXB0108
*        
************************************************************************/

#include <Wire.h>                             // appel de la bibliothèque
#define LPS22HB 0x5C                          // adresse I2C du LPS22HB


// Adresse des registres
#define PRESS_OUT_XL 0x28
#define PRESS_OUT_L 0x29
#define PRESS_OUT_H 0x2A
#define TEMP_OUT_L 0x2B
#define TEMP_OUT_H 0x2C
#define CTRL_REG2 0x11

long pres[2];
byte temp[1];
float temperature;
double pression;

void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
}

void loop() 
{
// Acquisition de la valeur de la température
Wire.beginTransmission(LPS22HB);                // acquisition d'une mesure     
Wire.write(CTRL_REG2);
Wire.write(0x01);
Wire.endTransmission();
Wire.beginTransmission(LPS22HB);                // acquisition de l'octet de poids faible de la température    
Wire.write(TEMP_OUT_L);
Wire.endTransmission();
Wire.requestFrom(LPS22HB,1);
temp[0]=Wire.read();                  
Wire.beginTransmission(LPS22HB);                // acquisition de l'octet de poids fort de la température    
Wire.write(TEMP_OUT_H);
Wire.endTransmission();
Wire.requestFrom(LPS22HB,1);
temp[1]=Wire.read();
// Acquisition de la valeur de la pression
Wire.beginTransmission(LPS22HB);                // acquisition d'une mesure     
Wire.write(CTRL_REG2);
Wire.write(0x21);
Wire.endTransmission();
Wire.beginTransmission(LPS22HB);                // acquisition de l'octet de poids faible de la pression    
Wire.write(PRESS_OUT_XL);
Wire.endTransmission();
Wire.requestFrom(LPS22HB,1);
pres[0]=Wire.read();                  
Wire.beginTransmission(LPS22HB);                // acquisition de l'octet central de la pression    
Wire.write(PRESS_OUT_L);
Wire.endTransmission();
Wire.requestFrom(LPS22HB,1);
pres[1]=Wire.read();
Wire.beginTransmission(LPS22HB);                // acquisition de l'octet de poids fort de la pression    
Wire.write(PRESS_OUT_H);
Wire.endTransmission();
Wire.requestFrom(LPS22HB,1);
pres[2]=Wire.read();
// Calcul de la température
temperature=(temp[1]<<8) | (temp[0]&0xff);
temperature=temperature/100.0;
// Calcul de la pression
pression=((pres[2]<<24) | (pres[1]<<16) | (pres[0]<<8))>>8;
pression=pression/4096;
// Affichage 
for (int i=0;i<2;i++)                           // affichage des octets de la température
    {
     Serial.print("temp[");
     Serial.print(i);
     Serial.print("]=");
     Serial.print(temp[i]);
     Serial.print('\t');
    }
Serial.print('\t');
Serial.print('\t');
Serial.print("Temperature=");                   // affichage de la température
Serial.print(temperature);
Serial.println(" degre Celcius");  
for (int i=0;i<3;i++)                           // affichage des octets de la pression
    {
     Serial.print("pres[");
     Serial.print(i);
     Serial.print("]=");
     Serial.print(pres[i]);
     Serial.print('\t');
    }
Serial.print("Pression=");                     // affichage de la pression
Serial.print(pression);
Serial.println(" mbar"); 
Serial.println("");   
delay(1000);
}
