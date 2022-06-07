/************************************************************************
*                                                                                                                                        
*    Test du module "IrThermo2 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1888                                                
* Les températures ambiante et de l'objet visé sont affichées 
* dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "IrThermo2 Click" inséré sur le support N°1 du shield       
*        
*                
* Bibliothèques
*        1. https://github.com/adafruit/Adafruit_TMP007_Library/archive/master.zip
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "Adafruit_TMP007.h"

Adafruit_TMP007 capteur;    // création de l'objet capteur

float temperature_objet;
float temperature;

void setup(void) 
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation du bus I2C
capteur.begin();                                  // initialisation du capteur                    
}

void loop() 
{
temperature_objet = capteur.readObjTempC();     // acquisition de la valeur de la température de l'objet
temperature = capteur.readDieTempC();           // acquisition de la valeur de la température ambiante
// Affichage
Serial.print("Temperature ambiante=");
Serial.print(temperature, 2);
Serial.println(" degre Celcius");
Serial.print("Temperature objet=");
Serial.print(temperature_objet, 2);
Serial.println(" degre Celcius");
Serial.println(); 
delay(1000);
}
