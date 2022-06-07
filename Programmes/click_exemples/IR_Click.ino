/************************************************************************
*                                                                                                                                        
*    Test du module "IR Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1377                                                
* Le code et la touche correspondante de la télécommande sont affichés
* dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "IR Click" inséré sur le support N°1 du shield       
*        4. Télécommande IR https://www.lextronic.fr/leds-infrarouges/15921-telecommande-ir.html 
*                     
*                     
* Bibliothèque
*        1. https://github.com/z3t0/Arduino-IRremote
*        
************************************************************************/
// Appel de la bibliothèque
#include <IRremote.h>

// Affectation des broches 
#define OUT A0                                // broche OUT du capteur IR
                           
IRrecv reception(OUT);                        // création de l'objet reception

decode_results code;


void setup() 
{
Serial.begin(9600);                           // initialisation de la liaison série
reception.enableIRIn();                       // initialisation de la reception
Serial.println("Veuillez appuyer sur une touche de la telecommande");
delay(2000);
}

void loop() 
{
if (reception.decode(&code)) 
  {
    Serial.print(code.value, HEX);
    Serial.print("\t");
    switch (code.value)
    {
      case 0xFFA25D:
      Serial.println("Touche A");
      break;
      case 0xFF629D:
      Serial.println("Touche B");
      break;
      case 0xFFE21D:
      Serial.println("Touche C");
      break;
      case 0xFF22DD:
      Serial.println("Touche D");
      break;
      case 0xFF02FD:
      Serial.println("Touche Haut");
      break;
      case 0xFFC23D:
      Serial.println("Touche E");
      break;
      case 0xFFE01F:
      Serial.println("Touche Gauche");
      break;
      case 0xFFA857:
      Serial.println("Touche Configuration");
      break;
      case 0xFF906F:
      Serial.println("Touche Droite");
      break;
      case 0xFF6897:
      Serial.println("Touche 0");
      break;
      case 0xFF9867:
      Serial.println("Touche Bas");
      break;
      case 0xFFB04F:
      Serial.println("Touche F");
      break;
      case 0xFF30CF:
      Serial.println("Touche 1");
      break;
      case 0xFF18E7:
      Serial.println("Touche 2");
      break;
      case 0xFF7A85:
      Serial.println("Touche 3");
      break;
      case 0xFF10EF:
      Serial.println("Touche 4");
      break;
      case 0xFF38C7:
      Serial.println("Touche 5");
      break;
      case 0xFF5AA5:
      Serial.println("Touche 6");
      break;
      case 0xFF42BD:
      Serial.println("Touche 7");
      break;
      case 0xFF4AB5:
      Serial.println("Touche 8");
      break;
      case 0xFF52AD:
      Serial.println("Touche 9");
      break;
      case 0xFFFFFFFF:
      Serial.println("Relachez la touche");
      break;
    }
    reception.resume();                       // preparation de reception pour le prochain code
  }
  delay(200);
}
