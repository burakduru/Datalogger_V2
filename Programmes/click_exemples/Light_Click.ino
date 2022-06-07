/************************************************************************
*                                                                                                                                        
*    Test du module "Light Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1424                                               
* Les led du module 4x4 RGB Click s'allume si la quantité de lumière 
* captée par le module Light Click est trop faible.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Light Click" inséré sur le support N°1 du shield
*        4. Module "4x4 RGB Click" inséré sur le support N°2 du shield
*        
* Bibliothèque
*        1. https://github.com/adafruit/Adafruit_NeoPixel
***********************************************************************/

// Affectation des broches
#define AN A0                                     // broche de sortie du module Light 
#define sortie A2                                 // DIN du module 4x4 RGB
#define nb_led 16                                 // le module 4x4 RGB comporte 16 led

#include <Adafruit_NeoPixel.h>                                // appel de la bibliothèque
Adafruit_NeoPixel module = Adafruit_NeoPixel(nb_led, sortie, NEO_GRB + NEO_KHZ800);  // création de l'objet module

int lumiere;

void setup()
{
module.begin();                                   // initialisation de module      
}

void loop()
{
lumiere=analogRead(AN);                           // conversion AN
if (lumiere<150)
{
  for(int t=0; t< 16; t++)                         // allumage successif des 16 led
    {
     module.setPixelColor(t, 255, 255, 255);                       
    }
  module.show(); 
} 
else
{
for(int t=0; t< 16; t++)                            // extinction successif des 16 led
    {
     module.setPixelColor(t, 0, 0, 0);                        
    }
  module.show();                                              
}
}

