/*-----------------------------------------------------------------------------------*/
/* BIBLOTHEQUES*/
#include <ADC.h>
#include <SPI.h>
/* DEFINE*/
#define SerialNMEA Serial5
#define TAILLE_NMEA 1024
/* VARIABLES*/
unsigned long int millis_last = 0;
const int SEL_0 = 21;
const int SEL_1 = 22;
const int SEL_2 = 23;

const int LED_VERTE = 35;
const int LED_ROUGE = 29;
const int LED_BLEU = 30;

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

#define TAILLE_BUFFER_RECEPTION_RS232 256
#define MAX_ARGUMENTS_MESSAGE 32

char buffer_reception_rs232[TAILLE_BUFFER_RECEPTION_RS232];
int index_buffer_reception_rs232 = 0;
char *tableau_ptr_token[MAX_ARGUMENTS_MESSAGE];
char buffer_debug_reader[TAILLE_BUFFER_RECEPTION_RS232];


void ecrire_debug(char *local_buffer_debug)
{
  Serial.println(local_buffer_debug);
}

void Delay_ms(int delai_ms)
 {
   delay(delai_ms);
 }

char crc_check(char *input_message)
{
    char crc = 0;
    int i;
    int longueur_message = strlen(input_message);
    if(longueur_message>(TAILLE_BUFFER_RECEPTION_RS232-1))
        return 0;

    // the first $ sign and the last two bytes of original CRC + the * sign
    for (i = 0; i < strlen(input_message); i++)
    {
        crc ^= input_message[i];
    } 
    return crc;
}

int etat_pwm_led_verte = 0;

void decoder_message_operateur(char *message_operateur)
{
    char local_buffer_debug[100];
    char str[] = "strtok needs to be called several times to split a string";
    int string_size = strlen(message_operateur);
    if (string_size < 3 || string_size > (TAILLE_BUFFER_RECEPTION_RS232 - 1))
    {
        return;
    }
    char delim[] = ";";
    int indice_argument = 0;

    tableau_ptr_token[indice_argument] = strtok(message_operateur, delim);
 
    while (tableau_ptr_token[indice_argument] != NULL&&(indice_argument<MAX_ARGUMENTS_MESSAGE))
    {
        sprintf(buffer_debug_reader, "Arg %d : '%s'\n", indice_argument, tableau_ptr_token[indice_argument]);
        //ecrire_debug(buffer_debug_reader);
        indice_argument++;
        tableau_ptr_token[indice_argument] = strtok(NULL, delim);
    }
    snprintf(local_buffer_debug, sizeof(local_buffer_debug), " Decodage Message %d Args\n", indice_argument);
    ecrire_debug(local_buffer_debug);         
    if (indice_argument > 2)
    {
        if (strcmp("#HEURE", tableau_ptr_token[0]) == 0)
        {
            //on va prendre les arguments suivants au nombre de 3 en plus
            //donc indice_argument == 4
            if (indice_argument == 5)
            {
                int heure = atoi(tableau_ptr_token[1]);
                int minute_i = atoi(tableau_ptr_token[2]);
                int seconde = atoi(tableau_ptr_token[3]);
                // last_setup_time_in_ms = millis();
                //startup_time_in_ms = ((heure)*3600 + (minute)*60 + (seconde)) * 1000;
                sprintf(buffer_debug_reader, "Update RTC a %.2d:%.2d:%.2d'\n", heure, minute_i, seconde);
                ecrire_debug(buffer_debug_reader);
                //setTime(heure,minute_i,seconde,8,9,2020);
                //Teensy3Clock.set(now());
            }
        }
        if (strcmp("#RGB", tableau_ptr_token[0]) == 0)
        {
            //on va prendre les arguments suivants au nombre de 3 en plus
            //donc indice_argument == 4
            analogWriteResolution(8);
            if (indice_argument == 5)
            {
                int red = atoi(tableau_ptr_token[1]);
                int green = atoi(tableau_ptr_token[2]);
                int blue = atoi(tableau_ptr_token[3]);

                analogWrite(PIN_CMD_RGB_BLUE, blue);
                analogWrite(PIN_CMD_RGB_RED, red);
                analogWrite(PIN_CMD_RGB_GREEN, green);
            }
        }
        if (strcmp("#LED", tableau_ptr_token[0]) == 0)
        {
            //on va prendre les arguments suivants au nombre de 3 en plus
            //donc indice_argument == 4
            if (indice_argument == 4)
            {
                int num_led = atoi(tableau_ptr_token[1]);
                int etat_led = atoi(tableau_ptr_token[2]);

                sprintf(buffer_debug_reader, "LED %d Nouvel ETAT %d\n", num_led, etat_led);
                ecrire_debug(buffer_debug_reader);
                switch (num_led)
                {
                case 0: //led verte , utilisée en pwm
                    etat_pwm_led_verte+=32;
                    if(etat_pwm_led_verte>255)
                    etat_pwm_led_verte=0;
                    analogWrite(LED_VERTE,etat_pwm_led_verte);
                    break;
                    /*
                    if (etat_led == 1)
                        digitalWrite(LED_VERTE, 1);
                    else
                        digitalWrite(LED_VERTE, 0);
                    break;
                    */
                case 1: //led orange
                    /*if (etat_led == 1)
                        digitalWrite(LED_ORANGE, 1);
                    else
                        digitalWrite(LED_ORANGE, 0);
                    break;*/
                    
                case 2: //led rouge
                  /*  if (etat_led == 1)
                        digitalWrite(LED_ROUGE, 1);
                    else
                        digitalWrite(LED_ROUGE, 0);
                    break;
                    */
                default:
                    break;
                }
            }
        }
        if (strcmp("#RELAIS", tableau_ptr_token[0]) == 0)
        {
            //on va prendre les arguments suivants au nombre de 3 en plus
            //donc indice_argument == 4
            if (indice_argument == 4)
            {
                int num_relais = atoi(tableau_ptr_token[1]);
                int etat_relais = atoi(tableau_ptr_token[2]);

                sprintf(buffer_debug_reader, "RELAIS %d Nouvel ETAT %d\n", num_relais, etat_relais);
                ecrire_debug(buffer_debug_reader);
                switch (num_relais)
                {
                case 0: 
                    break;
                case 1: 
                    if (etat_relais == 1)
                        digitalWrite(PIN_CMD_RELAIS_1, 1);
                    else
                        digitalWrite(PIN_CMD_RELAIS_1, 0);
                    break;
                case 2: 
                    if (etat_relais == 1)
                    {
                        digitalWrite(PIN_CMD_RELAIS_2_SET, 1);
                        Delay_ms(10);
                        digitalWrite(PIN_CMD_RELAIS_2_SET, 0);
                    }
                    else
                    {
                        digitalWrite(PIN_CMD_RELAIS_2_RESET, 1);
                        Delay_ms(10);
                        digitalWrite(PIN_CMD_RELAIS_2_RESET, 0);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        if (strcmp("#LOGGER", tableau_ptr_token[0]) == 0)
        {
            //on va prendre les arguments suivants au nombre de 3 en plus
            //donc indice_argument == 4
            if (indice_argument == 3)
            {
                int periode_logger_ms = atoi(tableau_ptr_token[1]);
                sprintf(buffer_debug_reader, "LOGGER NOUVELLE PERIODE %d \n",periode_logger_ms);
                ecrire_debug(buffer_debug_reader);

                if(periode_logger_ms<500)
                {
                    global_logger_state = false;
                }
                else
                {
                    global_logger_state = true;
                    if(periode_logger_ms<1000)
                    periode_logger_ms = 1000;//minimum
                    //global_periode_logger_ms = periode_logger_ms;
                }
            }
        }
    }
}
 
char copie_message_operateur_recu[TAILLE_BUFFER_RECEPTION_RS232];

bool verifier_crc_message_operateur(char *message_operateur_recu)
{
    char local_buffer_debug[120];
    bool retour_crc_bool = false;
    //copie le message avant de le manipuler
    strncpy(copie_message_operateur_recu,message_operateur_recu,TAILLE_BUFFER_RECEPTION_RS232);
    int longueur_message_operateur = strlen(copie_message_operateur_recu);

    if(longueur_message_operateur>6)
    {
        //pas de 0x0A qui n'est pas mis dans la chaine a la fin
         char hexa_1_recu = copie_message_operateur_recu[longueur_message_operateur-1];//ca finit en ;*hexa_2hexa_1
         char hexa_2_recu = copie_message_operateur_recu[longueur_message_operateur-2];//ca finit en ;*hexa_2hexa_1
         char car_star = copie_message_operateur_recu[longueur_message_operateur-3];
         if(car_star=='*')
         {
                copie_message_operateur_recu[longueur_message_operateur-2]=NULL;//on met NULL apres le *
                char retour_crc_char = crc_check(copie_message_operateur_recu);
                //ici on a donc le crc retourne
                //on le compare donc aux 2 hexa recus
                char hexa_calcule_majuscules[3];
                hexa_calcule_majuscules[2]=NULL;
                char hexa_calcule_minuscules[3];
                hexa_calcule_minuscules[2]=NULL;
                (void)snprintf(hexa_calcule_majuscules,3,"%.2X",retour_crc_char);
                (void)snprintf(hexa_calcule_minuscules,3,"%.2x",retour_crc_char);
                
                //verif en majuscules
                if(hexa_calcule_majuscules[0]==hexa_2_recu && hexa_calcule_majuscules[1]==hexa_1_recu)
                retour_crc_bool = true;
                
                //verif en minuscules
                if(hexa_calcule_minuscules[0]==hexa_2_recu && hexa_calcule_minuscules[1]==hexa_1_recu)
                retour_crc_bool = true;
                //if(retour_crc_bool==true)
                    snprintf(local_buffer_debug, sizeof(local_buffer_debug), "CRC recu OK %c %c - recalcule : %c %c \n",        hexa_2_recu,hexa_1_recu,hexa_calcule_majuscules[0],hexa_calcule_majuscules[1]);
                ecrire_debug(local_buffer_debug);
         }
         else
         {
              snprintf(local_buffer_debug, sizeof(local_buffer_debug), "PB CRC recu  * %c - length = %d -- h1 %c - h2 %c\n", car_star,longueur_message_operateur,hexa_1_recu,hexa_2_recu);
                ecrire_debug(local_buffer_debug);
         }
    }
    return retour_crc_bool;

}
/*-----------------------------------------------------------------------------------*/
/*MAIN*/
void setup(){
    pinMode(SHDN_MAX3227,OUTPUT);
    PORT_SERIE_OUTPUT_NMEA = 1;
    digitalWrite(SHDN_MAX3227, HIGH);
    Serial.begin(9600);
    Serial.println("Ethernet Webserver");
    SerialNMEA.begin(9600);
    //Serial1.begin(9600);
    
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

}
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

void gerer_rs232()
{
  int incomingByte = 0;
  int bytecount = 0;
  bool entete_reception_detecte = false;
  char local_buffer_debug[100];

        while (SerialNMEA.available())
        {
            incomingByte = SerialNMEA.read();
            // do something with incomingByte
            bytecount++;
            if (incomingByte == '#')
            {
                index_buffer_reception_rs232 = 0;
                entete_reception_detecte = true;
                snprintf(local_buffer_debug, sizeof(local_buffer_debug), "Thread Reader %d  : entete detecte \n", 0);
                ecrire_debug(local_buffer_debug);
            }
            if (incomingByte > 0)
            {
                if (entete_reception_detecte == true)
                {
                    buffer_reception_rs232[index_buffer_reception_rs232] = incomingByte;
                    index_buffer_reception_rs232++;
                    if (index_buffer_reception_rs232 > (TAILLE_BUFFER_RECEPTION_RS232 - 1))
                    {
                        index_buffer_reception_rs232 = 0;
                        entete_reception_detecte = false;
                        //il a du se produire un probleme
                        snprintf(local_buffer_debug, sizeof(local_buffer_debug), "Thread Reader %d  : message incorrect \n", 0);
                        ecrire_debug(local_buffer_debug);
                        break;
                    }
                    if (incomingByte == 0x0A)
                        break;
                }
                else
                {
                    //pas d'entete
                    snprintf(local_buffer_debug, sizeof(local_buffer_debug), "Thread Reader %d  : attention pas d'entete detecté \n", 0);
                    ecrire_debug(local_buffer_debug);
                }
            }
            else
                Delay_ms(1);
        }
        if (incomingByte == 0x0A && entete_reception_detecte == true)
        {
            buffer_reception_rs232[index_buffer_reception_rs232-1] = NULL;
            snprintf(local_buffer_debug, sizeof(local_buffer_debug), "Thread Reader Length  %d : %s \n", index_buffer_reception_rs232, buffer_reception_rs232);
            //ecrire_debug(local_buffer_debug);
            //verification du CRC
            //le CRC commence a N-2 caracteres car cela finit par ;*CRC_1CRC_2\n
            //c'est un XOR sur les données entrantes depuis le $ jusqu'à l'etoile comprise
            
            if(verifier_crc_message_operateur(buffer_reception_rs232)==true)
            {
                //on enleve le * et les 2 CRC
                //buffer_reception_rs232[longueur_message-2]=NULL; //a la place de *
                decoder_message_operateur(buffer_reception_rs232);
            }
            else
            {
                snprintf(local_buffer_debug, sizeof(local_buffer_debug), "Thread Reader %d  : ATTENTION CRC NOK \n", 0);
                //ecrire_debug(local_buffer_debug);
            }
            index_buffer_reception_rs232 = 0;
            entete_reception_detecte = false;
        }
        else
            Delay_ms(20);
}
void loop (){
     // fonction groupe (groupe en variable globale)
     gerer_rs232();
    if((millis()-millis_last) > 4999){
      scan_groupes_ports();
      millis_last = millis();
    }
    delay(500);//delais de 0.5s
/*
    if(GROUPE == 1 || GROUPE == 2)
    {
        TEMPERATURE = MOYENNAGE(true);
        Serial.println(TEMPERATURE);
    }
    else{
        TEMPERATURE = MOYENNAGE(false);
        Serial.println(TEMPERATURE);
    }
    if(GROUPE == 1)
    {
        YSI1[PORT] = TEMPERATURE;
        if(PORT == 7)
        {
            SerialNMEA.println("GROUPE1 =");
            SerialNMEA.println(GROUPE);
            GROUPE = GROUPE + 1;
        }
    }
    else if (GROUPE == 2)
    {
        SerialNMEA.println("Je suis la");
        YSI2[PORT] = TEMPERATURE;
        if (PORT == 7)
        {
            SerialNMEA.println("GROUPE2 =");
            SerialNMEA.println(GROUPE);
            GROUPE = GROUPE + 1;
        }
    }
    else if (GROUPE == 3)
    {
        U[PORT] = TEMPERATURE;
        Serial.println("OK TEST DU 1103");
        if(PORT == 7)
        {
            float TEMPS = (float) millis();
            
            do{
                TEMPS = (float) millis();
            }while ((TEMPS - T_DERNIER_ENVOI)<1000);
            float TEMPS_SECONDES = TEMPS / 1000;
            digitalWrite(LED_VERTE,HIGH);
            GERER_TRAME_NMEA(YSI1,YSI2,U,TEMPS_SECONDES);
            SerialNMEA.println(NMEA_DATA);

            T_DERNIER_ENVOI = TEMPS;
            CRC = NMEA0183_CHECKSUM(NMEA_DATA);
            if(PORT_SERIE_OUTPUT_NMEA == 1)
            {
                SerialNMEA.print("CRC =");
                SerialNMEA.println(CRC);
            }
            else if (PORT_SERIE_OUTPUT_NMEA == 0)
            {
                Serial.print("CRC= ");
                Serial.println(CRC);
            }
            TRAME_NMEA_CRC(NMEA_DATA,CRC);
            if(PORT_SERIE_OUTPUT_NMEA == 1)
            {
                SerialNMEA.println(TRAME_NMEA);
                SerialNMEA.println("I'M HERE !");
            }
            else if (PORT_SERIE_OUTPUT_NMEA ==0)
            {
                Serial.println(TRAME_NMEA);
            }
            GROUPE = 1;
            delay(10);
        }
    }
    if(PORT >= 7)
    {
        PORT = 0;
    }
    else
    {
        PORT= PORT+1;
    } */
    /*EthernetClient client = server.available();
    if (client) {
      Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          Serial.write(c);
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank) {
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            // output the value of each analog input pin
            client.print("test valeur : ");
            client.print(NMEA_DATA);
            client.println("<br />");
            //client.print(
            client.println("</html>");
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          } else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      client.stop();
      //Serial.println("client disconnected");
    //}*/
  
}

/*-----------------------------------------------------------------------------------*/
// FONCTIONS

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
