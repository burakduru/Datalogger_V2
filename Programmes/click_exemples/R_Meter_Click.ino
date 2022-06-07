/************************************************************************
*                                                                                                                                        
*    Test du module "R Meter Click"                                            
*                                                                    
*************************************************************************
* Description:  MIKROE-2396                                               
* La valeur de la resistance est affichée sur le moniteur série.
*  
* Matériel
*        1. Arduino Uno                                       
*        2. Shield "Click" pour arduino UNO
*        3. Module "R Meter Click" inséré sur le support N°1 du shield
*        
***********************************************************************/

// Affectation des broches
#define AN A0                          // broche de sortie du module
#define S1 6
#define S2 2
#define S3 17

int valeur;
unsigned long somme=0;
unsigned long moyenne;
float tension;
float Vin=86;                           // Vin doit être égale à 100 mV mais il y a une disparité selon les modules
float R_mesuree;
float R;

void mesure_tension ()
{
for (int i=0; i<10; i++)
{
valeur=analogRead(AN);                 // conversion AN     
somme=somme+valeur;
moyenne=somme/10;
delay(10);
}
tension=(moyenne*5000.00)/1024;        // calcul de la tension
somme=0;
return tension;
}

void calcul_resistance ()
{
R_mesuree=((tension/Vin)-1)*R;         // calcul de la resistance de contre-réaction de l'amplificateur
R_mesuree=R_mesuree-2000;              // la resistance à mesurer est en série avec deux resistance de 1KΩ
return R_mesuree;
}

void setup()
{
Serial.begin(9600);                    // initialisation de la liaison série 
pinMode(S1,OUTPUT);                    // configuration des broches S1 à S3 en sortie
pinMode(S2,OUTPUT);
pinMode(S3,OUTPUT);
}

void loop()
{
// S1 est actif, la gamme de mesure des resistances est compris entre 0 et 1KΩ
R=200;
digitalWrite(S1,HIGH);
digitalWrite(S2,LOW);
digitalWrite(S3,LOW);
mesure_tension();
if(tension>4000)
{
R_mesuree=0;
}
if(tension<2000)                      
{
 calcul_resistance (); 
}
else 
{
// S2 est actif, la gamme de mesure des resistances est compris entre 1KΩ et 100KΩ
R=1000;
digitalWrite(S1,LOW);
digitalWrite(S2,HIGH);
digitalWrite(S3,LOW);
mesure_tension();
    if(tension<2000)
    {
    calcul_resistance (); 
    }
    else 
    {
    // S3 est actif, la gamme de mesure des resistances est compris entre 100KΩ et 1MΩ
    R=100000;
    digitalWrite(S1,LOW);
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    mesure_tension();
      if(tension<2000)
      {
      calcul_resistance (); 
      }  
    }
}
Serial.print("Tension=");
Serial.print(tension);
Serial.print(" mV");
Serial.print("\t");
Serial.print("Resistance=");
Serial.print(R_mesuree);
Serial.println(" Ohm");
Serial.println(" ");
tension=0;
delay(1000);
}




