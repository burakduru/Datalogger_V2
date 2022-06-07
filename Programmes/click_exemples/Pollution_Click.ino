/************************************************************************
*                                                                                                                                        
*    Test du module "Pollution Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2516                                               
* La concentration de formaldéhyde est affichée sur le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Pollution Click" inséré sur le support N°1 du shield
*        
***********************************************************************/

/**********************************************************************
 * Le module Pollution Click nécessite d'être calibré avant de mesurer 
 * les taux d'alcool, de bunzène,etc...
 * Il faut donc charger sur la carte Arduino le programme suivant permettant 
 * de déterminer la résistance R0 lorsque le capteur est placé dans un 
 * environnement non pollué (idéalement à l'extérieur).
 * 

#define RST 17                              

void setup()
{
Serial.begin(9600);
pinMode(RST,OUTPUT);
digitalWrite(RST,HIGH);
}

void loop()
{
int valeur=analogRead(A0);
float R0=(1023.0/valeur)-1;
Serial.print("R0 = ");
Serial.println(R0);
delay(500);
}
 */
 
// Affectation des broches
#define AN A0                               // broche de sortie du module
#define RST 17                              // broche de validation du module

#define R0 15.77                            // résistance à déterminer grâce au programme précédent

#include <math.h>                           // appel de la bibliothèque
int valeur;
double RS;
double ppm;
float rapport;

void setup()
{
Serial.begin(9600);                         // initialisation de la liaison série 
pinMode(RST,OUTPUT);
}

void loop()
{
digitalWrite(RST,HIGH);                      // validation du module
valeur=analogRead(AN);                       // conversion AN
RS=(1023.0/valeur)-1;                        // calcul de RS
Serial.print("RS = ");                       // affichage de RS
Serial.println(RS);
rapport=RS/R0;                               // calcul du rapport RS/R0
Serial.print("Rapport RS/R0 = ");            // affichage du rapport si on souhaite déterminer graphiquement la valeur ppm (voir doc capteur)
Serial.println(rapport);
ppm=pow(10.0,((log10(RS/R0)-0.0827)/(-0.4807)));
Serial.print("Quantité de HCHO = ");
Serial.print(ppm);
Serial.println(" ppm");
Serial.println("");
digitalWrite(RST,LOW);                      // module en stand-by
delay(1000);                                // pause entre deux mesures
}




