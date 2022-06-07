/************************************************************************
*                                                                                                                                        
*    Test du module "IR Reflect Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1882                                               
* Une détection d'obstacle est affichée dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "IR Reflect Click" inséré sur le support N°1 du shield    
*               
***********************************************************************/


void setup()
{
Serial.begin(9600);                                             // initialisation de la liaison série  
attachInterrupt(0, detection, RISING);                          // attache l'interruption externe n°0 à la fonction detection    
}

void loop()
{
            
}

void detection()
{
Serial.println("Attention, obstacle !!!");                         // affichage du message
}
