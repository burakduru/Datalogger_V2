/************************************************************************
*                                                                                                                                        
*    Test du module "IR Sense Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2677                                               
* Le paramètre IR de chaque zone et la température sont affichés dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Module "IR Sense Click" 
*        3. Shifter Level TXB0108
*        
*        
* Bibliothèque
*        1. https://github.com/sparkfunX/Qwiic_Human_Presence_Sensor-AK9750
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "SparkFun_AK9750_Arduino_Library.h"

// Affectation des broches
#define PDN 10

AK9750 capteur;                                   // création de l'objet capteur

int IR_1;
int IR_2;
int IR_3;
int IR_4;
float temp_F;
float temp_C;

void setup()
{
pinMode(PDN,OUTPUT);
digitalWrite(PDN,HIGH);                           // configuration du AK9750 en mode actif
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation de la liaison I2C
capteur.begin();                                  // initialisation de capteur      
}

void loop()
{
  if (capteur.available())
  {
    // Acquisition des grandeurs
    IR_1 = capteur.getIR1();
    IR_2 = capteur.getIR2();
    IR_3 = capteur.getIR3();
    IR_4 = capteur.getIR4();
    temp_F = capteur.getTemperatureF();
    capteur.refresh();                            // préparation des registres pour une prochaine mesure
    temp_C=(temp_F-32)/1.8;                       // conversion de la température en degré Celcius
    // Affichage
    Serial.print("Zone 1=");
    Serial.print(IR_1);
    Serial.print("\t");
    Serial.print("Zone 2=");
    Serial.print(IR_2);
    Serial.print("\t");
    Serial.print("Zone 3=");
    Serial.print(IR_3);
    Serial.print("\t");
    Serial.print("Zone 4=");
    Serial.print(IR_4);
    Serial.print("\t");
    Serial.print("Temperature=");
    Serial.print(temp_C);
    Serial.println("°C");
    delay(100);
  }
}
