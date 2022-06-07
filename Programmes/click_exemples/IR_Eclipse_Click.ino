/************************************************************************
*                                                                                                                                        
*    Test du module "IR Eclipse Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1711                                               
* Une détection de passage d'objets dans la fourche déclenche l'incrémentation 
* d'une variable qui est affichée dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "IR Eclipse Click" inséré sur le support N°1 du shield    
*               
***********************************************************************/

volatile unsigned int comptage=0;

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
comptage=comptage+1;                                          // incrémentation de la variable comptage
Serial.print("Nombre impulsions : ");                         // affichage du nombre d'impulsions
Serial.println(comptage);
}
