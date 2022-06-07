/************************************************************************
*                                                                                                                                        
*    Test du module "UV Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1677                                               
* Une détection d'UV déclenche 5 flashs rouges sur le module 4x4 RGB Click.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "UV Click" inséré sur le support N°1 du shield
*        4. Module "4x4 RGB Click" inséré sur le support N°2 du shield
*        
* Bibliothèque
*        1. https://github.com/adafruit/Adafruit_NeoPixel
***********************************************************************/

// Affectation des broches
#define AN A0                                     // broche de sortie du module Light 
#define sortie A2                                 // DIN du module 4x4 RGB
#define nb_led 16                                 // le module 4x4 RGB comporte 16 led

#include <Adafruit_NeoPixel.h>                    // appel de la bibliothèque
Adafruit_NeoPixel module = Adafruit_NeoPixel(nb_led, sortie, NEO_GRB + NEO_KHZ800);  // création de l'objet module

int UV;

void setup()
{
module.begin();                                   // initialisation de module      
}

void loop()
{
UV=analogRead(AN);                                // conversion AN
if (UV>350)
{
for (int i=0; i<5; i++)                           // boucle des 5 clignotements
  { 
    for(int t=0; t< 16; t++)                      // allumage successif des 16 led
    {
     module.setPixelColor(t, 255, 0, 0);                       
    }
  module.show();                                  // rafraichissement des led
  delay(300);
  for(int t=0; t< 16; t++)                        // extinction successif des 16 led
    {
     module.setPixelColor(t, 0, 0, 0);                        
    }
  module.show();                                  // rafraichissement des led
  delay(300);
  }
}
}
