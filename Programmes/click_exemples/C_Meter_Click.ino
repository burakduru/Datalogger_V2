/************************************************************************
*                                                                                                                                        
*    Test du module "C Meter Click"                                            
*                                                                    
*************************************************************************                                              
* La valeur de la capacité est affichée sur le moniteur série. Attention,
* les valeurs des condensateurs doivent être comprises entre 1nF et 4700 uF.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "C Meter Click" inséré sur le support N°1 du shield
*        
***********************************************************************/

// Affectation des broches
#define INT 2                          // broche de sortie du module

unsigned long duree=0;
float capacite;

void setup()
{
Serial.begin(9600);                    // initialisation de la liaison série 
pinMode(INT, INPUT);                   // configuration de la broche en entrée
}

void loop()
{
duree = pulseIn(INT, LOW, 200000000);  // mesure de la durée à l'état bas du signal 
capacite=duree/(0.693*51000);          // calcul de la capacité
// Affichage
Serial.print("Duree etat bas=");
Serial.print(duree);
Serial.println("us");
Serial.print("Capacite=");
Serial.print(capacite);
Serial.println("uF");
delay(1000);
}
