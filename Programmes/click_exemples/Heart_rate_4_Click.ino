/************************************************************************
*                                                                                                                                        
*    Test du module "Heart rate 4 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2510                                                
* La fréquence cardiaque est affichée dans le moniteur série.
* 
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Heart rate 4 Click" inséré sur le support N°1 du shield
*        
* Bibliothèques 
*        1. https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library
*       
*  
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 capteur;                           //création de l'objet
long irValue;
const byte RATE_SIZE = 4; 
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0; 
long delta;
float beatsPerMinute;
int beatAvg;


void setup(void) 
{
Serial.begin(9600);
Serial.print("Initialisation du capteur....");
    if (!capteur.begin()) 
    {
        Serial.println("ECHEC");
        while(1);
    } 
    else 
    {
        Serial.println("SUCCES");
    }
delay(2000);
capteur.setup();                          // configuration du capteur
capteur.setPulseAmplitudeRed(0xFF);       // allumage de la led rouge du capteur
capteur.setPulseAmplitudeGreen(0);        // extinction de la led verte du capteur
}

void loop() 
{
irValue = capteur.getIR();                            // mesure de la quantité IR
if (checkForBeat(irValue) == true)                    // si un battement a été détecté
  {
    delta = millis() - lastBeat;                      // calcul de la durée entre chaque battement
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);           // calcul du nombre de battements par minute

// Calcul de la moyenne des battements
    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; 
      rateSpot %= RATE_SIZE; 
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Moyenne BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print("            Placer le doigt correctement sur le capteur ");

  Serial.println();
}
