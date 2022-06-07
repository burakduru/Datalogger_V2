/************************************************************************
*                                                                                                                                        
*    Test du module "Thermo 3 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1885                                                
* La température est affichée dans le moniteur série et la led de la carte Arduino 
* s’allume lorsque la température est supérieure à un seuil haut.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Thermo 3 Click" 
*        3. Shifter Level TXB0108
*        
*        
* Bibliothèques
*        1. https://github.com/sparkfun/SparkFun_TMP102_Arduino_Library/archive/master.zip
*        
************************************************************************/
// Appel des bibliothèques
#include <Wire.h> 
#include "SparkFunTMP102.h"                  
#define TMP102_Adresse 0x48                 // adresse I2C du TMP102
#define ALERT 2                             // affectation des broches
#define LED 13  

TMP102 capteur(TMP102_Adresse);             // création de l'objet capteur

float temperature;
boolean alarme;

void setup() 
{
Serial.begin(9600);                         // initialisation de la liaison série
pinMode(LED,OUTPUT);                        // configuration de la broche LED en sortie
pinMode(ALERT,INPUT);                       // configuration de la broche ALERT en entrée
capteur.begin();                            // initialisation du capteur
// Configuration du capteur TMP102
capteur.setFault(0);                        // déclenchement immédiat de la broche ALERT
capteur.setAlertPolarity(1);                // broche ALERT active à l'état haut  
capteur.setAlertMode(0);                    // broche ALERT configurée en trigger non inverseur  
capteur.setConversionRate(1);               // conversion toutes les secondes
capteur.setExtendedMode(0);                 // conversion sur 12 bits 
capteur.setHighTempC(30.0);                 // valeur du seuil haut  
capteur.setLowTempC(27.0);                  // valeur du seuil bas 
}
 
void loop()
{
capteur.wakeup();                           // fonctionnement du capteur en mode non économique
temperature = capteur.readTempC();          // acquisition de la température
alarme = digitalRead(ALERT);                // acquisition de ALERT
capteur.sleep();                            // fonctionnement du capteur en mode économique
digitalWrite(LED,alarme);                   // commande de la led
Serial.print("Temperature: ");              // affichage dans le moniteur série
Serial.print(temperature);
Serial.println(" degre Celcius");
delay(1000);                                // pause d'une seconde
}
