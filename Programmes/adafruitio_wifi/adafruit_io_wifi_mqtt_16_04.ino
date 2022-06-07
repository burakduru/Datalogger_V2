#include <SPI.h>
#include <WiFi101.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

#define RST_click 27
#define INT_click 28
#define CS_click 9
#define PWM_click 2

char ssid[] = "iPhone";        // your network SSID (name)
char pass[] = "haciduru66";    // your network password (use for WPA, or use as key for WEP)
unsigned long int millis_last_publish = 0;
int status = WL_IDLE_STATUS;

EthernetClient client;
WiFiClient wificlient;
PubSubClient mqttclient(client);
int x = 2;
byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0x40, 0x4F};
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);
//// PERMET DE RESET LE TEENSY POUR SE RECONNECTER WIFI
#define RESTART_ADDR       0xE000ED0C
#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

void setupioteensy()
{
  pinMode(16,OUTPUT); //setup relais1
  
}


void setupwifi(){
  int nb_tentative=0;
  Serial.println("VOUS ETES DANS LA FONCTION SETUP WIFI");
  pinMode(RST_click,OUTPUT);
  digitalWrite(RST_click, HIGH);
  digitalWrite(RST_click, LOW);
  delay(1000);
  digitalWrite(RST_click,HIGH);
  WiFi.setPins(CS_click,INT_click,RST_click,PWM_click);
  
  // attempt to connect to WiFi network:
  while (WiFi.status() != WL_CONNECTED && nb_tentative++<10 ) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    
    // wait 10 seconds for connection:
    if(WiFi.status() != WL_CONNECTED) delay(1000);
  }
  Serial.println("Connected to wifi");
  Serial.println(WiFi.localIP());
}
void setup() {
  Serial.begin(115200);
  setupioteensy();
  setupwifi();
  Serial.println("function wifi OK");
  mqttclient.setServer("io.adafruit.com",1883);
  mqttclient.setCallback(callback);
  if (mqttclient.connect("io.adafruit.com", "burakduru", "aio_WUKY97uE23bJxuS8o4EiKMkpj6XH")){
    mqttclient.publish("burakduru/feeds/digital","1883");
    mqttclient.subscribe("burakduru/feeds/led");
    mqttclient.publish("burakduru/feeds/led/get",0); //grâce au get renvoi de la dernière valeur de relais1
  }
}

void loop() {
  char chaine[80];
  // put your main code here, to run repeatedly:
  if (!mqttclient.connected()) {
    if(WiFi.status() != WL_CONNECTED)
      setupwifi();
      Serial.println("LOOP 105");
      reconnect();
    if(mqttclient.connected())
    {
      mqttclient.subscribe("burakduru/feeds/led");
      //mqttclient.publish("burakduru/feeds/led/get",0); //grâce au get renvoi de la dernière valeur de relais1
    }
    else
    delay(1000);  
  }
  if((millis()-millis_last_publish)>2999) //publish toute les 3 secondes 
  {
    sprintf(chaine,"%d",x++);
    millis_last_publish =millis();
    mqttclient.publish("burakduru/feeds/digital",chaine);
    }
  /*mqttclient.subscribe("burakduru/feeds/led");
  delay(1000);*/
  mqttclient.loop();
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttclient.connected()) {
    if(WiFi.status() != WL_CONNECTED)
    {
      setupwifi();
      WRITE_RESTART(0x5FA0004);
      delay(1000); 
    }
    else 
    {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (mqttclient.connect("teensyclient", "burakduru", "aio_WUKY97uE23bJxuS8o4EiKMkpj6XH")) {
        Serial.println("connected");
      } else {
      Serial.print("failed, rc=");
      Serial.print(mqttclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      }
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  int relay=0; 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    if((char)payload[i] == '0')
    {
      relay=0;
      Serial.println("OK DOGECOIN");
      digitalWrite(16,0);
    }
    if((char)payload[i] == '1')
    {
      relay=1;
      Serial.println("OK TO THE MOON");
      digitalWrite(16,1);
    }
  }
  //digitalWrite(16,relay);
  Serial.println();
  Serial.println("-----------------------");
 
}
