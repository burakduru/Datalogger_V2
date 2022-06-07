/************************************************************************
*                                                                                                                                        
*    Test du module "Alcohol Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1586                                               
* La valeur du taux d'alcool est affiché sur le moniteur série et 
* sur le module bargraph.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Alcohol Click" inséré sur le support N°1 du shield
*        4. Module "Bargraph Click" inséré sur le support N°2 du shield
*        
***********************************************************************/

 
// Affectation des broches du module Alcohol
#define AN A0                               // broche de sortie du module
// Affectation des broches du module Bargraph
#define SDIN 11    // broche DS des 74HC595
#define SCLK 13    // broche SHCP des 74HC595
#define LATCH 9   // broche STCP des 74HC595
#define DISPLAY 5  // broche de validation du bargraph

int valeur;
int led=15;
int MSB;
int LSB;

void setup()
{
Serial.begin(9600);                         // initialisation de la liaison série 
pinMode(SDIN, OUTPUT);
pinMode(SCLK, OUTPUT);
pinMode(LATCH, OUTPUT);
pinMode(DISPLAY, OUTPUT);
digitalWrite(DISPLAY,HIGH);                  // validation du bargraph
}

void loop()
{
valeur=0;
for(int i=0;i<10;i++)                       // acquisition de 10 échantillons
  {
    valeur= valeur+analogRead(AN);
    delay(10);
  }
valeur=valeur/10;                           // calcul de la valeur moyenne
// Gestion des led du bargraph
if (valeur<199)
{
  led=0;
}
if ((valeur>200)&&(valeur<283))
{
  led=1;
}
if ((valeur>284)&&(valeur<367))
{
  led=3;
}
if ((valeur>368)&&(valeur<451))
{
  led=7;
}
if ((valeur>452)&&(valeur<535))
{
  led=15;
}
if ((valeur>536)&&(valeur<619))
{
  led=31;
}
if ((valeur>620)&&(valeur<703))
{
  led=63;
}
if ((valeur>704)&&(valeur<787))
{
  led=127;
}
if ((valeur>788)&&(valeur<871))
{
  led=255;
}
if ((valeur>872)&&(valeur<955))
{
  led=511;
}
if (valeur>956)
{
  led=1023;
}
// Gestion du bargragh
LSB=led&0x00FF;                             // récupération des 8 bit de poids faibles de la variable i
MSB=led&0xFF00;                             // récupération des 8 bit de poids forts de la variable i
MSB=MSB>>8;                                 // décalage des 8 bits de poids forts pour les mettre au format octet
digitalWrite(LATCH, LOW);                   // blocage du registre de sortie des 74HC595
shiftOut(SDIN, SCLK, MSBFIRST, MSB);        // envoi des 8 bits de poids forts sur le second 74HC595
shiftOut(SDIN, SCLK, MSBFIRST, LSB);        // envoi des 8 bits de poids faibles sur le premier 74HC595
digitalWrite(LATCH, HIGH);                  // déblocage du registre de sortie des 74HC595
// Affichage
Serial.print("Valeur = ");            
Serial.println(valeur);
delay(1000);                                // pause entre deux mesures
}




