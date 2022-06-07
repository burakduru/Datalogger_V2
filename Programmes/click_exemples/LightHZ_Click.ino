/************************************************************************
*                                                                                                                                        
*    Test du module "LightHZ Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-990                                                
* Le nombre d'impulsions délivrées par le capteur en une seconde est affiché 
* dans le moniteur série et la quantité de lumière est affichée sur le bargraph.
*
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "LightHZ Click" inséré sur le support N°1 du shield       
*        4. Module "Bargraph Click" inséré sur le support N°2 du shield 
*                     
*
************************************************************************/

// Affectation des broches du module LightHZ
#define P0 6                                  // broche S0 du capteur
#define P1 14                                 // broche S1 du capteur
#define P2 10                                 // broche S2 du capteur
#define P3 17                                 // broche S3 du capteur
#define OUT 2                                 // broche de sortie du capteur
// Affectation des broches du module Bargraph
#define SDIN 11                               // broche DS des 74HC595
#define SCLK 13                               // broche SHCP des 74HC595
#define LATCH 9                               // broche STCP des 74HC595
#define DISPLAY 5                             // broche de validation du bargraph

volatile unsigned long impulsion=0;
volatile unsigned long nb_impulsion=0;
volatile unsigned long duree; 
volatile unsigned long top;
int index;
int MSB;
int LSB;

void setup() 
{
// Configuration des broches
pinMode(P0, OUTPUT);
pinMode(P1, OUTPUT);
pinMode(P2, OUTPUT);
pinMode(P3, OUTPUT);
pinMode(OUT, INPUT);
digitalWrite(P0,HIGH);                        // sensibilité du capteur
digitalWrite(P1,HIGH);
digitalWrite(P2,HIGH);                        // facteur de multiplication de la fréquence de sortie du capteur
digitalWrite(P3,HIGH);
pinMode(SDIN, OUTPUT);
pinMode(SCLK, OUTPUT);
pinMode(LATCH, OUTPUT);
pinMode(DISPLAY, OUTPUT);
digitalWrite(DISPLAY,HIGH);                  // validation du bargraph
Serial.begin(9600);
attachInterrupt(0, comptage, RISING);        // autorisation des interruptions sur la broche 2 de l'Arduino
}

void loop() 
{
Serial.print("Nb impulsions:");
Serial.println(nb_impulsion);
// Calcul d'un index en fonction du nombre d'impulsions recues
if ((nb_impulsion>0)&&(nb_impulsion<1500))
{
  index=0;
}
if ((nb_impulsion>1500)&&(nb_impulsion<3000))
{
  index=1;
}
if ((nb_impulsion>3000)&&(nb_impulsion<4500))
{
  index=3;
}
if ((nb_impulsion>4500)&&(nb_impulsion<6000))
{
  index=7;
}
if ((nb_impulsion>6000)&&(nb_impulsion<7500))
{
  index=15;
}
if ((nb_impulsion>7500)&&(nb_impulsion<9000))
{
  index=31;
}
if ((nb_impulsion>9000)&&(nb_impulsion<10500))
{
  index=63;
}
if ((nb_impulsion>10500)&&(nb_impulsion<12000))
{
  index=127;
}
if ((nb_impulsion>12000)&&(nb_impulsion<13500))
{
  index=255;
}
if ((nb_impulsion>13500)&&(nb_impulsion<15000))
{
  index=511;
}
if ((nb_impulsion>15000))
{
  index=1023;
}
// Gestion du bargraph
LSB=index&0x00FF;                             // récupération des 8 bit de poids faibles de la variable i
MSB=index&0xFF00;                             // récupération des 8 bit de poids forts de la variable i
MSB=MSB>>8;                                   // décalage des 8 bits de poids forts pour les mettre au format octet
digitalWrite(LATCH, LOW);                     // blocage du registre de sortie des 74HC595
shiftOut(SDIN, SCLK, MSBFIRST, MSB);          // envoi des 8 bits de poids forts sur le second 74HC595
shiftOut(SDIN, SCLK, MSBFIRST, LSB);          // envoi des 8 bits de poids faibles sur le premier 74HC595
digitalWrite(LATCH, HIGH);                    // déblocage du registre de sortie des 74HC595
}

// Programme d'interruption
void comptage()                    
{
 impulsion++;                                 // incrémentation du nombre d'impulsions sur front montant
 duree=millis();
  if(duree-top>=1000)                         // si la mesure du temps est supérieure ou égale à une seconde
    {
      nb_impulsion=impulsion;  
      impulsion=0;                            // RAZ de la variable de stockage des impulsions
      top=millis();                           // déclenchement du début du chronométrage
    }
 return;
}
