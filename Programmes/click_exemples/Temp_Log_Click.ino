/************************************************************************
*                                                                                                                                        
*    Test du module "Temp-Log Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2886                                               
* La température est affichée dans le moniteur série et 
* enregistrée dans l'EEPROM de l'AT30TSE758A.
*
*  Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Temp-Log Click" inséré sur le support N°1 du shield
*        
*                
* Bibliothèques
*        1. https://github.com/bkryza/i2cdevlib/tree/372078c1a2fd8e70f8405a16f2321cf72e7ea7c9
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>                           
#include "AT30TSE75x.h"

AT30TSE75x capteur(0x00, AT30TSE75x_758);                      // création de l'objet capteur

float temperature;

void setup(void) 
{
Serial.begin(9600);                                           // initialisation de la liaison série
Wire.begin();                                                 // initialisation du bus I2C
capteur.initialize();                                         // initialisation du capteur 
capteur.setConversionResolution(AT30TSE75x_RES_12BIT);        // résolution du capteur en 12 bit
}

void loop() 
{
temperature = capteur.getTemperatureCelcius();                // acquisition de la température
// Affichage
Serial.print("Temperature : ");
Serial.print(temperature);
Serial.println(" °C");

// Lecture/Ecriture d'un octet en EEPROM 
int page = 16;
int nb_octet = 5;
capteur.writeEEPROMByte(16*page+nb_octet, 13);
int valeur_eeprom = capteur.readEEPROMByte(16*page+nb_octet);
Serial.print("Octet lu dans EEPROM : ");
Serial.println(valeur_eeprom); 
 
// Lecture/Ecriture d'une page de l'EEPROM 
uint8_t numero_page[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
page = 3;
capteur.writeEEPROMPage(16*page, numero_page);
delay(10);
uint8_t page_lue[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
capteur.readEEPROMPage(16*page, page_lue);
Serial.print("Lecture page EEPROM: ");
for(int i=0; i<16; i++) 
  {
     Serial.print(page_lue[i]); 
     Serial.print(" ");
  }

Serial.println("");
delay(1000);                                 
}
