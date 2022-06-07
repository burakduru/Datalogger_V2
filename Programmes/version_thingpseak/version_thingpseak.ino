/*-----------------------------------------------------------------------------------*/
/* BIBLOTHEQUES*/
#include <SPI.h>
#include <Ethernet.h>
#include <SerialFlash.h>
#include "ThingSpeak.h"

/* DEFINE*/
#define SerialNMEA Serial5
#define TAILLE_NMEA 1024
#define SECRET_MAC {0x90, 0xA2, 0xDA, 0x10, 0x40, 0x4F}
#define SECRET_CH_ID 1352902      // CH_ID_Thingpseak
#define SECRET_WRITE_APIKEY "ZG0ZYJKP9T06NZG6"   // APIKEY_Thingpseak

/* ETHERNET */
byte mac[] = SECRET_MAC;
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;

/* VARIABLES*/
float temp_serv = 0;

const int SEL_0 = 21;
const int SEL_1 = 22;
const int SEL_2 = 23;

const int LED_VERTE = 35;
const int LED_ROUGE = 29;
const int LED_BLEU = 30;

const int GPIO_0 = 24;

const int PIN_CMD_RGB_BLUE=LED_BLEU;
const int PIN_CMD_RGB_RED=LED_ROUGE;
const int PIN_CMD_RGB_GREEN=LED_VERTE;

const int PIN_CMD_RELAIS_1 = 16;
const int PIN_CMD_RELAIS_2_SET = 6;
const int PIN_CMD_RELAIS_2_RESET = 5;

bool global_logger_state = true;

const int SS_PIN = 10; // CHIP SELECT
const int SHDN_MAX3227 = 18; 
const int COMPT = 0;
const int RESOLUTION_ADC = 16; // RESOLUTION DU CAN

const float VREF_ADC = 2.5;
const float VREF_PONT = 2.5;
const float RAMP = 100000; // RESISTANCE  DE l'AOP
const float RBAS = 680000; // RESISTANCE DU PONT DIV
const float RO = 10000; // RESISTANCE EQUIVALENTE 25 DEGRES
const float BETA = 3654; // COEFFICIENT CARACTERISTIQUE
const float TO = 298.15; // TEMPERATURE DE REFERENCE 25 DEGRES
const float A = 0.0010295; //COEFFICIENT LOI STEINTHART AND HART
const float B = 0.0002391; 
const float C = 1.568*pow(10,-07);
const float RES_1_PONT = 11; // RESEAU 1 DE PONT DIV EN ENTREE (POUR LES HAUTES TENSIONS)
const float RES_2_PONT = 4.9; // RESEAU 2 (POUR LES BASSES TENSIONS)
const float PONT_5V_MONITOR = 3; // PONT DIV 5V EN ENTREE

char TRAME_NMEA[TAILLE_NMEA];
char NMEA_DATA[TAILLE_NMEA];
char BUFFER_DEBUG[200];

int TEST = 1; // VARIABLE PERMET EFFECTUER CALCUL DE VITESSE 1 FOIS 
int PORT = 0; // PORT RECUPERER VALEUR NUM CORRESPONDANT A LA TEMPERATURE
int GROUPE = 3;
int PIN = -1;
int PASS = 0;
int PORT_SERIE_OUTPUT_NMEA = 1;
int CRC;

float TEMPERATURE;
float T_DERNIER_ENVOI = 0;
float YSI1[8]; // VARIABLE TEMP RESEAU THERMISTANCE GROUPE1
float YSI2[8]; // VARIABLE TEMP RESEAU THERMISTANCE GROUPE2
float U[8]; // VARIABLE TEMP RESEAU TENSION GROUPE3

/* FONCTIONS */
float CONVERT_NUM_TO_VOLTAGE(int channel_to_convert);
float CONVERT_VOLTAGE_TO_RESIST(float);
float CONVERT_RESISTANCE_TO_TEMP(float);
float MOYENNAGE(bool,int,int); // DECLARATION FONCTION MOYENNAGE RETOURNE LA TEMPERATURE MOYENNE 15 VAL

void CALCUL_VITESSE_ADC(void);
void MULTIPLEX(int);
void GERER_TRAME_NMEA(float * T1, float * T2, float * U, float TEMPS);
void TRAME_NMEA_CRC(char *NMEA_DATA, int CRC);

unsigned short Convertir_voie_ads8344(int channel);
int NMEA0183_CHECKSUM(char *NMEA_DATA);


/*-----------------------------------------------------------------------------------*/
/*MAIN*/
void setup(){
    Serial.begin(9600);
    SerialNMEA.begin(9600);
    SETUP_ETHERNET();
  
    ThingSpeak.begin(client);  // Initialize ThingSpeak
    
    pinMode(SHDN_MAX3227,OUTPUT);
    PORT_SERIE_OUTPUT_NMEA = 1;
    digitalWrite(SHDN_MAX3227, HIGH);
    Serial.begin(9600);
    Serial.println("Ethernet Webserver");
    
    SPI.begin();
    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0)); //MSBFIRST
    analogReadResolution(RESOLUTION_ADC); // FIXE RESOLUTION ADC

    pinMode(SEL_0,OUTPUT);
    pinMode(SEL_1,OUTPUT);
    pinMode(SEL_2,OUTPUT);
    pinMode(SS_PIN,OUTPUT);
    pinMode(LED_VERTE,OUTPUT);
    pinMode(LED_ROUGE,OUTPUT);


    pinMode(PIN_CMD_RELAIS_1,OUTPUT);
    pinMode(PIN_CMD_RELAIS_2_SET,OUTPUT);
    pinMode(PIN_CMD_RELAIS_2_RESET,OUTPUT);
    pinMode(GPIO_0,OUTPUT);
}

/*-----------------------------------------------------------------------------------*/
void loop (){
    scan_groupes_ports();
    delay(25);
    
    temp_serv = YSI1[0];
    Serial.println(temp_serv);
    
    int x = ThingSpeak.writeField(myChannelNumber, 1, temp_serv, myWriteAPIKey);
    if(x == 200){
    Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    
    Serial.println(NMEA_DATA); 
    delay(15000); 
}

/*-----------------------------------------------------------------------------------*/
/* FONCTIONS */
void scan_groupes_ports()
{
  float retour_groupe_1_f = 0;
  float retour_groupe_2_f = 0;
  float retour_groupe_3_f = 0;
  int numero_groupe_i=0;
    int iport_i=0;
    for(iport_i=0;iport_i<8;iport_i++)
    {
        MULTIPLEX(iport_i);
        delay(25);
        
        YSI1[iport_i] = MOYENNAGE(true,iport_i,0);//retourne une temperature YSI
        
        numero_groupe_i=2;
        retour_groupe_2_f = MOYENNAGE(true,iport_i,1);
        YSI2[iport_i] = retour_groupe_2_f;
        
        numero_groupe_i=3;
        retour_groupe_3_f = MOYENNAGE(false,iport_i,2);
        U[iport_i] = retour_groupe_3_f;
    }
   
              
    float TEMPS_SECONDES = millis() / 1000;
    
    GERER_TRAME_NMEA(YSI1,YSI2,U,TEMPS_SECONDES);
    
    if( global_logger_state == true)
      SerialNMEA.println(NMEA_DATA);
    
    CRC = NMEA0183_CHECKSUM(NMEA_DATA);
     if( global_logger_state == true)
      SerialNMEA.println(CRC);
    
   // T_DERNIER_ENVOI = TEMPS;
    
}

/*-----------------------------------------------------------------------------------*/
unsigned short Convertir_voie_ads8344(int channel)
{
    digitalWrite(SS_PIN,LOW); // ON METS A '1' SSpin
    switch(channel)
    {
        case 0 :
        SPI.transfer(B10000100); //CONVERSION CHANNEL 0
        break;
        case 1 :
        SPI.transfer(B11000100); //CONVERSION CHANNEL 1
        break;
        case 2 :
        SPI.transfer(B10010100); //CONVERSION CHANNEL 2
        break;
        default :
        SPI.transfer(B10000100); //CONVERSION CHANNEL 0
        break;
        break;
    }

    delayMicroseconds(10);
    unsigned char MSB = SPI.transfer(0); // 15 A 9
    unsigned char LSB = SPI.transfer(0); // 8 A 1
    unsigned char LSB_2 = SPI.transfer(0); // BIT 0
    digitalWrite(SS_PIN, HIGH); //SS_PIN A 0
    delayMicroseconds(10);

    if(LSB_2 & 0x80)
    {
        LSB_2 = 1;
    }
    else
    {
        LSB_2 = 0;
    }
    sprintf(BUFFER_DEBUG, "MSB = %d LSB = %d, et le bit(0) = %d",MSB,LSB,LSB_2);
    //SerialNMEA.println(BUFFER_DEBUG);
    MSB = MSB & 0x7F; // ON ENLEVE MSB
    
    return (MSB*256*2+LSB*2+LSB_2);
}

/*-----------------------------------------------------------------------------------*/
float CONVERT_NUM_TO_VOLTAGE(int channel_to_convert)
{
    int i;
    float VALEUR, VS = 0, VF;
    for(i=0;i<15;i++)
    {
        VALEUR = Convertir_voie_ads8344(channel_to_convert);
        //SerialNMEA.println(VALEUR);
        delayMicroseconds(15);
        VS = VS + VALEUR;
    }
    VF = VS/15;
    delayMicroseconds(10);
    //SerialNMEA.println("La valeur est :");
    //SerialNMEA.println(VF);
    int CADRAGE = pow(2,RESOLUTION_ADC);
    float TENSION = VF*VREF_ADC /CADRAGE;
    /*if(GROUPE == 1){
        if(PORT == 5){
            SerialNMEA.println(TENSION);
        }
    }*/
    //sprintf(BUFFER_DEBUG,"Tension est egale a %f", TENSION);
    //SerialNMEA.println(BUFFER_DEBUG);
    return TENSION;
}

/*-----------------------------------------------------------------------------------*/
float CONVERT_VOLTAGE_TO_RESIST(float TENSION)
{
    float R = RAMP*TENSION*RBAS / (VREF_PONT*RBAS-TENSION*(RAMP+RBAS));
    delayMicroseconds(10);
    /*sprintf(BUFFER_DEBUG,"Resistance = %f",R);
    SerialNMEA.println(BUFFER_DEBUG);
    if(GROUPE == 1)
    {
        if (PORT == 0){
            SerialNMEA.println(R);
        }
    }*/
    return R;
}

/*-----------------------------------------------------------------------------------*/
float CONVERT_RESISTANCE_TO_TEMP(float R)
{
    SerialNMEA.println("Je suis ici");
    float T = 1 / (A+B*log(R)+C*pow(log(R),3))-273.15;
    delayMicroseconds(10);
    Serial.println(T);
    return T;
}

/*-----------------------------------------------------------------------------------*/
float MOYENNAGE(bool TEMPERATURE_BOOL,int numero_port_max4781,int channel_ads8344)
{
    float TENSION = CONVERT_NUM_TO_VOLTAGE(channel_ads8344);
    if(TEMPERATURE_BOOL == true)
    {
        float RESISTANCE = CONVERT_VOLTAGE_TO_RESIST(TENSION);
        float TMOY = CONVERT_RESISTANCE_TO_TEMP(RESISTANCE);
        return TMOY;
        Serial.println("TRUE");
    }
    else
    {
        if(numero_port_max4781 == 0 || numero_port_max4781 == 1 || numero_port_max4781 == 2 || numero_port_max4781 == 3)
        {
            return(RES_1_PONT*TENSION);
        }
        else if(numero_port_max4781 == 4 || numero_port_max4781 == 5 || numero_port_max4781 == 6)
        {
            if(numero_port_max4781 == 6 && RES_2_PONT*TENSION<1.8)
            {
                digitalWrite(LED_ROUGE,HIGH);
            }
            else if (numero_port_max4781 == 6 && RES_2_PONT>1.8)
            {
                digitalWrite(LED_ROUGE,LOW);
            }
            return(RES_2_PONT*TENSION);
        }
        else
        {
            return(PONT_5V_MONITOR*TENSION);
        }
    }
}

/*-----------------------------------------------------------------------------------*/
void CALCUL_VITESSE_ADC(void)
{
    float N = 0.0;
    unsigned long TB = millis(); 

    do
    {
        //int valeur = analogRead();
        N = N + 1;
    } while(N!=1000000);
    unsigned long TA = millis();
    unsigned long TEMPS = TA - TB;
    float TIMER = (float)TEMPS;
    float VITESSE = N / (TIMER*pow(10.0,-3));
    float T_CONVERT = TIMER/N;
    if (PORT_SERIE_OUTPUT_NMEA == 1)
    {
        /*SerialNMEA.println("La vitesse est de : ");
        SerialNMEA.println(VITESSE);
        SerialNMEA.println(" valeurs/s");
        SerialNMEA.println("Le temps de conversion est de : ");
        SerialNMEA.println(T_CONVERT,5);
        SerialNMEA.println(" ms");*/
    }
}

/*-----------------------------------------------------------------------------------*/
void MULTIPLEX(int PORT)
{
    switch(PORT)
    {
        case 0 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,LOW);
        //Serial.println("Multiplexeur 0 !");
        break;
        case 1 :
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,LOW);
        //Serial.println("Multiplexeur 1 !");
        break;
        case 2 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,LOW);
        //Serial.println("Multiplexeur 2 !");
        break;
        case 3 :
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,LOW);
        //Serial.println("Multiplexeur 3 !");
        break;
        case 4 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,HIGH);
        //Serial.println("Multiplexeur 4 !");
        break;
        case 5 :
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,HIGH);
        //Serial.println("Multiplexeur 5 !");
        break;
        case 6 :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,HIGH);
        //Serial.println("Multiplexeur 6 !");
        break;
        case 7 :
        digitalWrite(SEL_0,HIGH);
        digitalWrite(SEL_1,HIGH);
        digitalWrite(SEL_2,HIGH);
        //Serial.println("Multiplexeur 7 !");
        break;
        default :
        digitalWrite(SEL_0,LOW);
        digitalWrite(SEL_1,LOW);
        digitalWrite(SEL_2,LOW);
        //Serial.println("Multiplexeur 8 !");
        break;
    }
}

/*-----------------------------------------------------------------------------------*/
void GERER_TRAME_NMEA(float T1[8],float T2[4],float U[8],float TEMPS)
{
  int longueur=snprintf(NMEA_DATA,TAILLE_NMEA,"$DTUDIAL,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",TEMPS,T1[0],T1[1],T1[2],T1[3],T1[4],T1[5],T1[6],T1[7],T2[0],T2[1],T2[2],T2[3],U[0],U[1],U[2],U[3],U[4],U[5],U[6],U[7]);
  
}


/*-----------------------------------------------------------------------------------*/
int NMEA0183_CHECKSUM(char *NMEA_DATA)
{
    int CRC = 0;
    int i;

    for(i=0;i<strlen(NMEA_DATA);i++)
    {
        CRC ^= NMEA_DATA[i]; 
    }
    return CRC;
}

/*-----------------------------------------------------------------------------------*/
void TRAME_NMEA_CRC(char * NMEA_DATA,int CRC)
{
   int longueur=snprintf(TRAME_NMEA,TAILLE_NMEA,"%s*%.2x",NMEA_DATA,CRC);
}
