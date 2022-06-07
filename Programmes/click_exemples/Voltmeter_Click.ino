/************************************************************************
*                                                                                                                                        
*    Test du module "Voltmeter Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2436                                               
* La valeur de la tension est affichée sur le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Voltmeter Click" inséré sur le support N°1 du shield
*        
***********************************************************************/

// Affectation des broches
#define AN A0                          // broche de sortie du module

int valeur;
int echantillon=10;
unsigned long somme=0;
unsigned long moyenne;
float tension;
float tension_mes;

void setup()
{
Serial.begin(9600);                       // initialisation de la liaison série 
}

void loop()
{
for (int i=0; i<10; i++)
{
valeur=analogRead(AN);                    // conversion AN     
somme=somme+valeur;
moyenne=somme/echantillon;
delay(10);
}
tension=(moyenne*5000.00)/1024;           // calcul de la tension de sortie du module
somme=0;
tension_mes=(tension-1024)/0.06;          // calcul de la tension différentielle appliquée sur les entrées du module
Serial.print("Tension=");
Serial.print(tension);
Serial.print(" mV");
Serial.print("\t");
Serial.print("Tension differentielle=");
Serial.print(tension_mes);
Serial.println(" mV");
delay(1000);
}




