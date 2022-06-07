/************************************************************************
*                                                                                                                                        
*    Test du module "Temp&Hum Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1971                                                
* L'humidité et la température sont affichées dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Temp&Hum Click" 
*        3. Shifter Level TXB0108
*        
*                
* Bibliothèques
*        1. https://github.com/ameltech/sme-hts221-library
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>                           
#include <HTS221.h>

double humidite;
double temp;

void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
smeHumidity.begin();                            // initialisation du capteur
}

void loop() 
{
humidite = smeHumidity.readHumidity();          // acquisition de la valeur de l'humidité
temp = smeHumidity.readTemperature();           // acquisition de la valeur de la température
// Affichage
Serial.print("Humidite=");
Serial.print(humidite);
Serial.print(" %");
Serial.print("\t");
Serial.print("Temperature=");
Serial.print(temp);
Serial.println(" degre Celcius");
Serial.println(); 
delay(1000);
}
