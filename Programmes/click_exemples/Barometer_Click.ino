/************************************************************************
*                                                                                                                                        
*    Test du module "Barometer Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1817                                                
* La température et la pression sont affichées dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Barometer Click" 
*        3. Shifter Level TXB0108
*        
*                
* Bibliothèques
*        1. https://github.com/closedcube/ClosedCube_LPS25HB_Arduino
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>                           
#include "ClosedCube_LPS25HB.h"

float temperature;
float pression;

ClosedCube_LPS25HB capteur;                     // création de l'objet capteur
void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
capteur.begin(0x5D);
}

void loop() 
{
temperature = capteur.readTemperature();        // acquisition de la valeur de la température
pression = capteur.readPressure();              // acquisition de la valeur de la pression
// Affichage
Serial.print("Temperature=");
Serial.print(temperature);
Serial.print(" °C");
Serial.print("\t");
Serial.print("Pression=");
Serial.print(pression);
Serial.println(" hPa");
delay(1000);
}
