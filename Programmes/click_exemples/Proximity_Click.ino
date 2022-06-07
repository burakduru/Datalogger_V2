/************************************************************************
*                                                                                                                                        
*    Test du module "Proximity Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1445                                               
* L'éclairement et la distance (variant de 0 à 65535 et inversement proportionnelle 
* à la proximité par rapport au capteur) sont affichés dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Proximity Click" inséré sur le support N°1 du shield
*        
*        
* Bibliothèque
*        1. https://github.com/adafruit/Adafruit_VCNL4010
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "Adafruit_VCNL4010.h"

Adafruit_VCNL4010 capteur;                         // création de l'objet capteur

int lumiere;
int proximite;

void setup()
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation de la liaison I2C
capteur.begin();                                  // initialisation de capteur      
}

void loop()
{
// Acquisition des grandeurs
lumiere = capteur.readAmbient();    
proximite = capteur.readProximity();              
// Affichage
Serial.print("Eclairement=");
Serial.print(lumiere);
Serial.println(" Lux");   
Serial.print("Proximite=");
Serial.println(proximite);
delay(500);
}
