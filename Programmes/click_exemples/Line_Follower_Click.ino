/************************************************************************
*                                                                                                                                        
*    Test du module "Line Follower Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1999                                                
* Les 5 premières led du bargraph indique l'état des 5 phototransistors 
* du module Line Follower Click. Led allumée est équivalent à la détection du blanc
* et led éteinte est équivalent à la détection du noir.
* Attention, le module Line Follower ne peut pas fonctionner sur le shield Click.
*
* Matériel
*        1. Arduino Uno                                       
*        2. Module "Line Follower Click"             
*        3. Module "Bargraph Click" 
*
************************************************************************/

// Affectation des broches du module Line Follower Click
#define OUT1 A3                               // broche OUT1 du module
#define OUT2 A0                               // broche OUT2 du module
#define OUT3 10                                // broche OUT3 du module
#define OUT4 2                                // broche OUT4 du module
#define OUT5 6                                // broche OUT5 du module
// Affectation des broches du module Bargraph
#define SDIN 11                               // broche DS des 74HC595
#define SCLK 13                               // broche SHCP des 74HC595
#define LATCH 9                               // broche STCP des 74HC595
#define DISPLAY 5                             // broche de validation du bargraph

boolean L1;
boolean L2;
boolean L3;
boolean L4;
boolean L5;
int index;
int MSB;
int LSB;


void setup() 
{
// Configuration des broches
pinMode(OUT1, INPUT);
pinMode(OUT2, INPUT);
pinMode(OUT3, INPUT);
pinMode(OUT4, INPUT);
pinMode(OUT5, INPUT);
pinMode(SDIN, OUTPUT);
pinMode(SCLK, OUTPUT);
pinMode(LATCH, OUTPUT);
pinMode(DISPLAY, OUTPUT);
digitalWrite(DISPLAY,HIGH);                  // validation du bargraph
}

void loop() 
{
L1=!(digitalRead(OUT1));
L2=!(digitalRead(OUT2));
L3=!(digitalRead(OUT3));
L4=!(digitalRead(OUT4));
L5=!(digitalRead(OUT5));
index=16*L5+8*L4+4*L3+2*L2+L1;                // élaboration d'un index en fonction de l'état des phototransistors

// Gestion du bargraph
LSB=index&0x00FF;                             // récupération des 8 bit de poids faibles de la variable i
MSB=index&0xFF00;                             // récupération des 8 bit de poids forts de la variable i
MSB=MSB>>8;                                   // décalage des 8 bits de poids forts pour les mettre au format octet
digitalWrite(LATCH, LOW);                     // blocage du registre de sortie des 74HC595
shiftOut(SDIN, SCLK, MSBFIRST, MSB);          // envoi des 8 bits de poids forts sur le second 74HC595
shiftOut(SDIN, SCLK, MSBFIRST, LSB);          // envoi des 8 bits de poids faibles sur le premier 74HC595
digitalWrite(LATCH, HIGH);                    // déblocage du registre de sortie des 74HC595
delay(10);
}


