/************************************************************************
*                                                                                                                                        
*    Test du module "Air Quality Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1630                                               
* La valeur de la qualité de l'air et un message d'alerte sont affichés 
* sur le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Air Quality Click" inséré sur le support N°1 du shield
*        
***********************************************************************/

// Affectation des broches
#define AN A0                          // broche de sortie du module

int valeur;
int echantillon=10;
unsigned long somme=0;
unsigned long moyenne;


void setup()
{
Serial.begin(9600);                       // initialisation de la liaison série 
}

void loop()
{
for (int i=0; i<10; i++)
{
valeur=analogRead(AN);                    // conversion AN     
somme=somme+valeur;                       // somme de 10 échantillons
moyenne=somme/echantillon;                // calcul de la moyenne de 10 échantillons
delay(10);
}
somme=0;
Serial.print("Valeur=");
Serial.println(moyenne);
if (valeur>250)
  {
  Serial.println("La qualite de l air est mediocre");
  }
else
 {
  Serial.println("La qualite de l air est bonne");
  }
delay(1000);
}




