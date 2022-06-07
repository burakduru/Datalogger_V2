/************************************************************************
*                                                                                                                                        
*    Test du module "Smoke Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2560                                                
* La présence de fumée est affichée dans le moniteur série.
* 
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Smoke Click" inséré sur le support N°1 du shield
*      
*        
* Bibliothèques 
*        1. https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/archive/master.zip
*       
*  
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "MAX30105.h"

byte ledBrightness = 0xFF;  // Options: 0=éteinte à 255=50mA
byte sampleAverage = 4;     // Options: 1, 2, 4, 8, 16, 32
byte ledMode = 2;           // Options: 1 = led rouge, 2 = led rouge + IR, 3 = led rouge + IR + verte
int sampleRate = 400;       // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
int pulseWidth = 411;       // Options: 69, 118, 215, 411
int adcRange = 2048;        // Options: 2048, 4096, 8192, 16384
long echantillon = 0;       // nombre échantillons
long val_moy_IR;            // valeur moyenne IR 
long particule;             // nombre particules
long difference;

MAX30105 capteur_particule;    //création de l'objet


void setup(void) 
{
Serial.begin(9600);
capteur_particule.begin();
capteur_particule.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);  // configuration du capteur
capteur_particule.setPulseAmplitudeRed(0);     // extinction led rouge
capteur_particule.setPulseAmplitudeGreen(0);   // extinction led verte

//Calcul de la valeur moyenne
  val_moy_IR = 0;
  for (byte x = 0 ; x < 100 ; x++)
  {
    val_moy_IR += capteur_particule.getIR();   // mesure de la quantité IR
  }
  val_moy_IR = val_moy_IR/100;

}

void loop() 
{
particule=capteur_particule.getIR();
difference = particule - val_moy_IR;
Serial.print("IR=");
Serial.print(particule);
Serial.print("\t");
Serial.print("\t");
Serial.print("Difference=");
Serial.print(difference); 
Serial.print("\t");
if (difference > 100)
  {
    Serial.print(" Au feu !!!");
  }
Serial.println();
}
