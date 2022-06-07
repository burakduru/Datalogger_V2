/************************************************************************
*                                                                                                                                        
*    Test du module "SHT1x Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-949                                                
* La température et l'humidité sont affichées 
* dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "SHT1x Click" inséré sur le support N°1 du shield       
*                     
*                                    
* Bibliothèques
*        1. https://github.com/practicalarduino/SHT1x
*
************************************************************************/

// Appel des bibliothèques
#include <SHT1x.h>

#define SDA  A4
#define SCL A5
SHT1x capteur(SDA,SCL);                            // création de l'objet capteur

float temperature;
float humidite;

void setup(void) 
{
Serial.begin(9600);                               // initialisation de la liaison série
}

void loop() 
{
// Acquisition des données
temperature=capteur.readTemperatureC();
humidite=capteur.readHumidity();           
// Affichage
Serial.print("Temperature=");
Serial.print(temperature);
Serial.print(" degre Celcius");
Serial.print("\t");
Serial.print("Humidite=");
Serial.print(humidite);
Serial.println(" %");
Serial.println(); 
delay(1000);
}
