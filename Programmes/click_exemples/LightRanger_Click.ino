/************************************************************************
*                                                                                                                                        
*    Test du module "LightRanger Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1897                                               
* L'éclairement et la distance sont affichés dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "LightRanger Click" inséré sur le support N°1 du shield
*        
*        
* Bibliothèque
*        1. https://github.com/adafruit/Adafruit_VL6180X
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X capteur = Adafruit_VL6180X();                         // création de l'objet capteur

float lumiere;
int proximite;
int etat;

void setup()
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation de la liaison I2C
capteur.begin();                                  // initialisation de capteur      
}

void loop()
{
// Acquisition des grandeurs
lumiere = capteur.readLux(VL6180X_ALS_GAIN_5);    
proximite = capteur.readRange();  
etat = capteur.readRangeStatus();           
// Affichage
Serial.print("Eclairement=");
Serial.print(lumiere);
Serial.println(" Lux");
if (etat==VL6180X_ERROR_RAWOFLOW)
  {
   Serial.println("Pas d'obstacle");  
  }
else
  {
  Serial.print("Proximite=");
  Serial.print(proximite);
  Serial.println(" mm");  
  }
delay(500);
}
