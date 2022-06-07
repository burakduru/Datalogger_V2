/************************************************************************
*                                                                                                                                        
*    Test du module "Flame Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1820                                               
* Une détection de flamme déclenche 5 flashs bleus sur le module 4x4 RGB Click.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Flame Click" inséré sur le support N°1 du shield
*        4. Module "4x4 RGB Click" inséré sur le support N°2 du shield
*        
* Bibliothèque
*        1. https://github.com/adafruit/Adafruit_NeoPixel
************************************************************************/

// Affectation des broches
#define sortie 16                                             // DIN du module 4x4 RGB
#define nb_led 16                                             // le module 4x4 RGB comporte 16 led

#include <Adafruit_NeoPixel.h>                                // appel de la bibliothèque
Adafruit_NeoPixel module = Adafruit_NeoPixel(nb_led, sortie, NEO_GRB + NEO_KHZ800);  // création de l'objet module



void setup() 
{
  module.begin();                                              // initialisation de module                                         
  
}

void loop()
{
 attachInterrupt(0, alarme, RISING);                          // attache l'interruption externe n°0 à la fonction alarme
}

void alarme()
{
  detachInterrupt(0);                                         // inhibition des interruptions                                                        
  for (int i=0; i<5; i++)
  { 
    for(int t=0; t< 16; t++)                                  // allumage successif des 16 led
    {
     module.setPixelColor(t, 0, 0, 128);                       
    }
  module.show();                                              // rafraichissement des led
  delay(300);
  for(int t=0; t< 16; t++)                                    // extinction successif des 16 led
    {
     module.setPixelColor(t, 0, 0, 0);                        
    }
  module.show();                                              // rafraichissement des led
  delay(300);
  }
}
