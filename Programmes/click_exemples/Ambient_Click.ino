/************************************************************************
*                                                                                                                                        
*    Test du module "Ambient Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1890                                               
* Des effets lumineux sont générés sur les led du module 4x4 RGB lorsque 
* la luminosité devient trop faible.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Ambient Click" inséré sur le support N°1 du shield
*        4. Module "4x4 RGB Click" inséré sur le support N°2 du shield
*        
* Bibliothèque
*        1. https://github.com/adafruit/Adafruit_NeoPixel
*        
***********************************************************************/

// Affectation des broches
#define AN A0                                                 // broche de sortie du module Ambient 
#define sortie A2                                             // DIN du module 4x4 RGB
#define nb_led 16                                             // le module 4x4 RGB comporte 16 led

#include <Adafruit_NeoPixel.h>                                // appel de la bibliothèque
Adafruit_NeoPixel module = Adafruit_NeoPixel(nb_led, sortie, NEO_GRB + NEO_KHZ800);  // création de l'objet module

int lumiere;
float j, f, k;

void setup()
{
module.begin();                                               // initialisation de module      
}

void loop()
{
lumiere=analogRead(AN);                                       // conversion AN
if (lumiere<75)
{
  for(int t = 0; t < 16; t++)                                 // allumage successif des 16 led
  {
    int rouge = 64 * (1 + sin(t / 2.0 + j / 4.0) );           // variation de la couleur rouge  
    int vert = 64 * (1 + sin(t / 1.0 + f / 9.0 + 2.1) );      // variation de la couleur verte
    int bleu = 64 * (1 + sin(t / 3.0 + k / 14.0 + 4.2) );     // variation de la couleur bleu
    module.setPixelColor(t, rouge, vert, bleu);               // allumage de la led t avec les couleurs rouge, verte et bleu
  }
  module.show();                                              // rafraichissement des led
  j += random(1, 6) / 6.0;                                    // changement aléatoire des variables j, f et k
  f += random(1, 6) / 6.0;
  k += random(1, 6) / 6.0;
} 
else
{
for(int t=0; t< 16; t++)                                      // extinction successif des 16 led
    {
     module.setPixelColor(t, 0, 0, 0);                        
    }
  module.show();                                              
}
}

