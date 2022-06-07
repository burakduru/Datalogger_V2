/************************************************************************
*                                                                                                                                        
*    Test du module "LightRanger 2 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1897                                               
* La distance est affichée dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "LightRanger 2 Click" inséré sur le support N°1 du shield
*        
*        
* Bibliothèque
*        1. https://github.com/adafruit/Adafruit_VL53L0X
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X capteur = Adafruit_VL53L0X();                         // création de l'objet capteur

int distance;

void setup()
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation de la liaison I2C
capteur.begin();                                  // initialisation de capteur      
}

void loop()
{
// Acquisition de la distance
VL53L0X_RangingMeasurementData_t measure;
capteur.rangingTest(&measure, false); 
distance=measure.RangeMilliMeter;         
// Affichage  
if (measure.RangeStatus != 4) 
  {  
    Serial.print("Distance= "); 
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm"); 
  } 
else 
  {
    Serial.println("Pas d'obstacle");
  }
delay(500);
}
