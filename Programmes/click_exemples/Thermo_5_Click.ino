/************************************************************************
*                                                                                                                                        
*    Test du module "Thermo 5 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2571                                                
* La température est affichée dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Thermo 5 Click" 
*        3. Shifter Level TXB0108
*        
************************************************************************/

#include <Wire.h>                             // appel de la bibliothèque
#define EMC1414_Addresse 0x4C                 // adresse I2C de l'EMC1414


// Adresses des registres
#define INT_DIODE_HIGH_BYTE 0x00
#define INT_DIODE_LOW_BYTE 0x29

byte MSB_temp;
byte LSB_temp;
int temp;
float decimal;
float temperature;

void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
}

void loop() 
{
// Acquisition de la valeur contenue dans le registre INT_DIODE_LOW_BYTE
Wire.beginTransmission(EMC1414_Addresse);      
Wire.write(INT_DIODE_LOW_BYTE);
Wire.endTransmission();
delay(10);
Wire.requestFrom(EMC1414_Addresse,1);
if(Wire.available() <=1)
  {    
    LSB_temp=Wire.read();
  } 
// Acquisition de la valeur contenue dans le registre INT_DIODE_HIGH_BYTE
Wire.beginTransmission(EMC1414_Addresse);         
Wire.write(INT_DIODE_HIGH_BYTE);
Wire.endTransmission();
delay(10);
Wire.requestFrom(EMC1414_Addresse,1);
if(Wire.available() <=1)
  {    
    MSB_temp=Wire.read();
  }
// Affichage de la valeur des registres INT_DIODE_LOW_BYTE et INT_DIODE_HIGH_BYTE
Serial.print("LSB_temp=");
Serial.print(LSB_temp);
Serial.print('\t');
Serial.print("MSB_temp=");
Serial.print(MSB_temp);
Serial.print('\t');
// Calcul de la valeur de la température
LSB_temp=LSB_temp>>5;
decimal=LSB_temp*0.125;
temperature=MSB_temp+decimal;
// Affichage de la température
Serial.print("Decimal=");
Serial.print(LSB_temp);
Serial.print('\t');
Serial.print("Temperature=");
Serial.print(temperature);
Serial.println(" degre Celcius");
delay(1000);
}
