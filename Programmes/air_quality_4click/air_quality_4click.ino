#include <Wire.h>

#define pinSDA 38
#define pinSCL 37

uint8_t data[8] = {0xBE,0xEF,0,0,0,0,0,0}; // CRC (0xBEEF) = 0x92   

// -------------------------------------------------------------

void setup()
{
  Wire1.begin();             // join i2c bus (address optional for master)
  Wire1.setSDA(pinSDA);
  Wire1.setSCL(pinSCL);
  Serial.begin(9600);       // start serial for output
}

// -------------------------------------------------------------

void loop()
{
  delay(1000);
  const uint8_t measure_air_quality[2] = {0x20, 0x08}; //Initialisation du capteur de la qualité de l'air
  const uint8_t init_air_quality[2] = {0x20, 0x03}; //Mesure de la qualité de l'air (CO2 et TVOC)*/

  int x=0; //INITIALISATION
  if(x=0)
  {
    Wire1.beginTransmission(0x58); //adresse I2C du composant
    Wire1.write(init_air_quality,2);
    Wire1.endTransmission();
    delay(10); //délai de 10ms
    x=1;
  }
  
  Wire1.beginTransmission(0x58); //MESURE DU CO2 ET DU TVOC
  Wire1.write(measure_air_quality,2);
  Wire1.endTransmission();
  
  delay(12); //délai de 12ms 
  
  Wire1.requestFrom(0x58,6); //requête de la data de 6 octets, CO2 data(MSB) / CO2 data(LSB) / CO2 checksum / TVOC data(MSB) / TVOC data (LSB) / TVOC checksum 
  uint16_t _CO2 = Wire1.read() << 8;
  _CO2 |= Wire1.read(); // VALEUR DU CO2
  uint8_t checksum_CO2 = Wire1.read(); //Checksum
  
  uint16_t _TVOC = Wire1.read() << 8;
  _TVOC |= Wire1.read(); // VALEUR DU TVOC
  uint8_t checksum_TVOC = Wire1.read(); //Checksum
  
  data[0] = _CO2/256;
  data[1] = _CO2; 
  uint8_t mycrc_CO2 = CRC8(data,2);
  
  data[0] = _TVOC/256;
  data[1] = _TVOC; 
  uint8_t mycrc_TVOC = CRC8(data,2);

  uint16_t CO2= _CO2; // Valeur du taux de CO2
  uint16_t TVOC= _TVOC; // Valeur du taux de TVOC
 
  Serial.println(mycrc_CO2); // Affichage crc CO2
  Serial.println(checksum_CO2); // Affichage checksum CO2
  Serial.println(mycrc_TVOC); // Affichage crc TVOC
  Serial.println(checksum_TVOC); // Affichage checksum TVOC 
  
  Serial.print("CO2: ");
  Serial.print(CO2); // Affichage du CO2
  Serial.print(" ppm\tTVOC: ");
  Serial.print(TVOC); // Affichage du TVOC
  Serial.println(" ppb");

  if(checksum_CO2!=mycrc_CO2)
  {
    Serial.println("ERREUR Checksum CO2"); //Affichage erreur CO2
  }
  if(checksum_TVOC!=mycrc_TVOC)
  {
    Serial.println("ERROR Checksum TVOC"); //Affichage erreur TVOC
  }
  
}

// -------------------------------------------------------------

/*void initAirQuality(void) // Fonction optionnelle pour initialiser 
{
  const uint8_t init_air_quality[2] = {0x20, 0x03};
  Wire.beginTransmission(0x58);
  Wire.write(init_air_quality,2);
  Wire.endTransmission();
}
*/
uint8_t CRC8(uint8_t *data,size_t longueur) // Fonction pour vérifier le checksum
{
  uint8_t crc =0xFF;
  size_t i,j;
  for (i = 0; i < longueur; i++) {
    crc ^=data[i];
    for(j = 0; j < 8; j++) {
      if ((crc & 0x80) !=0)
        crc = (uint8_t)((crc << 1) ^ 0x31);
      else
        crc <<= 1;
    }
    
  }
  return crc;
}
