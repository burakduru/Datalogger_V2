/************************************************************************
*                                                                                                                                        
*    Test du module "LDC1000 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1583                                                
* Le passage d'une pièce métallique devant le capteur incrémente une variable 
* qui est affichée dans le moniteur série.
* 
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "LDC1000 Click" inséré sur le support N°1 du shield
*      
************************************************************************/

#include <SPI.h>        // appel de la bibliothèque

// Affectation des broches
#define CS 10           // broches de la liaison SPI
#define MOSI 11
#define MISO 12       
#define SCK 13     

// Configuration des registres
#define POWER_CONFIGURATION 0x0B
#define RP_MAX 0x01
#define RP_MIN 0x02
#define INTB_CONFIGURATION 0x0A
#define COMPARATOR_TRESHOLD_HIGH_LSB 0x06
#define COMPARATOR_TRESHOLD_HIGH_MSB 0x07
#define COMPARATOR_TRESHOLD_LOW_LSB 0x08
#define COMPARATOR_TRESHOLD_LOW_MSB 0x09
#define PROXIMITY_LSB 0xA1
#define PROXIMITY_MSB 0xA2

unsigned int donnee=0;
unsigned int donnee_LSB = 0;
unsigned int donnee_MSB = 0;
unsigned int comptage=0;

void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
// Configuration des broches
pinMode(CS, OUTPUT);
pinMode(MOSI, OUTPUT);
pinMode(SCK, OUTPUT);
pinMode(MISO, INPUT);
// Configuration de la liason SPI
SPI.begin(); 
SPI.setBitOrder(MSBFIRST);
SPI.setDataMode(SPI_MODE0); 
SPI.setClockDivider(SPI_CLOCK_DIV4); 
// Configuration du LDC1000
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(POWER_CONFIGURATION);             // écriture dans le registre POWER_CONFIGURATION
SPI.transfer(0x00);                            // mode Standby 
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(RP_MAX);                          // écriture dans le registre RP_MAX
SPI.transfer(0x0E);                            // 83.111 KΩ 
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(RP_MIN);                          // écriture dans le registre RP_MAX
SPI.transfer(0x3B);                            // 2.394 KΩ 
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(INTB_CONFIGURATION);              // écriture dans le registre INTB_CONFIGURATION
SPI.transfer(0x00);                            // mode d'interruption dévalidé 
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(COMPARATOR_TRESHOLD_HIGH_LSB);    // écriture dans le registre COMPARATOR_TRESHOLD_HIGH_LSB
SPI.transfer(0x50);                             
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(COMPARATOR_TRESHOLD_HIGH_MSB);    // écriture dans le registre COMPARATOR_TRESHOLD_HIGH_MSB
SPI.transfer(0x14);                             
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(COMPARATOR_TRESHOLD_LOW_LSB);     // écriture dans le registre COMPARATOR_TRESHOLD_LOW_LSB
SPI.transfer(0xC0);                             
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(COMPARATOR_TRESHOLD_LOW_MSB);     // écriture dans le registre COMPARATOR_TRESHOLD_LOW_MSB
SPI.transfer(0x12);                            
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(POWER_CONFIGURATION);             // écriture dans le registre POWER_CONFIGURATION
SPI.transfer(0x01);                            // mode actif
digitalWrite(CS,HIGH);                         // fin de la communication SPI
}

void loop() 
{
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(PROXIMITY_LSB);                   // lecture dans le registre PROXIMITY_LSB
donnee_LSB=SPI.transfer(0x00);                 // récupération de l'octet de poids faible de la donnée
digitalWrite(CS,HIGH);                         // fin de la communication SPI
digitalWrite(CS,LOW);                          // début de la communication SPI
SPI.transfer(PROXIMITY_MSB);                   // lecture dans le registre PROXIMITY_MSB
donnee_MSB=SPI.transfer(0x00);                 // récupération de l'octet de poids fort de la donnée
digitalWrite(CS,HIGH);                         // fin de la communication SPI
donnee=(donnee_MSB << 8) | (donnee_MSB);       // reconstitution de la donnée sur deux octets
if (donnee>9000)                               // si détection de pièce
{
  comptage=comptage+1; 
  delay(400);                                  // pause pour éviter de compter la même pièce plusieurs fois
}
// Affichage dans le moniteur série
Serial.print("Valeur : ");
Serial.print(donnee);
Serial.print("\t");
Serial.print("Nombre de pieces : ");
Serial.println(comptage);
}
