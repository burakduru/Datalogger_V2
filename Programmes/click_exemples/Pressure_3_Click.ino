/************************************************************************
*                                                                                                                                        
*    Test du module "Pressure 3 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2293                                                
* Un tableau de 16 mesures de température et pression est affiché 
* dans le moniteur série.
*
* Matériel
*        1. Arduino Uno                                        
*        2. Module "Barometer Click" 
*        3. Shifter Level TXB0108
*        
*                
* Bibliothèques
*        1. https://github.com/Infineon/DPS310-Pressure-Sensor
*
************************************************************************/

// Appel des bibliothèques
#include <Wire.h>                           
#include <Dps310.h>

#define temp_mr  2
#define temp_osr  2
#define prs_mr  2
#define prs_osr  2

int valeur;

Dps310 capteur = Dps310();                      // création de l'objet capteur

void setup(void) 
{
Serial.begin(9600);                             // initialisation de la liaison série
Wire.begin();                                   // initialisation du bus I2C
capteur.begin(Wire, 0x76);                      // initialisation du capteur avec l'adresse 0x76
valeur = capteur.startMeasureBothCont(temp_mr, temp_osr, prs_mr, prs_osr);
}

void loop() 
{
unsigned char nb_echantillon_pression = 16;
int32_t pression[nb_echantillon_pression];
unsigned char nb_echantillon_temperature = 16;
int32_t temperature[nb_echantillon_temperature];
valeur = capteur.getContResults(temperature, nb_echantillon_temperature, pression, nb_echantillon_pression); // acquisition des valeurs de température et de pression
// Affichage
Serial.println();
Serial.print(nb_echantillon_temperature);
Serial.println(" echantillons de temperature mesures: ");
    for (int i = 0; i < nb_echantillon_temperature; i++)
    {
      Serial.print(temperature[i]);
      Serial.println(" °C");
    }

Serial.println();
Serial.print(nb_echantillon_pression);
Serial.println(" echantillons de pression mesures: ");
    for (int i = 0; i < nb_echantillon_pression; i++)
    {
      Serial.print(pression[i]);
      Serial.println(" Pa");
    }
delay(10000);                                 // attente pour acquérir les mesures
}
