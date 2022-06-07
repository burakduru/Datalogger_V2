/************************************************************************
*                                                                                                                                        
*    Test du module "Heart rate Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2000                                                
* La fréquence cardiaque et la saturation en oxygène du sang sont affichés 
* dans le moniteur série.
* 
* Matériel
*        1. Arduino Uno                                       
*        2. Module "Heart rate Click" 
*        3. Shifter Level TXB0108
*        
* Bibliothèques 
*        1. https://github.com/oxullo/Arduino-MAX30100/
*       
*  
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

PulseOximeter capteur;    //création de l'objet

int periode=1000;         // période de la mesure
int duree=0;

void Detection()
{
    Serial.println("Battement!");
}

void setup(void) 
{
Serial.begin(9600);
Serial.print("Initialisation du capteur....");
    if (!capteur.begin()) 
    {
        Serial.println("ECHEC");
        while(1);
    } 
    else 
    {
        Serial.println("SUCCES");
    }
capteur.setOnBeatDetectedCallback(Detection);
}

void loop() 
{
capteur.update();
if (millis() - duree > periode) 
{
  Serial.print("Frequence cardiaque:");
  Serial.print(capteur.getHeartRate());
  Serial.print(" battements/s");
  Serial.print("\t");
  Serial.print("Saturation en oxygène:");
  Serial.print(capteur.getSpO2());
  Serial.println(" %");
  duree = millis();
}
}
