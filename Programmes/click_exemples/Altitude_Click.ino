/************************************************************************
*                                                                                                                                        
*    Test du module "Altitude Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1489                                                
* L'altitude et la température sont affichées dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Altitude Click" 
*        3. Shifter Level TXB0108
*        
*                
* Bibliothèques
*        1. https://github.com/sparkfun/SparkFun_MPL3115A2_Breakout_Arduino_Library/archive/master.zip
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>                             
#include "SparkFunMPL3115A2.h"

MPL3115A2 capteur;                              // création de l'objet capteur

float altitude;
float temperature;
void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
capteur.begin();                                // initialisation du capteur

//Configuration du capteur
capteur.setModeAltimeter();                     // fonctionnement en mode altimètre
capteur.setOversampleRate(7);                   // taux d'échantillonage configuré à 128
capteur.enableEventFlags();                     
}

void loop() 
{
altitude = capteur.readAltitude();              // acquisition de la valeur de l'altitude
temperature = capteur.readTemp();               // acquisition de la valeur de la température
// Affichage
Serial.print("Altitude=");
Serial.print(altitude, 2);
Serial.print(" metres");
Serial.print("\t");
Serial.print("Temperature=");
Serial.print(temperature, 2);
Serial.println(" degre Celcius");
Serial.println(); 
delay(1000);
}
