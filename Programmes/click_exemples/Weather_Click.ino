/************************************************************************
*                                                                                                                                        
*    Test du module "Weather Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1978                                                
* La température, la pression et l'humidité sont affichées 
* dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Weather Click" 
*        3. Shifter Level TXB0108
*        
*                
* Bibliothèques
*        1. https://github.com/sparkfun/SparkFun_BME280_Arduino_Library/archive/master.zip
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>                           
#include "SparkFunBME280.h"

float humidite;
float temperature;
float pression;

BME280 capteur;                                 // création de l'objet capteur
void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
//Configuration du capteur
capteur.settings.commInterface = I2C_MODE;      // fonctionnement du BME280 en I2C
capteur.settings.I2CAddress = 0x76;             // adresse I2C du BME280 lorsque SD0=0
capteur.settings.runMode = 3;                   // le BME280 est en mode "sleep" entre deux mesures
capteur.settings.tStandby = 0;                  // la durée de standby entre deux mesures est de 0.5ms  
capteur.settings.filter = 0;                    // filtrage inactif
capteur.settings.tempOverSample = 1 ;           // résolution de la température sur 16 bit
capteur.settings.humidOverSample = 1;           // résolution de l'humidité sur 16 bit
capteur.settings.pressOverSample = 1;           // résolution de la pression sur 16 bit
delay(10);                                      // attente de la mise en route du capteur (2 ms minimum)
capteur.begin();
}

void loop() 
{
temperature = capteur.readTempC();              // acquisition de la valeur de la température
humidite = capteur.readFloatHumidity();         // acquisition de la valeur de l'humidité
pression = capteur.readFloatPressure()/100;     // acquisition de la valeur de la pression
// Affichage
Serial.print("Temperature=");
Serial.print(temperature);
Serial.print(" °C");
Serial.print("\t");
Serial.print("Humidite=");
Serial.print(humidite);
Serial.print(" %");
Serial.print("\t");
Serial.print("Pression=");
Serial.print(pression);
Serial.println(" hPa");
delay(1000);
}
