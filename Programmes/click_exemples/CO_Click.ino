/************************************************************************
*                                                                                                                                        
*    Test du module "CO Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1626                                               
* La concentration de monoxyde de carbone est affichée sur le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "CO Click" inséré sur le support N°1 du shield     
*        
*        
* Bibliothèque
*        1. https://github.com/swatish17/MQ7-Library
*        
***********************************************************************/

#include "MQ7.h"                            // appel de la bibliothèque
MQ7 capteur(A0,5.0);                        // création de l'objet capteur


void setup()
{
Serial.begin(9600);                         // initialisation de la liaison série 
}

void loop()
{
Serial.print("Concentration en monoxyde de carbone:"); 
Serial.print(capteur.getPPM());
Serial.println(" ppm");
delay(1000);                                // pause entre deux mesures
}

 
