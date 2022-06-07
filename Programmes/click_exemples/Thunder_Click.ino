/************************************************************************
*                                                                                                                                        
*    Test du module "Thunder Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-1444                                               
* La détection de foudre est affichée dans le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "Thunder Click" inséré sur le support N°1 du shield    
*        
*        
* Bibliothèque
*        1. https://github.com/raivisr/AS3935-Arduino-Library
*               
***********************************************************************/

// Appel des bibliothèques
#include <SPI.h>
#include <AS3935.h>
 
void printAS3935Registers();                         // Sous-programme de configuration de l'AS3935
byte SPItransfer(byte sendByte);
int tunecap;
 

void AS3935Irq();                                   
AS3935 AS3935(SPItransfer,10,2); 
 
void setup()
{
Serial.begin(9600);                                 // initialisation de la liaison série
// Configuration de la liaison SPI
SPI.begin();
SPI.setDataMode(SPI_MODE1);
SPI.setClockDivider(SPI_CLOCK_DIV16);
SPI.setBitOrder(MSBFIRST);

// Configuration de l'AS3935  
AS3935.reset();
delay(10);
AS3935.setIndoors();                                // le module fonctione à l'intérieur ou s'il fonctionne à l'extérieur AS3935.setOutdoors();
AS3935.registerWrite(AS3935_NF_LEV,2);
AS3935.enableDisturbers();
printAS3935Registers();
 
attachInterrupt(0,AS3935Irq,RISING);                // autorisation des interruptions sur front montant de la broche 2 de l'Arduino  
}
 
void loop()
{
Serial.println("Tout est calme...");
delay(1000);  
}
 
void printAS3935Registers()
{
int noiseFloor = AS3935.getNoiseFloor();
int spikeRejection = AS3935.getSpikeRejection();
int watchdogThreshold = AS3935.getWatchdogThreshold();
Serial.print("Noise floor : ");
Serial.println(noiseFloor,DEC);
Serial.print("Spike rejection : ");
Serial.println(spikeRejection,DEC);
Serial.print("Watchdog threshold : ");
Serial.println(watchdogThreshold,DEC);
}

byte SPItransfer(byte sendByte)
{
  return SPI.transfer(sendByte);
}
 

void AS3935Irq()                                     // sous-programme de gestion de la détection de foudre
{
int irqSource = AS3935.interruptSource();
if (irqSource & 0b0001)
  {
   Serial.println("Niveau de bruit trop haut");
  }
if (irqSource & 0b0100)
  {
   Serial.println("Perturbation detectee");
  }
if (irqSource & 0b1000)
  {
   int strokeDistance = AS3935.lightningDistanceKm();
   if (strokeDistance == 1)
      {
       Serial.println("Attention, orage !!!");
      }
   if (strokeDistance == 63)
      { 
       Serial.println("Foudre hors de portee detectee.");
      }
    if (strokeDistance < 63 && strokeDistance > 1)
      {
        Serial.print("Foudre detectee a ");
        Serial.print(strokeDistance,DEC);
        Serial.println(" kilometres environ.");
      }
    }
}
