# Datalogger_V2
![example workflow](https://github.com/burakduru/dataloggerv2/actions/workflows/main.yml/badge.svg)
__Ce répertoire Github regroupe la partie électronique et informatique embarquée de la carte d'acquistion Datalogger_V2 rédigé par Burak DURU en tant qu'apprenti ingénieur en électronique à la [Division Technique de l'INSU Meudon](https://www.dt.insu.cnrs.fr/).__

## Carte d'acquisition : 

Réalisation et mise en œuvre d'une carte d'acquisition avec un microcontrôleur [Teensy 3.6](https://www.pjrc.com/store/teensy36.html) dans le but de recevoir/traiter des valeurs de thermistances/résistances chauffantes et transmettre/reçevoir des informations via plusieurs mode de communications.

Fonctionnalités de la carte d’acquisition :
* [Socket MikroBUS](https://www.mikroe.com/mikrobus/) pour l’utilisation des modules “Click”
* Ethernet ([Module WIZ820-IO](https://www.wiznet.io/product-item/wiz820io/))
* Sortie DAC, GPIO et commande Interlock Laser
* Bus CAN
* Liaison RS232
* Connectique pour le réseau de thermistances/tensions
* Alimentation externe
* Relais

![datalogger_v2_3D](https://user-images.githubusercontent.com/106671909/172331493-1acb83ba-82b5-4351-945c-fa104018d79a.png)

## Programmes utilisés

* __Datalogger__ : *Permettant de récupérer les informations de températures et de tensions des connectiques thermistances YSI[...] et de tensions V[...] via une transmission RS232*  

```c
/*-----------------------------------------------------------------------------------*/
/* BIBLOTHEQUES*/
//#include <ADC.h>
#include <SPI.h>

/* DEFINE*/
#define SerialNMEA Serial5
#define TAILLE_NMEA 1024

/* VARIABLES*/
bool global_logger_state = true;

const int SEL_0 = 21;
const int SEL_1 = 22;
const int SEL_2 = 23;

const int LED_VERTE = 35;
const int LED_ROUGE = 29;
const int LED_BLEU = 30;

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

// FONCTIONS
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
    
    pinMode(SHDN_MAX3227,OUTPUT);
    PORT_SERIE_OUTPUT_NMEA = 1;
    digitalWrite(SHDN_MAX3227, HIGH);
    
    SPI.begin();
    SPI.beginTransaction(SPISettings(10000, MSBFIRST, SPI_MODE0)); //MSBFIRST
    analogReadResolution(RESOLUTION_ADC); // FIXE RESOLUTION ADC

    pinMode(SEL_0,OUTPUT);
    pinMode(SEL_1,OUTPUT);
    pinMode(SEL_2,OUTPUT);
    pinMode(SS_PIN,OUTPUT);
    pinMode(LED_VERTE,OUTPUT);
    pinMode(LED_ROUGE,OUTPUT);
}

/*-----------------------------------------------------------------------------------*/
void loop (){
    scan_groupes_ports();
    Serial.println(NMEA_DATA);
    delay(1000);
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
    
    //T_DERNIER_ENVOI = TEMPS;
    
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
                //digitalWrite(LED_ROUGE,HIGH);
            }
            else if (numero_port_max4781 == 6 && RES_2_PONT>1.8)
            {
                //digitalWrite(LED_ROUGE,LOW);
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
```

* __Bus CAN__ : *Permettant de recevoir/transmettre des trames de données depuis le bus de donnée CAN. (Dans notre exemple, transmission d’une donnée de compteur et de thermistances)*
* __Ethernet__ : *Programme d’exemples pour l’initialisation module Ethernet / Envoi et réception de trames UDP / Connexion à un broker MQTT et le statut de connexion* 
* __Programmes de tests__ : *Modules “Click” utilisés*
* __WIFI__ : *Programme pour le scan/connexion des réseaux et connexion à un serveur/broker MQTT* 
* __LoRa__ : *Plusieurs programmes permettant d’initialiser et d’envoyer des trames de données via le protocol de communication bas débit LoRa* 
* __AdafruitIO | ThinkSpeak__ : *Envoi/réception de données en utilisant une connexion Internet (via Ethernet ou WIFI) à un serveur MQTT* 

## Matériel

* PCB : Fichiers schématique (.schDoc) et PCB (.pcbDoc) sous Altium Designer

## Schéma fonctionnement complet

[MicroDial_Data_Log_V2_SCH.pdf](https://github.com/burakduru/Datalogger_V2/files/8852100/MicroDial_Data_Log_V2_SCH.pdf)


Guide d'utilisation 
===================

<!--ts-->
* [Installation de Teensyduino](#installation-de-teensyduino)    
* [Installation de bibliothèques sous Teensyduino](#installation-de-bibliothèques-sous-teensyduino)  
* [Modules Click Board](#modules-click-board)
* [Pinout DataloggerV2](#pinout-dataloggerV2)
* [Liaison RS232](#liaison-rs232)
* [Visual Studio](#visual-studio) 
* [Bus CAN](#bus-can)  
* [Ethernet](#ethernet)  
* [WIFI](#wifi)
* [AdafruitIO](#adafruitio) 
* [Thingspeak](#thingspeak)
* [LoRa](#lora)  
* [Bluetooth Low Energy](#bluetooth-low-energy)
<!--te-->

Installation de Teensyduino
---------------------------

Télécharger l’Add-On pour l’IDE d’Arduino en utilisant ce lien sous Windows: https://www.pjrc.com/teensy/td_download.html.
Avec Windows 10, il n’y a pas de driver à installer : On choisit donc, le chemin d’accès où se trouve arduino.exe : 

![teensyduino1](https://user-images.githubusercontent.com/106671909/172332083-79fcae2e-37c1-4916-aa26-3ab04869ae83.png)

Choisir comme type de carte sur Arduino :
Teensy 3.6 / USB Type : “Serial” / CPU Speed : “180MHz” / Port : “Numéro de Port du Teensy”

![teensyduino2](https://user-images.githubusercontent.com/106671909/172332130-be41e609-0de8-48ad-a56c-abf7fe5d57f6.png)

Installation de bibliothèques sous Teensyduino
----------------------------------------------

Toutes les bibliothèques nécessaires à l’implémentation des programmes sont disponibles dans le fichier “Bibliothèques”. Copier-coller ces librairies dans les bibliothèques source de la carte Teensy :

![biblitotheque_teensy](https://user-images.githubusercontent.com/106671909/172332763-e17d26d9-7418-440c-8dd6-14237c40ee09.png)

Programmes de tests : 
- Programme de test pour chacun des Click Board présentés
- LED RGB/ Relais/ DAC/ Bus CAN
- Programme de tests des thermistances/résistance chauffante (relevé de température et de tension)
- Plusieurs programmes pour tester la liaison RJ45 (Ethernet) (connexion à un broker MQTT/ UDP/ Webserver et l’état de l’ethernet).
- Test de la liaison WIFI Click Board (Vérification mise à jour firmware/ scan et connexion à des réseaux).
- Connection au broker MQTT IO.Adafruit et ThinkSpeak (Ethernet et WIFI)
- Envoi données de température en passant par le réseau LoRa (LoRa Click)
- Application sous Visual Studio afin de récupérer les valeurs de températures/tensions et contrôler les relais/led RGB et le DAC.

Modules Click Board
-------------------

Liste des modules MikroElektronika testés et compatible avec la carte d'acquisition : 
- [GNSS 4 Click](https://www.mikroe.com/gnss-4-click/)
- [GNSS Click](https://www.mikroe.com/gnss-click/)
- [Temp-Hum 4 Click](https://www.mikroe.com/temp-hum-4-click/)
- [Air-Quality 4 Click](https://www.mikroe.com/air-quality-4-click/)
- [Oled-W/B Click](https://www.mikroe.com/oled-w-click/)
- [WIFI 7 Click](https://www.mikroe.com/wifi-7-click/)
- [CAN SPI Click](https://www.mikroe.com/can-spi-33v-click/)
- [BLE ⅔ Click](https://www.mikroe.com/ble-p-click/)
- [LoRa Click](https://www.mikroe.com/lr-click/)

Pinout DataloggerV2
--------------------

![pinout_datalogger](https://user-images.githubusercontent.com/106671909/172334709-dba5243d-9064-475c-aca5-53261286cae3.png)

Liaison RS232
-------------

Programme __Datalogger__ regroupé en plusieurs fonctions permettant la conversion d’une valeur analogique à numérique puis cette valeur en tension, de cette tension en une valeur de résistance puis en température. Finalement ces valeurs sont transmises via la liaison RS232 ci-dessous :

![rs232](https://user-images.githubusercontent.com/106671909/172340766-75d503e6-aabd-4293-be31-35496c7b558d.png)

Visual Studio
-------------

Pour l’[utilisation](https://visualstudio.microsoft.com/fr/) de l’application Windows Forms “DataLoggerNMEA”, il faut tout d’abord implémenté le code “interface_C_sharp.ino” dans la carte Data_Logger. L’envoi de la trame NMEA comportant les valeurs de températures des thermistances et de tensions se fait toutes les 5 secondes :

Pour la réception de données envoyées depuis l’application DataLoggerNMEA, le décodage est effectué par la fonction “decoder_message_operateur” et vérifié par “crc_check”.
Depuis le moniteur série, on peut confirmer la donnée reçu par la vérification de l’entête du message (exemple : #RELAIS;2;0 ce qui correspond à une commande d’état bas au relais n°2.) et l’envoi de la trame NMEA.

![moniteur_serie](https://user-images.githubusercontent.com/106671909/172342110-12c74c61-65fc-4e02-b386-ce03dea6783a.png)

Interface de l'application :

![visual_studio](https://user-images.githubusercontent.com/106671909/172341977-24833648-a93b-49fd-901d-f0b4b682870a.png)


Bus CAN
-------

Exemple d'application du bus CAN : Implémentation d’un code de transmission et de réception de trames de données en utilisant un identificateur pour chacune des cartes avec des données de température et de compteurs pour réaliser des tests  

![datalogger_buscan_1](https://user-images.githubusercontent.com/106671909/172338673-980f9af3-078b-422a-92e9-5cd07e13cfd2.png)

![datalogger_buscan](https://user-images.githubusercontent.com/106671909/172338438-1c823222-5f7a-4a7c-a5c3-8f21b0a64516.png)

![image](https://user-images.githubusercontent.com/106671909/172338918-d32aca89-d908-472b-9454-c5b0193fbb7e.png)

Nous avons ici implémenté le code “bus_can.ino” pour la carte Data_Logger et “can_teensy4_1.ino” pour la Teensy 4.1. Lorsque l’on observe le signal CAN_H/CAN_L à l’oscilloscope, on constate deux trames de données. La première trame correspond à l’envoi depuis la carte Data_Logger et la seconde à l’envoi depuis la Teensy 4.1. On peut identifier ces 2 trames par leur ID, “0x7DF” et “0x7E8”. Pour la trame “0x7DF”, data[0] correspond à une donnée de température et data[1..7] correspond à  “2” à “8” en ASCII. Les 4 derniers octets correspondent au CRC (“cyclic redundancy check”).
Pour la trame “0x7E8”, data[0..5] correspond à la donnée recopiée à la réception de “0x7DF” et data[6..7] correspondent à des compteurs qui s’incrémentent à chaque envoi de données.

Ethernet
--------

Installation de la librairie Ethernet :
* Librairie disponible lors de l'installation de Teensyduino  
* Avec la Teensy 3.6 : 2 ports SPI matériels disponible pour notre cas, WIZNET est connecté à SPI1. Pour le bon fonctionnement du module WIZNET, il faut récupérer le fichier "Ethernet" et le copier-coller ici :  

![ethernet](https://user-images.githubusercontent.com/106671909/172344124-17801747-908b-47a3-8511-138c516a800c.png)

Initialisation du module WIZNET :
* Le code ci-dessous permet de réinitialiser le WIZNET :

![code_wiznet](https://user-images.githubusercontent.com/106671909/172344491-011cbab6-65be-4b1a-85f5-7cde4f0c7a6a.png)

* Fixer une adresse MAC et une adresse IP pour un réseau local en utilisant la fonction begin

![adresse_mac](https://user-images.githubusercontent.com/106671909/172344818-f23e0f02-5614-4126-8a7c-3df93deb1182.png)

Programmes d'exemples disponibles : 
* __wiz820io_linkstatus__ : *Confirmation connexion Ethernet*
* __wiz820io_mqtt_ethernet__ : *Exemple connexion serveur MQTT adafruit.io*
* __wiz820io_udp_send_receive__ : *Envoi/Réception trames UDP via Hercules*
* __wiz820io_webserver__ : *Création d’un serveur WEB qui renvoi des valeurs des pins analogiques*

WIFI 
----

Utilisation du module [ATWINC1510](https://www.mikroe.com/wifi-7-click/) :
* Installation de la bibliothèque du module ATWINC1510 depuis le gestionnaire de bibliothèque

![atwinc1510](https://user-images.githubusercontent.com/106671909/172352384-a76fdfb2-3c82-4efc-a831-fe7f0613be19.png)

* Vérification de la version du firmware en compilant le programme "__CheckWifi101FirmwareVersion.ino"__
* Installation du firmware WIFI depuis Arduino :

![firmware_wifi](https://user-images.githubusercontent.com/106671909/172353071-97e8f645-1cf1-4306-bd9e-ff015d17b0a6.png)

* Scan des différents réseaux disponibles en compilant le programme  "__ScanNetworksAdvanced.ino__"

![scan_wifi](https://user-images.githubusercontent.com/106671909/172353435-a53d1b6d-dbe0-4c37-8b7e-a88d5f95bfe1.png)

On obtient ici plusieurs informations concernant les réseaux :
- Nom du réseau (SSID)
- Amplitude du signal en dBm
- Channel disponible 
- Protocol de chiffrement 

* Connexion à un réseau WIFI WPA en compilant le programme "__ConnectWithWPA.ino__"
Ouvrir tout d’abord “arduino_secrets.h” puis définir le nom du réseau (secret_ssid) puis le mot de passe (secret_pass)  
Compiler finalement le programme le programme : si la connection est bien effectué, on obtient plusieurs information dont une adresse IP attribuée automatiquement au module  

* Connexion au broker MQTT IO.Adafruit via une connexion WIFI en compilant le programme ""__adafruitio_wifi.ino__"

AdafruitIO
----------

[AdafruitIO](https://io.adafruit.com/) est une plateforme IoT qui propose un service cloud permettant à la fois d’afficher/d’envoyer des informations sur une interface web avec une affichage en temps réels de données comme par exemple le contrôle de relais/LEDs pour notre cas

Utilisation d'IO.Adafruit.com :

* Création d’un compte adafruitIO sur https://io.adafruit.com
* Récupération à partir de l’interface “My Key” de l’username et de l’active key

![ioadafruit](https://user-images.githubusercontent.com/106671909/172347334-0758e24b-9c18-4d0f-bfe5-2447485c1ba9.png)

* Utilisation du programme d’exemple IOAdafruit : Contrôle des relais / led et affichage d’une température 
Avant l'utilisation, remplacer la bibliothèque : __8. Ethernet__   
Puis, ajout des bibliothèques AdafruitIO : (dossier “Bibliothèques” sinon “Adafruit IO Arduino” dans le gestionnaire de bibliothèque)

![biblitotheque_adafruit](https://user-images.githubusercontent.com/106671909/172348082-0582ec85-0281-4b48-be03-dcfc316d525c.png)

__Architecture du broker MQTT AdafruitIO :__  

![broker_mqtt](https://user-images.githubusercontent.com/106671909/172346036-7d6264b6-0e51-4404-89f0-e9b7b56e5306.png)

* Température comme data et io.adafruit.com comme MQTT-Broker
* Données de températures acquise par la carte Teensy
* Envoi de data depuis la Teensy vers le broker MQTT en utilisant la bibliothèque Ethernet du module WIZ820-IO et AdafruitMQTT pour publier/souscrire

__Dashboard IO.Adafruit pour gérer/revoir les informations de la carte d'acquisition :__  

Pour comprendre la création/utilisation du dashboard IO.Adafruit se référer aux slides 21 et 22 du guide d'utilisation *"MicroDial_Data_Logger_V2.pdf"*  

![dashboard_adafruitio](https://user-images.githubusercontent.com/106671909/172346611-2a9bdb0a-44f6-4dc0-a7ad-8a38cd631114.png)

Thingspeak
----------

[Thingspeak](https://thingspeak.com/) est une plateforme IoT semblable à IO.Adafruit :  
* Création d’un compte Thingpseak via thingspeak.com  
* Création d’un channel afin d’afficher sur un dashboard les “fields” :

![channel_thingspeak](https://user-images.githubusercontent.com/106671909/172357852-b4e74429-14e9-43e6-a81f-8685408307a3.png)

* Récupération du Channel ID et de l’API Key pour une connexion à la plateforme 

![thingspeak_lora](https://user-images.githubusercontent.com/106671909/172367679-2a6120ce-828d-46e3-9491-8cccf8b9ccee.png)

LoRa
----

Protocole de télécommunication LoRa bas débit et à faible consommation électrique :

![lorawan](https://user-images.githubusercontent.com/106671909/172371094-faaf80fd-14d3-4c1f-9724-59f1fa514c7b.png)

Bluetooth Low Energy
--------------------------

fdsfsdfsdf
fdsfsdfsdfsd

















