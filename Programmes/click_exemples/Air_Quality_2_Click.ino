/************************************************************************
*                                                                                                                                        
*    Test du module "Air Quality 2 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2529                                               
* Les taux de CO2 et composés organiques volatils totaux sont affichés sur 
* le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Air Quality 2 Click" inséré sur le support N°1 du shield
*        
*        
* Bibliothèques
*        1. https://github.com/maarten-pennings/iAQcore
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "iAQcore.h"

int eco2;
int stat;
long resist;
int etvoc;

iAQcore capteur;                                                 // création de l'objet capteur

void setup()
{
Serial.begin(9600);                                               // initialisation de la liaison série
Wire.begin();                                                     // initialisation de la liaison I2C
capteur.begin();                                                  // initialisation du capteur
}

void loop()
{
// Acquisition des paramètres de l'air
 capteur.read(&eco2,&stat,&resist,&etvoc);
// Affichage
Serial.print("CO2=");
Serial.print(eco2);
Serial.print(" ppm");
Serial.print("\t");
Serial.print("TVOC=");
Serial.print(etvoc);
Serial.println(" ppb");
delay(500);  
}
