/************************************************************************
*                                                                                                                                        
*    Test du module "DHT22 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1798                                                
* La température et l'humidité sont affichées dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "DHT22 Click" inséré sur le support N°1 du shield       
*        
*                
* Bibliothèques
*        1.https://playground.arduino.cc/Main/DHTLib
*
************************************************************************/
 
#include <dht.h>                                  // appel de la bibliothèque
#define dataPin 10                                // broche sur laquelle les données sont transmises
dht capteur;                                      // création de l'objet capteur

int donnee;
float temperature;
float humidite;

void setup() 
{
Serial.begin(9600);                               // initialisation de la liaison série
}

void loop() 
{
donnee=capteur.read22(dataPin);                   // acquisition des données
temperature= capteur.temperature;                 // extraction de la température
humidite=capteur.humidity;                        // extraction de l'humidité
// Affichage
Serial.print("Temperature=");
Serial.print(temperature);
Serial.println(" degre Celcius");
Serial.print("Humidite=");
Serial.print(humidite);
Serial.println(" %");
Serial.println(); 
delay(2000);                                    // pause de 2 s indispensable car le capteur est très lent
}
