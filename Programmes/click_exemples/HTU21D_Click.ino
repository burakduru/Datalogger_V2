/************************************************************************
*                                                                                                                                        
*    Test du module "HTU21D Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1687                                                
* La température et l'humidité sont affichées dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "HTU21D Click" 
*        3. Shifter Level TXB0108      
*        
*                
* Bibliothèques
*        1. https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library/archive/master.zip
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "SparkFunHTU21D.h"

HTU21D capteur;                                   // création de l'objet capteur

float temp;
float humidite;

void setup(void) 
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation du bus I2C
capteur.begin();                                  // initialisation du capteur                    
}

void loop() 
{
temp=capteur.readTemperature();                   // acquisition de la valeur de la température 
humidite=capteur.readHumidity();                  // acquisition de la valeur de l'humidité
// Affichage
Serial.print("Temperature=");
Serial.print(temp);
Serial.println(" degre Celcius");
Serial.print("Humidite=");
Serial.print(humidite);
Serial.println(" %");
Serial.println(); 
delay(1000);
}
