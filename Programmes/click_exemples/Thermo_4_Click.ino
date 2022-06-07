/************************************************************************
*                                                                                                                                        
*    Test du module "Thermo 4 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2632                                                
* La température est affichée dans le moniteur série et la led de la carte Arduino 
* s’allume lorsque la température est supérieure à un seuil haut.
*
* * Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO 
*        3. Module "Thermo 4 Click" inséré sur le support N°1 du shield
*               
*               
* Bibliothèques
*        1. https://www.arduinolibraries.info/libraries/i2-c-temperature-sensors-derived-from-the-lm75
*        
************************************************************************/
// Appel des bibliothèques
#include <Wire.h> 
#include <Temperature_LM75_Derived.h>                

#define INT 2                                 // affectation des broches
#define LED 13  

Generic_LM75 capteur;                         // création de l'objet capteur

float temperature;
float seuil_bas;
float seuil_haut;
boolean alarme;

void setup() 
{
Serial.begin(9600);                           // initialisation de la liaison série
Wire.begin();                                 // initialisation de la liaison I2C
pinMode(LED,OUTPUT);                          // configuration de la broche LED en sortie
pinMode(INT,INPUT);                           // configuration de la broche ALERT en entrée

// Configuration du capteur LM75
capteur.setAlertActiveHigh();                 // broche INT active à l'état haut  
capteur.setTemperatureHighC(27.0);            // valeur du seuil haut  
capteur.setTemperatureLowC(25.0);             // valeur du seuil bas 
}
 
void loop()
{
temperature=capteur.readTemperatureC();               // acquisition de la température
seuil_bas=capteur.readTemperatureLowC();              // acquisition du seuil bas
seuil_haut=capteur.readTemperatureHighC();            // acquisition du seuil haut
alarme = digitalRead(INT);                            // acquisition de INT
digitalWrite(LED,alarme);                             // commande de la led
Serial.print("Temperature: ");                        // affichage dans le moniteur série de la température
Serial.print(temperature);
Serial.println(" degre Celcius");
Serial.print("Seuil bas: ");                          // affichage dans le moniteur série du seuil bas
Serial.print(seuil_bas);
Serial.println(" degre Celcius");
Serial.print("Seuil haut: ");                         // affichage dans le moniteur série du seuil haut
Serial.print(seuil_haut);
Serial.println(" degre Celcius");
Serial.println("");                                   // saut de ligne
delay(1000);                                          // pause d'une seconde
}
