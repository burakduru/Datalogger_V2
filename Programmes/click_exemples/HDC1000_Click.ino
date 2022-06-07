/************************************************************************
*                                                                                                                                        
*    Test du module "HDC1000 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1797                                                
* L'humidité et la température sont affichées dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "HDC1000 Click" 
*        3. Shifter Level TXB0108
*        
*                
* Bibliothèques
*        1. https://github.com/adafruit/Adafruit_HDC1000_Library
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>                             
#include "Adafruit_HDC1000.h"

Adafruit_HDC1000 capteur = Adafruit_HDC1000();                            // création de l'objet capteur

float humidite;
float temperature;

void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
capteur.begin();                                // initialisation du capteur
}

void loop() 
{
humidite = capteur.readHumidity();              // acquisition de la valeur de l'humidité
temperature = capteur.readTemperature();        // acquisition de la valeur de la température
// Affichage
Serial.print("Humidite=");
Serial.print(humidite);
Serial.print(" %");
Serial.print("\t");
Serial.print("Temperature=");
Serial.print(temperature);
Serial.println(" degre Celcius");
Serial.println(); 
delay(1000);
}
