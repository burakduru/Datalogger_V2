/************************************************************************
*                                                                                                                                        
*    Test du module "Illuminance Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1688                                               
* L'éclairement lumineux est affiché dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Module "Illuminance Click" 
*        3. Shifter Level TXB0108
*        
*        
* Bibliothèques
*        1. https://github.com/adafruit/Adafruit_TSL2561
*        2. https://github.com/adafruit/Adafruit_Sensor
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified capteur = Adafruit_TSL2561_Unified(0x49, 12345);    // création de l'objet capteur

void setup()
{
Serial.begin(9600);                                                         // initialisation de la liaison série
Wire.begin();                                                               // initialisation de la liaison I2C
capteur.begin();                                                            // initialisation du capteur
capteur.enableAutoRange(true);                                              // configuration du gain en mode automatique
capteur.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);                   // configuration de la résolution   
}

void loop()
{
// Acquisition de la valeur de l'éclairement
sensors_event_t event;
capteur.getEvent(&event);
// Affichage
Serial.println("Eclairement:");
Serial.print(event.light); 
Serial.println(" lux"); 
delay(500);
}
