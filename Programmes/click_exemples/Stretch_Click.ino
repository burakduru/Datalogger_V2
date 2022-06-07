/************************************************************************
*                                                                                                                                        
*    Test du module "Stretch Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2064                                               
* La valeur numérique de l'étirement est affichée dans le moniteur série 
* et si cette valeur dépasse un seuil prédéfini, la led du module s'allume.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Stretch Click" inséré sur le support N°1 du shield
*        
* Attention : le cordon est conducteur et doit former une boucle pour 
* ne pas perturber la mesure.     
***********************************************************************/

// Affectation des broches
#define AN A0                          
#define LED 6                         
int valeur;
int seuil=150;                             // seuil à définir en fonction de l'étirement du cordon

void setup()
{
Serial.begin(9600);                       // initialisation de la liaison série 
pinMode(LED,OUTPUT);                      // configuration de la broche LED en sortie
}

void loop()
{
valeur=analogRead(AN);                    // conversion AN     
Serial.print("Valeur=");                  // affichage de la valeur
Serial.println(valeur);
if (valeur>seuil)                         
{
  digitalWrite(LED,HIGH);                 // allumage de la led
}
else
{
  digitalWrite(LED,LOW);                 // extinction de la led
}
delay(1000);
}




