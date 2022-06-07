/************************************************************************
*                                                                                                                                        
*    Test du module "Thermo 2 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1840                                                
* La température est affichée dans le moniteur série.
* 
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Thermo 2 Click" 
*        3. Shifter Level TXB0108
*        
*        
* Bibliothèques
*        1. https://github.com/PaulStoffregen/OneWire
*        
************************************************************************/

#include <OneWire.h>                        // appel de la bibliothèque                       

#define GPIO0 6                             // affectation des broches

byte i;
byte data[12];
byte adresse[8];
float temperature=0.0;
                              
OneWire DS1825(GPIO0);                      // création de l'objet DS1825

void setup() 
{
Serial.begin(9600);                        // initialisation de la liaison série
}

void loop() 
{
DS1825.search(adresse);                    // recherche de l'adresse du capteur
// Sélection du capteur sur le bus et demande d'acquisition de la température
DS1825.reset();
DS1825.select(adresse);
DS1825.write(0x44);
delay(850);
// Sélection du capteur sur le bus et demande de lecture du scratchpad 
DS1825.reset();
DS1825.select(adresse);
DS1825.write(0xBE);
// Lecture du scratchpad
for (i = 0; i < 9; i++) {
data[i] = DS1825.read();
}
temperature = ( (data[1] << 8) + data[0] )*0.0625; // calcul de la température
Serial.print("Temperature : ");
Serial.print(temperature);
Serial.println(" degré Celcius");
}


