/************************************************************************
*                                                                                                                                        
*    Test du module "Ambient 2 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1903                                               
* L'éclairement lumineux est affiché dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Module "Ambient 2 Click" 
*        3. Shifter Level TXB0108
*        
*        
* Bibliothèques
*        1. https://github.com/closedcube/ClosedCube_OPT3001_Arduino
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include <ClosedCube_OPT3001.h>

ClosedCube_OPT3001 capteur;                                                 // création de l'objet capteur

void setup()
{
Serial.begin(9600);                                                         // initialisation de la liaison série
Wire.begin();                                                               // initialisation de la liaison I2C
capteur.begin(0x44);                                                        // initialisation du capteur
// Configuration du capteur
OPT3001_Config newConfig;
newConfig.RangeNumber = B1100;  
newConfig.ConvertionTime = B0;
newConfig.Latch = B1;
newConfig.ModeOfConversionOperation = B11;
}

void loop()
{
// Acquisition de la valeur de l'éclairement
OPT3001 result = capteur.readResult();
// Affichage
if (result.error == NO_ERROR) 
{
   Serial.print("Eclairement:");
   Serial.print(result.lux);
   Serial.println(" lux");
} 
  delay(500);  
}
