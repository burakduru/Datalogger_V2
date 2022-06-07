/************************************************************************
*                                                                                                                                        
*    Test du module "Color 2 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1988                                               
* Les trois composantes (rouge, verte et bleue) de la couleur sont affichées
* dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Module "Color 2 Click" 
*        3. Shifter Level TXB0108       
*        
*        
* Bibliothèque
*        1. https://github.com/sparkfun/SparkFun_ISL29125_Breakout_Arduino_Library
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "SFE_ISL29125.h"

SFE_ISL29125 capteur;                             // création de l'objet capteur

unsigned int rouge;
unsigned int vert;
unsigned int bleu;

void setup()
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation de la liaison I2C
capteur.init();                                   // initialisation de capteur      
}

void loop()
{
// Acquisition de la distance
rouge = capteur.readRed();
vert = capteur.readGreen();
bleu = capteur.readBlue();        
// Affichage  
Serial.print("R=");
Serial.print(rouge);
Serial.print("\t");
Serial.print("V=");
Serial.print(vert);
Serial.print("\t");
Serial.print("B=");
Serial.print(bleu);
Serial.println(); 
delay(1000);
}
