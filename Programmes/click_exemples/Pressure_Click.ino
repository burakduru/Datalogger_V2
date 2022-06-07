/************************************************************************
*                                                                                                                                        
*    Test du module "Pressure Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1422                                                
* La pression, l'altitude et la température sont affichées 
* dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Pressure Click" inséré sur le support N°1 du shield       
*                     
*                     *                
* Bibliothèques
*        1. https://github.com/pololu/lps-arduino
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include <LPS.h>

LPS capteur;                                      // création de l'objet capteur

float pression;
float altitude;
float temperature;

void setup(void) 
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation du bus I2C
capteur.enableDefault();                          // configuration du capteur                    
}

void loop() 
{
// Acquisition des données
pression=capteur.readPressureMillibars();
altitude = capteur.pressureToAltitudeMeters(pression);
temperature = capteur.readTemperatureC();           
// Affichage
Serial.print("Pression=");
Serial.print(pression);
Serial.print(" mbar");
Serial.print("\t");
Serial.print("Altitude=");
Serial.print(altitude);
Serial.print(" metres");
Serial.print("\t");
Serial.print("Temperature=");
Serial.print(temperature);
Serial.println(" degre Celcius");
Serial.println(); 
delay(1000);
}
