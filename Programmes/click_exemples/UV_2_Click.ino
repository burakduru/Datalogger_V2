/************************************************************************
*                                                                                                                                        
*    Test du module "UV 2 Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2378                                               
* Les UVA, UVB et Index UV sont affichés dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Module "UV 2 Click" 
*        3. Shifter Level TXB0108
*        
*        
* Bibliothèque
*        1. https://github.com/sparkfun/SparkFun_VEML6075_Arduino_Library
*        
***********************************************************************/

// Appel des bibliothèques
#include <Wire.h>
#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 UV;                                      // création de l'objet UV

int UV_A;
int UV_B;
int UV_index;

void setup()
{
Serial.begin(9600);                               // initialisation de la liaison série
Wire.begin();                                     // initialisation de la liaison I2C
UV.begin();                                       // initialisation de UV      
}

void loop()
{
// Acquisition des grandeurs
UV_A = UV.uva();    
UV_B = UV.uvb();
UV_index = UV.index();
// Affichage
Serial.print("UVA=");
Serial.println(UV_A);
Serial.print("UVB=");
Serial.println(UV_B);   
Serial.print("Index UV=");
Serial.println(UV_index);
delay(500);
}
