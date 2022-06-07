#include <ADC.h>
  #include <SPI.h>
  //#define SerialNMEA  Serial
  #define TAILLE_NMEA 1024  
  //e Port_c=pinMode(23,OUTPUT)
  
  
  
  // DECLARATION DE VARIABLES ::::
  const int SEL_0=21;
  const int SEL_1=22;
  const int SEL_2=23;
  const int LED_VERTE=17;
  const int LED_ROUGE=15;
  const int SSpin=10; // le pin du chip select
  const int Pin_jumper=20; // jumper placé sur la pin 16(A2) de la teensy
  const int SHDN_MAX3227=18;
  const int compt=0;
  const float Vref_adc=2.5;
  const float Vref_pont=2.5;
  const float Ramp=100000; // resistance de l ampliop faisant le pont diviseur
  const float Rbas=680000;//  resistance du pont diviseur de l'ampli du bas
  const float Ro=10000; // resistance equivalente a 25 degre
  const float Beta=3654; // coefficient carcteristique
  const float To = 298.15; // 25 degre temperature de reference
  const float A = 0.0010295; // coefficient de la loi steinhart and hart
  const float B = 0.0002391;
  const float C = 1.568*pow(10,-07);
  const float Res_1_pont=11; // reseau 1 de pont diviseur de tension en entree ( pour les hautes tensions)
  const float Res_2_pont=4.9; // reseau 2 de pont diviseur de tension en entree ( pour les basses tensions)
  const float pont_5V_monitor=3;// Pont diviseur de 5V en entree 
  char trame_nmea[TAILLE_NMEA];
  char nmea_data[TAILLE_NMEA];
  int groupe=1; 
  char buffer_debug[200];
  const int resolution_adc=16; // resolution du CAN
  int test=1; // variable qui va permettre d'effectuer le calcul de la vitesse 1 seule fois car on ne peut pas le faire plusieurs fois : on utilise millis et ça n'arrete pas d'augmenter => vitesse augmente 
  int port=0; // sur quel port nous allons recuperer la valeur numerique correspondant a la temperature
  float temperature;
  ADC *adc=new ADC();
  int pin=-1;
  int pass=0;
  float t_dernier_envoi=0;
  int port_serie_output_nmea = 1;
  //const int Port_serie=20;
  float YSI1[8]; //   variable qui va prendre la temperature reseau de thermistance goupe 1
  float YSI2[8]; //   variable qui va prendre la temperature reseau de thermistance goupe 2
  float U[8]; // variable qui va prendre la temperature reseau de tension goupe 3
  int crc;
  
  
  
  // DECLARATION DE FONCTIONS :::
  float convert_numerique_to_tension(void);
  float convert_tension_to_resistance(float);
  float convert_resistance_to_temperature(float);
  float moyennage(bool); // declaration de la fonction moyennage qui nous retourne la temperature moyenne de 15 valeurs
  void calcul_vitesse_adc(void);
  void multiplex(int);
  unsigned short CAN_ext(void);
  //void thermistance_1(float); // fonction qui va gere l'affectation de la temperature de chaque
  //void thermistance_2(float);
  //void tension_3(float);
  void gerer_trame_nmea(float * T1,float  * T2, float * U,float temps);
  void trame_nmea_crc(char * nmea_data,int crc);
  int nmea0183_checksum(char *nmea_data);
  
  
  
  
  // MAIN ARDUINO :::
  void setup() {
    // put your setup code here, to run once:
    pinMode(Pin_jumper,INPUT);
    pinMode(SHDN_MAX3227,OUTPUT);
    //pinMode(14,OUTPUT);
    int jumper=digitalRead(Pin_jumper);
    jumper=0;
    //Serial1.println(jumper);
    if(jumper==1)
      {
        port_serie_output_nmea = 1;
        digitalWrite(SHDN_MAX3227,HIGH);
      }
    else
     {
        port_serie_output_nmea = 0;
        digitalWrite(SHDN_MAX3227,LOW); // on éteint le MAX3227, pour consommer moins d'électricité.
     }
    Serial.begin(9600);
    if(port_serie_output_nmea==1)
      {
        Serial1.begin(9600);
      }
   Serial1.begin(9600);
    
    SPI.begin();// initialise la SPI 
    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0));// MSBFIRST signifie qu on envoie le bit de poids fort en premier
    analogReadResolution(resolution_adc); // fixation de la resolution du CAN
    //analogWriteResolution(16);
    pinMode(SEL_0,OUTPUT);
    pinMode(SEL_1,OUTPUT);
    pinMode(SEL_2,OUTPUT);
    pinMode(SSpin,OUTPUT);
    pinMode(LED_VERTE,OUTPUT);
    pinMode(LED_ROUGE,OUTPUT);
   
    
    
   // adc->adc1->setAveraging(16);
    //adc->adc1->setReference(ADC_REFERENCE::REF_EXT);
    
   // analogReference(EXTERNAL);
  }
  
  void loop() {
  
    //digitalWrite(14,HIGH);
   // put your main code here, to run repeatedly:
   /*unsigned short mot=CAN_ext();
   sprintf(buffer_debug,"Le mot est donc : %d",mot);
   SerialNMEA.println(buffer_debug);  */  
   /*if(pass==0)
   {  
      pass=1;
      
      do
      {
        SerialNMEA.println("Entrez une valeur entre 0 et 7 : ");
        delay(3000);
        if(Serial.available()>0)
        {
          pin=Serial.read();
          pin=pin-'0';
          sprintf(buffer_debug,"pin= %d",pin);
          SerialNMEA.println(buffer_debug);
          //delay(1000);
        }
      } while(pin>7 || pin<0);
   }*/
   
    //multiplex(pin);
   multiplex(port);
   delay(25);
   if(groupe==1 || groupe==2)
   {
    temperature=moyennage(true);
   }
   else
   {
    temperature=moyennage(false);
   }
   
  // digitalWrite(LED_VERTE,HIGH);
   //SerialNMEA.println(temperature);
   if(groupe==1) // groupe correspond au reseau de thermistance ou tension
   {
    
     YSI1[port]=temperature;
     if (port==7)
     {
         /*Serial1.println("groupe1=");
         Serial1.println(groupe);*/
         groupe=groupe+1;
     }
   }
   else if (groupe==2)
   {
    //SerialNMEA.println("je suis là");  
    YSI2[port]=temperature;
    if (port==7)
     {
         /*Serial1.println("groupe2=");
         Serial1.println(groupe);*/
         groupe=groupe+1;
     }
   }
   
   else if (groupe==3)
   {
     U[port]=temperature;
     
     if (port==7)
     {
      //nmea fonction,,, je ne fais pas un autre else if car sinon on perdra le temps d'une boucle et donc une valeur de temperature
      float temps=(float) millis();
      
       do 
       {
        // Serial1.println("je suis là");
         temps=(float) millis();
       }while((temps-t_dernier_envoi)<1000); // le reste de temps par 1000 pour avoir seconde
       float temps_secondes=temps/1000;
         digitalWrite(LED_VERTE,HIGH);
       gerer_trame_nmea(YSI1,YSI2,U,temps_secondes);
       
       //digitalWrite(LED_VERTLOWE,);
       /*if(port_serie_output_nmea==1)
       Serial1.println(trame_nmea);
       else
       Serial.println(trame_nmea);*/
       //SerialNMEA.println(nmea_data);
      
       t_dernier_envoi=temps;
       crc=nmea0183_checksum(nmea_data);
       if(port_serie_output_nmea==1)
          {
           Serial1.print("CRC= ");
           Serial1.println(crc);
          }
       else if(port_serie_output_nmea==0)
       {
           Serial.print("CRC= ");
           Serial.println(crc);
       }   
       trame_nmea_crc(nmea_data,crc);
        if(port_serie_output_nmea==1)
          {
            Serial1.println(trame_nmea);
            Serial1.println("I'AM HERE !!!!!");
            Serial1.println(digitalRead(Pin_jumper));
            
          }
        else if(port_serie_output_nmea==0)
         {
           Serial.println(trame_nmea);
         }   
      
       groupe=1;
       delay(10);
     }
     
   }
   //Serial1.println(trame_nmea);
  // sprintf(buffer_debug,"Multiplexeur %d sa température est de : %f",port,temperature);
   //SerialNMEA.println(buffer_debug);
   //Serial.println(temperature);
  // delay(1000);
   //pass=0;
   if(port>=7)
    {
      port=0;
    }
    else
    {
      port++;
    }
   
  
   //  analogWrite(2,32768);
  
    
  
  
  
  /* if(test==1)
   {
    calcul_vitesse_adc();
    test=2; 
   }*/
      
  }
  
  //FONCTION :::
  
  
  unsigned short CAN_ext(void)
  {
    digitalWrite(SSpin,LOW); // on mets à 1 le CHIP SELECT
   switch (groupe)
    {
      case 1 :
        SPI.transfer(B10000100); // conversion des données sur le  channel 0
        break;
      case 2 :
        SPI.transfer(B11000100); // conversion des données sur le channel 1
        break;
      case 3 :
        SPI.transfer(B10010100); // conversion des données sur le channel 2
        break;
       default :
        break;
    }
        
   // SPI.transfer(B11000100); // conversion des données sur le channel 1
    
    delayMicroseconds(10);
    unsigned char msb=SPI.transfer(0); // de 15 à 9
    unsigned char lsb=SPI.transfer(0);// de 8 à 1
    unsigned char lsb_2=SPI.transfer(0); //  le bit 0 car le premier clock sert à pousser les données (voir doc)
    digitalWrite(SSpin,HIGH); // on deselectionne le CHIP SELECT
    delayMicroseconds(10);
    if(lsb_2&0x80)
    {
      lsb_2=1;
    }
    else
    {
      lsb_2=0;
    }
    sprintf(buffer_debug,"msb = %d lsb = %d, et le bit(0)= %d",msb,lsb,lsb_2);
   // SerialNMEA.println(buffer_debug);
    msb=msb&0x7F; // on vire le bit le plus fort 
    
    return (msb*256*2+lsb*2+lsb_2);
    
  }
  float convert_numerique_to_tension(void) // fonction retournant la conversion de la valeur numerique du port A0 en tension
  {
    int i;
    float valeur,vs=0,vf;
    for(i=0;i<15;i++)
    {   
      
      //valeur=analogRead(A0);
      valeur=CAN_ext();
      //SerialNMEA.println(valeur);
      delayMicroseconds(15);
      vs=vs+valeur;
    }
    vf=vs/15;
    delayMicroseconds(10);
    //SerialNMEA.println("La valeur est : ");
    //SerialNMEA.println(vf);
    int cadrage=pow(2,resolution_adc); // cadrage est represente la valeur en bit que prendra la tension de reference
    float tension= vf*Vref_adc /cadrage ; // echantillonage on a un 0.005 car on avait une petite erreur de tension 
    if(groupe==1){
      if(port==5){
        
        Serial1.println(tension);
      }
    }
    //sprintf(buffer_debug,"Tension est egale a %f",tension);
    //SerialNMEA.println(buffer_debug);
    return tension;
  }
  float convert_tension_to_resistance(float tension) // fonction retournant la conversion de la tension en resistance
  {
  
     float R = Ramp*tension*Rbas/(Vref_pont*Rbas-tension*(Ramp+Rbas)); // calcul de la resistance equivalente de la tension a l aide du point diviseur du schema
     delayMicroseconds(10);
     //sprintf(buffer_debug,"Resistance = %f",R);
     //SerialNMEA.println(buffer_debug);
     if(groupe==1){
         
      if(port==0){
        
        Serial1.println(R);
      }
      
      
    }
     return R;
  }
  float convert_resistance_to_temperature(float R) //fonction retournant la conversion de la resistance en temperature
  {
      //SerialNMEA.println("Je suis ici");
      float T=1/(A+B*log(R)+C*pow(log(R),3))-273.15; // loi steinhart and hart
      delayMicroseconds(10);
      return T;
  }
  
  float moyennage(bool temperature_bool) // moyennage sur 15 valeurs
  {
    
     float tension=convert_numerique_to_tension(); // on recuperer la tension du port correspondant
     if(temperature_bool==true)
     {
     float resistance=convert_tension_to_resistance(tension);
     float Tmoy=convert_resistance_to_temperature(resistance);
     return Tmoy;
     }
     else
     {
      if(port==0 || port==1 || port==2 || port==3 )// si on est dans le reseau 1 ( les hautes tensions) 
      {    
        return (Res_1_pont*tension); 
      }
      else if ( port==4 || port==5 || port==6 )// si on est dans le reseau 2 ( les basses tensions + Vbat) 
      {
        if(port==6 && Res_2_pont*tension<1.8) //  pour Vbat : si la pile est débranché ou que la tension est trop grande on allume un voyant rouge
        {
          digitalWrite(LED_ROUGE,HIGH);
        }
        else if (port==6 && Res_2_pont*tension>1.8)
        {
          digitalWrite(LED_ROUGE,LOW);
        }
        
        return (Res_2_pont*tension);
      }
      else
      {
        return (pont_5V_monitor*tension);// si on est dans le pont diviseur avec pour tension 5V
      }
     }
  }
  void calcul_vitesse_adc(void)
  {
    float n=0.0;
    unsigned long Tb=millis();
    
    do 
    {
        int valeur=analogRead(A1);
        n=n+1.0;
    } while(n!=1000000); // on prends 1000 valeurs du port
    unsigned long Ta=millis();
    unsigned long temps=Ta-Tb;
  //  Serial.println(temps);
    float Timer=(float)temps;
    float vitesse=n/(Timer*pow(10.0,-3)); // on divise les mille valeurs par le temps pour obtenir la vitesse
    float t_convert=Timer/n; // pour avoir le temps de conversion on divise le temps par le nombre de valeur
     if(port_serie_output_nmea==1)
     {
      Serial1.println("La vitesse est de : ");
      Serial1.println(vitesse);
      Serial1.println(" valeurs/s");
      Serial1.println("Le temps de conversion est de : ");
      Serial1.println(t_convert,5);
      Serial1.println(" ms");
     }
     else if(port_serie_output_nmea==0)
     {
        Serial.println("La vitesse est de : ");
        Serial.println(vitesse);
        Serial.println(" valeurs/s");
        Serial.println("Le temps de conversion est de : ");
        Serial.println(t_convert,5);
        Serial.println(" ms");
     }
    
  }
  void  multiplex(int Port)
  { 
    switch(Port)
    {
      case 0 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,LOW);
       // SerialNMEA.println("Multiplexeur 0 !!!");
        
        break;
       case 1 :
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,LOW);
        //SerialNMEA.println("Multiplexeur 1 !!!");
        
        break;
       case 2 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,LOW);
      //  SerialNMEA.println("Multiplexeur 2 !!!");
        
        break;
       case 3 :
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,LOW);
        //SerialNMEA.println("Multiplexeur 3 !!!");
        
        break;
       case 4 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,HIGH);
      //  SerialNMEA.println("Multiplexeur 4 !!!");
        
        break;
       case 5 :
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,HIGH);
        //SerialNMEA.println("Multiplexeur 5 !!!");
        break;
       case 6 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,HIGH);
        //SerialNMEA.println("Multiplexeur 6 !!!");
        break;
       case 7 :
       // Serial.println(SEL_0);
        
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,HIGH);
        //SerialNMEA.println("Multiplexeur 7 !!!");
        break;
       default :
         //SerialNMEA.println("Multiplexeur 8 !!!");
        break;
       
    }
    
  }
  void gerer_trame_nmea(float T1[8],float  T2[8], float U[8],float temps)
  {  
    
    int longueur=snprintf(nmea_data,TAILLE_NMEA,"$DTUDIAL,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,",temps,T1[0],T1[1],T1[2],T1[3],T1[4],T1[5],T1[6],T1[7],T2[0],T2[1],T2[2],T2[3],T2[4],T2[5],T2[6],T2[7],U[0],U[1],U[2],U[3],U[4],U[5],U[6],U[7]);
    
  }
  int nmea0183_checksum(char *nmea_data)
  {
      int crc = 0;
      int i;
  
      // the first $ sign and the last two bytes of original CRC + the * sign
      for (i = 0; i < strlen(nmea_data); i ++) {
          crc ^= nmea_data[i];
      }
  
      return crc;
  }
  void trame_nmea_crc(char * nmea_data,int crc)
  {
     int longueur=snprintf(trame_nmea,TAILLE_NMEA,"%s*%.2x",nmea_data,crc);
  }
  /*void thermistance_1(float T,int Port)
  {
      
  switch(Port)
    {
      case 0 :
        YSI1[0]=T;
        break;
      case 1 :
        YSI1[1]=T;
        break;
      case 2 :
        YSI1[2]=T;
        break;
      case 3 :
        YSI1[3]=T;
        break;
      case 4 :
       YSI1[4]=T;
        break;
      case 5 :
        YSI1[5]=T;
        break;
      case 6 :
        YSI1[6]=T;
        break;
      case 7 :
        YSI1[7]=T;
        break;
      default :
         SerialNMEA.println("Multiplexeur 8 !!!");
        break;
  
    }
  }
  
  void thermistance_2(float T,int Port)
  {
      
  switch(Port)
    {
      case 0 :
        YSI2[0]=T;
        break;
      case 1 :
        YSI2[1]=T;
        break;
      case 2 :
        YSI2[2]=T;
        break;
      case 3 :
        YSI2[3]=T;
        break;
      case 4 :
       YSI2[4]=T;
        break;
      case 5 :
        YSI2[5]=T;
        break;
      case 6 :
        YSI2[6]=T;
        break;
      case 7 :
        YSI2[7]=T;
        break;
      default :
         SerialNMEA.println("Multiplexeur 8 !!!");
        break;
  
    }
  }
  void tension_3(float T,int Port)
  { 
      switch(Port)
      {
       case 0 :
         U[0]=T;
         break;
      case 1 :
        U[1]=T;
        break;
      case 2 :
        U[2]=T;
        break;
      case 3 :
        U[3]=T;
        break;
      case 4 :
       U[4]=T;
       break;
      case 5 :
        U[5]=T;
        break;
      case 6 :
        U[6]=T;
        break;
      case 7 :
        U[7]=T;
        break;
      default :
         SerialNMEA.println("Multiplexeur 8 !!!");
        break;
        
      }
  }*/
